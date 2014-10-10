#!/bin/bash
raw=$1
folder=$2

if [ ! -f  "$raw" ];
then
  echo "You should pass a raw file, that contains key/value"

  exit -1
fi

name=(${raw//./ })
vocab="${name[0]}.vocab"
idngram="${name[0]}.idngram"
arpa="${name[0]}.arpa"


if [ ! -d "$folder" ];
then
  folder="."
fi

lm="$folder/${name[0]}.lm.dmp"

text2wfreq < $raw | wfreq2vocab > $vocab
text2idngram -vocab $vocab -idngram $idngram < $raw
idngram2lm -vocab_type 0 -idngram $idngram -vocab $vocab -arpa $arpa
sphinx_lm_convert -i $arpa -o $lm

echo "lm file has been written in $lm"

exit 0
