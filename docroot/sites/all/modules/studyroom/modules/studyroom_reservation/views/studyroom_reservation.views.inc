<?php

/**
 * @file
 * Providing extra functionality for the StudyroomReservation UI via views.
 */


/**
 * Implements hook_views_data().
 */
function studyroom_reservation_views_data_alter(&$data) {
  $data['studyroom_reservation']['table']['group'] = t('Reservation');

  // Advertise this table as a possible base table.
  $data['studyroom_reservation']['table']['base'] = array(
    'field' => 'reservation_id',
    'title' => t('Studyroom Reservation'),
  );

  // Expose reservation type.
  $data['studyroom_reservation']['type'] = array(
    'title' => t('Reservation type'),
    'help' => t('The actual label of the reservation type.'),
    'field' => array(
      'handler' => 'studyroom_reservation_handler_field_type',
    ),
    'filter' => array(
      'handler' => 'studyroom_reservation_handler_filter_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose reservation type.
  $data['studyroom_reservation']['space_id'] = array(
    'title' => t('Space'),
    'help' => t('The actual name of the space.'),
    'field' => array(
      'handler' => 'studyroom_reservation_handler_field_space',
    ),
  );
  // Expose the creator uid.
  $data['studyroom_reservation']['uid'] = array(
    'title' => t('Owner'),
    'help' => t('Relate a reservation to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Reservation Owner'),
    ),
  );
  // Expose the reservation status.
  $data['studyroom_reservation']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('The duration of the reservation.'),
    'field' => array(
      'handler' => 'views_handler_field_time_interval',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Expose the created and changed timestamps.
  $data['studyroom_reservation']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the reservation was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['studyroom_reservation']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['studyroom_reservation']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['studyroom_reservation']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['studyroom_reservation']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['studyroom_reservation']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['studyroom_reservation']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['studyroom_reservation']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the product was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['studyroom_reservation']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['studyroom_reservation']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['studyroom_reservation']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['studyroom_reservation']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['studyroom_reservation']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['studyroom_reservation']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );
  $data['studyroom_reservation']['link_studyroom_reservation'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('A simple link to the studyroom reservation.'),
      'handler' => 'studyroom_reservation_handler_field_link',
    ),
  );
  $data['studyroom_reservation']['edit_studyroom_reservation'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the studyroom reservation.'),
      'handler' => 'studyroom_reservation_handler_field_link_edit',
    ),
  );
  $data['studyroom_reservation']['delete_studyroom_reservation'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the studyroom reservation.'),
      'handler' => 'studyroom_reservation_handler_field_link_delete',
    ),
  );
  $data['studyroom_reservation']['operations'] = array(
    'field' => array(
      'title' => t('Operations'),
      'help' => t('Display all operations available for this studyroom reservation.'),
      'handler' => 'studyroom_reservation_handler_field_operations',
    ),
  );

  // Reservation types table.
  $data['studyroom_reservation_type']['table']['group'] = t('Reservation type');

  // Explain how this table joins to others.
  $data['studyroom_reservation_type']['table']['join'] = array(
    'studyroom_reservation' => array(
      'left_field' => 'type',
      'field' => 'type',
    ),
  );

  // Reservation Types.
  // Expose the public status.
  $data['studyroom_reservation_type']['public'] = array(
    'title' => t('Public'),
    'help' => t('Whether or not the reservation type is public.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Yes'), t('No')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Public'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

}

/**
 * Implements hook_views_query_alter().
 *
 * Add a where condition to only show ui = 1 types.
 */
function studyroom_reservation_views_query_alter(&$view, &$query) {
  if ($view->name == 'studyroom_reservations') {
    $query->add_table('studyroom_reservation_type');
    $query->add_where(0, 'studyroom_reservation_type.ui', 1, '=');
  }
}
