bin_PROGRAMS = data_logger
data_logger_SOURCES= \
		    src/main.cpp \
		    src/bluetooth.cpp \
		    src/serial.cpp \
		    src/I2Cdev.cpp \
		    src/error_handling.cpp \
		    src/front_controls.cpp \
		    src/I2Cdev.h \
		    src/bluetooth.h \
		    src/serial.h \
		    src/error_handling.h \
		    src/front_controls.h \
		    src/cb1100f-app_generated.h \
		    src/definitions.h
AM_DISTCHECK_CONFIGURE_FLAGS = \
  --with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)
if HAVE_SYSTEMD
dist_systemdsystemunit_DATA = \
			 data/data_logger.service
install-data-hook:
	sudo systemctl enable data_logger || true
endif

myconfdir=$(sysconfdir)/data_logger
dist_myconf_DATA = \
		    etc/data_logger.conf
