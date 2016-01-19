<?php
/**
 * @file
 * Views hooks implementations.
 */

/**
 * Implements hook_views_data().
 */
function registration_views_views_data() {
  return array(
    'registration_node' => array(
      'table' => array(
        'group' => t('Registration info'),
        'base' => array(
          'field' => 'nid',
          'title' => t('Registration Info'),
          'help' => t('Registration info is always linked to one node.'),
        ),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
          'registration' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),
      'nid' => array(
        'title' => t('Nid'),
        'help' => t('The node id of the node this registration info belongs to'),
        'field' => array(
          'handler' => 'views_handler_field_node',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_node_nid',
          'numeric' => TRUE,
          'validate type' => 'nid',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'label' => t('Node'),
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'base field' => 'nid',
          'relationship field' => 'nid',
        ),
      ),
      'capacity' => array(
        'title' => t('Capacity'),
        'help' => t('The maximum amount of registrations for this node, 9999 for no limit.'),
        'field' => array(
          'handler' => 'views_handler_field_registration_capacity',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_field_registration_capacity',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'occupied' => array(
        'title' => t('Occupied'),
        'real field' => 'nid',
        'help' => t('The number of occupied slots for this node'),
        'field' => array(
          'handler' => 'views_handler_field_registration_occupied',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_field_registration_occupied',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_field_registration_occupied',
        ),
      ),
      'available' => array(
        'title' => t('Available'),
        'real field' => 'nid',
        'help' => t('The number of available slots for this node (9999 if this registration has no limit)'),
        'field' => array(
          'handler' => 'views_handler_field_registration_available',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_field_registration_available',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_field_registration_available',
        ),
      ),
    ),
  );
}
