<?php
/**
 * @file
 * Drush commands allowing languages to be added, switched, enabled, disabled,
 * imported, exported and update prefix from the commandline. This module only
 * provides drush commands, so you will see no functionality in the UI.
 */

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return array An associative array describing your command(s).
 *   An associative array describing your command(s).
 */
function language_drush_command() {
  $commands = array();

  // Language commands.
  $commands['language-add'] = array(
    'description' => "Add and import a new language definition",
    'arguments' => array(
      'langcode' => dt('The langcode of the language for which a definition will be added.'),
    ),
    'aliases' => array('langadd'),
  );
  $commands['language-enable'] = array(
    'description' => "Enable an already defined language",
    'arguments' => array(
      'langcode' => dt('The langcode of the language which will be enabled.'),
    ),
    'aliases' => array('langen'),
  );
  $commands['language-disable'] = array(
    'description' => "Disable an already defined language",
    'arguments' => array(
      'langcode' => dt('The langcode of the language which will be disabled.'),
    ),
    'aliases' => array('langdis'),
  );
  $commands['language-default'] = array(
    'description' => "Assign an enabled language as default",
    'arguments' => array(
      'langcode' => dt('The langcode of the language which will be set as the default language.'),
    ),
    'aliases' => array('langdef'),
  );
  $commands['language-prefix'] = array(
    'description' => "Alter language prefix",
    'arguments' => array(
      'langcode' => dt('The langcode of the language.'),
      'prefix' => dt('The prefix of the language. Prefix is optional, if not provide the command will remove the prefix'),
    ),
    'aliases' => array('langpref'),
  );

  return $commands;
}

/**
 * Add a language.
 */
function drush_language_add() {
  if (!function_exists('locale_add_language')) {
    drush_set_error(dt('Could not add language. Is the \'locale\' module enabled?'));
    return;
  }

  $args = func_get_args();
  if (count($args) == 0) {
    drush_set_error(dt('Please provide one or more language codes as arguments.'));
    return;
  }

  foreach ($args as $langcode) {
    if (array_key_exists($langcode, language_list())) {
      drush_log(dt('The language with code !code already exists.', array('!code' => $langcode)), 'warning');
    }
    else {
      // Predefined language selection.
      include_once DRUPAL_ROOT . '/includes/iso.inc';
      $predefined = _locale_get_predefined_list();
      if (!isset($predefined[$langcode])) {
        drush_log(dt('Invalid language code !language', array('!language' => $langcode)), 'warning');
      }
      else {
        // Add the language definition
        locale_add_language($langcode);
        // See if we have language files to import for the newly added
        // language, collect and import them.
        if ($batch = locale_batch_by_language($langcode, '_locale_batch_language_finished')) {
          batch_set($batch);

          $batch =& batch_get();
          $batch['progressive'] = FALSE;

          // Process the batch.
          drush_backend_batch_process();
        }
        drush_log(dt('Added language: !language', array('!language' => $langcode)), 'ok');
      }
    }
  }
}

/**
 * Enable a language.
 */
function drush_language_enable() {
  $args = func_get_args();
  if (count($args) == 0) {
    drush_set_error(dt('Please provide one or more language codes as arguments.'));
    return;
  }

  foreach ($args as $langcode) {
    $languages = language_list();
    if (array_key_exists($langcode, $languages)) {
      if (!$languages[$langcode]->enabled) {
        // disable the default english
        db_update('languages')
          ->condition('language', $langcode)
          ->fields(array(
            'enabled' => 1,
          ))
          ->execute();

        // Changing the language settings impacts the interface.
        cache_clear_all('*', 'cache_page', TRUE);
        drush_log(dt("Enabled language : !language ", array('!language' => $langcode)), 'ok');
      }
      else {
        drush_log(dt("Language already enabled: !language ", array('!language' => $langcode)), 'warning');
      }
    }
    else {
      drush_log(dt("Specified language does not exist !language", array('!language' => $langcode)), 'warning');
    }
  }
}

/**
 * Disables a language.
 */
function drush_language_disable() {
  $args = func_get_args();
  if (count($args) == 0) {
    drush_set_error(dt('Please provide one or more language codes as arguments.'));
    return;
  }

  foreach ($args as $langcode) {
    $languages = language_list();
    if (array_key_exists($langcode, $languages)) {
      if ($languages[$langcode]->enabled) {
        // disable the default english
        db_update('languages')
          ->condition('language', $langcode)
          ->fields(array(
            'enabled' => 0,
          ))
          ->execute();

        // Changing the language settings impacts the interface.
        cache_clear_all('*', 'cache_page', TRUE);
        drush_log(dt("Disabled language : !language ", array('!language' => $langcode)), 'ok');
      }
      else {
        drush_print(dt("Language already disabled: !language ", array('!language' => $langcode)), 'warning');
      }
    }
  }
}

/**
 * Assigns the default language.
 */
function drush_language_default() {
  $args = func_get_args();
  if (count($args) == 0) {
    drush_set_error(dt('Please provide one or more language codes as arguments.'));
    return;
  }

  foreach ($args as $langcode) {
    // get all the languages
    $languages = language_list();
    if (array_key_exists($langcode, $languages)) {
      variable_set('language_default', (object) $languages[$langcode]);
      drush_log(dt("!language assigned as default", array('!language' => $langcode)), 'ok');
    }
    else {
      drush_log(dt("Specified language does not exist !language", array('!language' => $langcode)), 'warning');
    }
  }
}

/**
 * Alter the language prefix.
 */
function drush_language_prefix() {
  $args = func_get_args();
  if (count($args) == 0) {
    drush_set_error(dt('Please provide one langcode as arguments. Prefix is optional, if not provide the command will remove the prefix'));
    return;
  }

  // Get arguments
  $langcode = array_shift($args);
  $prefix = array_shift($args);

  if (module_exists('locale')) {

    // Add some security filters
    if (!empty($langcode)) {
      $langcode = check_plain($langcode);
      $langcode = filter_xss($langcode);
    }

    if (!empty($prefix)) {
      $prefix = check_plain($prefix);
      $prefix = filter_xss($prefix);
    }

    // Alter prefix if language exist
    if (array_key_exists($langcode, language_list())) {
      db_update('languages')
        ->condition('language', $langcode)
        ->fields(array('prefix' => $prefix))
        ->execute();

      if (!empty($prefix)) {
        drush_log(dt("Prefix !prefix set for !language", array(
          '!prefix' => $prefix,
          '!language' => $langcode
        )), 'ok');
      }
      else {
        drush_log(dt("Prefix has been removed for !language", array(
          '!prefix' => $prefix,
          '!language' => $langcode
        )), 'ok');
      }
    }

    // Return error if language is missing
    else {
      drush_log(dt("Specified language does not exist !language", array('!language' => $langcode)), 'warning');
    }
  }
  // Return error if locale module is missing
  else {
    drush_log(dt('Locale module is required', 'warning'));
  }
}
