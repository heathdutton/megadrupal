<?php

/**
 * @file
 * Table.
 * Displays a table with some summary metrics around events.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Event: Overall metrics'),
  'description' => t('Displays overview metrics about events stored in the system.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'event_metrics_form',
  'render callback' => 'event_metrics_render',
  'admin info' => 'event_metrics_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function event_metrics_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Event: Overall metrics');
  $block->content = t('Displays overview metrics about events stored in the system.');

  return $block;
}

/**
 * Settings form.
 */
function event_metrics_form($form, &$form_state) {

  $conf = $form_state['conf'];

  return $form;
}

/**
 * Submit handler for settings form.
 */
function event_metrics_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function event_metrics_render($subtype, $conf, $panel_args, $context = NULL) {
  $content = '';

  $active_events = db_query("
  SELECT COUNT(n.nid) AS events
FROM {node} n
WHERE n.status = '1' AND n.type = 'cmcev_event'
  ");
  $active_events = (int) $active_events->fetchField();

  $registrations = db_query("
  SELECT COUNT(registration.activity_id) AS registrations
FROM {crm_core_activity} registration
WHERE registration.type = 'cmcev_event_registration'
  ");
  $registrations = (int) $registrations->fetchField();

  $attended = db_query("
  SELECT COUNT(registration.activity_id) AS attended
FROM {crm_core_activity} AS registration
LEFT JOIN {field_data_field_cmcev_status} AS status ON status.entity_id = registration.activity_id
WHERE registration.type = 'cmcev_event_registration' AND status.field_cmcev_status_value = 'Attended'
  ");
  $attended = (int) $attended->fetchField();

  $header = array(
    t('Total attendees'),
    t('Total events'),
    t('Average attendance'),
  );
  $rows = array();

  if (empty($registrations)) {
    $average_attendance = '0%';
  }
  else {
    $average_attendance = floor($attended / $registrations * 100) . '%';
  }

  $rows[] = array(
    'data' => array(
      $registrations,
      $active_events,
      $average_attendance,
    ),
  );
  $attr = array('class' => array('summary-table'));
  $vars = array('rows' => $rows, 'header' => $header, 'attributes' => $attr);
  $content .= '<div class="table-container">' . theme('table', $vars) . '</div>';

  $block = new stdClass();
  $block->title = '';
  $block->content = $content;

  return $block;
}
