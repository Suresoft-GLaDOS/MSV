// Copyright (C) 2016 Fan Long, Martin Rianrd and MIT CSAIL
// Prophet
//
// This file is part of Prophet.
//
// Prophet is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Prophet is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Prophet.  If not, see <http://www.gnu.org/licenses/>.
#include <array>
#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define MAXSZ 1048576
#define ONE_OR_N_BIT 50

static bool init = false;
static bool enable = false;
static bool mutant_init = false;
static int mutant_id = 0;
static unsigned long current_cnt = 0;
static unsigned long records[MAXSZ];
static unsigned long records_sz;

static void __attribute((constructor)) _init() {
  memset(records, 0, sizeof(records));
  records_sz = 0;
  char *tmp = getenv("IS_NEG");
  if (!tmp)
    return;
  std::string is_neg = tmp;
  if (is_neg == "")
    return;
  pid_t pid = getpid();
  std::ostringstream sout;
  sout << "/proc/" << pid << "/cmdline";
  FILE *f = fopen(sout.str().c_str(), "rb");
  assert(f && "Cannot get the cmdline str");

  std::vector<std::string> res;
  res.clear();
  char c;
  std::string now_str = "";
  int read_cnt = fread(&c, 1, 1, f);
  while (read_cnt == 1 && !feof(f)) {
    if (c == '\0' || c == '\n') {
      res.push_back(now_str);
      now_str = "";
    } else
      now_str.push_back(c);
    read_cnt = fread(&c, 1, 1, f);
  }
  fclose(f);

  // FIXME: very hacky for php, php invokes multiple times
  enable = true;
  for (unsigned long i = 0; i < res.size(); i++) {
    if (res[i].find("run-test") != std::string::npos) {
      enable = false;
      break;
    }
    if (res[i].find("echo PHP_VERSION") != std::string::npos) {
      enable = false;
      break;
    }
  }
}

bool isGoodAddr(void *pointer, size_t size) {
  int nullfd = open("/dev/random", O_WRONLY);

  if (write(nullfd, pointer, size) < 0) {
    close(nullfd);
    return false;
  }
  close(nullfd);

  return true;
}

extern "C" int __get_mutant() {
  if (!mutant_init) {
    char *tmp = getenv("MUTANT_ID");
    sscanf(tmp, "%d", &mutant_id);
    mutant_init = true;
  }
  // fprintf(stderr, "running mutant: %d\n", mutant_id);
  return mutant_id;
}

extern "C" int __choose(const char *switch_id) {
  // fprintf(stderr,"id: %d\n",id);
  char *env = getenv(switch_id);
  if (env == NULL)
    return 0;
  int result = atoi(env);
  return result;
}

#define MAGIC_NUMBER -123456789

extern "C" int __is_neg(const char *location, int count, ...) {
  std::string locStr(location);
  size_t split = locStr.find("-");
  size_t switchNum = atoi(locStr.substr(0, split).c_str());
  size_t caseNum = atoi(locStr.substr(split + 1).c_str());

  if (switchNum == 101 && caseNum == 2) {
    switch (__choose(std::string("__CONDITION_101_2").c_str())) {
    case 0:
      return 1;
    case 1: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 0 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 1);
    }
    case 2: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 1 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 3: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 2 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 4: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 4 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 5: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 5 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 6: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 6 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 7: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 9 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    }
  }
  if (switchNum == 103 && caseNum == 1) {
    switch (__choose(std::string("__CONDITION_103_1").c_str())) {
    case 0:
      return 1;
    case 1: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 0 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 1);
    }
    case 2: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 1 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 3: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 2 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 4: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 3 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 5: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 4 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 6: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 6 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 7: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 9 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    }
  }
  if (switchNum == 104 && caseNum == 1) {
    switch (__choose(std::string("__CONDITION_104_1").c_str())) {
    case 0:
      return 1;
    case 1: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 0 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 1);
    }
    case 2: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 1 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 3: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 2 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 4: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 3 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    case 5: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 4 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 6: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 6 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 0);
    }
    case 7: {
      va_list ap;
      va_start(ap, count);
      void *p;
      unsigned long sz;
      long long v = 0;
      for (size_t __i = 0; __i < 9 + 1; __i++) {
        p = va_arg(ap, void *);
        sz = va_arg(ap, unsigned long);
      }
      if (isGoodAddr(p, sz)) {
        memcpy(&v, p, sz);
      } else
        return 0;
      return (v == 4);
    }
    }
  }
}
