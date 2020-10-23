etcdir=$(srcdir)/etc
bin_PROGRAMS = enginedatalogger
bin_SCRIPTS = bin/shutdowncheck.py
enginedatalogger_SOURCES= \
		    src/main.cpp \
		    src/bluetooth.cpp \
		    src/bluetooth.h \
		    src/serial.cpp \
		    src/serial.h \
		    src/error_handling.cpp \
		    src/error_handling.h \
		    src/front_controls.cpp \
		    src/front_controls.h \
		    src/cmdline.c \
		    src/cmdline.h \
		    src/cb1100f-app_generated.h \
		    src/definitions.h

if I2C
	enginedatalogger_SOURCES += \
	src/I2Cdev.cpp \
	src/I2Cdev.h
endif

myconfdir=$(sysconfdir)/enginedatalogger
dist_myconf_DATA = \
		    $(etcdir)/enginedatalogger.conf

$(bin_SCRIPTS):
	
$(dist_myconf_DATA):
	
edit = sed \
             -e 's|@datadir[@]|$(pkgdatadir)|g' \
             -e 's|@myconfdir[@]|$(myconfdir)|g' \
             -e 's|@bindir[@]|$(bindir)|g' \
             -e 's|@runstatedir[@]|$(runstatedir)|g' \
             -e 's|@prefix[@]|$(prefix)|g'

AM_DISTCHECK_CONFIGURE_FLAGS = \
  --with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)
if HAVE_SYSTEMD
dist_systemdsystemunit_DATA = \
			 $(etcdir)/enginedatalogger.service \
			 $(etcdir)/enginedatalogger.path \
			 $(etcdir)/shutdowncheck.service

$(etcdir)/enginedatalogger.service: $(etcdir)/enginedatalogger.service.in
$(etcdir)/enginedatalogger.path: $(etcdir)/enginedatalogger.path.in
$(etcdir)/shutdowncheck.service: $(etcdir)/shutdowncheck.service.in

install-data-hook:
	sudo systemctl enable enginedatalogger.service || true
	sudo systemctl enable enginedatalogger.path || true
	sudo systemctl enable shutdowncheck.service || true

$(dist_systemdsystemunit_DATA): Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	chmod og-w $@.tmp
	mv $@.tmp $@

MOSTLYCLEANFILES = $(dist_systemdsystemunit_DATA)

endif

$(dist_myconf_DATA): Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	chmod og-w $@.tmp
	mv $@.tmp $@

$(etcdir)/enginedatalogger.conf: $(etcdir)/enginedatalogger.conf.in
