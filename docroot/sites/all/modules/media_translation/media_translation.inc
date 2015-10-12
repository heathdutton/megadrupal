<?php
/**
 * @file
 * Defines the custom i18n classes used by this module.
 */

/**
 * Translation set handler
 */
class media_translation_translation_set extends i18n_translation_set {

  /**
   * Get items as renderable links.
   */
  public function get_items() {
    $language_list = language_list();
    $items = array();
    foreach ($this->get_translations() as $langcode => $item) {
      $title = $item->filename;
      $path = 'media/' . $item->fid;
      $language = isset($language_list[$langcode]) ? $language_list[$langcode] : NULL;
      $items[$langcode] = array(
        'title' => $title,
        'href' => $path ? $path : NULL,
        'language' => $language,
      );
      if ($language && function_exists('languageicons_link_add')) {
        languageicons_link_add($items[$langcode]);
      }
    }
    return $items;
  }

  /**
   * Get placeholder values for path replacement
   */
  function get_path_placeholders($op = 'list') {
    $values = parent::get_path_placeholders();
    $translations = $this->get_translations();
    $language = i18n_context_language();
    $langcode = $language->language;

    // Get the fid of the file in the active site language
    // if it has been defined in the set.  Otherwise,
    // get the fid of the first file in the set.
    if (array_key_exists($langcode, $translations)) {
      // Type document:$translations[$langcode]->fid
      $values['%file'] = $translations[$langcode]->get_field('fid');
    }
    else {
      $file = reset($translations);
      if (!empty($file)) {
        $values['%file'] = $file->get_field('fid');
      }
      else {
        // There are no files in the translations set which is an error condition.
        $values['%file'] = 0;
        $message = t('Translation set @tsid does not contain any files.');
        $args = array('@tsid' => $this->tsid);
        watchdog('media_translation', $message, $args, WATCHDOG_ERROR);
        drupal_set_message(check_plain(t($message, $args)), 'error');
      }
    }

    return $values;
  }

  /**
   * Retrieves the translations available for this translation set.
   */
  public function get_translations() {
    if (!isset($this->translations)) {
      $translations = $this->get_objects();

      // Load the full file object for each translation.
      foreach ($translations as $langcode => $translation_set) {
        $file = file_load($translation_set->get_field('fid', 0));
        $this->add_item($file, $langcode);
      }
    }
    return $this->translations;
  }

  /**
   * Get edit path for this translation set
   */
  public function get_edit_path() {
    if ($path = $this->get_info('edit path')) {
      $placeholders = $this->get_path_placeholders('delete');
      if (isset($placeholders['%file']) && $placeholders['%file'] != 0) {
        return strtr($path, $this->get_path_placeholders('delete'));
      }
    }
    return '';
  }

}
