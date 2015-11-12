<?php
/**
 * @file
 * This module creates a room reservation application.
 *
 * @author Bob Humphrey, Web and Applications Developer, UNCW Randall Library
 */


/**
 * Implements hook_node_info().
 * 
 * 
 */
function room_reservations_node_info() {
  return array(
    'room_reservations_category' => array(
      'name' => t('Room Reservations Category'),
      'base' => 'room_reservations',
      'description' => t('Category of reservable rooms.'),
      'has_title' => TRUE,
      'title_label' => t('Name'),
      'has_body' => FALSE,
      'locked' => TRUE,
    ),
    'room_reservations_room' => array(
      'name' => t('Room Reservations Room'),
      'base' => 'room_reservations',
      'description' => t('A reservable room.'),
      'has_title' => TRUE,
      'title_label' => t('Name'),
      'has_body' => TRUE,
      'body_label' => t('Description'),
      'locked' => TRUE,
    ),            
    'room_reservations_reservation' => array(
      'name' => t('Room Reservations Reservation'),
      'base' => 'room_reservations',
      'description' => t('A room reservation.'),
      'has_title' => TRUE, 
      'title_label' => t('Group Name'),
      'title_description' => t('Identifies your group on the reservation calendar.'), 
      'has_body' => FALSE,
      'locked' => TRUE,
    ),        
  );
}

/**
 * Implements hook_node_type_insert().
 *
 * Much like hook_node_insert() lets us know that a node is being
 * inserted into the database, hook_node_type_insert() lets us know
 * that a new content type has been inserted.
 *
 * Since Drupal will at some point insert our new content type,
 * this gives us a chance to add the fields we want.
 *
 * It is called for all inserts to the content type database, so
 * we have to make sure we're only modifying the type we're
 * concerned with.
 */
function room_reservations_node_type_insert($content_type) {
  switch ($content_type->type) {
    case 'room_reservations_category':
      // Create all the fields we are adding to our content type.
      foreach (_room_reservations_installed_fields_category() as $field) {
        field_create_field($field);
      }
      // Create all the instances for our fields.
      foreach (_room_reservations_installed_instances_category() as $instance) {
        $instance['entity_type'] = 'node';
        $instance['bundle'] = 'room_reservations_category';
        field_create_instance($instance);
      } 
      break;
      
    case 'room_reservations_room':
      $body_instance = node_add_body_field($content_type, t('Description'));
      // Save our changes to the body field instance.
      field_update_instance($body_instance);
      // Create all the fields we are adding to our content type.
      foreach (_room_reservations_installed_fields_room() as $field) {
        field_create_field($field);
      }
      // Create all the instances for our fields.
      foreach (_room_reservations_installed_instances_room() as $instance) {
        $instance['entity_type'] = 'node';
        $instance['bundle'] = 'room_reservations_room';
        field_create_instance($instance);
      } 
      break;
      
    case 'room_reservations_reservation':
      // Create all the fields we are adding to our content type.
      foreach (_room_reservations_installed_fields_reservation() as $field) {
        field_create_field($field);
      }
      // Create all the instances for our fields.
      foreach (_room_reservations_installed_instances_reservation() as $instance) {
        $instance['entity_type'] = 'node';
        $instance['bundle'] = 'room_reservations_reservation';
        field_create_instance($instance);
      }
      break;      
  }
}

/**
 * Implements hook_form().
 *
 * Drupal needs for us to provide a form that lets the user
 * add content. This is the form that the user will see if
 * they go to node/add/node-example.
 *
 * You can get fancy with this form, or you can just punt
 * and return the default form that node_content will provide.
 */
function room_reservations_form($node, $form_state) {
  return node_content_form($node, $form_state);
}

/**
 * Define the fields for our content type.
 *
 * This big array is factored into this function for readability.
 *
 * @return array
 *   An associative array specifying the fields we wish to add to our
 *   new node type.
 */
