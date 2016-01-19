#!/bin/sh
# Set up script for the client.

# Enable and configure search API.
drush en sharedcontent_client search_api_db -y
drush sc-csdb sharedcontent

# Enable client.
drush en ${SITE}_sharedcontent_ui -y
drush en ${SITE}_sharedcontent_client_rules -y
drush cc all

# Configure linkable content.
drush sc-link add article
drush sc-link add page

# Add connections to all servers.
IFS=',' ;for server in `echo "${SC_SERVERS}"`
do
# Avoid creating connection to itself
if [ "${SITE}" != "$server" ]; then
  echo "Creating connection to http://${server}.${DOMAIN}/sharedcontent"
  drush sc-conc http://${server}.${DOMAIN}/sharedcontent sc-client-user sc-client-user "System ${server}" system_${server}
fi
done
