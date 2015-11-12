<?php

/**
 * @file
 * Definition of views_handler_argument_entity_gallery_language.
 */

/**
 * Argument handler to accept a language.
 */
class views_handler_argument_entity_gallery_language extends views_handler_argument {
  function construct() {
    parent::construct('language');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the language.
   */
  function summary_name($data) {
    return $this->entity_gallery_language($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * entity gallery type.
   */
  function title() {
    return $this->entity_gallery_language($this->argument);
  }

  function entity_gallery_language($langcode) {
    $languages = views_language_list();
    return isset($languages[$langcode]) ? $languages[$langcode] : t('Unknown language');
  }
}
