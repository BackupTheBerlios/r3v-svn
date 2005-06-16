SUBDIRS = libroam translations

moc_%.cpp: %.h
	$(QT_BIN_PATH)/moc $< -o $@ 

clean-generic:
	rm -f moc_glwidget.cpp

INCLUDES= $(all_includes) $(QT_INCLUDES)
bin_PROGRAMS = r3v
r3v_SOURCES =  glwidget.cpp main.cpp glwidget.h
r3v_LDADD = libroam/libroam.la $(QT_LIB_LA)
nodist_r3v_SOURCES = moc_glwidget.cpp

messages: main.cpp glwidget.cpp
	for i in translations/*.ts; do $(QT_BIN_PATH)/lupdate main.cpp glwidget.cpp -ts $$i; done
