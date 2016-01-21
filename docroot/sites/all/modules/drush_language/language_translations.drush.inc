<?php

/**
 * @file
 * Drush language commands related to translations.
 * - language-import-translations
 * - language-export-translations
 * - language-refresh-translations
 */

/**
 * Implements of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return array An associative array describing your command(s).
 *   An associative array describing your command(s).
 */
function language_translations_drush_command() {
  $commands = array();

  // Show enabled locale groups.
  $commands['language-groups-info'] = array(
    'description' => "Print a list with enabled locale groups.",
    'aliases' => array('language-groups', 'language-info'),
    'callback' => 'drush_language_translations_groups_info',
  );

  // Import translations.
  $commands['language-import-translations'] = array(
    'description' => "Import translations to one or more locale groups.",
    'arguments' => array(
      'langcode' => "The langcode of the language to be imported.",
      'path' => "Path to a directory with the translation files. Can be a file-name if one locale-group is imported.",
    ),
    'options' => array(
      'groups' => array(
        'description' => "Comma separated list of locale groups. Defaults to 'all'.",
        'value' => 'optional',
      ),
      'replace' => array(
        'description' => "Replace existing translations in the database. Defaults to 'false'.",
        'value' => 'optional'
      ),
    ),
    'examples' => array(
      'Import all locale groups' => "$ drush language-import-translations fr sites/all/translations/fr" . "\n" .
        "Reads from directory with filename pattern <locale-group>.<langcode>.po  (default.fr.po)",
      'Import specific locale groups, replace existing translations' => "$ drush language-import-translations fr sites/all/translations/fr --groups=default,menu --replace",
      'Import menu locale group from file' => "$ drush language-import-translations fr sites/all/translations/fr/menu.fr.po --groups-menu",
    ),
    'aliases' => array('langimp', 'language-import'),
    'callback' => 'drush_language_translations_import',
  );

  // Export translations.
  $commands['language-export-translations'] = array(
    'description' => "Export translations from one or more locale groups.",
    'arguments' => array(
      'langcode' => array(
        'description' => "The langcode of the language to be exported.",
        'value' => 'required'
      ),
      'path' => "Path to a directory where the translation files should be saved. Can be a file-name if one locale-group is exported.",
    ),
    'options' => array(
      'groups' => array(
        'description' => "Comma separated list of locale groups. Defaults to 'all'.",
        'value' => 'optional',
      ),
      'replace' => array(
        'description' => "Replace existing translations in the file-system. Defaults to 'false'.",
        'value' => 'optional'
      ),
      'sort' => array(
        'description' => "Sorts the generated po file by source and context, defaults to 'true'.",
        'value' => 'optional'
      ),
      'filter' => array(
        'description' => "Removes empty translations from the generated po file, defaults to 'true'.",
        'value' => 'optional'
      ),
    ),
    'examples' => array(
      'Export all locale groups' => "$ drush language-export-translations fr sites/all/translations/fr" . "\n" .
        "Saves to directory with filename pattern <locale-group>.<langcode>.po  (default.fr.po)",
      'Export specific locale groups, replace existing export files' => '$ drush language-export-translations fr sites/all/translations/fr --groups=default,menu --replace',
    ),
    'aliases' => array('langexp', 'language-export'),
    'callback' => 'drush_language_translations_export',
  );

  // Refresh translations.
  $commands['language-refresh-translations'] = array(
    'description' => "Refresh translations from one or more locale groups.",
    'options' => array(
      'groups' => array(
        'description' => "Comma separated list of locale groups. Defaults to 'all'.",
        'value' => 'optional',
      ),
      'delete' => array(
        'description' => "Delete left over strings.",
      ),
    ),
    'examples' => array(
      'Refresh strings in all locale groups' => '$ drush language-export-translations fr',
      'Refresh strings in specific locale groups, delete unused' => '$ drush language-export-translations fr --groups=default,menu --delete',
    ),
    'aliases' => array('language-refresh'),
    'callback' => 'drush_language_translations_refresh',
  );

  return $commands;
}

/**
 * Returns a list with enabled locale groups.
 */
function drush_language_translations_groups_info() {

  // Print header.
  $locale_groups = array('**Group**' => '**Label**');

  // Get enabled system text groups.
  $locale_groups += module_invoke_all('locale', 'groups');

  // Output.
  drush_print_table(drush_key_value_to_array_table($locale_groups));
}

/**
 * Import translations to one or more locale groups.
 *
 * @todo Add language if not existing on site.
 *
 * @param string $langcode
 *   The langcode of the language to be exported.
 * @param string $path
 *   Path to a directory with the translation files. Can be a file-name if one
 *   locale-group is imported.
 *
 * @option array $groups
 *   Comma separated list of locale groups. Defaults to 'all'.
 * @option bool $replace
 *   Replace existing translations in the database. Defaults to 'false'.
 *
 * @return void
 */
