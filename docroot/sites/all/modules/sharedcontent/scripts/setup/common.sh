#!/bin/sh
# Set up script common to both server and client.

# Enable SC so we have the drush commands available.
drush en -y sharedcontent

# Change permissions so that drush script is able to write inside the site dir.
chmod -R 777 ${SITE_DIR}
# Create instance features from template.
drush scf ${SITE} ${SITE_DIR}/modules

drush cc all

# Set variables.
drush vset sharedcontent_include_local TRUE
drush vset --format=json sharedcontent_indexed_entities '{"node":{"article":"article","page":"page"}}'

# Generate testing content.
drush en devel_generate -y
drush genc 30 3 --uri=http://${SITE}.${DOMAIN}
