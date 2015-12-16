<?php
/**
 * @file
 * Displays opportunity attendance table.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Opportunity attendance'),
  'description' => t('Displays a table with opportunity attendance.'),
  'category' => t('CRM Core Volunteer'),
  'render callback' => 'cmcv_opportunity_attendance_render',
  'admin info' => 'cmcv_opportunity_attendance_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function cmcv_opportunity_attendance_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Opportunity attendance');
  $block->content = t('Displays a table with opportunity attendance.');

  return $block;
}

/**
 * Render callback.
 */
function cmcv_opportunity_attendance_render($subtype, $conf, $panel_args, $context = NULL) {
  $content = '';

  $opportunities = db_query("
SELECT opportunity.nid , opportunity.title,
COUNT(registration.entity_id) AS registrations
FROM {node} AS opportunity
LEFT JOIN {field_data_field_cmcv_vo_reference} AS registration ON registration.field_cmcv_vo_reference_target_id = opportunity.nid
LEFT JOIN {field_data_field_cmcv_date} AS opportunity_date ON opportunity_date.entity_id = opportunity.nid
WHERE opportunity.type = 'cmcv_volunteer'
GROUP BY opportunity.nid
ORDER BY opportunity_date.field_cmcv_date_value2 DESC
LIMIT 0, 10
");

  $attended = db_query("
SELECT opportunity.nid,
COUNT(registration.entity_id) AS attendees
FROM {node} as opportunity
LEFT JOIN {field_data_field_cmcv_vo_reference} AS registration ON registration.field_cmcv_vo_reference_target_id = opportunity.nid
LEFT JOIN {field_data_field_cmcv_signup_status} AS status ON status.entity_id = registration.entity_id
WHERE opportunity.type = 'cmcv_volunteer'
AND (status.field_cmcv_signup_status_value = 'attended' )
");
  $attended = $attended->fetchAllKeyed();

  $header = array(t('Opportunity'), t('Registered'), t('Attended'));
  $rows = array();

  foreach ($opportunities as $opportunity) {

    $rows[] = array(
      'data' => array(
        array(
        	'data' => l(check_plain($opportunity->title), 'crm-core/reports/cmcv/opportunities/summary/' . $opportunity->nid),
        	'class'=> 'table-field-opportunity-title'
        ),
        array(
        	'data' => $opportunity->registrations,
        	'class'=> 'table-field-attendance-registrations'
        ),
        array(
        	'data' => isset($attended[$opportunity->nid]) ? $attended[$opportunity->nid] : 0,
        	'class'=> 'table-field-attendance-percent'
        ),
      ),
    );
  }

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content .= '<div class="table-container">';
  $content .= theme('table', array('rows' => $rows, 'header' => $header));
  $content .= '</div>';

  $block = new stdClass();

  $block->title = t('Attendance');
  $block->content = $content;

  return $block;
}
