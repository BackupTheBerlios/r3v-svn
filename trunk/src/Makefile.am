
# set the include path found by configure
INCLUDES= $(all_includes) -I ./libroam -I $(QTDIR)/include

moc_%.cpp: %.h
	$(QTDIR)/bin/moc $< -o $@ 

# the library search path.
SUBDIRS =  libroam
bin_PROGRAMS = r3v
r3v_COMPILE_FIRST = glwidget.moc
r3v_SOURCES =  glwidget.cpp moc_glwidget.cpp main.cpp
r3v_LDADD = libroam/libroam.la -lqt-mt
