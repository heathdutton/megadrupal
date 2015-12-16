<?php

/**
 * @file
 * Some useful functions for Location taxonomize
 */

define('LT_MODULE_ID', 'location_taxonomize');
define('LT_MODULE_NAME', 'Location taxonomize');
define('LT_VOCAB_NAME', 'location_taxonomize');
define('LT_TERM_ATTACH_FIELD', 'location_taxonomize_terms');


/**
 * Resets the module, as if it was uninstalled and installed again and the
 * vocab was emptied.
 */
function _location_taxonomize_reset() {
  location_taxonomize_empty_vocab();
  _location_taxonomize_set_defaults();
  drupal_set_message(t('Location Taxonomize has been reset'));
}

/**
 * Keeps a list of all the variables maintained by this module, with their
 * default values. These default value are applied during installation. Some
 * defaults are set elsewhere (e.g. during initialization).
 */
function _location_taxonomize_variables() {
  return array(
    'location_taxonomize_vid'   => NULL,
    'location_taxonomize_vocab' => array(
      'source'        => NULL,
      'method'        => 'existing',
      'possible_vid'  => NULL,
      'hierarchy'        => NULL,
    ),
    'location_taxonomize_settings' => array(
      'enable'  => 1,
      'term_attach' => 0,
      'hide_terms_field' => 0,
      'naming'  => array(
        'country'   => 'name',
        'province'  => 'name',
        'usa'   => 1,
      ),
      'na_text'         => 'Unknown',
      'longname_enable' => 0,
      'longname'        => array(
        'main'            => 0,
        // this is set by the initialization process
        'fields'          => NULL,
        'separator'       => ',',
        'country_naming'  => 'code',
        'province_naming' => 'code',
        'usa' => 1,
      ),
    ),
    'location_taxonomize_show_messages' => 1,
  );
}

/**
 * Empties the Location Vocabulary (called as an AJAX callback from the admin form)
 */
function location_taxonomize_empty_vocab() {
  $terms = taxonomy_get_tree(variable_get('location_taxonomize_vid'));
  foreach ($terms as $term) {
    taxonomy_term_delete($term->tid);
  }
  $termsnow = taxonomy_get_tree(variable_get('location_taxonomize_vid'));
  if (!$termsnow) return t('Location vocabulary emptied.');
  return t('There was a problem emptying the Location Vocabulary.');
}

/**
 * Returns the hierarchy configured for the current Location Vocabulary
 * @param $assoc - whether to return an associative array (TRUE) or
 *                keyed array (FALSE)
 * @param $labels - if TRUE and $assoc is TRUE, returns the labels
 *                  of the fields as the array values 
 */
function _location_taxonomize_get_hierarchy($assoc = FALSE, $labels = FALSE) {
  $settings = variable_get('location_taxonomize_vocab');
  $fields = $settings['hierarchy'];
  if ($labels) $withlabels = _location_taxonomize_get_fields(TRUE, TRUE);
  $hierarchy = array();
  foreach ($fields as $key => $value) {
    if ($value) {
      if ($assoc) {
        if ($labels) $hierarchy[$key] = $withlabels[$key];
        else $hierarchy[$key] = $key;
      }
      else $hierarchy[] = $key;
    }
  }
  return $hierarchy;
}

/**
 * Initializes the Long Name fields variable (called during initialization)
 */
function _location_taxonomize_init_var_longname() {
  $hierarchy = _location_taxonomize_get_hierarchy();
  $defaults = array();
  foreach ($hierarchy as $field) {
    if (location_taxonomize_convert_field_name($field) != 'country' &&
        location_taxonomize_convert_field_name($field) != 'province' &&
        location_taxonomize_convert_field_name($field) != 'city') {
          $defaults[$field] = 0;
    }
    else $defaults[$field] = $field;
  }
  $settings = variable_get('location_taxonomize_settings');
  $settings['longname']['fields'] = $defaults;
  variable_set('location_taxonomize_settings', $settings);
}

/**
 * Returns an array indicating which of the possible Source modules are installed
 */
function location_taxonomize_get_sources() {
  // Get source modules
  $possibilities = module_invoke_all('location_taxonomize_source');
  
  $available = array();
  foreach ($possibilities as $module => $name) {
    if (module_exists($module)) {
      $available[$module] = $name;
    }
  }
  return $available;
}

/**
 * Disassociates the vocabulary - unlinks the current Location Vocabulary
 * from the module
 */
function location_taxonomize_disassociate($form, $form_state) {
  // unset the vid variable
  variable_del('location_taxonomize_vid');
  drupal_set_message(t('Location Taxonomy successfully disassociated'));
}

/**
 * Sets all this module's variables to their default values
 */
function _location_taxonomize_set_defaults() {
  $defaults = _location_taxonomize_variables();
  foreach ($defaults as $key => $value) {
    if ($value) variable_set($key, $value);
    else variable_del($key);
  }
}

/**
 * Deletes all variables set by this module
 */
