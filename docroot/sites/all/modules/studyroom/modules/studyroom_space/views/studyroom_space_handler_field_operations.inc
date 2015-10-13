<?php

/**
 * @file
 * Definition of studyroom_space_handler_field_operations.
 */

/**
 * Operations links as field.
 *
 * This field handler aggregates operations that can be done
 * on a studyroom_space under a single field providing a more
 * flexible way to present them in a view.
 */
class studyroom_space_handler_field_operations extends views_handler_field {
  /**
   * Construct a new field handler adding our field.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['space_id'] = 'space_id';
  }

  /**
   * Called to add ensure_my_table and add_additional_fields.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $links = menu_contextual_links('studyroom_space', 'space', array($this->get_value($values, 'space_id')));
    $destination = drupal_get_destination();
    if (!empty($links)) {
      foreach ($links as &$link) {
        $link['query'] = array($destination);
      }
      $classes = array('links', 'inline', 'operations');
      return theme('links', array('links' => $links, 'attributes' => array('class' => $classes)));
    }
  }
}
