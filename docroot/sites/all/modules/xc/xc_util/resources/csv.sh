#!/bin/bash
####################################################################
# Shows a MySQL table as CSV rows
####################################################################

DB=$1
USER=$2
PW=$3
TABLE=$4
mysql -u $USER -p$PW $DB -B -e "select * from $TABLE;" | sed 's/\t/";"/g;s/^/"/;s/$/"/;s/\n//g;s/"NULL"/NULL/g'

