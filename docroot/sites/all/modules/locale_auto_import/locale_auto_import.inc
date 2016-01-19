<?php
/**
 * @file
 * Administration functions for locale_auto_import.module
 */

/**
 * Get additionnals functions and constants from "locale".
 */
require_once 'includes/locale.inc';

/**
 * Constants definitions
 */
// Name of the folder containing the .po files.
define('LOCALE_AUTO_IMPORT_PO_FOLDER', 'auto_translations');
// String importation default mode.
define('LOCALE_AUTO_IMPORT_DEFAULT_MODE', _locale_auto_import_get_default_mode());

/**
 * Get the string importation default mode.
 */
function _locale_auto_import_get_default_mode() {
  // Defined mode (can be overide in settings.php)
  $locale_auto_import_default_mode = variable_get('locale_auto_import_default_mode', LOCALE_IMPORT_OVERWRITE);
  // Be sure that the value provided is valid, if not, set default.
  if ($locale_auto_import_default_mode !== LOCALE_IMPORT_OVERWRITE && $locale_auto_import_default_mode !== LOCALE_IMPORT_KEEP) {
    $locale_auto_import_default_mode = LOCALE_IMPORT_OVERWRITE;
  }
  return ($locale_auto_import_default_mode);
}

/**
 * Prepare a batch to run for importing .po files.
 */
function locale_auto_import_batch_by_component($components, $mode = LOCALE_AUTO_IMPORT_DEFAULT_MODE, $finished = '_locale_auto_import_batch_system_finished') {
  $files = array();
  $languages = language_list('enabled');
  unset($languages[1]['en']);
  if (count($languages[1])) {
    $language_list = join('|', array_keys($languages[1]));
    // Collect all files to import for all $components.
    $result = db_query("SELECT name, filename FROM {system} WHERE status = 1");
    foreach ($result as $component) {
      if (in_array($component->name, $components)) {
        // Collect all files for this component in all enabled languages, named
        // as $langcode.po, with names ending with $langcode.po or with names
        // ending with $text_group.$langcode.po. This allows for filenames
        // like feature-module.views.fr.po to import the translations for a
        // specific text group for a specific language.
        $files = array_merge($files, file_scan_directory(dirname($component->filename) . '/' . LOCALE_AUTO_IMPORT_PO_FOLDER, '/(^|\.)(' . $language_list . ')\.po$/', array('recurse' => FALSE)));
      }
    }

    return _locale_auto_import_batch_build($files, $mode, $finished);
  }
  return FALSE;
}

/**
 * Build a locale_auto_import batch from an array of files.
 *
 * The files are analysed to find a language code and a text group.
 *
 * @param array $files
 *   Array of files to import.
 * @param int $mode
 *   Optional mode in which the string are imported.
 * @param string|null $finished
 *   Optional finished callback for the batch.
 * @param array $components
 *   Optional list of module names the batch covers.
 *
 * @return array|bool
 *   A batch structure.
 */
function _locale_auto_import_batch_build($files, $mode = LOCALE_AUTO_IMPORT_DEFAULT_MODE, $finished = NULL, $components = array()) {
  $t = get_t();
  if (count($files)) {
    // List of batch's operations.
    $operations = array();
    // List of available text groups.
    $text_groups = module_invoke_all('locale', 'groups');

    foreach ($files as $file) {
      $filepath = $file->uri;
      // The filename is either {langcode}.po, {prefix}.{langcode}.po,
      // {text_group}.{langcode}.po or {prefix}.{text_group}.{langcode}.po,
      // so we can extract the text group and the language code to use
      // for the import from the end.
      if (preg_match('!(/|\.)(([^\./]+)(\.))?([^\./]+)\.po$!', $filepath, $infos)) {
        // $infos[3] = Text group.
        // $infos[5] = Language code.
        $text_group = $infos[3];
        // If the text group is not recognize use "default"
        if (array_key_exists($text_group, $text_groups) === FALSE) {
          $text_group = 'default';
        }
        $file = (object) array(
          'filename' => drupal_basename($filepath),
          'uri' => $filepath,
        );
        // We call _locale_auto_import_batch_import for every batch operation.
        $operations[] = array(
          '_locale_auto_import_batch_import',
          array($file, $mode, $infos[5], $text_group),
        );
      }
    }
    $batch = array(
      'operations' => $operations,
      'title' => $t('Importing translations'),
      'init_message' => $t('Starting import'),
      'error_message' => $t('Error importing translations'),
      'file' => drupal_get_path('module', 'locale_auto_import') . '/locale_auto_import.inc',
    );
    if (isset($finished)) {
      $batch['finished'] = $finished;
    }
    return $batch;
  }
  return FALSE;
}

/**
 * Perform interface translation import as a batch step.
 */
function _locale_auto_import_batch_import($file, $mode, $lang, $group, &$context) {
  _locale_import_read_po('db-store', $file, $mode, $lang, $group);
  $context['results'][] = $file->uri;
}

/**
 * Finished callback of import batch. Show status of files imported.
 */
function _locale_auto_import_batch_system_finished($success, $results) {
  if ($success) {
    drupal_set_message(format_plural(count($results), 'One translation file imported.', '@count translation files imported.'));
  }
}
