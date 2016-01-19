<?php

/**
 * @file
 *   drush integration for GeoServer.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function geoserver_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['geoserver-configure'] = array(
    'callback' => 'drush_geoserver_configure',
    'description' => dt('Send layer and style configuration to GeoServer. Requires a user with admin privilegs.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function geoserver_drush_help($section) {
  switch ($section) {
    case 'drush:geoserver-configure':
      return dt('Send layer and style configuration to GeoServer. Requires a user with admin privilegs.');
  }
}

/**
 * Command to configure GeoServer.
 */
function drush_geoserver_configure() {

  $username = drush_prompt(dt('Username'), NULL, TRUE);
  $password = drush_prompt(dt('Password'), NULL, TRUE, TRUE);
  $geoserver_workspace = variable_get('geoserver_workspace');

  // Try to login.
  $geoserver_login = geoserver_login($username, $password);
  if ($geoserver_login !== TRUE) {
    drush_log($geoserver_login, 'error');
    return;
  }

  // Configure workspace and datastore.
  $status = geoserver_configure_workspace($geoserver_workspace);
  if ($status !== TRUE) {
    drush_log($status, 'error');
    return;
  }

  // Save layers.
  $layers = geoserver_layers_load();
  foreach ($layers as $layer) {
    $status = $layer->save();
    if ($status !== TRUE) {
      drush_log(dt('Could not save layer @name.', array('@name' => $layer->name)), 'error');
    } else {
      drush_log(dt('Saved layer @name.', array('@name' => $layer->name)), 'success');
    }
  }

  // Save styles.
  $styles = geoserver_styles();
  foreach ($styles as $style) {
    $status = geoserver_style_save($style);
    if ($status !== TRUE) {
      drush_log(dt('Could not save style @name.', array('@name' => $style->name)), 'error');
    } else {
      drush_log(dt('Saved style @name.', array('@name' => $style->name)), 'success');
    }
  }
}
