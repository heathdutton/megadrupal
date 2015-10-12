#!/bin/bash
####################################################################
# Exports a MySQL table as CSV file
####################################################################

DB=$1
USER=$2
PW=$3
TABLE=$4
sh csv.sh $DB $USER $PW $TABLE > $TABLE.csv
