AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I${srcdir}/include ${opacity_profile_CFLAGS} ${es_CFLAGS} ${linerout_CFLAGS} ${kurucz_data_CFLAGS} ${opacity_project_CFLAGS}  ${xmath_CFLAGS}  ${xio_CFLAGS}  ${xastro_CFLAGS}  ${xstdlib_CFLAGS}  ${xflash_CFLAGS} ${hdf5_CFLAGS} ${radiation_CFLAGS} ${velocity_field_CFLAGS} ${msdb_CFLAGS}  

bin_PROGRAMS = statepop
lib_LTLIBRARIES = libsp.la
include_HEADERS = include/statepop.h


libsp_la_SOURCES = src/state_pop_lib_cursor.cpp src/state_pop_lib_ang_mom.cpp src/state_pop_lib_get_def_cfg.cpp src/state_pop_lib_prt_cfg_vect.cpp src/state_pop_lib_read_K_state.cpp src/state_pop_lib_read_OP_state.cpp src/state_pop_lib_ion_rec_eqv.cpp src/state_pop_Find_Equivalent_Level.cpp src/state_pop_Find_Equivalent_Recombined_Levels.cpp src/state_pop_Get_Boltzmann_Populations.cpp src/state_pop_Get_Populations.cpp src/state_pop_Get_Saha_Populations.cpp src/state_pop_Set_Param.cpp src/state_pop_Get_Relative_Line_Strengths.cpp src/state_pop_Generate_Matrix.cpp src/state_pop_lib_Get_Relaxational_Populations.cpp
libsp_la_LDFLAGS = -version-info 5:0:1


statepop_SOURCES = src/state_pops.cpp
statepop_LDADD = libsp.la ${opacity_profile_LIBS} ${es_LIBS}  ${linerout_LIBS} ${kurucz_data_LIBS} ${opacity_project_LIBS}  ${xmath_LIBS}  ${xio_LIBS}  ${xastro_LIBS}  ${xstdlib_LIBS}  ${xflash_LIBS} ${radiation_LIBS} ${velocity_field_LIBS} ${msdb_LIBS}
statepop_CXXFLAGS =  ${opacity_profile_CFLAGS} ${es_CFLAGS} ${linerout_CFLAGS} ${kurucz_data_CFLAGS} ${opacity_project_CFLAGS}  ${xmath_CFLAGS}  ${xio_CFLAGS}  ${xastro_CFLAGS}  ${xstdlib_CFLAGS}  ${xflash_CFLAGS} ${hdf5_CFLAGS} ${radiation_CFLAGS} ${velocity_field_CFLAGS} ${msdb_CFLAGS}  

pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc



