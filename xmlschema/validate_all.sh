#!/bin/sh

for file in ../extensions/*/*.xml ../MainApp/presets/*.xml; do
    make validate FILE=$file
done
