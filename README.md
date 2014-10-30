
**POCKETVOX**
=============

![enter image description here](https://lh6.googleusercontent.com/-lAbMX8K--nU/VBlPcvFbZRI/AAAAAAAAB1Q/gUlz82rmoYk/s256-no/icons.png)

Pocketvox is a both an application and a  library written in C by [Benoit Franquet](https://plus.google.com/117186375453277453598/posts). It uses [Pocketsphinx](http://cmusphinx.sourceforge.net/wiki/tutorialpocketsphinx) to do voice recognition. The voice recognition is done offline and doesn't need an Internet connexion.  It's aim is to provide an eficient way to integrate voice recognition on the Linux desktop. More particularly, its main goal is to give to visual impaired (as I am) a powerfull tool to control their desktop. 

The philosophy of pocketvox is to make a powerfull and collaborative voice recognition application/library. In order to do this, pocketvox will dispose of a Python interface in order to make everyone able to develop modules
It give two main way to develop modules:

* In C object oriented (GObject, GLib) using autotools
* In Python by heriting the Module class from the Pocketvox package

For basics users, it gives a very easy way to use voice recognition on their computer. I've also developed a very common way to configure Pocketvox.

**HOW IT WORKS ?**
=================

STRUCTURE
--------------

The project gives first an application/library to enable voice controlled commands on your desktop and to develop custom Python modules. Pocketvox also comes with a GUI that will let the user manage configuration and store it usings GSettings. Both are accessible from the menu.

![Pocketvox in the desktop](http://pix.toile-libre.org/?img=1414695396.jpg)

**CONFIGURE POCKETVOX**
================
The first thing to do once you have installed pocketvox is to launch pocketvox-gtk to configure pocketvox.

![User config](http://pix.toile-libre.org/?img=1414695516.jpg)

Pocketvox-gtk first lets the user configure some personal parameters, like his name, his language or the activation keyword. This word will be use by pocketvox to detect that the user is currently launching a command.

![notif config](http://pix.toile-libre.org/?img=1414695666.jpg)

Then, the user can allow or not allow visual/sound notifications.

![pocketsphinx config](http://pix.toile-libre.org/?img=1414695834.jpg)

Some of the most important parameters here let the user configure different files that have to be passed to pocketsphinx.

![gstreamer config](http://pix.toile-libre.org/upload/original/1414695996.jpg)

Here the user can choose the source of the voice between:

 - Default (Gstreamer automatically detect the good microphone to use)
 - Alsa (The user can specify a custom microphone like hw:0)
 - Network (Tcp streaming, the user can set host and port)




BUT WHAT IS A MODULE ?
------------------------------

A **PocketvoxModule** is an object that let Pocketvox able to do action on your computer. A module can be define by 2 main params:

 - An unique ID
 - A dictionnary

A **dictionnary** is basically a set of key/value, each key is a sentence, each value is an action or a value to execue an action in a switch/case structure.

open my web browser = xdg-open www.google.com
open my musics = xdg-open $HOME/Musics

When the PocketvoxRecognizer detects that the user is speaking and get a result. Then all modules is looking in its dictionnary the nearest sentence and its associated action.

SOME WORDS ON DICTIONNARY REQUEST !
--------------------------------------

In order to find the nearest sentence, I've choosen to use tf-idf indexation technique. You can find the information [here](http://stevenloria.com/finding-important-words-in-a-document-using-tf-idf/) 

DEPENDANCIES
============

First of all install sphinxbase, pocketsphinx and cmuclmtk from th CmuSphinx website. 

[sphinxbase/pocketsphinx installation from source](http://cmusphinx.sourceforge.net/wiki/tutorialpocketsphinx)

Then you can also intall cmuclmtk to generate your own language model 
[cmuclmtk  installation](http://cmusphinx.sourceforge.net/wiki/cmuclmtkdevelopment) 

Once you have downloaded and installed pocketsphinx, sphinxbase and cmuclmtk, you could also download a language model, a dictionnary and an acoustic model for you language [here](http://sourceforge.net/projects/cmusphinx/files/Acoustic%20and%20Language%20Models/)

Now we are going to install other dependancies:

    sudo apt-get update
    sudo apt-get upgrade
    sudo apt-get install libglib2.0-0 libgirepository-1.0.1 libgirepository1.0-dev libgstreamer-plugins-base0.10-0 libgstreamer-plugins-bad0.10-0 gir1.2-gstreamer-0.10 gir1.2-appindicator3-0.1 libappindicator3-1 libappindicator3-dev libespeak-dev libespeak1 libnotify-dev gir1.2-gtk-3.0 libgtk-3-0 gobject-introspection gtk-doc-tools libgstreamer-plugins-base0.10-dev libgstreamer0.10-dev libgstreamer0.10-0 python-dev python-pip bison


**BUILD THE PROJECT FROM SOURCE**
=============================

Download ar clone the project on Github..

    wget https://github.com/benoitfragit/pocketVox/archive/master.zip
 
 or 
 
    `git clone https://github.com/benoitfragit/pocketVox`

Then you have to setup the environment variable using the setup.sh script (look in the utils folder to find this script). This script will set the following variable:
 
 1. LD_LIBRARY_PATH
 2. PKG_CONFIG_PATH  
 3. GST_PLUGIN_PATH
 4. GI_TYPELIB_PATH

To load variable do the following:

    . ./setup.sh

Once you have done this, you have to build the project autotools.

    ./autogen.sh
    make

if you want to install on your system then type

    sudo make install

Note, the first launch you should generate the documentation using

    ./autogen.sh --enable-gtk-doc

instead of 

    ./autogen.sh

**DEVELOPING WITH THE LIBRARY**
=============================

Now that you have built the library you can develop for pocketvox.  If you want to develop a module the first thing to do is do write a dictionnary.

DEVELOPING NEW MODULE IN C
-------------------------------------
If you want to develop a module in C you just have to create a GObject based object if you don't know how to proceed you can take an example by opening following files: pocketvox-notifier.h and pocketvox-notifier.c

What is very powerfull with GObject is that you can herite from the PocketvoxModule type by modifying the following line in the .c file

    G_DEFINE_TYPE (PocketvoxModule, pocketvox_module, TYPE_POCKETVOX_MODULE);

Then add your methode in the pocketvox-your-module.c and add headers in pocketvox-your-module.h.

Some best practices must be respected, for example:

functions that user shouldn't access could be declared static in the .c file
Private members could be declare in the .c file in the _PocketvoxYourModulePrivate structure

If you need to implement a signal take example in the PocketvoxIndicator and in the PocketvoxController


DEVELOPING NEW MODULE IN PYTHON
---------------------------------------------

In order to be dynamic and interactive, pocketvox has been design in order to have a python interface. 

Be sure that the GI_TYPELIB_PATH point to the typelib created for pocketvox and the LD_LIBRARY_PATH point to the libpocketvox-1.0.la

the first line to create your module is the next one:

`from gi.repository import Pocketvox as pv`

then create your module by heriting from PocketvoxModule

    class MyModule(pv.Module):
        	def __init__(self, id, path, tfidf):
         		# call the parent constructor
        		pv.Module.__init__(self)
        		
        		# set the module ID (will be display in the applet)
        		# set the dictionnary path
         		pv.Module.set_property(self, "id", id)
        		pv.Module.set_property(self, "dict", path)
        	
        	# overwrite the execute method of PocketvoxModule	
        	def do_execute(self):
        		# manage actions or execute commands
        		cmd = pv.Module.get_property(self, "cmd")
        		print "Result: ", cmd
        		os.system(cmd+" &")
    

Done, you only need to add your module to a PocketvoxApplication	

To create a PocketvoxApplication you only need to type the following line
`Application = pv.Application.new("/home/benoit/Bureau/benoit.profile")`

create your new personnal module
`mod = MyModule("MyModule", "MyModule.dic", False)`

add your module
`Application.add_module(mod)`

Then you can start the application using:
`Application.start()`


ANNEXE
======

WRITE DICTIONNARIES AND CREATE CUSTOM LANGUAGE MODELS
-------

Because I want everybody to be able to use Pocketvox, I've added to the Github project somes scripts that automatize and make very easy dictionnaries creation and langage model creation.
The main purpose of those module is to make users able to create their own dictionnary for their modules. those scripts require that cmuclmtk is installed on the system.

When you know which sentence associate to your module actions then you can write a dictionnary file like this

dictionnary1.dic>

    open my documents=xdg-open $HOME/Documents
    open my musics=xdg-open $HOME/Musics
    open my pictures=xdg-open $HOME/Images

dictionnary2.dic>

    cut=xdotool key Alt+x
    copy=xdotool key Alt+c
    paste=xdotool key Alt+v

Then use the RAWBuilder.sh scripts in th utils folder to output

    ./utils/RAWBuilder.sh  dictionnary1.dic dictionnary2.dic

If you have a lot of dictionnary then you can simply give the folder containing all these dic files

    ./utils/RAWBuilder.sh $HOME/dict_folder
    
This will output the following

    <s> open my documents </s>
    <s> open my musics </s>
    <s> open my pictures </s>
    <s> copy </s>
    <s> paste </s>
    <s> cut </s>

copy this output to a file and save it. Then add the keyword that will let you start action (keyword activation), so if I choose the word listen to make Pocketvox start an action. Then my final raw file 

raw_file.txt>

    <s> open my documents </s>
    <s> open my musics </s>
    <s> open my pictures </s>
    <s> copy </s>
    <s> paste </s>
    <s> cut </s>
    <s> listen </s>

Hop, the raw file is ready then build the language module file using the LMBuilder.sh script located in the utils folder.

    ./utils/LMBuilder path_to_raw_file/raw_file.txt output_lm_dir

This script will generate a language model file in the current folder or in the output_lm_dir if it is given.

Then you can use this language model with your application.

TRANSLATE POCKETVOX IN MY LANGAGE
------
Ok pocketvox seems to be a great project but it is not avalaible in my language. How can I extand it ? is it difficult ?

Don't be afraid the way to translate pocketvox is very easy and everybody can do it.

1. Write the .po file

    The first thing you have to do is to create a .po for you language.
    
    `cd po/`
    `msginit --input=Pocketvox.pot --output=<LANGAGE_CODE>.po`
    
    
    You only need to change <LANGAGE_CODE> by your langage code for example fr for french users.
    

2. Edit the <LANGAGE_CODE>.po file
    
    Then you need to open the file and translate fill all msgstr field

    
    `msgid "Hello world"`
    `msgstr "Bonjour le monde"`
    

3. Then add your new langage to the LINGUAS file

    `echo "<LANGAGE_CODE>" >> LINGUAS`
 
Done
