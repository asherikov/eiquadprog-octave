% ------------------------------------------------------------
% compilation 
% ------------------------------------------------------------

%setenv('CXXFLAGS', ' -g -pedantic -Wall -std=c++98 -O0')
%setenv('DL_LDFLAGS', ' -g -shared -Wl,-x ');
%setenv('CXXFLAGS', cstrcat(octave_config_info.CXXFLAGS, ' -pedantic -Wall -O3 -DNDEBUG '))


cc = ['mex -v -I/usr/local/include/eigen3/', ' eiquadprog_interface.cpp'];
eval(cc);

%%%EOF
