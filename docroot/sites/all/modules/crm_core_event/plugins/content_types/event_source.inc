<?php

/**
 * @file
 * Pie chart.
 * Displays signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Event sources'),
  'description' => t('Displays a pie chart showing attendees registration sources.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'event_sources_form',
  'render callback' => 'event_sources_render',
  'admin info' => 'event_sources_admin_info',
  'defaults' => array(
    'height' => '',
    'color_series' => '',
    'display_labels' => 0,
    'include_legend' => 0,
    '#return_value' => 1,
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function event_sources_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Event sources');
  $block->content = t('Displays a pie chart showing attendees registration sources.');

  return $block;
}

/**
 * Settings form.
 */
function event_sources_form($form, &$form_state) {

  $conf = $form_state['conf'];

  $form['include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a legend'),
    '#description' => t('Check this box to include a legend in your chart.'),
    '#default_value' => !empty($conf['include_legend']) ? $conf['include_legend'] : 0,
    '#return_value' => 1,
  );

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

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
    '#default_value' => !empty($conf['display_labels']) ? $conf['display_labels'] : 0,
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
function event_sources_form_validate($form, &$form_state) {

  // Height.
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function event_sources_form_submit($form, &$form_state) {
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
function event_sources_render($subtype, $conf, $panel_args, $context = NULL) {

  $sources = db_query("
SELECT source.field_cmcev_source_value AS source_name,
COUNT(registration.activity_id) AS registrations
FROM {crm_core_activity} AS registration
LEFT JOIN field_data_field_cmcev_source AS source ON source.entity_id = registration.activity_id
WHERE registration.type = 'cmcev_event_registration'
GROUP BY source_name
  ");

  $source_data = array();
  foreach ($sources as $source) {
    $source_data[] = new flotData(array(array(0, $source->registrations)));
    $source_data[count($source_data) - 1]->label = isset($source->source_name) ? check_plain($source->source_name) : 'N/A';
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_event_height', '200') : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_event_series_colors', '') : $conf['color_series'];
  $display_labels = empty($conf['display_labels']) ? variable_get('crm_core_event_display_labels', 0) : $conf['display_labels'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_event_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_event_include_tooltips', 0) : $conf['include_tooltip'];
 
  // Set up the chart.
  $options = new flotStylePie();
  $options->series->pie->radius = 1;
  $options->series->pie->innerRadius = 0.6;
  $options->series->useLabel = true;
  $options->series->suffix = ' registrations';
  
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
  $options->legend = new stdClass();
  $options->legend->show = FALSE;
  $options->legend->position = 'bottom';

  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }
  
  $content = theme('flot_graph', array(
    'data' => $source_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  $block = new stdClass();

  $block->title = t('Sources');
  $block->content = $content;

  return $block;
}
