<?php

/**
 * @file
 * Pie chart.
 * Displays a chart containing donation totals broken out by source.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Volunteer: Volunteer Sources'),
  'description' => t('Displays a pie chart detailing the sources of volunteers.'),
  'category' => t('CRM Core Volunteer'),
  'edit form' => 'cmcv_source_overview_form',
  'render callback' => 'cmcv_source_overview_render',
  'admin info' => 'cmcv_source_overview_admin_info',
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
function cmcv_source_overview_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Volunteer: Volunteer Sources');
  $block->content = t('Displays a pie chart detailing the sources of volunteers.');

  return $block;
}

/**
 * Settings form.
 */
function cmcv_source_overview_form($form, &$form_state) {
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
 * Validation for settings form.
 */
function cmcv_source_overview_form_validate($form, &$form_state) {
  // Height.
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a positive number (i.e. 200).'));
  }
}

/**
 * Validation for settings form.
 */
function cmcv_source_overview_form_submit($form, &$form_state) {

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
function cmcv_source_overview_render($subtype, $conf, $panel_args, $context = NULL) {

  $sources = db_query("
  SELECT COUNT(activity.activity_id) AS volunteers,
  fds.field_cmcv_source_value AS source
  FROM {crm_core_activity} AS activity
  JOIN {field_data_field_cmcv_source} AS fds ON activity.activity_id = fds.entity_id
  WHERE activity.type = 'cmcv_volunteer_commitment'
  GROUP BY source
  ORDER BY source;
  ");

  $source_data = array();
  foreach ($sources as $source) {
    $source_data[] = new flotData(array(array(0, $source->volunteers)));
    $source_data[sizeof($source_data) - 1]->label = $source->source;
  }

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
  $options->series->pie->innerRadius = 0.5;
  $options->series->useLabel = true;
  $options->series->suffix = ' Registrations';
  
  // Add the color series.
  if (!empty($color_series)) {
    $options->colors = explode("\n", $color_series);
  }

  // Add the labels.
  if (!empty($display_labels)) {
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

  $block = new stdClass();
  $block->title = t('Volunteer Sources');
  $block->content = $content;

  return $block;
}
