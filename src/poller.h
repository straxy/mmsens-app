/**
 * @file Poller.h
 * @author Strahinja Jankovic (strahinja.p.jankovic[at]gmail.com)
 * @details Implementation of polling mechanism
 */

#ifndef _POLLER_H_
#define _POLLER_H_

#include <string>
#include <map>
#include "sysfsattr.h"

using namespace sysfsattr;

namespace poller
{

class Poller
{
public:
    /**
     * Constructor
     * 
     * @param path - path to sysfs device
     */
    explicit Poller(std::string path);

    /* Disable copy constructor and assignment. */
    Poller(const Poller&)              = delete;
    Poller & operator=(const Poller&)  = delete;

    /** Destructor. */
    virtual ~Poller();

    /**
     * init
     * 
     * Initialize device and enable interrupt generation.
     */
    void init(void);

    /**
     * deinit
     * 
     * Disable interrupt generation and turn off device.
     */
    void deinit(void);

    /**
     * runThread
     * 
     * Start executing thread.
     */
    void runThread(void);

    /**
     * stopThread
     * 
     * Stop executing thread.
     */
    void stopThread(void);

private:
    /**
     * do_poll
     * 
     * Thread that polls sysfs interrupt attribute.
     */
    void do_poll(void);

    /** Path to sysfs device. */
    std::string m_path;

    /** Map where all attributes are stored. */
    std::map<std::string, SysfsAttr *> m_attributes;

    /** Thread handle. */
    std::thread m_thread;

    /** Flag used to control thread execution. */
    bool m_running;
};

}   // namespace poller

#endif  // _POLLER_H_
