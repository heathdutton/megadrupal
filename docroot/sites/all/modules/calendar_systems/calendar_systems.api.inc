<?php
/**
 * Internal helper to load calendar system's required dependencies.
 */
function _calendar_systems_load_dependencies() {
  // This static variable will not need to be reset during
  // the page request, so we're not using drupal_static() here.
  static $loaded = FALSE;

  if (!$loaded) {
    if (!module_exists('cml')) {
      module_load_include('class.inc.php', 'calendar_systems', 'calendar/lib/exception');
      module_load_include('class.inc.php', 'calendar_systems', 'calendar/lib/classesCore');
    }
    if (!class_exists('cmfcCalendar')) {
      module_load_include('class.inc.php', 'calendar_systems', 'calendar/calendar');
    }
    $loaded = TRUE;
  }
}

/**
 * Internal helper which defines all available calendars manually.
 *
 * @return
 *   An array defined calendars.
 *
 * @todo
 *   Define a pluggable API, so other modules can hook the hell in.
 */
function _calendar_systems_plugins() {
  static $result;
  if (!$result) {
    $result = array(
	  'default' => array(
        'title' => t('Default'),
        'installed' => TRUE,
        'installed version' => 2,
      )
    );
    $calendar_systems = calendar_systems_get_calendar_instance();
	$plugins = $calendar_systems->getPlugins();

    foreach ($plugins as $plugin_name => $plugin_info) {
      $result[$plugin_name] = $plugin_info;
      $result[$plugin_name]['title'] = t(ucfirst($plugin_name));
      $result[$plugin_name]['installed'] = TRUE;
      $result[$plugin_name]['installed version'] = 2;
    }
  }
  return $result;
}

/**
 * Internal helper to get all active languages.
 *
 * @return
 *   An array of active languages.
 */
function _calendar_systems_langauges() {
  // Default:
  $languages['default'] = array(
    'name' => 'Default'
  );

  // Check if there's any additional locale available:
  $locales = function_exists('locale_language_list') ?
    locale_language_list('name', TRUE) : NULL;

  // If found, append 'em:
  if (!is_null($locales)) {
    foreach ($locales as $name => $title) {
      $languages[$name] = array(
        'name' => $title,
      );
    }
  }

  return $languages;
}

/**
 * Internal helper to return site's current language.
 *
 * @return
 *   Current language.
 */
function _calendar_systems_languages_active() {
  global $language;
  // Better?
  return ($language && $language->language) ? $language->language : 'default';
}

/**
 * Internal static cache helper to get all available profiles.
 *
 * @return
 *   An array of available profiles.
 */
function _calendar_systems_profiles() {
  $cache = &drupal_static(__FUNCTION__);

  if (!isset($cache)) {
    $cache = array();
    $profiles = db_query('SELECT language, calendar_system, settings FROM {calendar_systems}');
    // Build an array of profiles keyed by the identifiers:
    foreach ($profiles as $profile) {
      $profile->settings = unserialize($profile->settings);
      $cache[$profile->language] = $profile;
    }
  }

  return $cache;
}


/**
 * Internal static cache helper to get all available profiles for using as form select element options.
 *
 * @return
 *   An array of available profiles.
 */
function _calendar_systems_profiles_simple($default_options = FALSE) {
  $cache = &drupal_static(__FUNCTION__);

  if (!isset($cache)) {
    $calendar_systems = _calendar_systems_plugins();
    $cache = array();
    if ($default_options !== FALSE) {
      $cache = $default_options;
    }
    // Build available calendar systems array:
    foreach ($calendar_systems as $id => $calendar_system) {
      $cache[$id] = $calendar_system['title'];
    }
  }

  return $cache;
}

/**
 * Helper to determine the active calendar system
 */
function calendar_systems_get_calendar_system_name($language = NULL) {
  // Load all available profiles:
  $profiles = _calendar_systems_profiles();
  // Get site's current language:
  if (is_null($language)) {
    $language = _calendar_systems_languages_active();
  }

  if (isset($profiles[$language])) {
    $calendar_system = $profiles[$language]->calendar_system;
  }
  elseif (isset($profiles['default'])) {
    $calendar_system = $profiles['default']->calendar_system;
  }

  if (empty($calendar_system))  {
    $calendar_system = 'default';
  }

  return $calendar_system;
}

/**
 * Gets an instance of the calendar object for
 * the selected calendar system acording to the settings
 * or the passed arguments
 *
 * @return
 *   Calendar object or default on fail.
 *
 */
function calendar_systems_get_calendar_instance($calendar_system = NULL, $language = NULL) {
  // Load module dependencies:
  _calendar_systems_load_dependencies();
  if (is_null($calendar_system)) {
    $calendar_system = calendar_systems_get_calendar_system_name($language);
  }
  if ($calendar_system == 'default') $calendar_system = 'gregorian';
  $calendar = cmfcCalendar::factory('v1', array(
    'name' => $calendar_system,
  ));

  return $calendar;
}