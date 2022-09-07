#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "poller.h"

using namespace poller;

namespace
{
    /** Names of all attributes that are used. */
    constexpr std::array attributes =
    {
        "enable",
        "enable_interrupt",
        "data",
        "frequency"
    };
};  // namespace anonymous

//-----------------------------------------------------------------------------

Poller::Poller(std::string path) : m_path{path}, m_running{false}
{
    // Create attributes
    for (auto attr : attributes)
    {
        m_attributes[attr] = new SysfsAttr(m_path + "/" + attr);
    }
}

//-----------------------------------------------------------------------------

Poller::~Poller()
{
    // Cleanup attributes list
    for (auto attr : attributes)
    {
        delete(m_attributes[attr]);
    }
    m_attributes.clear();
}

//-----------------------------------------------------------------------------

void Poller::init(void)
{
    // Enable device
    m_attributes["enable"]->write(1);

    // Configure frequency
    m_attributes["frequency"]->write("normal");

    // Enable interrupt
    m_attributes["enable_interrupt"]->write(1);
}

//-----------------------------------------------------------------------------

void Poller::deinit(void)
{
    // Disable interrupt
    m_attributes["enable_interrupt"]->write(0);

    // Disable device
    m_attributes["enable"]->write(0);
}

//-----------------------------------------------------------------------------

void Poller::runThread(void)
{
    if (!m_thread.joinable())
    {
        m_thread = std::thread(&Poller::do_poll, this);
    }
}

//-----------------------------------------------------------------------------

void Poller::stopThread(void)
{
    m_running = false;

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

//-----------------------------------------------------------------------------

void Poller::do_poll(void)
{
    m_running = true;

    int sysfs_fd;
    struct pollfd pfd;
    uint8_t value;

    // Open file that is polled
    sysfs_fd = open(std::string(m_path + "/interrupt").c_str(), O_RDONLY);
    if (sysfs_fd == -1)
    {
        return;
    }

    // Kick with first read
    if (read(sysfs_fd, &value, sizeof(value)) > 0)
    {
        lseek(sysfs_fd, 0, SEEK_SET);
    }

    pfd.fd = sysfs_fd;
    pfd.events = POLLPRI|POLLERR;

    while (m_running)
    {
        // Poll with timeout so we can capture signal
        int ret = poll(&pfd, 1, 500);
        // Error is <0, timeout =0
        if (ret > 0)
        {
            std::string data;

            // Must do a read to retrigger (together with lseek)
            ret = read(sysfs_fd, &value, sizeof(value));

            // Read and print data value
            m_attributes["data"]->read(&data);
            std::cout << data << std::endl;

            lseek(pfd.fd, 0, SEEK_SET);
        }
    }
}