function _room_reservations_installed_fields_room() {
  return array(
    'reservations_room_category' => array(
      'field_name' => 'reservations_room_category',
      'cardinality' => 1,
      'type'        => 'entityreference',
      'settings'    => array(
        'target_type' => 'node', 
        'default_widget' => 'options_select',     
        'handler' => 'base',
        'handler_settings' => array('target_bundles' => array('room_reservations_category')),
      ),
    ),
    'reservations_room_capacity' => array(
      'field_name' => 'reservations_room_capacity',
      'cardinality' => 1,
      'type'        => 'number_integer',
      'settings' => array(
        'default_widget' => 'text',
        'max_length' => 255,
      ),
    ),
  );
}

function _room_reservations_installed_instances_room() {
  return array(
    'reservations_room_capacity' => array(
      'field_name' => 'reservations_room_capacity',           
      'required' => true,
      'label'       => t('Capacity'),
      'widget'      => array(
        'type'    => 'text_textfield',
      ),
      'settings' => array(
        'text_processing' => 0,
      ),
    ),
    'reservations_room_category' => array(
      'field_name' => 'reservations_room_category',           
      'required' => true,
      'label'       => t('Category'),
      'widget'      => array(
        'type'    => 'options_select',
      ),
    ),
    'reservations_display_order' => array(
      'bundle' => 'room_reservations_category',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'reservations_display_order',
      'label' => 'Display Order',
      'required' => TRUE,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => 1,
      ),
    ),
  );
}

/**
 * Define the fields for our content type.
 *
 * This big array is factored into this function for readability.
 *
 * @return array
 *   An associative array specifying the fields we wish to add to our
 *   new node type.
 */
function _room_reservations_installed_fields_category() {
  $field_bases = array();
  
  // Exported field_base: 'reservations_display_order'
  $field_bases = array(
    'reservations_display_order' => array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'reservations_display_order',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
          7 => 7,
          8 => 8,
          9 => 9,
          10 => 10,
          11 => 11,
          12 => 12,
          13 => 13,
          14 => 14,
          15 => 15,
          16 => 16,
          17 => 17,
          18 => 18,
          19 => 19,
          20 => 20,
          21 => 21,
          22 => 22,
          23 => 23,
          24 => 24,
          25 => 25,
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    ),
  
    'reservations_minadvbooking' => array(
      'field_name' => 'reservations_minadvbooking',
      'cardinality' => 1,
      'type'        => 'number_integer',
      'settings' => array(
        'default_widget' => 'text',
        'max_length' => 255,
      ),
    ),    
  );
  
    // Exported field_base: 'field_reservation_length'
  $field_bases['reservations_prebuffer'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservations_prebuffer',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'none',
        30 => '30 minutes',
        60 => '1 hour',
        90 => '1.5 hours',
        120 => '2 hours',
      ),
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );
  
      // Exported field_base: 'field_reservation_length'
  $field_bases['reservations_postbuffer'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservations_postbuffer',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'none',
        30 => '30 minutes',
        60 => '1 hour',
        90 => '1.5 hours',
        120 => '2 hours',
      ),
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  return $field_bases;
}

function _room_reservations_installed_instances_category() {
  $field_instances = array();
  
  // Exported field_instance: 'reservations_display_order'
  $field_instances['reservations_display_order'] = array(
    'bundle' => 'room_reservations_category',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservations_display_order',
    'label' => 'Display Order',
    'required' => TRUE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );
  
  $field_instances['reservations_minadvbooking'] = array(
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'field_name' => 'reservations_minadvbooking',           
    'required' => true,
    'label'       => 'Minimum Advance Booking',
    'description' => 'Sets the minimum days in advance a room may be booked by a standard user.',
    'widget'      => array(
      'type'    => 'text_textfield',
      'weight' => 1.1,
    ),
    'settings' => array(
      'text_processing' => 0,
    ),
  ); 
  
  $field_instances['reservations_prebuffer'] = array(
    'bundle' => 'room_reservations_category',
    'default_value' => array(0 => array('value' => '0')),
    'deleted' => 0,
    'description' => 'Setup time required for reservations for all room in this category. This time will be added to each reservation to extend the reserved calendar time.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservations_prebuffer',
    'label' => 'Room setup buffer',
    'required' => true,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1.2,
    ),
  ); 
  
  $field_instances['reservations_postbuffer'] = array(
    'bundle' => 'room_reservations_category',
    'default_value' => array(0 => array('value' => '0')),
    'deleted' => 0,
    'description' => 'Takedown time required for reservations for all rooms in this category. This time will be added to each reservation to extend the reserved calendar time.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservations_postbuffer',
    'label' => 'Room takedown buffer',
    'required' => true,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1.3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Display Order');

  return $field_instances;
}