function drush_language_translations_import($langcode, $path) {

  // Check required arguments.
  if (empty($langcode) || empty($path)) {
    drush_log(dt('drush language: missing required argument'), 'error');
    return;
  }


  // Parse arguments and options.
  $path = _drush_language_translations_get_absolute_path($path);
  $language = _drush_language_translations_get_language($langcode);
  $groups = explode(',', drush_get_option('groups', 'all'));
  $groups = _drush_language_translations_groups_filter($groups);

  if (empty($path) || empty($language) || empty($groups)) {
    drush_log(dt("drush language: could not parse arguments or groups"), 'error');
    return;
  }


  // If one group is given, it is possible to read from one file.
  if (count($groups) == 1 && file_exists($path)) {
    // Execute single file import.
    _drush_language_translations_import_file($path, $language, reset($groups));
    return;
  }


  // Default behavior is reading from a directory with pattern matching.
  elseif (!is_dir($path)) {
    drush_set_error(dt("drush language-export: you must supply a valid directory path." . "\n" .
      "drush language-export: supplied path = " . $path));
    return;
  }
  // Execution.
  foreach ($groups as $group) {
    $file_name = $group . "." . $langcode . ".po";
    $tmp_file_path = $path . DIRECTORY_SEPARATOR . $file_name;

    _drush_language_translations_import_file($tmp_file_path, $language, $group);
  }
}

/**
 * Export translations from one or more locale groups.
 *
 * @param string $langcode
 *   The langcode of the language to be exported.
 * @param string $path
 *   Path to a directory where the translation files should be saved. Can be a
 *   file-name if one locale-group is exported.
 *
 * @option array $groups
 *   Comma separated list of locale groups. Defaults to 'all'.
 * @option bool $replace
 *   Replace existing translations in the file-system. Defaults to 'false'.
 * @option bool $sort
 *   Sorts the generated po file by source and context, defaults to 'true'.
 * @option bool $filter
 *   Removes empty translations from the generated po file, defaults to 'true'.
 *
 * @return void
 *   Save a file or prints it to StOut.
 */
function drush_language_translations_export($langcode, $path) {

  // Check required arguments.
  if (empty($langcode) || empty($path)) {
    drush_log(dt('drush language: missing required argument'), 'error');
    return;
  }


  // Parse arguments and options.
  $path = _drush_language_translations_get_absolute_path($path);
  $language = _drush_language_translations_get_language($langcode);
  $groups = explode(',', drush_get_option('groups', 'all'));
  $groups = _drush_language_translations_groups_filter($groups);

  if (empty($path) || empty($language) || empty($groups)) {
    drush_log(dt("drush language: could not parse arguments or groups"), 'error');
    return;
  }


  // If one group is exported, it is possible to save to one file.
  // Default behavior is saving to a directory.
  if (!is_dir($path)) {
    drush_set_error(dt("drush language-export: you must supply a valid directory path." . "\n" .
      "drush language-export: supplied path = " . $path));
    return;
  }


  // Execution.
  foreach ($groups as $group) {
    $file_name = $group . "." . $langcode . ".po";
    $tmp_file_path = $path . DIRECTORY_SEPARATOR . $file_name;

    _drush_language_translations_export_file($tmp_file_path, $language, $group);
  }
}

/**
 * Refresh translations from one or more locale groups.
 *
 * @option array $groups
 *   Comma separated list of locale groups. Defaults to 'all'.
 * @option bool $delete
 *   Delete left over strings.
 *
 * @return void
 */
function drush_language_translations_refresh() {

  // Parse arguments and options.
  $groups = explode(',', drush_get_option('groups', 'all'));
  $groups = _drush_language_translations_groups_filter($groups);

  if (empty($groups)) {
    drush_log(dt("drush language: could not parse arguments or groups"), 'error');
    return;
  }

  // Perform batch operations
  if (!empty($groups)) {
    module_load_include('admin.inc', 'i18n_string');
    // Should left over strings be deleted?
    $delete = drush_get_option('delete') ? TRUE : FALSE;
    $batch = i18n_string_refresh_batch($groups, $delete);
    batch_set($batch);
    drush_backend_batch_process();
    drush_log(dt('Strings refreshed'), 'success');
  }
}

/**
 * Sort callback for sorting translation strings by source and context.
 *
 * @param array $a
 *   Translation string information from _locale_export_get_strings().
 * @param array $b
 *   Translation string information from _locale_export_get_strings().
 *
 * @return int
 */
function _drush_language_translations_strings_sort($a, $b) {
  if ($a['source'] != $b['source']) {
    return ($a['source'] > $b['source']) ? 1 : -1;
  }
  if ($a['context'] != $b['context']) {
    return ($a['context'] > $b['context']) ? 1 : -1;
  }
}

/**
 * Filter callback for empty translation strings.
 *
 * @param array $item
 *   Translation string information from _locale_export_get_strings().
 *
 * @return bool
 */
function _drush_language_translations_strings_filter($item) {
  // Filter string, if no translation is given.
  if ($item['translation'] === '') {
    return FALSE;
  }

  return TRUE;
}

