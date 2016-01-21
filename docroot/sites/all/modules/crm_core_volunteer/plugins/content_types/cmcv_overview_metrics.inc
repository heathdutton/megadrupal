<?php
/**
 * @file
 * Displays a metrics table for overview report.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Overview metrics table'),
  'description' => t('Displays a metrics table for overview report.'),
  'category' => t('CRM Core Volunteer'),
  'edit form' => 'cmcv_overview_metrics_form',
  'render callback' => 'cmcv_overview_metrics_render',
  'admin info' => 'cmcv_overview_metrics_admin_info',
  'defaults' => array(),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function cmcv_overview_metrics_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Overview metrics table');
  $block->content = t('Displays a metrics table for overview report.');

  return $block;
}

/**
 * Settings form.
 */
function cmcv_overview_metrics_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * Validate handler for settings form.
 */
function cmcv_overview_metrics_form_validate($form, &$form_state) {
}

/**
 * Submit handler for settings form.
 */
function cmcv_overview_metrics_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function cmcv_overview_metrics_render($subtype, $conf, $panel_args, $context = NULL) {

  // Set up a table for displaying raw data.
  $header = array(
    'Total volunteers',
    'Active opportunities',
    'Average attendance',
  );
  $rows = array();

  // Registered attendees.
  $registered = db_query("
  SELECT COUNT(DISTINCT commitment.activity_id) as attended
  FROM {crm_core_activity} AS commitment
  WHERE commitment.type = 'cmcv_volunteer_commitment'
");
  $registered = $registered->fetchField();

  $attended = db_query("
  SELECT COUNT(DISTINCT commitment.activity_id) as attended
  FROM {crm_core_activity} AS commitment
  LEFT JOIN {field_data_field_cmcv_signup_status} AS status ON status.entity_id = commitment.activity_id
  WHERE commitment.type = 'cmcv_volunteer_commitment'
  AND status.field_cmcv_signup_status_value = 'attended'
");
  $attended = $attended->fetchField();

  $opportunities = db_query("
SELECT COUNT(opportunity.nid)
FROM node AS opportunity
LEFT JOIN field_data_field_cmcv_date AS opportunity_date ON opportunity_date.entity_id = opportunity.nid
WHERE opportunity.type = 'cmcv_volunteer' AND  FROM_UNIXTIME(opportunity_date.field_cmcv_date_value) > now()
");
  $opportunities = $opportunities->fetchField();

  $average = empty($attended) ? 0 : floor(($attended / $registered) * 100) . '%';
  $rows[] = array(
    'data' => array(
      $registered,
      $opportunities,
      $average,
    ),
  );

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content = '<div class="table-container">';
  $content .= theme('table', array('rows' => $rows, 'header' => $header));
  $content .= '</div>';

  // Return everything.
  $block = new stdClass();
//  $block->title = t('Attendance');
  $block->content = $content;

  return $block;
}
