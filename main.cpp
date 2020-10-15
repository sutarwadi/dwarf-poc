//------------------------------------------------------------------------------
// SPDX-License-Identifier: "Apache-2.0 OR MIT"
// Copyright (C) 2020, Jayesh Badwaik <jayesh@badwaik.in>
//------------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/core/demangle.hpp>

#include <elfutils/libdwfl.h>
#include <execinfo.h>

struct DebugInfoSession {
  Dwfl_Callbacks callbacks = {};
  char* debuginfo_path = nullptr;
  Dwfl* dwfl = nullptr;

  DebugInfoSession()
  {
    callbacks.find_elf = dwfl_linux_proc_find_elf;
    callbacks.find_debuginfo = dwfl_standard_find_debuginfo;
    callbacks.debuginfo_path = &debuginfo_path;

    dwfl = dwfl_begin(&callbacks);
    assert(dwfl);

    int r;
    r = dwfl_linux_proc_report(dwfl, getpid());
    assert(!r);
    r = dwfl_report_end(dwfl, nullptr, nullptr);
    assert(!r);
    static_cast<void>(r);
  }

  ~DebugInfoSession() { dwfl_end(dwfl); }

  DebugInfoSession(DebugInfoSession const&) = delete;
  DebugInfoSession& operator=(DebugInfoSession const&) = delete;
};

struct DebugInfo {
  void* ip;
  std::string function;
  char const* file;
  int line;

  DebugInfo(DebugInfoSession const& dis, void* ip) : ip(ip), file(), line(-1)
  {
    // Get function name.
    uintptr_t ip2 = reinterpret_cast<uintptr_t>(ip);
    Dwfl_Module* module = dwfl_addrmodule(dis.dwfl, ip2);
    char const* name = dwfl_module_addrname(module, ip2);
    function = name ? boost::core::demangle(name) : "<unknown>";

    // Get source filename and line number.
    Dwfl_Line* dwfl_line = dwfl_module_getsrc(module, ip2);
    Dwarf_Addr addr;
    file = dwfl_lineinfo(dwfl_line, &addr, &line, nullptr, nullptr, nullptr);
  }
};

std::ostream& operator<<(std::ostream& s, DebugInfo const& di)
{
  s << di.ip << ' ' << di.function;
  if (di.file)
    s << " at " << di.file << ':' << di.line;
  return s;
}

__attribute__((always_inline)) void terminate_with_stacktrace()
{
  void* stack[512];
  int stack_size = ::backtrace(stack, sizeof stack / sizeof *stack);

  DebugInfoSession dis;
  std::cerr << "Stacktrace of " << stack_size << " frames:\n";
  for (int i = 0; i < stack_size; ++i) {
    std::cerr << i << ": " << DebugInfo(dis, stack[i]) << '\n';
  }
  std::cerr.flush();

  std::_Exit(EXIT_SUCCESS);
}

__attribute__((noinline)) void hodormania()
{
  terminate_with_stacktrace();
}

__attribute__((noinline)) void godormania()
{
  hodormania();
}

__attribute__((noinline)) void fodormania()
{
  godormania();
}

int main()
{
  fodormania();
}
