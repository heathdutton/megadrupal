<?php
/**
 * @file
 * Internationalization (i18n) module - Translation set.
 */

/**
 * Class i18n_mosaik_translation_set.
 */
class i18n_mosaik_translation_set extends i18n_translation_set {

  /**
   * Load all mosaik translations.
   */
  public function load_translations() {
    $criteria = array('i18n_tsid' => $this->tsid);
    return i18n_translation_set_index(mosaik_load_multiple_by_criteria($criteria));
  }

  /**
   * Get placeholder values for path replacement.
   */
  function get_path_placeholders($op = 'list') {
    $values = parent::get_path_placeholders($op);
    if (!empty($this->bundle)) {
      $values['%mosaik'] = $this->bundle;
    }
    return $values;
  }
}

/**
 * Mosaik textgroup handler.
 */
class i18n_mosaik extends i18n_string_object_wrapper {

  /**
   * Get link for item.
   */
  public function get_path() {
    if ($this->object['path']) {
      return $this->object['path'];
    }
  }

  /**
   * Get title from item.
   */
  public function get_title() {
    return $this->object['title'];
  }

  /**
   * Access to object translation.
   *
   * This should check object properties and permissions.
   */
  protected function translate_access() {
    return user_access('administer mosaik') && $this->get_langcode() && user_access('translate interface');
  }
}
