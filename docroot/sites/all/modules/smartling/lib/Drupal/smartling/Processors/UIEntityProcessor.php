<?php

/**
 * @file
 * Contains Drupal\smartling\Processors\EntityProcessorInterface.
 */

namespace Drupal\smartling\Processors;

class UIEntityProcessor implements EntityProcessorInterface {
  protected $locale_import_mode = LOCALE_IMPORT_KEEP;

  public function __construct($smartling_submission, $smartling_settings, $log, $entity_api_wrapper, $smartling_utils) {
    $this->smartling_submission = $smartling_submission;

    $this->log = $log;
    $this->entity_api_wrapper = $entity_api_wrapper;
    $this->smartling_utils = $smartling_utils;
    $this->settings = $smartling_settings;
  }

  protected function prepareFileObjByURI($uri) {
    global $user;

    $file = NULL;
    if ($uri) {
      $file_result = entity_load('file', FALSE, array('uri' => $uri));
      $key = key($file_result);
      // If file exist.
      if (!is_null($key) && isset($file_result[$key]) && ($file_result[$key]->uri == $uri)) {
        $file = $file_result[$key];
      }
      else {
        // Create a file object.
        $file = new \stdClass();
        $file->fid = NULL;
        $file->uri = $uri;
        $file->filename = drupal_basename($uri);
        $file->filemime = file_get_mimetype($file->uri);
        $file->uid = $user->uid;
        $file->status = FILE_STATUS_PERMANENT;
      }
      $file = file_save($file);
    }

    return $file;
  }

  public function updateEntity($content) {
    $mode = $this->settings->getUITranslationsMergeMode();

    $submission = $this->smartling_submission;

    $langcode = $submission->getTargetLanguage();
    $translated_file_name = $submission->getFileTranslatedName();
    $group = $submission->getBundle();

    $uri = $this->settings->getDir($translated_file_name);

    $file = $this->prepareFileObjByURI($uri);
    if (!empty($file->fid)) {
      // Now import strings into the language.
      $res = _locale_import_po($file, $langcode, $mode, $group) == FALSE;
      if ($res) {
        $variables = array('%filename' => $file->filename);
        drupal_set_message(t('The translation import of %filename failed.', $variables), 'error');
        watchdog('locale', 'The translation import of %filename failed.', $variables, WATCHDOG_ERROR);
      }
    }
    else {
      drupal_set_message(t('File for import not found.'), 'error');
    }
    return !$res;
  }


  public function exportContent() {
    $group = $this->smartling_submission->getBundle();
    // $language = NULL for generate .pot files. Save .pot as .po file.
    $output = _locale_export_po_generate(NULL, _locale_export_get_strings(NULL, $group));

    $custom_regexp = $this->settings->getCustomRegexpPlaceholder();
    if (!empty($custom_regexp)) {
      $output = "# smartling.placeholder_format_custom = $custom_regexp \n" . $output;
    }

    return $output;
  }

  public function exportContentToArray() {
    return array();
  }
}