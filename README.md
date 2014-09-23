

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

The profile could be registered in any text file but should keep the same structure.

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
    sudo apt-get install libglib2.0-0 libgirepository-1.0.1 libgirepository1.0-dev libgstreamer-0.10-plugin-base gstramer-0.10-plugin-bad gir1.2-gstreamer-0.10 gstreamer-0.10-plugin-good  gir1.2-appindicator3-0.1 libappindicator3-1 libappindicator3-dev libespeak-dev libespeak1 libnotify-dev gir1.2-gtk-3.0 libgtk-3-0 gobject-introspection