function _location_taxonomize_del_variables() {
  $vars = _location_taxonomize_variables();
  foreach ($vars as $key => $value) {
    variable_del($key);
  }
  return t('All variables deleted');
}

/**
 * Returns the possible fields for the hierarchy
 * if $assoc, return value is in form key => key
 * if $labels, return value is in form key => value with field labels as values
 * if $source is set, gets the fields for that source (defaults to current source)
 */
function _location_taxonomize_get_fields($assoc = TRUE, $labels = FALSE, $source = NULL) {
  if ($source == NULL) {
    $source = _location_taxonomize_get_source();
  }
  $suffix = '_get_fields';
  $function = $source . $suffix;
  return $function($assoc, $labels);
}

/**
 * Checks the given form to make sure that the term attach field exists and is 
 * set up correctly
 */
function location_taxonomize_term_attach_check_field($form) {
  $field_name = 'field_' . LT_TERM_ATTACH_FIELD;
  // make sure that a field by this name exists
  if (!isset($form[$field_name])) return FALSE;
  // make sure that this field has the right settings
  $field_info = field_info_field($field_name);
  if ($field_info['deleted'] == 1) return FALSE;
  if ($field_info['type'] != 'taxonomy_term_reference') return FALSE;
  if ($field_info['settings']['allowed_values'][0]['vocabulary'] != LT_MODULE_ID) return FALSE;
  return $field_name;
}

/**
 * Returns the name of the current source module
 */
function _location_taxonomize_get_source() {
  $settings = variable_get('location_taxonomize_vocab');
  return $settings['source'];
}

/**
 * Returns the settings array for the current source module, or a given source
 */
function _location_taxonomize_get_source_settings($source = NULL) {
  if ($source == NULL) {
    $source = _location_taxonomize_get_source();
  }
  $suffix = '_settings';
  return variable_get($source . $suffix);
}

/**
 * Sets the default source to $module if the default source is not already set
 * or if the default source is set to a module that is no longer enabled
 * Returns TRUE if set, FALSE if not set
 */
function location_taxonomize_source_default_set($module) {
  $settings = variable_get('location_taxonomize_vocab');
  if(empty($settings['source']) || !module_exists($settings['source'])) {
    $settings['source'] = $module;
    variable_set('location_taxonomize_vocab', $settings);
    return TRUE;
  }
  return FALSE;
}

/**
 * Some fields are defined as 'Primary Fields'. 
 * This function returns an array mapping internal field names
 * to source field names, or the other direction, if $reverse is TRUE
 */
function location_taxonomize_get_primary_fields($reverse = FALSE) {
  $source = _location_taxonomize_get_source();
  $suffix = '_primary_field_names';
  $function = $source . $suffix;
  $source_fields = $function();
  $internal_fields = array(
    'country',
    'country_name',
    'province',
    'province_name',
    'city',
  );
  if(!$reverse) {
    return array_combine($internal_fields, $source_fields);
  }
  else {
    return array_combine($source_fields, $internal_fields);
  }
}

/**
 * Converts a field name from the current source to an internal primary field
 * name if necessary. Otherwise returns the given name.
 */
function location_taxonomize_convert_field_name($name) {
  $primary_fields = location_taxonomize_get_primary_fields();
  foreach ($primary_fields as $key => $value) {
    if ($value == $name) {
      $name = $key;
      break;
    }
  }
  return $name;
}

/** 
 * Converts an internal name to a source name
 */
function location_taxonomize_source_field_name($internal_name) {
  $primary_fields = location_taxonomize_get_primary_fields();
  if (isset($primary_fields[$internal_name])) return $primary_fields[$internal_name];
  return $internal_name;
}

/**
 * Returns whether the option to enable this module is set
 */
function location_taxonomize_enabled() {
  $settings = variable_get('location_taxonomize_settings');
  $vid = variable_get('location_taxonomize_vid');
  $enabled = $settings['enable'] && isset($vid);
  return $enabled;
}

/**
 * Returns whether the term attach option is enabled
 */
function location_taxonomize_term_attach_enabled() {
  $settings = variable_get('location_taxonomize_settings');
  $enabled = $settings['term_attach'];
  return $enabled;
}

/**
 * Returns the main settings array for this module
 */
function location_taxonomize_get_settings() {
  $settings = variable_get('location_taxonomize_settings');
  return $settings;
}

/**
 * Uses the core countries list to return a country code given a name
 */
function location_taxonomize_get_country_code($name) {
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();
  $names = array_keys($countries, $name);
  if (!empty($names)) return array_pop($names);
  return NULL;
}

/**
 * Uses the core countries list to return a country name given a code
 */
function location_taxonomize_get_country_name($code) {
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();
  if (isset($countries[strtoupper($code)])) return $countries[strtoupper($code)];
  return NULL;
}

/**
 * Provides a states list for the US
 */
function location_taxonomize_us_states() {
  return array(
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District Of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'ID' => 'Idaho',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'OR' => 'Oregon',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WA' => 'Washington',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming',
  );
}