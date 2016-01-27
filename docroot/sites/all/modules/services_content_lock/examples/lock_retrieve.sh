#!/bin/bash
# NOTE! Edit set_cookie.sh first with an active drupal session cookie key=value pair.
#       to set the value of $COOKIE
source ./set_cookie.sh
if [ -z "$COOKIE" ]
then
  echo "ERROR: COOKIE not set. Edit set_cookie.sh first with an active drupal session cookie key=value pair."
  exit
fi

# Drupal hostname where services is running
DRUPAL_HOST=localhost 

# ENDPOINT corresponds to the path of the Services endpoint, that has 
# the content_lock resource enabled that you configured under: 
#   admin/structure/services
# You can also import this endpoint by clicking the "Import" link at the page
# above and then copy and pasting the contents of the this file:  
#   services_content_lock/examples/endpoint_import.txt
ENDPOINT=content_lock_api

# NID you want to get retrieve content_lock info from
NID=1

curl -vkL -X GET -b $COOKIE -H "Accept: application/xml" "http://$DRUPAL_HOST/?q=$ENDPOINT/content_lock/$NID"
