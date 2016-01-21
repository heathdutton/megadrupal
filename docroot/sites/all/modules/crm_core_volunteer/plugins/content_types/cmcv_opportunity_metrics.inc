<?php
/**
 * @file
 * Displays opportunity attendance table.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Opportunity metrics'),
  'description' => t('Displays a table with opportunity metrics.'),
  'category' => t('CRM Core Volunteer'),
  'render callback' => 'cmcv_oppotunity_metrics_render',
  'admin info' => 'cmcv_oppotunity_metrics_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function cmcv_oppotunity_metrics_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Opportunity metrics');
  $block->content = t('Displays a table with opportunity metrics.');

  return $block;
}

/**
 * Render callback.
 */
function cmcv_oppotunity_metrics_render($subtype, $conf, $panel_args, $context = NULL) {

  $nid = $panel_args[0];

  $header = array(t('Date'), t('Status'), t('Volunteers'), t('Attendance'));
  $rows = array();

  $opportunity = node_load($nid);
  $ow = entity_metadata_wrapper('node', $opportunity);
  $start_date = $ow->field_cmcv_date->value->value();
  $end_date = $ow->field_cmcv_date->value2->value();
  if (REQUEST_TIME < $start_date) {
    $status = t('Scheduled');
  }
  elseif (REQUEST_TIME > $end_date) {
    $status = t('Completed');
  }
  else {
    $status = t('In progress');
  }

  $query = new EntityFieldQuery();
  $volunteers = $query->entityCondition('entity_type', 'crm_core_activity')
    ->entityCondition('bundle', 'cmcv_volunteer_commitment')
    ->fieldCondition('field_cmcv_vo_reference', 'target_id', $nid)
    ->count()
    ->execute();

  $attended = $query->entityCondition('entity_type', 'crm_core_activity')
    ->entityCondition('bundle', 'cmcv_volunteer_commitment')
    ->fieldCondition('field_cmcv_vo_reference', 'target_id', $nid)
    ->fieldCondition('field_cmcv_signup_status', 'value', 'attended')
    ->count()
    ->execute();
  $attended = (int) $attended;

  $attendance = empty($attended) ? '0%' : floor(($attended / $volunteers) * 100) . '%';

  $rows[] = array(
    'data' => array(
      format_date($start_date, 'long'),
      $status,
      $volunteers,
      $attendance,
    ),
  );

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content = '<div class="table-container">';
  $content .= theme('table', array('rows' => $rows, 'header' => $header));
  $content .= '</div>';

  $block = new stdClass();

  $block->title = '';
  $block->content = $content;

  return $block;
}
