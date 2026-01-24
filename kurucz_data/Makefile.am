AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include -DDATADIR=\"$(pkgdatadir)\" ${es_CFLAGS} ${xastro_CFLAGS} ${radiation_CFLAGS} ${linerout_CFLAGS} ${msdb_CFLAGS} ${opacity_profile_CFLAGS} ${xmath_CFLAGS} ${velocity_field_CFLAGS}

lib_LTLIBRARIES = libkurucz_data.la
libkurucz_data_la_SOURCES = src/kdd_compute_H_Z.cpp src/kdd_initialize.cpp src/kdd_process_level.cpp src/kld_operators.cpp src/klnd_labels.cpp src/klnd_mw18.cpp src/klnd_process_datafile_line.cpp src/kurucz_data.cpp src/ld_constructor.cpp src/ld_operators.cpp
libkurucz_data_la_LDFLAGS = -version-info 1:0:1

noinst_PROGRAMS = testkurucz

testkurucz_SOURCES = src/testkurucz.cpp
testkurucz_LDADD = libkurucz_data.la ${es_LIBS} ${xastro_LIBS} ${radiation_LIBS} ${linerout_LIBS} ${msdb_LIBS} ${opacity_profile_LIBS} ${xmath_LIBS}

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

include_HEADERS = include/kurucz_data.h

datasub0dir = ${pkgdatadir}
datasub1dir = ${datasub0dir}
datasub2dir = ${datasub0dir}
datasubNdir = ${datasub0dir}


dist_datasub0_DATA = data/00gffix.readme data/getall data/gf0100.all data/gf0200.all data/gf0201.all data/gf0300.all data/gf0301.all data/gf0400.all data/gf0401.all data/gf0402.all data/gf0500.all data/gf0501.all data/gf0502.all data/gf0503.all data/gf0600.all data/gf0601.all data/gf0602.all data/gf0603.all data/gf0700.all data/gf0701.all data/gf0702.all data/gf0703.all data/gf0704.all data/gf0705.all data/gf0800.all data/gf0801.all data/gf0802.all data/gf0803.all data/gf0804.all data/gf0805.all data/gf0900.all data/gf0901.all data/gf0902.all data/gf0903.all data/gf0904.all data/gf0905.all

dist_datasub1_DATA = data/gf1000.all data/gf1001.all data/gf1002.all data/gf1003.all data/gf1004.all data/gf1005.all data/gf1100.all data/gf1101.all data/gf1102.all data/gf1103.all data/gf1104.all data/gf1105.all data/gf1200.all data/gf1201.all data/gf1202.all data/gf1203.all data/gf1204.all data/gf1205.all data/gf1300.all data/gf1301.all data/gf1302.all data/gf1303.all data/gf1304.all data/gf1305.all data/gf1400.all data/gf1401.all data/gf1402.all data/gf1403.all data/gf1404.all data/gf1405.all data/gf1500.all data/gf1501.all data/gf1502.all data/gf1503.all data/gf1504.all data/gf1505.all data/gf1600.all data/gf1601.all data/gf1602.all data/gf1603.all data/gf1604.all data/gf1605.all data/gf1700.all data/gf1701.all data/gf1702.all data/gf1703.all data/gf1704.all data/gf1800.all data/gf1801.all data/gf1802.all data/gf1803.all data/gf1804.all data/gf1900.all data/gf1901.all data/gf1902.all data/gf1903.all data/gf1904.all

dist_datasub2_DATA = data/gf2000.all data/gf2001.all data/gf2002.all data/gf2003.all data/gf2004.all data/gf2005.all data/gf2006.all data/gf2007.all data/gf2008.all data/gf2100.all data/gf2101.all data/gf2102.all data/gf2103.all data/gf2104.all data/gf2105.all data/gf2106.all data/gf2107.all data/gf2108.all data/gf2200.all data/gf2201.all data/gf2202.all data/gf2203.all data/gf2204.all data/gf2205.all data/gf2206.all data/gf2207.all data/gf2208.all data/gf2300.all data/gf2301.all data/gf2302.all data/gf2303.all data/gf2304.all data/gf2305.all data/gf2306.all data/gf2307.all data/gf2308.all data/gf2400.all data/gf2401.all data/gf2402.all data/gf2403.all data/gf2404.all data/gf2405.all data/gf2406.all data/gf2407.all data/gf2408.all data/gf2500.all data/gf2501.all data/gf2502.all data/gf2503.all data/gf2504.all data/gf2505.all data/gf2506.all data/gf2507.all data/gf2508.all data/gf2600.all data/gf2601.all data/gf2602.all data/gf2603.all data/gf2604.all data/gf2605.all data/gf2606.all data/gf2607.all data/gf2608.all data/gf2700.all data/gf2701.all data/gf2702.all data/gf2703.all data/gf2704.all data/gf2705.all data/gf2706.all data/gf2707.all data/gf2708.all data/gf2800.all data/gf2801.all data/gf2802.all data/gf2803.all data/gf2804.all data/gf2805.all data/gf2806.all data/gf2807.all data/gf2808.all data/gf2900.all data/gf2901.all 

dist_datasubN_DATA = data/gf3000.all data/gf3001.all data/gf3002.all data/gf3100.all data/gf3101.all data/gf3200.all data/gf3201.all data/gf3300.all data/gf3400.all data/gf3700.all data/gf3800.all data/gf3801.all data/gf3900.all data/gf3901.all data/gf4000.all data/gf4001.all data/gf4100.all data/gf4101.all data/gf4200.all data/gf4201.all data/gf4400.all data/gf4401.all data/gf4500.all data/gf4501.all data/gf4600.all data/gf4601.all data/gf4700.all data/gf4701.all data/gf4800.all data/gf4801.all data/gf4900.all data/gf4901.all data/gf5000.all data/gf5001.all data/gf5601.all data/gf5701.all data/gf5801.all data/gf5901.all data/gf6001.all data/gf6201.all data/gf6301.all data/gf6401.all data/gf6501.all data/gf6601.all data/gf6701.all data/gf6801.all data/gf6901.all data/gf7001.all data/gf7101.all data/gf7201.all data/gf7301.all data/gf7401.all data/gf7501.all data/gf7601.all data/gf7701.all data/gf8001.all data/gf8201.all data/gf9001.all data/gf9201.all data/gfall.ref

