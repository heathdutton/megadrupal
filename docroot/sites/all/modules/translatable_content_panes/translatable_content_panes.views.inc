<?php

/**
 * Implements hook_views_data().
 */
function translatable_content_panes_views_data() {
  // The base table.
  $data['translatable_content_pane']['table']['group'] = t('Translatable content panes');

  // Make it a standalone table.
  $data['translatable_content_pane']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Translatable content panes'),
    'help' => t('Contains the translatable content panes.'),
    'weight' => -10,
  );

  // The ID table field.
  $data['translatable_content_pane']['id'] = array(
    'title' => t('ID'),
    'help' => t('The ID of the translatable content pane.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // The admin title table field.
  $data['translatable_content_pane']['admin_title'] = array(
    'title' => t('Admin title'),
    'help' => t('The admin title as shown in Panels.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // The code content table field.
  $data['translatable_content_pane']['code_content'] = array(
    'title' => t('Code content'),
    'help' => t('The code content checkbox as shown in Panels.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // The edit link field.
  $data['translatable_content_pane']['edit'] = array(
    'title' => t('Edit link'),
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the translatable content pane.'),
      'handler' => 'views_handler_field_translatable_content_pane_edit',
    ),
  );

  return $data;
}
