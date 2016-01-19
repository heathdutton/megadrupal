<?php
/**
 * @file
 * Displays information about attendance for a specific opportunity.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Opportunity: Attendance Summary'),
  'description' => t('Displays a chart showing metrics around attendance for a specific opportunity.'),
  'category' => t('CRM Core Volunteer'),
  'edit form' => 'cmcv_opportunity_summary_attendance_form',
  'render callback' => 'cmcv_opportunity_summary_attendance_render',
  'admin info' => 'cmcv_opportunity_summary_attendance_admin_info',
  'defaults' => array(
    'height' => '',
    'color_series' => '',
    'display_labels' => '',
    'include_legend' => '0',
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function cmcv_opportunity_summary_attendance_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Opportunity: Attendance Summary');
  $block->content = t('Displays a chart showing metrics around attendance for a specific opportunity.');

  return $block;
}

/**
 * Settings form.
 */
function cmcv_opportunity_summary_attendance_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a legend'),
    '#description' => t('Check this box to include a legend in your chart.'),
    '#default_value' => !empty($conf['include_legend']) ? $conf['include_legend'] : '0',
    '#return_value' => 1,
  );
  
  $form['include_tooltip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips'),
    '#description' => t('Check this box to display tooltips when hovering over a point in your chart.'),
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 1,
    '#return_value' => 1,
  );  

  // Display settings.
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
  );

  // Height.
  $form['display_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Please enter the height of the chart, as an integer.'),
    '#default_value' => !empty($conf['height']) ? $conf['height'] : '',
  );

  // Display labels.
  $form['display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels'),
    '#description' => t('Check this box to display labels for every region on your chart.'),
    '#default_value' => !empty($conf['display_labels']) ? $conf['display_labels'] : '0',
    '#return_value' => 1,
  );

  // Color series.
  $form['display_settings']['color_series'] = array(
    '#type' => 'textarea',
    '#title' => t('Color series'),
    '#description' => t('Please enter the colors to use for the data series in valid #RRGGBB or rgba format. Please separate each value with a new line. Leave blank to use the default colors.'),
    '#default_value' => !empty($conf['color_series']) ? $conf['color_series'] : '',
  );

  return $form;
}

/**
 * Validate handler for settings form.
 */
function cmcv_opportunity_summary_attendance_form_validate($form, &$form_state) {
  // Height.
  if (!is_numeric($form_state['values']['height']) || $form_state['values']['height'] <= 0) {
    form_set_error('height', t('The height of the chart must be entered as a positive number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function cmcv_opportunity_summary_attendance_form_submit($form, &$form_state) {

  // Legend.
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];
  
  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];  

  // Height.
  $form_state['conf']['height'] = $form_state['values']['height'];

  // Colors.
  $form_state['conf']['color_series'] = $form_state['values']['color_series'];

  // Display labels.
  $form_state['conf']['display_labels'] = $form_state['values']['display_labels'];
}

/**
 * Render callback.
 */
function cmcv_opportunity_summary_attendance_render($subtype, $conf, $panel_args, $context = NULL) {

  // Set up a table for displaying raw data.
  $header = array('Status', 'Percentage');
  $rows = array();
  $total_donors = 0;

  // Storing information that will go into the chart.
  $source_data = array();

  $registered = db_query("
    SELECT COUNT( DISTINCT fdp.entity_id ) AS registered
    FROM field_data_field_activity_participants fdp
    LEFT JOIN field_data_field_cmcv_vo_reference fvo ON fvo.entity_id = fdp.entity_id
    WHERE fvo.field_cmcv_vo_reference_target_id = :nid
    ", array(':nid' => $panel_args[0]));
  $registered = $registered->fetchField();

  $attended = db_query("
    SELECT COUNT(DISTINCT fdp.entity_id) AS attended
    FROM field_data_field_activity_participants fdp
    LEFT JOIN field_data_field_cmcv_vo_reference fvo ON fvo.entity_id = fdp.entity_id
    LEFT JOIN field_data_field_cmcv_signup_status fss ON fdp.entity_id = fss.entity_id
    WHERE fvo.field_cmcv_vo_reference_target_id = :nid
    AND fss.field_cmcv_signup_status_value = 'attended'
    ", array(':nid' => $panel_args[0]));
  $attended = $attended->fetchField();

  $noshow = db_query("
    SELECT COUNT(DISTINCT fdp.entity_id) AS noshow
    FROM field_data_field_activity_participants fdp
    LEFT JOIN field_data_field_cmcv_vo_reference fvo ON fvo.entity_id = fdp.entity_id
    LEFT JOIN field_data_field_cmcv_signup_status fss ON fdp.entity_id = fss.entity_id
    WHERE fvo.field_cmcv_vo_reference_target_id = :nid
    AND fss.field_cmcv_signup_status_value <> 'attended'
    ", array(':nid' => $panel_args[0]));
  $noshow = $noshow->fetchField();

  $rows[] = array(
    'data' => array(
      array(
      	'data' => t('Attended'),
      	'class'=> 'table-field-attendance-status'
      ),
      array(
      	'data' => floor(($attended / $registered) * 100) . '%',
      	'class'=> 'table-field-attendance-percentage'
      ),
    ),
  );
  $rows[] = array(
    'data' => array(
      array(
      	'data' => t('No-show'),
      	'class'=> 'table-field-attendance-status'
      ),
      array(
      	'data' => floor(($noshow / $registered) * 100) . '%',
      	'class'=> 'table-field-attendance-percentage'
      ),
    ),
  );

  $temp = new flotData(array(array(0, $attended)));
  $temp->label = t('Attended');  
  $source_data[] = $temp;

  $temp = new flotData(array(array(0, $noshow)));
  $temp->label = t('No-show');
  $source_data[] = $temp;

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_volunteer_chart_height', CMCV_CHART_HEIGHT) : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_volunteer_series_colors', '') : $conf['color_series'];
  $display_labels = empty($conf['display_labels']) ? variable_get('crm_core_volunteer_display_labels', 1) : $conf['display_labels'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_volunteer_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_volunteer_include_tooltips', 0) : $conf['include_tooltip'];

  // Create a new pie chart.
  $options = new flotStylePie();

  // Create a radius, make it a donut chart.
  $options->series->pie->radius = 1;
  $options->series->pie->innerRadius = 0.6;
  $options->series->useLabel = 1;

  // Add the color series.
  if (!empty($color_series)) {
    $options->colors = explode("\n", $color_series);
  }

  // Add the labels.
  if ($display_labels) {
    $options->series->pie->label->show = TRUE;
    $options->series->pie->label->radius = 2 / 3;
    $options->series->pie->label->threshold = 0.01;
    $options->series->pie->label->backgroundOpacity = 1;
  }
  else {
    $options->series->pie->label->show = FALSE;
  }
  
  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }

  // Display the chart.
  $content = theme('flot_graph', array(
    'data' => $source_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  // Output the table.
  // Adding wrapper block for fancy styling.
  $content .= '<div class="table-container">';
  $content .= theme('table', array('rows' => $rows, 'header' => $header));
  $content .= '</div>';

  // Return everything.
  $block = new stdClass();
  $block->title = t('Attendance');
  $block->content = $content;

  return $block;
}
