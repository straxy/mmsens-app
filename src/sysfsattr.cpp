/**
 * @file sysfsattr.cpp
 * @author Strahinja Jankovic (strahinja.p.jankovic[at]gmail.com)
 * @details Implementation of sysfs attribute class
 */

#include "sysfsattr.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace sysfsattr {

SysfsAttr::SysfsAttr(const std::string &path) : m_path{path} {}

SysfsAttr::~SysfsAttr() {}

//-----------------------------------------------------------------------------

bool SysfsAttr::read(std::string *buf) {
  bool read = false;
  if (fs::is_regular_file(m_path) &&
      ((fs::status(m_path).permissions() & fs::perms::owner_read) !=
       fs::perms::none)) {
    std::ifstream ifile(m_path);
    ifile >> *buf;
    read = true;
    ifile.close();
  }
  return read;
}

//-----------------------------------------------------------------------------

bool SysfsAttr::write(const std::string &data) {
  bool written = false;
  if (fs::is_regular_file(m_path) &&
      ((fs::status(m_path).permissions() & fs::perms::owner_write) !=
       fs::perms::none)) {
    std::ofstream ofile(m_path);
    ofile << data;
    ofile.close();
    written = true;
  }
  return written;
}

//-----------------------------------------------------------------------------

bool SysfsAttr::read(uint32_t *buf) {
  std::string tmp;
  bool ret = false;

  if (read(&tmp)) {
    *buf = std::atoi(tmp.c_str());
    ret = true;
  }

  return ret;
}

//-----------------------------------------------------------------------------

bool SysfsAttr::write(const uint32_t &data) {
  std::string tmp = std::to_string(data);

  return write(tmp);
}

//-----------------------------------------------------------------------------

} // namespace sysfsattr
