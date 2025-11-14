#include <Event.h>

Event::Event(bool initialState, EventTypeEnum t)
    : m_eventType(t),
      m_signaled(initialState) {}

void Event::set() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_signaled = true;
    if (m_eventType == ManualReset) {
        m_conditionVar.notify_all();
    } else {
        m_conditionVar.notify_one();
    }
}

void Event::reset() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_signaled = false;
}

WaitResultEnum Event::wait(int64_t timeout) {
    std::unique_lock<std::mutex> lock(m_mutex);
    WaitResultEnum res = WAIT_OK;

    if (timeout < 0) {
        m_conditionVar.wait(lock, [this]() {
            return m_signaled;
        });

        if (m_eventType != ManualReset) {
            m_signaled = false;
        }
    } else {
        bool result = m_conditionVar.wait_for(lock, (std::chrono::milliseconds)timeout, [this]() {
            return m_signaled;
        });

        if (result) {
            if (m_eventType != ManualReset) {
                m_signaled = false;
            }
        } else {
            res = WAIT_TIMEOUT;
        }
    }
    return res;
}
