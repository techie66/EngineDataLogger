etcdir=$(srcdir)/etc
bin_PROGRAMS = enginedatalogger
bin_SCRIPTS = bin/shutdowncheck.py
enginedatalogger_SOURCES= \
		    src/main.cpp \
		    src/hexconvert.cpp \
		    src/serial.cpp \
		    src/serial.h \
		    src/can.cpp \
		    src/can.h \
		    src/error_handling.cpp \
		    src/error_handling.h \
		    src/cmdline.c \
		    src/cmdline.h \
		    src/definitions.h

if DASHBOARD
enginedatalogger_SOURCES += \
		    src/bluetooth.cpp \
		    src/cb1100f-app_generated.h \
		    src/bluetooth.h
endif

if FRONTCONTROLS
enginedatalogger_SOURCES += \
		    src/front_controls.cpp \
		    src/front_controls.h
endif

myconfdir=$(sysconfdir)/enginedatalogger
dist_myconf_DATA = \
		    $(etcdir)/enginedatalogger.conf.dist

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
			 $(etcdir)/shutdowncheck.service

$(etcdir)/enginedatalogger.service: $(etcdir)/enginedatalogger.service.in
$(etcdir)/shutdowncheck.service: $(etcdir)/shutdowncheck.service.in

install-data-hook:
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

$(etcdir)/enginedatalogger.conf.dist: $(etcdir)/enginedatalogger.conf.dist.in
