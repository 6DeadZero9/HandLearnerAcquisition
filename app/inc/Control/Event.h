#ifndef _EVENT_H_
#define _EVENT_H_

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

enum WaitResultEnum {
    WAIT_OK = 0,
    WAIT_TIMEOUT = 1,
    WAIT_ERROR = 2
};

enum EventTypeEnum {
    ManualReset = 0,
    AutoReset
};

/**
 * @brief Alternative event emitter class implementation for event based programming
 */
class Event {
public:
    Event(bool initialState, EventTypeEnum t = ManualReset);

    /**
     * @brief Set the event
     */
    void set();

    /**
     * @brief Reset the event
     */
    void reset();

    /**
     * @brief Wait until the event will be set with a specified timeout
     *
     * @param timeout milliseconds till the waiting timeouts
     * @returns enum entry that tells the result of waiting for the event
     */
    WaitResultEnum wait(int64_t timeout = -1);

private:
    std::mutex m_mutex;
    std::condition_variable m_conditionVar;
    EventTypeEnum m_eventType;
    bool m_signaled;
};

#endif /* _EVENT_H_ */
