<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_field_operations.
 */

/**
 * Operations links as field.
 *
 * This field handler aggregates operations that can be done
 * on a studyroom_space under a single field providing a more
 * flexible way to present them in a view.
 */
class studyroom_reservation_handler_field_operations extends views_handler_field {
  /**
   * Construct a new field handler adding our field.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['reservation_id'] = 'reservation_id';
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
    $links = menu_contextual_links('studyroom_reservation', 'reservation', array($this->get_value($values, 'reservation_id')));
    if (!empty($links)) {
      $classes = array('links', 'inline', 'operations');
      return theme('links', array('links' => $links, 'attributes' => array('class' => $classes)));
    }
  }
}
