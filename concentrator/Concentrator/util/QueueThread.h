#ifndef QUEUETHREAD_H
#define QUEUETHREAD_H
#include <mutex>
#include <condition_variable>
#include <deque>

namespace NUtil {

  template <typename T>
  class CQueueThread
  {
  private:
    std::mutex              mutex;
    std::condition_variable condition;
    std::deque<T>           queue;
  public:
    CQueueThread() = default;

    void push(T const& value)
    {
      {
        std::unique_lock<std::mutex> lock(this->mutex);
        queue.push_front(value);
      }
      this->condition.notify_one();
    }

    T pop()
      {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [=]{ return !this->queue.empty(); });
        T rc(std::move(this->queue.back()));
        this->queue.pop_back();
        return rc;
      }

    decltype (queue.size()) size() const{
      return queue.size();
    }
  };
}


#endif // QUEUETHREAD_H
