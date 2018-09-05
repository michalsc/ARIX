#!/bin/sh

HEAD=`git rev-parse --short HEAD`
TAG=`git describe --tag --abbrev=4 2>/dev/null`
if [[ -n $(git status --porcelain) ]]; then DIRTY=1; fi
BUILD_DATE=`date +"%d.%m.%Y"`
TAG="v60.1"

if [[ $TAG ]]; then
  MAJOR=`echo $TAG | awk -F\. '{sub(/v/,""); printf("%s", $1)}'`
  MINOR=`echo $TAG | awk -F\. '{split($2,b,"-"); printf("%s", b[1]);}'`
  EXTRAREV=`echo $TAG | awk -F\. '{if (split($2,b,"-") > 1) printf(" +%s", b[2]); else printf("");}'`
else
  MAJOR=0
  MINOR=0
  EXTRAREV=""
fi
if [[ $DIRTY ]]; then EXTRAREV="$EXTRAREV dirty"; fi

echo "-DVERSION_STRING='\"$MAJOR.$MINOR ($BUILD_DATE) git: $HEAD$EXTRAREV\"'"
