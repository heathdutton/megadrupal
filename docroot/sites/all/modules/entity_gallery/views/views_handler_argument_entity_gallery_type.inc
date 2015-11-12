<?php

/**
 * @file
 * Definition of views_handler_argument_entity_gallery_type.
 */

/**
 * Argument handler to accept an entity gallery type.
 */
class views_handler_argument_entity_gallery_type extends views_handler_argument_string {
  function construct() {
    parent::construct('type');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the entity gallery type.
   */
  function summary_name($data) {
    return $this->entity_gallery_type($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * entity gallery type.
   */
  function title() {
    return $this->entity_gallery_type($this->argument);
  }

  function entity_gallery_type($type) {
    $output = entity_gallery_type_get_name($type);
    if (empty($output)) {
      $output = t('Unknown content type');
    }
    return check_plain($output);
  }
}
