#!/bin/bash
# this script has been written by Benoit Franquet
# it provides and easy way to create a raw file to use with the cmuclmtk kit
# to create your own langage model.
# In order to create a raw file avalaible for cmuclmtk you should give
# a folder containing dictionnary files
# or a list of such a folder
# a dictionnary is simply a list of key=value like this
# open my documents=xdg-open ~/Documents
# This script will browse all your dictionnaries, get all formated line to
# create a valid raw that you can paste to a raw file
# check if there is files
if [ $@ == "" ] && [ $1 == ""  ];
then
    echo "No file given !"

    exit -1
fi

# list all files
if [ -d "$1" ];
then
    current_folder="$1"

    for file in $(ls $current_folder);
    do
        if [ -f "$current_folder/$file" ];
        then
              FILES_LIST+="$current_folder/$file "
        fi
    done
else
    #files are passed by command line args
    FILES_LIST=$@
fi

if [ "$FILES_LIST" == "" ] || [ "$FILES_LIST" == " " ] ;
then
    echo "No file given"
    exit -1
fi

FILES=(${FILES_LIST// / })

for file in ${FILES[*]};
do
    while read line;
    do
        key=`echo $line | cut -f1 -d"="`
        value=$(echo $line | cut -f2 -d "=")

        if [ "$key" != " " ] && [ "$key" != "" ] && [ "$key" != "$value" ];
        then
            echo "<s> $key </s>"
        fi
    done < $file
done


exit 0
