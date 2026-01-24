AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${es_CFLAGS}

lib_LTLIBRARIES = libmsdb.la
libmsdb_la_SOURCES =  src/model_spectra_db.cpp
libmsdb_la_LDFLAGS = -version-info 0:0:0

sbin_PROGRAMS = genmsdb
genmsdb_SOURCES = src/gen_msdb.cpp
genmsdp_CXXFLAGS =  ${xstdlib_CFLAGS}
genmsdb_LDADD = libmsdb.la ${es_LIBS}

include_HEADERS = include/model_spectra_db.h


pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

