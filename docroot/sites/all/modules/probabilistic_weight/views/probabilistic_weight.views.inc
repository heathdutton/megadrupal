<?php

/**
 * @file
 * Provide views data and handlers for probabilistic_weight.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data_alter().
 */
function probabilistic_weight_views_data_alter(&$data) {

  $fields = field_info_fields();

  foreach ($fields as $key => $field) {
    if ($field['module'] == 'probabilistic_weight') {
      $group = 'field_data_' . $field['field_name'];
      $data[$group] = array(
        'table' => array(
          'join' => array(
            'node' => array(
              'left_field' => 'nid',
              'field' => 'entity_id',
              'extra' => array(
                0 => array(
                  'field' => 'entity_type',
                  'value' => 'node',
                ),
                1 => array(
                  'field' => 'deleted',
                  'value' => 0,
                  'numeric' => TRUE,
                ),
              ),
            ),
          ),
        ),
        'entity_id' => array(
          'field' => array(
            'moved to' => array(
              0 => $group,
              1 => $field['field_name'],
            ),
          ),
        ),
      );
      $data[$group][$field['field_name'] . '_weight'] = array(
        'group' => t('Probabilistic weight'),
        'title' => $field['field_name'],
        'help' => t('Probabilistic weight fields: !field_name', array('!field_name' => $field['field_name'])),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'probabilistic_weight_handler_sort_weight',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
        ),
      );
    }

  }

}

/**
 * Implements hook_views_handlers().
 */
function probabilistic_weight_views_handlers() {

  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'probabilistic_weight') . '/views',
    ),
    'handlers' => array(
      'probabilistic_weight_handler_sort_weight' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );

}
