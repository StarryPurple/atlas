#pragma once

#include <future>

#include "atlas/common/config.hpp"
#include "atlas/concurrency/mutex.hpp"
#include "atlas/concurrency/condition_variable.hpp"
#include "atlas/memory/memory.hpp"

namespace atlas {
namespace detail {
  template <typename T>
  struct SharedState {
    // State::Consumed is designed for implementing atlas::shared_future.
    // atlas::future throw another exception before reaching the code revealing the consumed state.
    enum class State { NotReady, Ready, Consumed };

    State state = State::NotReady;
    shared_ptr<T> value;
    std::exception_ptr exception;
    [[no_unique_address]] mutex mtx;
    [[no_unique_address]] condition_variable cv;

    void mark_ready() {
      lock_guard<mutex> lock(mtx);
      if (state != State::NotReady) {
        throw std::future_error(std::future_errc::promise_already_satisfied);
      }
      state = State::Ready;
      // not notify one, for detecting multiple uses of one future (which throws future_error)
      cv.notify_all();
    }

    void set_value(const T& v) {
      auto tmp = make_shared(v);
      {
        lock_guard<mutex> lock(mtx);
        if (state != State::NotReady) {
          throw std::future_error(std::future_errc::promise_already_satisfied);
        }
        value = std::move(tmp);
      }
      mark_ready();
    }

    void set_value(T&& v) {
      auto tmp = make_shared(std::move(v));
      {
        lock_guard<mutex> lock(mtx);
        if (state != State::NotReady) {
          throw std::future_error(std::future_errc::promise_already_satisfied);
        }
        value = std::move(tmp);
      }
      mark_ready();
    }

    void set_exception(std::exception_ptr e) {
      {
        lock_guard<mutex> lock(mtx);
        if (state != State::NotReady) {
          throw std::future_error(std::future_errc::promise_already_satisfied);
        }
        exception = std::move(e);
      }
      mark_ready();
    }

    bool ready() {
      return state != State::NotReady;
    }

    void wait() {
      unique_lock<mutex> lock(mtx);
      cv.wait(lock, [this] { return ready(); });
    }

    T get() {
      unique_lock<mutex> lock(mtx);
      cv.wait(lock, [this] { return ready(); });
      if (state == State::Consumed) {
        throw std::future_error(std::future_errc::future_already_retrieved);
      }
      state = State::Consumed;
      if (exception) {
        std::rethrow_exception(exception);
      }
      return std::move(*value);
    }
  };

  template <>
  struct SharedState<void> {
    // State::Consumed is designed for implementing atlas::shared_future.
    // atlas::future throw another exception before reaching the code revealing the consumed state.
    enum class State { NotReady, Ready, Consumed };

    State state = State::NotReady;
    std::exception_ptr exception;
    [[no_unique_address]] mutex mtx;
    [[no_unique_address]] condition_variable cv;

    void mark_ready() {
      lock_guard<mutex> lock(mtx);
      if (state != State::NotReady) {
        throw std::future_error(std::future_errc::promise_already_satisfied);
      }
      state = State::Ready;
      // not notify one, for detecting multiple uses of one future (which throws future_error)
      cv.notify_all();
    }

    void set_value() {
      mark_ready();
    }

    void set_exception(std::exception_ptr e) {
      {
        lock_guard<mutex> lock(mtx);
        if (state != State::NotReady) {
          throw std::future_error(std::future_errc::promise_already_satisfied);
        }
        exception = std::move(e);
      }
      mark_ready();
    }

    bool ready() {
      return state != State::NotReady;
    }

    void wait() {
      unique_lock<mutex> lock(mtx);
      cv.wait(lock, [this] { return ready(); });
    }

    void get() {
      unique_lock<mutex> lock(mtx);
      cv.wait(lock, [this] { return ready(); });
      if (state == State::Consumed) {
        throw std::future_error(std::future_errc::future_already_retrieved);
      }
      state = State::Consumed;
      if (exception) {
        std::rethrow_exception(exception);
      }
    }
  };
}

template <typename T>
class promise;

template <typename T>
class future {
public:
  future() = default;
  ~future() = default;

  ATLAS_DISABLE_COPY(future);
  ATLAS_DEFAULT_MOVE(future);

  T get() {
    if (!state_) {
      throw std::future_error(std::future_errc::no_state);
    }
    auto state = std::move(state_);
    return state->get();
  }

  void wait() const {
    if (!state_) {
      throw std::future_error(std::future_errc::no_state);
    }
    state_->wait();
  }

private:
  using state_ptr = shared_ptr<detail::SharedState<T>>;

  friend class promise<T>;
  explicit future(state_ptr state): state_(std::move(state)) {}

  state_ptr state_;
};

template <typename T>
class promise {
public:
  promise(): state_(shared_ptr<detail::SharedState<T>>()) {}

  ~promise() {
    if (state_ && !state_->ready()) {
      state_->set_exception(
        std::make_exception_ptr(std::future_error(std::future_errc::broken_promise))
      );
    }
  }

  ATLAS_DISABLE_COPY(promise);
  ATLAS_DEFAULT_MOVE(promise);

  future<T> get_future() {
    if (future_retrieved_) {
      throw std::future_error(std::future_errc::future_already_retrieved);
    }
    future_retrieved_ = true;
    return future<T>(state_);
  }

  void set_value(const T& t) { state_->set_value(t); }
  void set_value(T&& t) { state_->set_value(std::move(t)); }

  void set_exception(std::exception_ptr e) {
    state_->set_exception(e);
  }

private:
  using state_ptr = shared_ptr<detail::SharedState<T>>;
  bool future_retrieved_ = false;
  state_ptr state_;
};

template <>
class promise<void> {
public:
  promise(): state_(shared_ptr<detail::SharedState<void>>()) {}

  ~promise() {
    if (state_ && !state_->ready()) {
      state_->set_exception(
        std::make_exception_ptr(std::future_error(std::future_errc::broken_promise))
      );
    }
  }

  ATLAS_DISABLE_COPY(promise);
  ATLAS_DEFAULT_MOVE(promise);

  future<void> get_future() {
    if (future_retrieved_) {
      throw std::future_error(std::future_errc::future_already_retrieved);
    }
    future_retrieved_ = true;
    return future<void>(state_);
  }

  void set_value() { state_->set_value(); }

  void set_exception(std::exception_ptr e) {
    state_->set_exception(e);
  }

private:
  using state_ptr = shared_ptr<detail::SharedState<void>>;
  bool future_retrieved_ = false;
  state_ptr state_;
};

template <typename Signature>
class packaged_task;

template <typename Res, typename... Args>
class packaged_task<Res(Args...)> {
public:
  packaged_task() = default;

  template <typename Func>
  requires (!std::is_same_v<std::remove_cvref_t<Func>, packaged_task>)
  explicit packaged_task(Func&& f):
    func_(std::forward<Func>(f)), promise_() {}

  ATLAS_DISABLE_COPY(packaged_task);
  ATLAS_DEFAULT_MOVE(packaged_task);

  ~packaged_task() = default; // broken_promise handled at promise_.~promise()

  future<Res> get_future() {
    return promise_.get_future();
  }

  void operator()(Args... args) {
    try {
      if constexpr (std::is_void_v<Res>) {
        func_(std::forward<Args>(args)...);
        promise_.set_value();
      } else {
        promise_.set_value(func_(std::forward<Args>(args)...));
      }
    } catch (...) {
      promise_.set_exception(std::current_exception());
    }
  }

private:
  std::function<Res(Args...)> func_;
  promise<Res> promise_;
};
} // namespace atlas