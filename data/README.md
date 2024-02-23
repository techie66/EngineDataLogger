#Data files
## \*.dbc
All the \*.dbc files are CAN definition files that we run through cantools to output basic processing functions.
## cb1100f-app.fbs
Data file for flatbuffers that defines the buffer protocol used in communicating with the Dashboard app
## \*.rd
These are Dashboard setup files for the "RealDash" mobile app. This is another option as a dashboard interface and has seen significant development.
Also is the 'realdash_obd2.xml' that defines specific "OBDII" PIDs for RealDash.
## Command line options
cmdline.ggo.in is the source for command line options that gets run through Autoconf and then gengetopt.

