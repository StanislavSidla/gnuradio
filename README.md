# gnuradio

gr_modtool newmod customeModule
cd gr_modtool
gr_modtool add calc
sync
cpp
a
bool processing = false,bool restart = false
n
n
cd gr-customeModule
cd lib
gedit ber_impl.h [copypaste]
gedit ber_impl.cc [copypaste]
cd ..
cd grc
gedit costumeModule_ber_impl.yaml [copypaste]
cd ..
mkdir build
cd build
cmake ..
sudo make install
sudo ldconfig
gnuradio-companion &
