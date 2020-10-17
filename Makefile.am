etcdir=$(srcdir)/etc
bin_PROGRAMS = enginedatalogger
bin_SCRIPTS = shutdowncheck.py
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

myconfdir=$(sysconfdir)/enginedatalogger
dist_myconf_DATA = \
		    $(etcdir)/enginedatalogger.conf

$(bin_SCRIPTS):
	
$(dist_myconf_DATA):
	
AM_DISTCHECK_CONFIGURE_FLAGS = \
  --with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)
if HAVE_SYSTEMD
dist_systemdsystemunit_DATA = \
			 $(etcdir)/enginedatalogger.service \
			 $(etcdir)/enginedatalogger.path \
			 $(etcdir)/shutdowncheck.service
install-data-hook:
	sudo systemctl enable enginedatalogger.service || true
	sudo systemctl enable enginedatalogger.path || true
	sudo systemctl enable shutdowncheck.service || true

edit = sed \
             -e 's|@datadir[@]|$(pkgdatadir)|g' \
             -e 's|@myconfdir[@]|$(myconfdir)|g' \
             -e 's|@bindir[@]|$(bindir)|g' \
             -e 's|@runstatedir[@]|$(runstatedir)|g' \
             -e 's|@prefix[@]|$(prefix)|g'

$(dist_systemdsystemunit_DATA): Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	chmod og-w $@.tmp
	mv $@.tmp $@
endif


