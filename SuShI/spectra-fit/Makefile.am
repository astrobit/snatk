AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${opacity_profile_CFLAGS} ${jsoncpp_CFLAGS} ${es_CFLAGS} ${epsplot_CFLAGS} ${osc_CFLAGS} ${linerout_CFLAGS} ${xml2_CFLAGS}  ${xmath_CFLAGS} ${xastro_CFLAGS} ${xio_CFLAGS} ${xstdlib_CFLAGS} ${msdb_CFLAGS} ${radiation_CFLAGS} ${velocity_field_CFLAGS} ${opacity_profile_CFLAGS} 

lib_LTLIBRARIES = libsf.la
bin_PROGRAMS = sf

libsf_la_SOURCES = src/sf_bracket_temp.cpp src/sf_calc_observables.cpp src/sf_continuum_fit.cpp src/sf_deredden.cpp src/sf_fit_function.cpp src/sf_fit_results.cpp src/sf_generate_fit.cpp src/sf_get_fit.cpp src/sf_get_norm_flux.cpp src/sf_inc_index.cpp src/sf_load_data.cpp src/sf_load_data_files.cpp src/sf_load_models.cpp src/sf_output_results.cpp src/sf_parse_xml.cpp src/sf_perform_fits.cpp src/sf_pew_fit.cpp src/sf_unredshift.cpp src/sf_validate_json_data.cpp src/sf_write_fit.cpp src/sf_write_target_fit.cpp
libsf_la_LDFLAGS = -version-info 0:0:0


# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

sf_SOURCES = src/sf.cpp
sf_LDADD = libsf.la ${es_LIBS} ${epsplot_LIBS} ${osc_LIBS} ${linerout_LIBS} ${opacity_profile_LIBS} ${xml2_LIBS}  ${xmath_LIBS} ${xastro_LIBS} ${xio_LIBS} ${xstdlib_LIBS} ${msdb_LIBS} ${jsoncpp_LIBS}  

pkgdata_DATA = data/specfit.dtd data/specfit_example.xml
