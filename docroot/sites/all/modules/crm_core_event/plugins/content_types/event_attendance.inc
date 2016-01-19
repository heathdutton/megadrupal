<?php

/**
 * @file
 * Line chart displaying signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Event attendance'),
  'description' => t('Displays a table with event attendance.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'event_attendance_form',
  'render callback' => 'event_attendance_render',
  'admin info' => 'event_attendance_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function event_attendance_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Event attendance');
  $block->content = t('Displays a table with event attendance.');

  return $block;
}

/**
 * Settings form.
 */
function event_attendance_form($form, &$form_state) {

  $conf = $form_state['conf'];

  return $form;
}

/**
 * Submit handler for settings form.
 */
function event_attendance_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function event_attendance_render($subtype, $conf, $panel_args, $context = NULL) {
  $content = '';

  $events = db_query("
SELECT event.nid , event.title,
COUNT(registration.entity_id) AS registrations
FROM node as event
LEFT JOIN field_data_field_cmcev_event AS registration ON registration.field_cmcev_event_target_id = event.nid
LEFT JOIN field_data_field_cmcev_date AS event_date ON event_date.entity_id = event.nid
WHERE event.type = 'cmcev_event'
GROUP BY event.nid
ORDER BY event_date.field_cmcev_date_value2 DESC
LIMIT 0, 10
");

  $attended = db_query("
SELECT event.nid,
COUNT(registration.entity_id) AS attendees
FROM node as event
LEFT JOIN field_data_field_cmcev_event AS registration ON registration.field_cmcev_event_target_id = event.nid
LEFT JOIN field_data_field_cmcev_status AS status ON status.entity_id = registration.entity_id
WHERE event.type = 'cmcev_event'
AND (status.field_cmcev_status_value = 'Attended' )
");
  $attended = $attended->fetchAllKeyed();

  $header = array(t('Event'), t('Registered'), t('Attended'));
  $rows = array();

  foreach ($events as $event) {

    $rows[] = array(
      'data' => array(
        array(
          'data' => l(check_plain($event->title), 'crm-core/reports/cmcev/events/summary/' . $event->nid),
        	'class'=> 'table-field-event-title'
        ),    
        array(
          'data' => $event->registrations,
        	'class'=> 'table-field-event-registered'
        ),    
        array(
          'data' => isset($attended[$event->nid]) ? $attended[$event->nid] : 0,
        	'class'=> 'table-field-event-attended'
        ),    
      ),
    );
  }

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content .= '<div class="fancy-headers">';
  $content .= '<div class="table-container">' . theme('table', array('rows' => $rows, 'header' => $header)) . '</div>';
  $content .= '</div>';

  $block = new stdClass();

  $block->title = t('Attendance');
  $block->content = $content;

  return $block;
}
