bin_PROGRAMS = enginedatalogger
bin_SCRIPTS = bin/shutdowncheck.py
enginedatalogger_SOURCES= \
		    src/main.cpp \
		    src/hexconvert.cpp \
		    src/serial.cpp \
		    src/serial.h \
		    src/error_handling.c \
		    src/error_handling.h \
		    src/cmdline.c \
		    src/cmdline.h \
				src/logfile.c \
				src/logfile.h \
		    src/definitions.h

if CANBUS
enginedatalogger_SOURCES += \
		    src/can.cpp \
		    src/can.h \
		    src/imu_can.c \
		    src/imu_can.h \
		    src/ignitech_can.c \
		    src/ignitech_can.h \
		    src/obd2.c \
		    src/obd2.h
endif

if DASHBOARD
enginedatalogger_SOURCES += \
		    src/bluetooth.cpp \
		    src/cb1100f-app_generated.h \
		    src/bluetooth.h
endif

if POWERCALC
enginedatalogger_SOURCES += \
		    src/powercalc.cpp \
		    src/powercalc.h
endif

if FRONTCONTROLS
enginedatalogger_SOURCES += \
		    src/front_controls.cpp \
		    src/front_controls.h
endif

BUILT_SOURCES = 

if DASHBOARD
BUILT_SOURCES += \
		    src/cb1100f-app_generated.h
endif

if HAVE_GENGETOPT
BUILT_SOURCES += \
				src/cmdline.h \
				src/cmdline.c
endif

datadir=$(builddir)/data

if DASHBOARD
$(builddir)/src/cb1100f-app_generated.h: $(datadir)/cb1100f-app.fbs
	flatc -c --gen-object-api -o $(srcdir)/src $(datadir)/cb1100f-app.fbs
	touch $(datadir)/cb1100f-app.fbs
endif

if HAVE_GENGETOPT
$(builddir)/src/cmdline.c: $(datadir)/cmdline.ggo
	gengetopt -C --output-dir=$(builddir)/src/ -i $(datadir)/cmdline.ggo 
endif

etcdir=$(builddir)/etc
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
	sudo systemctl enable enginedatalogger.service || true

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

doc: Doxyfile
	PROJECT_NUMBER=$(VERSION) doxygen Doxyfile

BUILT_SOURCES += $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

