<?php
/**
 * @file
 * Block displaying metrics table about source.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Source summary metrics'),
  'description' => t('Displays summary metrics for source.'),
  'category' => t('CRM Core Volunteer'),
  'admin info' => 'crm_core_volunteer_source_summary_totals_info',
  'edit form' => 'crm_core_volunteer_source_summary_totals_form',
  'render callback' => 'crm_core_volunteer_source_summary_totals_block',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function crm_core_volunteer_source_summary_totals_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Volunteer: Source Summary Totals');
  $block->content = t('Displays a opportunity summary totals.');

  return $block;
}

/**
 * Settings form.
 */
function crm_core_volunteer_source_summary_totals_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * Validation for settings form.
 */
function crm_core_volunteer_source_summary_totals_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function crm_core_volunteer_source_summary_totals_block($subtype, $conf, $panel_args, $context = NULL) {

  $source = $panel_args[0];
  $content = '';

  $volunteers = db_query("
SELECT
COUNT(entity_id) AS volunteers
FROM {field_data_field_cmcv_source} AS fds
WHERE fds.bundle = 'cmcv_volunteer_commitment'
AND fds.field_cmcv_source_value = :source
", array(':source' => $source))
    ->fetchField();

  $attended = db_query("
SELECT
COUNT(fds.entity_id) AS volunteers
FROM {field_data_field_cmcv_source} AS fds
JOIN {field_data_field_cmcv_signup_status} AS fss ON fds.entity_id = fss.entity_id
WHERE fds.bundle = 'cmcv_volunteer_commitment'
AND fds.field_cmcv_source_value = :source
AND fss.field_cmcv_signup_status_value = 'attended'
", array(':source' => $source))
    ->fetchField();

  $participation_data = db_query("
SELECT
fap.field_activity_participants_target_id AS contact_id,
count(fvo.field_cmcv_vo_reference_target_id) AS opportunities
FROM {field_data_field_cmcv_source} AS fds
JOIN {field_data_field_cmcv_vo_reference} AS fvo ON fds.entity_id = fvo.entity_id
JOIN {field_data_field_activity_participants} AS fap ON fap.entity_id = fvo.entity_id
WHERE fds.bundle = 'cmcv_volunteer_commitment'
AND fds.field_cmcv_source_value = :source
GROUP BY fap.field_activity_participants_target_id
", array(':source' => $source))
    ->fetchAllKeyed();

  $average_participation = array_sum($participation_data) / count($participation_data);

  $block = new stdClass();
  $block->title = '';
  $header = array('Volunteers', 'Average attendance', 'Average participation');
  $rows[0] = array(
  
    array(
      'data' => $volunteers,
    	'class'=> 'table-field-total-volunteers'
    ),      
    array(
      'data' => round($attended / $volunteers * 100, 0) . ' %',
    	'class'=> 'table-field-average-attendance'
    ),
    array(
      'data' => round($average_participation, 1),
    	'class'=> 'table-field-average-participation'
    ),
  );
  
  // Adding wrapper block for fancy styling.
  $content .= '<div class="table-container">';  
  $content .= theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));
  $content .= '</div>';  
  
  $block->content = $content;

  return $block;
}
