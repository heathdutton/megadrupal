<?php

/**
 * @file
 * Protected Node views declarations.
 */

/**
 * Dates for the protected node.
 */
function _protected_node_views_extended_dates(&$data, $field, $label) {
  $entries = array(
    'fulldate' => array(
      'help' => t('CCYYMMDD'),
      'label' => t('full date'),
    ),
    'year_month' => array(
      'help' => t('CCYYMM'),
      'label' => t('year + month'),
    ),
    'year' => array(
      'help' => t('CCYY'),
      'label' => t('year'),
    ),
    'month' => array(
      'help' => t('MM (01 - 12)'),
      'label' => t('month'),
    ),
    'day' => array(
      'help' => t('DD (01 - 31)'),
      'label' => t('day'),
    ),
    'week' => array(
      'help' => t('WW (01 - 53)'),
      'label' => t('week'),
    ),
  );

  foreach ($entries as $format => $entry) {
    $data['protected_nodes'][$field . '_' . $format] = array(
      'title' => $label . ' ' . $entry['label'],
      'help' => t('In the form of !form.', array('!form' => $entry['help'])),
      'argument' => array(
        'field' => $field,
        'handler' => 'views_handler_argument_node_created_' . $format,
      ),
    );
  }
}

/**
 * Implements hook_views_data().
 */
function protected_node_views_views_data() {
  $data = array();

  // Views: protected node.
  $data['protected_nodes']['table']['group'] = t('Protected node list');

  $data['protected_nodes']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  _protected_node_views_extended_dates($data, 'protected_node_passwd_changed', 'Password last changed');
  $data['protected_nodes']['protected_node_passwd_changed'] = array(
    'title' => t('Password last changed'),
    'help' => t('Date when the password for this node was last changed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['protected_nodes']['protected_node_is_protected'] = array(
    'title' => t('Node is protected'),
    'help' => t('Whether the node is protected.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Protected node'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['protected_nodes']['protected_node_show_title'] = array(
    'title' => t('Protected node, but show title'),
    'help' => t('Whether the node title is protected.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Protected title'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
