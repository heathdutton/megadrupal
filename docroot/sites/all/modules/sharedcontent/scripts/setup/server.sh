#!/bin/sh
# Set up script for the server.

# Create user and role with permissions to access SC services.
drush ucrt sc-client-user --password="sc-client-user"
drush ev "
  \$role = (object) array('name' => 'sc-client-role');
  user_role_save(\$role);
  \$account = user_load_by_name('sc-client-user');
  user_role_grant_permissions(\$role->rid, array('access endpoint restricted'));"
drush urol sc-client-role sc-client-user

# Install server modules
drush en sharedcontent_server_feature -y
drush en ${SITE}_sharedcontent_server_rules -y
drush cc all

# Configure the SC overlay
drush en stark -y
drush vset --format=json theme_stark_settings '{"toggle_slogan":0,"toggle_node_user_picture":0,"toggle_comment_user_picture":0,"toggle_comment_user_verification":0,"toggle_main_menu":0,"toggle_secondary_menu":0}'
drush ev "db_update('block')->fields(array('status' => 0))->condition('theme', 'stark')->condition('delta', array('help','main'), 'NOT IN')->execute();"
drush vset sharedcontent_overlay_theme stark
