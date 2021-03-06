#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([ChaNGa], [3.0], [changa-users@u.washington.edu])
AC_DEFINE_UNQUOTED([NBODY_PACKAGE_VERSION], [$PACKAGE_VERSION])
AC_CONFIG_SRCDIR([Reductions.h])
AC_CONFIG_HEADER([config.h $srcdir/../utility/structures/config.h])

AC_ARG_VAR([CHARMC], "Charm compiler")
# Checks for programs.
AC_PATH_PROG([CHARMC], [charmc], [no], [$PATH:$CHARM_DIR/bin:../charm/bin:../../charm/bin])

if test "$CHARMC" = "no" ; then
   AC_MSG_WARN([Cannnot locate charmc])
   AC_MSG_WARN([Try setting your CHARM_DIR environment variable to point at the charm source directory])
   exit
fi

CHARM_PATH=${CHARMC%/bin/charmc}
CONV_CONFIG=${CHARM_PATH}/tmp/conv-config.sh
CHARMINC=${CHARM_PATH}/include
AC_SUBST([CHARM_PATH])
. ${CONV_CONFIG}
CXX=${CMK_SEQ_CXX}
CC=${CMK_SEQ_CC}
LD=${CMK_SEQ_LD}
LDXX=${CMK_SEQ_LDXX}
AC_SUBST([AR], [${CMK_AR}])
CPPFLAGS="$CPPFLAGS -I.."
AC_CHECK_FILE(../libs, [LDFLAGS="$LDFLAGS -L../libs"])

AC_PROG_CXX($CHARMC)
# AC_PROG_CC($CHARMC)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/bittypes.h endian.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h sys/param.h unistd.h values.h])
AC_CHECK_HEADER(netinet/in.h,,[MISSING_NETINET=yes])
if test "${MISSING_NETINET}" = "yes" ; then
   CPPFLAGS="$CPPFLAGS -I/usr/include"
fi

AC_CHECK_HEADERS([rpc/types.h])
AC_CHECK_HEADER([rpc/xdr.h],,[MISSING_XDR=yes],
[#ifdef HAVE_RPC_TYPES_H
#include <rpc/types.h>
#endif
])
AC_SEARCH_LIBS([xdr_float], [xdr],,[MISSING_XDR=yes])
if test "${MISSING_XDR}" = "yes" ; then
   echo "XDR missing, attempting to use xdr with package"
   if test -d ../xdr ; then
   	ln -s $PWD/../xdr ../rpc
   else
	echo "Try downloading the xdr package and place it in the parent directory."
	echo "And do a make in ../xdr"
	exit
   fi
   CPPFLAGS="$CPPFLAGS -I.."
   LDFLAGS="$LDFLAGS -L../xdr"
fi

# 128 bit keys:
AC_ARG_ENABLE([bigkeys],
	[AS_HELP_STRING([--enable-bigkeys], [enable 128 bit hash keys])],
	[bigkeys=yes], [bigkeys=no])
if test "$bigkeys" = "yes" ; then
    echo "BIGKEYS selected"
    FLAG_BIGKEYS="-DBIGKEYS"
else
    FLAG_BIGKEYS=""
fi

AC_SUBST([FLAG_BIGKEYS])

# Cooling:
AC_ARG_ENABLE([cooling],
	[AS_HELP_STRING([--enable-cooling], [enable gas cooling (planet,cosmo)])],
	[cooling=$enableval], [cooling=no])
OBJECTS_COOLING=""
if test "$cooling" = "no" ; then
   echo "Adiabatic SPH only selected"
   FLAG_COOLING="-DCOOLING_NONE"
elif test "$cooling" = "planet" ; then
   echo "Planet cooling selected"
   FLAG_COOLING="-DCOOLING_PLANET"
   OBJECTS_COOLING="cooling_planet.o stiff.o"
elif test "$cooling" = "cosmo" ; then
   echo "Cosmo cooling selected"
   FLAG_COOLING="-DCOOLING_COSMO"
   OBJECTS_COOLING="cooling_cosmo.o stiff.o"
else
	echo "Unknown cooling selected, using Adiabatic"
	FLAG_COOLING="-DCOOLING_NONE"
	cooling="no"
fi
AC_ARG_ENABLE([hexadecapole],
        [AS_HELP_STRING([--disable-hexadecapole], [disable use of hexadecapole expansions])],
        # Note that "$enableval" being "yes" means ENABLE hexadecapole -- the
        # help string, as one would expect, only affects how we document
        # the option.
        [case "$enableval" in
              yes | no ) hexadecapole="$enableval";;
              *) AC_MSG_ERROR("$enableval: invalid argument to `--disable-hexadecapole'") ;;
         esac],
        [hexadecapole="yes"])

HEXADECAPOLE=""
if test "$hexadecapole" = "yes"; then
    HEXADECAPOLE="-DHEXADECAPOLE"
else
    HEXADECAPOLE=""
fi

AC_SUBST([FLAG_COOLING])
AC_SUBST([OBJECTS_COOLING])
AC_SUBST([HEXADECAPOLE])
 
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([u_int64_t, u_quad_t, int64_t, quad_t, quad])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset pow sqrt strchr])
AC_CHECK_LIB([xdr], [xdr_hyper])
AC_CHECK_FUNCS([xdr_hyper])

####################### CUDA #######################
m4_include([cuda.ac])
##################### END CUDA #####################


AC_CONFIG_FILES([Makefile
		 $srcdir/../utility/structures/Makefile
		])
AC_OUTPUT
