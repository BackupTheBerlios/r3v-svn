SUBDIRS = libroam translations

moc_%.cpp: %.h
	$(QT_BIN_PATH)/moc $< -o $@ 

clean-generic:
	rm -f moc_glwidget.cpp

INCLUDES= $(all_includes) $(QT_INCLUDES)
bin_PROGRAMS = r3v
r3v_SOURCES =  glwidget.cpp moc_glwidget.cpp main.cpp
r3v_LDADD = libroam/libroam.la $(QT_LIB_LA)

messages: main.cpp glwidget.cpp
	for i in translations/*.ts; do $(QT_BIN_PATH)/lupdate main.cpp glwidget.cpp -ts $$i; done
