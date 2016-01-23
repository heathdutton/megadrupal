<?php

/**
 * @file
 * Provides a field handler for the Views module.
 */

/**
 * Field handler to show commerce recurring operations in a dropbutton.
 */
class commerce_recurring_ui_handler_field_operations_dropbutton extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';

    // Set real_field in order to make it generate a field_alias.
    $this->real_field = 'id';
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $recurring_id = $this->get_value($values, 'id');
    $recurring_uid = $values->commerce_recurring_uid;

    // Prepare the links.
    $links = array();

    $links[] = array(
      'href'  => 'user/' . $recurring_uid . '/recurring/' . $recurring_id .'/orders',
      'title' => t('view orders'),
    );

    $links[] = array(
      'href'  => 'user/' . $recurring_uid . '/recurring/' . $recurring_id . '/cancel/' . drupal_get_token($recurring_id),
      'title' => t('cancel'),
    );

    return theme('links__ctools_dropbutton', array('links' => $links));
  }
}
