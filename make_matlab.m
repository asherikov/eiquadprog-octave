% ------------------------------------------------------------
% compilation settings
% ------------------------------------------------------------
CXX           = 'g++';


% ------------------------------------------------------------
% compilation 
% ------------------------------------------------------------

cc = ['mex -v -I/usr/local/include/eigen3/', ' CXX=', CXX, ' eiquadprog_interface.cpp'];

disp(cc);
eval(cc);

%%%EOF