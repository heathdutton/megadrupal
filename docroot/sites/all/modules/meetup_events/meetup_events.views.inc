<?php

/**
 * Implementation of hook_views_api().
 */
 function meetup_events_views_api() {
   return array(
     'api' => 3,
     'path' => drupal_get_path('module', 'meetup_events'),
   );
 }

/**
 * Implementation of hook_views_data().
 */
function meetup_events_views_data() {

  $data;
  $data['meetup_events']['table']['group'] = t('Meetup Events');
  $data['meetup_events']['table']['base'] = array(
    'field' => 'id',
    'title' => t("Meetup Events"),
    'help' => t("Info associated with meetup event")
  );

  $data['meetup_events']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid'
  );

  $data['meetup_events']['id'] = array(
    'title' => t("Meetup Event id"), // The item it appears as on the UI,
    'help' => t("Numeric id for meetup event"), // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => 'id', // the real field
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );

  $data['meetup_events']['meetup_link'] = array(
    'title' => t("Meetup Event Link"), // The item it appears as on the UI,
    'help' => t("A preformatted Meetup Event Link which supports the RSVP popup"), // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => 'id', // the real field
      'handler' => 'views_handler_field_meetuplink',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort'),
  );

  # TODO: add a custom views_handler to generate the RSVP url

  # _meetup_events_define_view_reference($data, 'meetup_events', 'nid', 'Meetup Id', 'Meetup event id for the node', 'node', 'nid');

  return $data;
}

/**
 * Implementation of hook_views_handlers()
 */
function meetup_events_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'meetup_events'),
    ),
    'handlers' => array(
      'views_handler_field_meetuplink' => array(
        'parent' => 'views_handler_field',
      ),
    )
  );
}


function _meetup_events_define_view_reference(&$data, $table, $column, $title, $help, $base, $field) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
    ),
    // Information for displaying a title as a field

    'relationship' => array(
      'base' => $base,
      'field' => $field,
      'handler' => 'views_handler_relationship',
      'label' => $title,
    ),
  );
}
