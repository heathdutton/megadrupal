<?php

/**
 * @file
 * Providing extra integration with views.
 */


/**
 * Implements hook_views_data_alter()
 */
function breadcrumb2_views_data_alter(&$data) { 
  $data['breadcrumb']['link_breadcrumb'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the breadcrumb.'),
      'handler' => 'breadcrumb2_handler_link_field',
    ),
  );
  $data['breadcrumb']['edit_breadcrumb'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the breadcrumb.'),
      'handler' => 'breadcrumb2_handler_edit_link_field',
    ),
  );
  $data['breadcrumb']['delete_breadcrumb'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the breadcrumb.'),
      'handler' => 'breadcrumb2_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows breadcrumb/%breadcrumb2/op
  $data['breadcrumb']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this breadcrumb.'),
      'handler' => 'breadcrumb2_handler_breadcrumb_operations_field',
    ),
  );
}