/**
 * Filters and validates locale groups.
 *
 * @param array $groups
 *   Array of locale groups.
 *   Defaults to 'default'.
 *   Placeholder 'all' returns all enabled locale_groups.
 *
 * @return array
 *   Array with existing locale group.
 */
function _drush_language_translations_groups_filter($groups = array('default')) {

  // Enabled system text groups.
  $locale_groups = module_invoke_all('locale', 'groups');

  // 'all' placeholder.
  if ($groups == array('all')) {
    $groups = array_keys($locale_groups);
  }

  // Copy values to keys for better parsing.
  $groups = array_combine($groups, $groups);


  // Filter un-existing groups.
  foreach ($groups as $group) {
    if (!in_array($group, array_keys($locale_groups))) {
      unset($groups[$group]);

      // Print a readable error message.
      $locale_groups_readable = implode(", ", array_keys($locale_groups));

      drush_log(dt("drush language: locale group '" . $group . "' is not valid. Available groups are: " . $locale_groups_readable), 'warning');
    }
  }

  return $groups;
}

/**
 * Import function for language files.
 *
 * @param string $file_path
 *   Absolute file path.
 * @param string $language
 *   Language code.
 * @param string $group
 *   Text group to export (field, default).
 *
 * @option bool $replace
 *   Replace existing translations in the file-system. Defaults to 'false'.
 *
 * @return void
 *   Imports .po file to database.
 */
function _drush_language_translations_import_file($file_path = '-', $language, $group) {

  // Parse $replace option.
  drush_get_option('replace') ? $mode = LOCALE_IMPORT_OVERWRITE : $mode = LOCALE_IMPORT_KEEP;

  // Ensure we have the file intended for upload
  if (file_exists($file_path)) {

    // Construct fake file object
    $file = new stdClass();
    $file->uid = 1;
    $file->status = 0;
    $file->filename = trim(drupal_basename($file_path), '.');
    $file->uri = $file_path;

    // Now import strings into the language
    if ($return = _locale_import_po($file, $language->language, $mode, $group) == FALSE) {
      $variables = array('%filename' => $file->filename);
      drush_log(dt('The translation import of %filename failed.', $variables), 'error');
      watchdog('locale', 'The translation import of %filename failed.', $variables, WATCHDOG_ERROR);
    }
    else {
      drush_log(dt('drush language-import: ' . $file_path), 'success');
    }
  }
  else {
    $variables = array('!filepath' => $file_path);
    drush_log(dt('File to import at !filepath not found.', $variables), 'error');
  }
}

/**
 * Saving function for language exports.
 *
 * @param string $file_path
 *   Absolute file path.
 * @param string $language
 *   Language code.
 * @param string $group
 *   Text group to export (fields, default).
 *
 * @option bool $replace
 *   Replace existing translations in the file-system. Defaults to 'false'.
 * @option bool $sort
 *   Sorts the generated po file by source and context, defaults to 'true'.
 * @option bool $filter
 *   Removes empty translations from the generated po file, defaults to 'true'.
 *
 * @return void
 *   Saves the export file to file system.
 */
function _drush_language_translations_export_file($file_path = '-', $language, $group) {

  // Don't override destination file
  // Unlike file_exists(), this condition allow the use of /dev/stdin.
  if (!drush_get_option('replace') && (is_file($file_path) || is_dir($file_path))) {
    drush_log(dt('drush language-export: will not override destination file: ' . $file_path), 'warning');
    return;
  }

  $strings = _locale_export_get_strings($language, $group);

  // In the case the filter option is given, we remove all empty translations.
  if (drush_get_option('filter', TRUE)) {
    $strings = array_filter($strings, '_drush_language_translations_strings_filter');
  }

  // In case the sort option is given, we sort the strings by source string and
  // context.
  if (drush_get_option('sort', TRUE)) {
    uasort($strings, '_drush_language_translations_strings_sort');
  }

  if ($file_path == '-') {
    echo _locale_export_po_generate($language, $strings);
  }
  else {
    file_put_contents($file_path, _locale_export_po_generate($language, $strings));
    drush_log(dt('drush language-export: ' . $file_path), 'success');
  }
}

/**
 * Returns an absolute path.
 *
 * @param string $path
 *  Absolute or relative (to drupal root) path.
 *
 * @return string
 *  Absolute path.
 */
function _drush_language_translations_get_absolute_path($path) {

  // relative path should be relative to cwd(), rather than Drupal root-dir
  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');

  if (drush_is_absolute_path($path)) {
    return $path;
  }
  else {
    return $drupal_root . DIRECTORY_SEPARATOR . $path;
  }
}

/**
 * Returns a language object.
 *
 * @param string $langcode
 *   Langcode.
 *
 * @return object
 *   Language object.
 */
function _drush_language_translations_get_language($langcode) {
  // Ensure the langcode match an existing language.
  drupal_static_reset('language_list');
  $languages = language_list('language');

  // Return language object.
  if (isset($languages[$langcode])) {
    return $languages[$langcode];
  }

  drush_log(dt("drush language: langcode '" . $langcode . "' not installed . "), 'error');
}
