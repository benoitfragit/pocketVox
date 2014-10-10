
**POCKETVOX**
=============

![enter image description here](https://lh6.googleusercontent.com/-lAbMX8K--nU/VBlPcvFbZRI/AAAAAAAAB1Q/gUlz82rmoYk/s256-no/icons.png)

Pocketvox is a library written in C by [Benoit Franquet](https://plus.google.com/117186375453277453598/posts). It uses [Pocketsphinx](http://cmusphinx.sourceforge.net/wiki/tutorialpocketsphinx) to do voice recognition. The voice recognition is done offline and doesn't nee any Internet connexion.  It's aim is to provide an efficient way to integrate voice recognition on the Linux desktop. More particularly, its main goal is to give to visual impaired (as I am) a powerfull tool to control their desktop. 

The philosophy of pocketvox is to make a powerfull and collaborative voice recognition library. In order to do this, pocketvox will dispose of a Python interface in order to make everyone able to develop modules
It give two main way to develop modules:

* In C object oriented (GObject, GLib) using autotools
* In Python by heriting the Module class from the Pocketvox package

A first basic C example of how building a little application is disponible [here](https://github.com/benoitfragit/pocketVox/blob/master/src/main.c)

**HOW IT WORKS ?**
=================

STRUCTURE
--------------

Let me explain how it works !

So the main object of Pocketvox is a **PocketvoxApplication** that encapsulate everything.
This object first read a **PocketvoxProfile** that make the user able to pass some informations to the application

    [profile]
    name=Benoit
    voice=fr
    dict=/home/benoit/Projet/voicerecognition/frenchWords62K.dic
    acoustic=/home/benoit/Projet/voicerecognition/lium_french_f0
    lm=/home/benoit/Projet/voicerecognition/lm/dictionnary.lm.dmp
    [applications]
    Thunar=/home/benoit/Bureau/MyModule/MyModule.dic

The profile could be registered in any text file but should keep the same structure.

The section "applications" let you start some specific modules those module will be activated when the given window is activated. For example here commands in the dictionnary MyModule.dic will  be executed if Thunar is activated. This structure let you separate commands for differents applications.


Once the PocketvoxProfile has been readen, some objects are started:

 **1. PocketvoxNotifier**
 To manage sonore and visual notifications
  
 **2. PocketvoxIndicator**
 To start/stop the recognition and to manage settings like Pocketsphinx's settings or Espeak's voice
 
 **3. PocketvoxRecognizer**
 That make continuous recording and can be started/stopped with the applet

 **4. PocketvoxController**
The controller is a manager that connect all elements (design to do something very similar of the MVC design pattern). So the controller receives signal from the PocketvoxIndicator and start actions on the PocketvoxRecognizer. It manages also signals received from the PocketvoxRecognizer and find the module that contains the nearest sentence with the one that the user say.

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
 
    git clone https://github.com/benoitfragit/pocketVox
  
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


ANNEXE: WRITE DICTIONNARIES AND CREATE CUSTOM LANGUAGE MODELS
===========================


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
