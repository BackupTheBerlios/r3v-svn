SUBDIRS =  libroam

moc_%.cpp: %.h
	$(QT_MOC_PATH)/moc $< -o $@ 

clean-generic:
	rm -f moc_glwidget.cpp

INCLUDES= $(all_includes) $(QT_INCLUDES)
bin_PROGRAMS = r3v
r3v_SOURCES =  glwidget.cpp moc_glwidget.cpp main.cpp
r3v_LDADD = libroam/libroam.la $(QT_LIB_LA)
