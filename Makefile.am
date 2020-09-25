etcdir=$(srcdir)/etc
bin_PROGRAMS = enginedatalogger
enginedatalogger_SOURCES= \
		    src/main.cpp \
		    src/bluetooth.cpp \
		    src/serial.cpp \
		    src/I2Cdev.cpp \
		    src/error_handling.cpp \
		    src/front_controls.cpp \
		    src/cmdline.c \
		    src/I2Cdev.h \
		    src/bluetooth.h \
		    src/serial.h \
		    src/error_handling.h \
		    src/front_controls.h \
		    src/cb1100f-app_generated.h \
		    src/definitions.h \
		    src/cmdline.h
AM_DISTCHECK_CONFIGURE_FLAGS = \
  --with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)
if HAVE_SYSTEMD
dist_systemdsystemunit_DATA = \
			 $(etcdir)/enginedatalogger.service
install-data-hook:
	sudo systemctl enable data_logger || true
endif

myconfdir=$(sysconfdir)/enginedatalogger
dist_myconf_DATA = \
		    $(etcdir)/enginedatalogger.conf

edit = sed \
             -e 's|@datadir[@]|$(pkgdatadir)|g' \
             -e 's|@myconfdir[@]|$(myconfdir)|g' \
             -e 's|@prefix[@]|$(prefix)|g'

$(etcdir)/enginedatalogger.conf $(etcdir)/enginedatalogger.service: Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

$(etcdir)/enginedatalogger.conf: $(etcdir)/enginedatalogger.conf.in
$(etcdir)/enginedatalogger.service: $(etcdir)/enginedatalogger.service.in
	
