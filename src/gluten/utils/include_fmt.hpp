#pragma once

#if USE_FMT
#  if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunknown-pragmas"
#  endif // defined(__GNUC__)

#  include <fmt/format.h>

#  if defined(__GNUC__)
#    pragma GCC diagnostic pop
#  endif // defined(__GNUC__)
#endif

