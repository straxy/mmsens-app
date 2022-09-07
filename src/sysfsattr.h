/**
 * 
 * @file sysfsattr.h
 * @author Strahinja Jankovic (strahinja.p.jankovic[at]gmail.com)
 * @details Implementation of sysfs attribute class
 */

#ifndef _SYSFS_ATTR_H_
#define _SYSFS_ATTR_H_

#include <string>

namespace sysfsattr
{

class SysfsAttr
{
public:
    /**
     * Constructor
     * 
     * @param path - path to sysfs attribute
     */
    explicit SysfsAttr(const std::string& path);

    /* Disable copy constructor and assignment. */
    SysfsAttr(const SysfsAttr&)              = delete;
    SysfsAttr & operator=(const SysfsAttr&)  = delete;

    /** Destructor. */
    virtual ~SysfsAttr();

    /**
     * read
     * 
     * Read string value from sysfs attribute.
     * 
     * @param buf - pointer to buffer where read data will be stored
     *
     * @returns bool - True if read was successful
     */
    bool read(std::string *buf);

    /**
     * write
     * 
     * Write string value to sysfs attribute.
     * 
     * @param data - data to write
     * 
     * @returns bool - True if write was successful
     */
    bool write(const std::string &data);

    /**
     * bool_read
     * 
     * Read one byte value interpreted as uint8_t
     * 
     * @param buf - pointer to buffer where read data will be stored
     * 
     * @returns bool - True if read was successful
     */
    bool read(uint8_t *buf);

    /**
     * bool_write
     * 
     * Write one byte value interpreted as uint8_t
     * 
     * @param data - data to write
     * 
     * @returns bool - True if read was successful
     */
    bool write(const uint8_t &data);

private:
    /** Stored attribute path. */
    std::string m_path;
};

}   // namespace sysfsattr

#endif  // _SYSFS_ATTR_H_
