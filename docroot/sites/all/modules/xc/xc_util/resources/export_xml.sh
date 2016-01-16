#!/bin/bash
####################################################################
# Exports a MySQL table as xml file
####################################################################

DB=$1
USER=$2
PW=$3
TABLE=$4
sh xml.sh $DB $USER $PW $TABLE > $TABLE.xml
