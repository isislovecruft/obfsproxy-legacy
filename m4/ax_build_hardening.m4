
#
# SYNOPSIS
#
#   AX_ENABLE_HARDENING()
#
# DESCRIPTION
#
#   Attempt to enable various gcc compile time and linker security hardening
#   options.
#
# LICENSE
#
#   Copyright (c) 2012 Jacob Appelbaum <jacob@appelbaum.net>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 2

dnl This macro depends on AX_CHECK_COMPILE_FLAG and AX_CHECK_LINKER_FLAG
AC_DEFUN_ONCE([AX_ENABLE_HARDENING], [

### Compiler Hardening ###
# This requires ax_check_compile_flag.m4
AX_CHECK_COMPILE_FLAG([-D_FORTIFY_SOURCE=2], [CFLAGS="$CFLAGS -D_FORTIFY_SOURCE=2"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-fstack-protector-all], [CFLAGS="$CFLAGS -fstack-protector-all"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-fwrapv], [CFLAGS="$CFLAGS -fwrapv"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-fPIE], [CFLAGS="$CFLAGS -fPIE"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([--param ssp-buffer-size=1], [CFLAGS="$CFLAGS --param ssp-buffer-size=1"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-fno-strict-aliasing], [CFLAGS="$CFLAGS -fno-strict-aliasing"], [], [-Werror])
AX_CHECK_COMPILE_FLAG([-fno-strict-overflow], [CFLAGS="$CFLAGS -fno-strict-overflow"], [], [-Werror])

### Linker Hardening ###
# This requires ax_check_link_flag.m4
AX_CHECK_LINK_FLAG([-pie], [LDFLAGS="$LDFLAGS -pie"], [], [-Werror])
AX_CHECK_LINK_FLAG([-Wl,-pie], [LDFLAGS="$LDFLAGS -Wl,-pie"], [], [-Werror])
AX_CHECK_LINK_FLAG([-z relro], [LDFLAGS="$LDFLAGS -z relro"], [], [-Werror])
AX_CHECK_LINK_FLAG([-z now], [LDFLAGS="$LDFLAGS -z now"], [], [-Werror])

])
