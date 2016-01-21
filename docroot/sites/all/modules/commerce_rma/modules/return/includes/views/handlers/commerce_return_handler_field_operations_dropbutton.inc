<?php

/**
 * Field handler to show commerce return operations in dropbutton.
 */
class commerce_return_handler_field_operations_dropbutton extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['return_id'] = 'return_id';

    // Set real_field in order to make it generate a field_alias.
    $this->real_field = 'return_id';
  }

  function render($values) {
    $return_id = $this->get_value($values, 'return_id');

    $entity_info = entity_get_info('commerce_return');
    $path = $entity_info['admin ui']['path'] . '/manage';

    // Prepare the links.
    $links = array(
      array(
        'href' => $entity_info['admin ui']['path'] . "/$return_id/view",
        'title' => t('view')
      ),
      array(
        'href' => "$path/$return_id/edit",
        'title' => t('edit')
      ),
      array(
        'href' => "$path/$return_id/delete",
        'title' => t('delete'),
        'query' => drupal_get_destination()
      ),
    );

    return theme('links__ctools_dropbutton', array('links' => $links));
  }
}
