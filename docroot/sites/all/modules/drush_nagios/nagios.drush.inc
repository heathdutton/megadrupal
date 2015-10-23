<?php

include_once 'includes/nagios.inc';

/**
 * @file
 * The nagios implementation.   
 */

/**
 * @defgroup drush_hooks Drush hooks
 * Implementation of general drush hooks
 * @{  
 */

/**
 * Implementation of hook_drush_help().
 * @param
 *   A string with the help section (prepend with 'drush:')
 * @return
 *   A string with the help text for your command.
 */
function nagios_drush_help($section) {
  $help_header = dt('
Exit status

* NAGIOS_OK (0)      - if monitored Drupal site is up to date
* NAGIOS_WARNING (1) - if there is a code (core, contrib and theme) or pending database update available
* NAGIOS_CRITICAL(2) - if there is a code update with severity \'Security update\' available

OK Message

* /var/www/example.com/drupal/sites/default: Everythings OK...
');
  switch ($section) {
    case 'meta:nagios:title':
      return dt('Nagios commands');
    case 'drush:check-updates':
      $help = dt('Checks a Drupal site for code (core and contrib) updates.');
      return $help . $help_header;
      break;
    case 'drush:check-db-updates':
      $help = dt('Checks a Drupal site for pending database updates.');
      return $help . $help_header;
      break;
  }
}

/**
 * Implementation of hook_drush_command().
 * @See drush_parse_command() for a list of recognized keys.
 * @return
 *   An associative array describing your command(s).
 */
function nagios_drush_command() {
  $items = array();

  $items['check-updates'] = array(
    'callback' => 'drush_nagios_updates',
    'description' => 'For the usage as nagios plugin: Monitores a Drupal site and print a message and return with an exit status.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'examples' => array(
      'drush @example check-updates' => '/path/to/drupal/sites/default:  UPDATES: admin_menu,ckeditor,ckeditor_link,conditional_fields,date,devel,devel_themer,entity,features,field_collection,link,menu_attributes,menu_link,references,transliteration,views_datasource,webform SECURITY UPDATES: ctools,email',
      'drush @example check-updates --ignore=ctools,email' => '/path/to/drupal/sites/default:  UPDATES: admin_menu,ckeditor,ckeditor_link,conditional_fields,date,devel,devel_themer,entity,features,field_collection,link,menu_attributes,menu_link,references,transliteration,views_datasource,webform IGNORED BY SETTINGS: ctools(SECURITY UPDATE available),email(SECURITY UPDATE available)',
     ), 
    'options' => array(
      //'changelog' => 'Show release link / link to changelog for the recommended version of a affected projects.',
      //'show-versions' => 'Show used / current and recommended versions for a affected project..',
      'ignore' => array(
        'description' => 'Project or comma seperated list of projects which should be ignored.',
        'value' => 'required',
        'example-value' => 'project1,...,projectN',
      ),
      'ignore-locked' => array(
        'description' => 'Ignore projects which are locked by drush pm-update --lock.',
      ),
      /** @todo Implement recommended-version option  in @ref _nagios_print_message().*/
      /*  
      'recommended-version' => array(
        'value' => 'optional',  
        'description' => 'Show recomended version for affected projects .',
      ),
      */
      /** @todo Implement changelog option in @ref _nagios_print_message(). */
      /*
      'changelog' => array(
        'value' => 'optional',
        'description' => 'Show uri of changelog for affected projects.',
      ),
      */
    ),
  );
  $items['check-db-updates'] = array(
    'callback' => 'drush_nagios_updates',
    'description' => 'Checks for pending database updates.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'examples' => array(
      'drush @example check-db-updates' => '\'/path/to/drupal/sites/default: has pending database updates, run \'drush updatedb\' or visit update.php in your browser for more details.\' with exit status 1',
    ),
  );
  return $items;
}
/** @} */ 

/**
 * @defgroup drush_callbacks Drush callbacks
 * Implementation of general drush callbacks
 * @{  
 */

/**
 * Command callback, print status message 
 * and exit with exit status
 *
 * @return int 
 *   Exit status of the referenced functionn
 */
function drush_nagios_updates() {
  $command = drush_get_command();
  if ($command['command'] == 'check-db-updates') {
    $parsed_update_info = _nagios_get_db_update_info();
  }
  else if ($command['command'] == 'check-updates') {
    $update_info = _nagios_get_update_info();
    $parsed_update_info = _nagios_parse_update_info($update_info);
  }
  _nagios_print_message($parsed_update_info);
  drush_set_context('DRUSH_EXECUTION_COMPLETED', TRUE);
  exit(_nagios_exit_status($parsed_update_info));
}
/** @} */ 
