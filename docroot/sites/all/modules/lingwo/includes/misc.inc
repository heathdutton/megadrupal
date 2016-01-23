<?php

/**
 * @file
 * Miscellaneous helper code for use by all lingwo_* modules
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_name_value_list($text) {
  $list = explode("\n", $text);
  $list = array_map('trim', $list);
  $list = array_filter($list, 'strlen');
  $options = array();
  foreach ($list as $opt) {
    if (strpos($opt, '|') !== FALSE) {
      list($key, $value) = explode('|', $opt);
    }
    else {
      $key = $value = $opt;
    }
    $options[$key] = $value;
  }
  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_allowed_values($settings, $name, $language, $translate = FALSE, $option_name = NULL) {
  // TODO: a static cache would be nice!
  $list = _lingwo_name_value_list(_lingwo_language_value($settings, $name . '_allowed_values', $language));

  // Only translate if we have i18n_string
  $translate = $translate && function_exists('i18n_string');

  $options = array();
  foreach ($list as $key => $value) {
    // if we are only looking for one key, skip all the rest
    if (!is_null($option_name) && $option_name != $key) {
      continue;
    }

    if ($translate) {
      $value = i18n_string('lingwo:field:' . $name . ':option_' . $key, $value);
    }

    if ($option_name == $key) {
      return $value;
    }

    $options[$key] = $value;
  }

  if (!is_null($option_name)) {
    // we requested an option but it isn't here!  Return the original.
    return $option_name;
  }

  asort($options);

  return $options;
}

// Returns the top-level project path
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_project_path() {
  static $project_path = NULL;
  if (is_null($project_path)) {
    $project_path = dirname(drupal_get_path('module', 'lingwo_entry'));
  }
  return $project_path;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_project_dirname() {
  return DRUPAL_ROOT . '/' . dirname(drupal_get_path('module', 'lingwo_entry'));
}

// Helper for generating UUIDs
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_make_uuid() {
  static $uuid = NULL;

  // use the OSSP uuid module (we have to test for uuid_make because the PECL uuid
  // module also defines the uuid_create function)
  if (is_null($uuid) && function_exists('uuid_make')) {
    uuid_create($uuid);
  }
  if (!is_null($uuid)) {
    uuid_make($uuid, UUID_MAKE_V4);
    uuid_export($uuid, UUID_FMT_STR, $uuidstring);
    return trim($uuidstring);
  }

  // use the PECL uuid module
  if (function_exists('uuid_create')) {
    return uuid_create(UUID_TYPE_DEFAULT);
  }

  // use the database to generate a UUID
  if (db_driver() != 'pgsql') {
    return db_query('SELECT UUID()')->fetchField();
  }

  // Copied graciously from the 'uuid' module.
  // Fall back to generating a Universally Unique IDentifier version 4.
  // Base on http://php.net/manual/en/function.uniqid.php#65879
  // The field names refer to RFC 4122 section 4.1.2.
  return sprintf('%04x%04x-%04x-%03x4-%04x-%04x%04x%04x',
    // 32 bits for "time_low".
    mt_rand(0, 65535), mt_rand(0, 65535),
    // 16 bits for "time_mid".
    mt_rand(0, 65535),
    // 12 bits before the 0100 of (version) 4 for "time_hi_and_version".
    mt_rand(0, 4095),
    bindec(substr_replace(sprintf('%016b', mt_rand(0, 65535)), '01', 6, 2)),
    // 8 bits, the last two of which (positions 6 and 7) are 01, for "clk_seq_hi_res"
    // (hence, the 2nd hex digit after the 3rd hyphen can only be 1, 5, 9 or d)
    // 8 bits for "clk_seq_low" 48 bits for "node".
    mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535)
  );
}

// Helper: returns TRUE if the given node is a translation
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _lingwo_is_translation($node) {
  return isset($node->translation_source) ||
    (!empty($node->nid) && !empty($node->tnid) && $node->nid != $node->tnid);
}

