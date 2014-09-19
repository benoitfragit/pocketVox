GOAL
====

Pocketvox is a library written in C by Benoit Franquet. It uses Pocketsphinx to do voice recognition. Besides
it will provides an easy way for developers to create cool module.

It give two main way to develop modules:

* In C object oriented (GObject, GLib)
* In Python 

An exemple already exists for C. There no module for the moment.

DEPENDANCIES
============

Before using this librar you need to install all dependancies:

* pocketsphinx
* sphinxbase
* glib-2.0
* gstreamer-0.10
* gobject-2.0
* libnotify
* appindicator3-0.1
* espeak
* cmuclmtk

If you want to deelop for the root of pocketvox you need to install gobject-introspection-1.0

you can type first

sudo apt-get install glib-2.0 gobject-2.0 glib-2.0 gstreamer-0.10 libnotify appindicator3-0.10 espeak

Then install the lates version of pocketsphin, sphinxbase, cmuclmtk

