#!/bin/bash
####################################################################
# Shows a MySQL table as XML
####################################################################

DB=$1
USER=$2
PW=$3
TABLE=$4
mysql -u $USER -p$PW $DB --xml -e "select * from $TABLE" | sed 's/<field name="\([^<>]*\)">\([^<>]*\)<\/field>/<\1>\2<\/\1>/g'

