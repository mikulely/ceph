EXTRA_DIST += $(srcdir)/pybind/setup.py $(srcdir)/pybind/rbd.pyx

if ENABLE_CLIENT
if WITH_RADOS
if WITH_RBD
if WITH_CYTHON

PY_DISTUTILS = \
	VERSION_RELEASE="@PACKAGE_VERSION@" \
	CPPFLAGS="-iquote \${abs_srcdir}/include ${AM_CPPFLAGS} ${CPPFLAGS}" \
	CFLAGS="-iquote \${abs_srcdir}/include ${AM_CFLAGS} ${CFLAGS}" \
	LDFLAGS="-L\${abs_builddir}/.libs $(subst -pie,,${AM_LDFLAGS}) ${LDFLAGS}" \
	${PYTHON} ./setup.py

pybind-all: librbd.la
	cd $(srcdir)/pybind; $(PY_DISTUTILS) build \
	--build-base $(shell readlink -f $(builddir))/build \
	--verbose

pybind-clean:
	cd $(srcdir)/pybind; $(PY_DISTUTILS) clean \
	--build-base $(shell readlink -f $(builddir))/build \
	--verbose

pybind-install-exec:
	if test "$(DESTDIR)" ; then \
		if lsb_release -si | grep --quiet 'Ubuntu\|Debian\|Devuan' ; then \
			options=--install-layout=deb ; \
		else \
			options=--prefix=/usr ; \
		fi ; \
		root="--root=$(DESTDIR)" ; \
	else \
		options=--prefix=$(prefix) ; \
	fi ; \
	cd $(srcdir)/pybind; $(PY_DISTUTILS) install \
	$$options $$root \
	--single-version-externally-managed \
	--record $(DESTDIR)$(pythondir)/rbd_install_files.txt \
	--verbose

LOCAL_ALL += pybind-all
LOCAL_CLEAN += pybind-clean
LOCAL_INSTALLEXEC += pybind-install-exec

endif
endif
endif
endif