/**
 * Define the fields for our content type.
 *
 * This big array is factored into this function for readability.
 *
 * @return array
 *   An associative array specifying the fields we wish to add to our
 *   new node type.
 */
function _room_reservations_installed_fields_reservation() {
  $field_bases = array();

  // Exported field_base: 'field_block_title'
  $field_bases['reservation_block_title'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_block_title',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_reservation_date'
  $field_bases['reservation_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_date',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  // Exported field_base: 'field_reservation_length'
  $field_bases['reservation_length'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_length',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        30 => '30 minutes',
        60 => '60 minutes',
        90 => '90 minutes',
        120 => '2 hours',
        150 => '2.5 hours',
        180 => '3 hours',        
        210 => '3.5 hours',
        240 => '4 hours',    
        270 => '4.5 hours',
        300 => '5 hours',
        330 => '5.5 hours',
        360 => '6 hours',        
        390 => '6.5 hours',    
        420 => '7 hours',
        450 => '7.5 hours',
        480 => '8 hours',
        510 => '8.5 hours',        
        540 => '9 hours',
        600 => '10 hours',
        720 => '12 nours',
      ),
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  // Exported field_base: 'field_reservation_repeat_type'
  $field_bases['reservation_repeat_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_repeat_type',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => 'No repeat',
        2 => 'Repeat all days until',
        3 => 'Repeat this day of the week until',
      ),
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  // Exported field_base: 'field_reservation_repeat_until'
  $field_bases['reservation_repeat_until'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_repeat_until',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  // Exported field_base: 'field_reservation_room'
  $field_bases['reservation_room'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_room',
    'foreign keys' => array(
      'node' => array(
        'columns' => array(
          'target_id' => 'nid',
        ),
        'table' => 'node',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(
          'room_reservations_room' => 'room_reservations_room',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_reservation_series_id'
  $field_bases['reservation_series_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_series_id',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_reservation_time'
  $field_bases['reservation_time'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'reservation_time',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 4,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}

function _room_reservations_installed_instances_reservation() {
  $field_instances = array();

  // Exported field_instance: 'reservation_block_title'
  $field_instances['reservation_block_title'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this to hide the Group Name for this reservation.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_block_title',
    'label' => 'Private',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_date'
  $field_instances['reservation_date'] = array(
    'bundle' => 'room_reservations_reservation',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
        ),
        'type' => 'date_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_date',
    'label' => 'Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'Y-m-d H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_text',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_length'
  $field_instances['reservation_length'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_length',
    'label' => 'Length',
    'required' => true,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_repeat_type'
  $field_instances['reservation_repeat_type'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_repeat_type',
    'label' => 'Repeat Type',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_repeat_until'
  $field_instances['reservation_repeat_until'] = array(
    'bundle' => 'room_reservations_reservation',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
        ),
        'type' => 'date_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_repeat_until',
    'label' => 'Repeat Until',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'Y-m-d H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_room'
  $field_instances['reservation-reservation_room'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_room',
    'label' => 'Room',
    'required' => 0,
    'settings' => array(
      'nodeaccess_nodereference' => array(
        'all' => array(
          'view' => 0,
        ),
        'all_published' => 0,
        'author' => array(
          'delete' => 0,
          'update' => 0,
          'view' => 0,
        ),
        'author_published' => 0,
        'priority' => 0,
        'referenced' => array(
          'delete' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
          'published' => 0,
          'update' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
          'view' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
        ),
        'unused' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-room_reservations_reservation-field_reservation_series_id'
  $field_instances['reservation_series_id'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_series_id',
    'label' => 'Series ID',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'reservation_time'
  $field_instances['reservation_time'] = array(
    'bundle' => 'room_reservations_reservation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'reservation_time',
    'label' => 'Time',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Block title');
  t('Check this to hide the Group Name for this reservation.');
  t('Date');
  t('Length');
  t('Repeat Type');
  t('Repeat Until');
  t('Room');
  t('Series ID');
  t('Time');

  return $field_instances;
}

