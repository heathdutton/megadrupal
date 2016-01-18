<?php


/**
 * @file
 * Expose Reservations information to Views.
 */

/**
 * Implements hook_views_data().
 */
function reservations_views_data() {
  $data = array();

  // reservations_node_type
  $data['reservations_node_type']['table']['group'] = t('Reservations');
  $data['reservations_node_type']['table']['join']['node'] = array(
    'left_field' => 'type',
    'field' => 'type',
    'type' => 'INNER',
  );
  $data['reservations_node_type']['type_setting'] = array(
    'title' => t('Reservations type'),
    'help' => t('The Reservations type (Bucket|Resource).'),
    'real field' => 'reservations_type_setting',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_node_type_type_setting',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_node_type_type_setting',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['max_hours_per_reservation'] = array(
    'title' => t('Max hours per reservation'),
    'help' => t('The maximum hours that the item can be reserved for.'),
    'real field' => 'reservations_max_hours_per_reservation',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['allow_overnight'] = array(
    'title' => t('Allow overnight'),
    'help' => t('TRUE if the item can be kept overnight.'),
    'real field' => 'reservations_allow_overnight',
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Allowed overnight'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['allow_weekends'] = array(
    'title' => t('Allow weekends'),
    'help' => t('TRUE if the item can be kept over weekends.'),
    'real field' => 'reservations_allow_weekends',
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Allowed over weekends'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['late_fee_per_hour'] = array(
    'title' => t('Late fee per hour -- default'),
    'help' => t('Default per-hour fee if the item is returned late (as defined by Reservations type).'),
    'real field' => 'reservations_late_fee_per_hour',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['rate_per_hour'] = array(
    'title' => t('Rate per hour -- default'),
    'help' => t('Default per-hour fee for use of the item (as defined by Reservations type).'),
    'real field' => 'reservations_rate_per_hour',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['fee_free_hours'] = array(
    'title' => t('Fee free hours -- default'),
    'help' => t('Default number of hours the item can be used before fees are charged (as defined by Reservations type).'),
    'real field' => 'reservations_fee_free_hours',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['status'] = array(
    'title' => t('Bucket/Resource Status'),
    'help' => t('The status of the Bucket/Resource.'),
    'real field' => 'reservations_active_status',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_node_type_status',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_node_type_status',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_node_type']['spare_items'] = array(
    'title' => t('Bucket spare items'),
    'help' => t('Number of spare items to leave unreserved for the bucket.'),
    'real field' => 'reservations_spare_items',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // reservations_reservation
  $data['reservations_reservation']['table']['group'] = t('Reservations');
  $data['reservations_reservation']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'INNER',
  );
  $data['reservations_reservation']['status'] = array(
    'title' => t('Reservation status'),
    'help' => t('The status of the reservation.'),
    'real field' => 'reservations_reservation_status',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_reservation_status',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_reservation_status',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );

  // reservations_reservation_detail
  $data['reservations_reservation_detail3']['table']['group'] = t('Reservations');
  $data['reservations_reservation_detail3']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'reservations_item_nid',
    'table' => 'reservations_reservation_detail',
  );
  $data['reservations_reservation_detail3']['date_argument'] = array(
    'group' => t('Date'),
    'title' => t('Date for reservation detail (for item nid)'),
    'help' => t('Filter any Views date field by a date argument'),
    'argument' => array(
      'handler' => 'date_api_argument_handler',
      'empty name field' => t('Undated'),
    ),
  );
  $data['reservations_reservation_detail3']['vid'] = array(
    'title' => t('Reservation nid (for item nid)'),
    'help' => t('The node id of the reservation for this item id.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'skip base' => array('node'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'vid',
      'label' => t('Reservation for item node id'),
    ),
  );

  // reservations_reservation_detail
  $data['reservations_reservation_detail2']['table']['group'] = t('Reservations');
  $data['reservations_reservation_detail2']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'reservations_placeholder_nid',
    //'type' => 'INNER',
    'table' => 'reservations_reservation_detail',
  );
  $data['reservations_reservation_detail2']['date_argument'] = array(
    'group' => t('Date'),
    'title' => t('Date for reservation detail'),
    'help' => t('Filter any Views date field by a date argument'),
    'argument' => array(
      'handler' => 'date_api_argument_handler',
      'empty name field' => t('Undated'),
    ),
  );
  $data['reservations_reservation_detail2']['nid'] = array(
    'title' => t('Reservation nid'),
    'help' => t('The node id of the reservation for this placeholder item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'skip base' => array('node'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('Reservation for placeholder item node id'),
    ),
  );


  // reservations_reservation_detail
  $data['reservations_reservation_detail']['table']['group'] = t('Reservations');
  $data['reservations_reservation_detail']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    //'type' => 'INNER',
  );
  $data['reservations_reservation_detail']['placeholder_nid'] = array(
    'title' => t('Reserved placeholder nid'),
    'help' => t('The node id of the reserved placeholder item.'),
    'real field' => 'reservations_placeholder_nid',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'skip base' => array('node'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'relationship field' => 'reservations_placeholder_nid',
      'label' => t('Reserved placeholder item node id'),
    ),
  );


  $data['reservations_reservation_detail']['item_nid'] = array(
    'title' => t('Reserved item nid'),
    'help' => t('The node id of the reserved item.'),
    'real field' => 'reservations_item_nid',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'skip base' => array('node'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'reservations_views_handler_filter_reserved_item_nid',
      'numeric' => TRUE,
      'help' => t('<strong>Reservations</strong> are filtered for specific reserved items.'),
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'relationship field' => 'reservations_item_nid',
      'label' => t('Reserved item node id'),
    ),
  );
  $data['reservations_reservation_detail']['item_nid2'] = array(
    'title' => t('Reserved items'),
    'help' => t('Display all items reserved by this reservation.'),
    'field' => array(
      'title' => t('Reserved items'),
      'help' => t('Display all items reserved by this reservation.'),
      'handler' => 'views_handler_field_item_node_nid',
    ),
  );


  // reservations_bucket_node
  $data['reservations_bucket_node']['table']['group'] = t('Reservations');
  $data['reservations_bucket_node']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'LEFT',
  );

  // reservations_resource_node
  $data['reservations_resource_node']['table']['group'] = t('Reservations');
  $data['reservations_resource_node']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'LEFT',
  );
  $data['reservations_resource_node']['late_fee_per_hour'] = array(
    'title' => t('Late fee per hour -- resource'),
    'help' => t('Per-hour fee if the item is returned late (as defined by the individual resource).'),
    'real field' => 'reservations_late_fee_per_hour',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_resource_node']['rate_per_hour'] = array(
    'title' => t('Rate per hour -- resource'),
    'help' => t('Per-hour fee for use of the item (as defined by the individual resource).'),
    'real field' => 'reservations_rate_per_hour',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_resource_node']['fee_free_hours'] = array(
    'title' => t('Fee free hours -- resource'),
    'help' => t('Number of hours the item can be used before fees are charged (as defined by the individual resource).'),
    'real field' => 'reservations_fee_free_hours',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // reservations_resource_node
  $data['reservations_reservation_item_node']['table']['group'] = t('Reservations');
  $data['reservations_reservation_item_node']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'LEFT',
  );
  $data['reservations_reservation_item_node']['default_availability'] = array(
    'title' => t('Default booking availability -- resource/bucket'),
    'help' => t('The default booking availability of a Resource or Bucket type (ex. Available|Unavailable).'),
    'real field' => 'reservations_default_availability',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_bucket_resource_node_default_availability',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_bucket_resource_node_default_availability',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_reservation_item_node']['sub_type'] = array(
    'title' => t('Sub type -- resource/bucket'),
    'help' => t('The sub type of the Resource or Bucket (Item|Reservation placeholder).'),
    'real field' => 'reservations_sub_type',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_bucket_resource_node_sub_type',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_bucket_resource_node_sub_type',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['reservations_reservation_item_node']['item_status'] = array(
    'title' => t('Reservation Status of Item'),
    'help' => t('Is the reservable item currently checked out.'),
    'real field' => 'reservations_item_status',
    'field' => array(
      'handler' => 'reservations_handler_field_reservations_reservation_item_node_item_status',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'reservations_handler_filter_reservations_reservation_item_node_item_status',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function reservations_views_plugins() {
  return array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'argument validator' => array(
      'reservations' => array(
        'title' => t('Reservations'),
        'handler' => 'views_plugin_argument_validate_reservations_node',
        'path' => drupal_get_path('module', 'reservations'), // not necessary for most modules
      ),
    ),
  );
}
