<?php
/**
 * @file
 * Bar chart
 * Displays information about sources.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Source Chart'),
  'description' => t('Displays a chart showing all sources for event registrations.'),
  'category' => t('CRM Core Event'),
  'admin info' => 'sources_chart_admin_info',
  'edit form' => 'sources_chart_form',
  'render callback' => 'sources_chart_render',
  'defaults' => array(
    'height' => '',
    'color_series' => '',
    'display_labels' => 0,
    'chart_color' => '',
    'column_width' => '',
    'include_legend' => 0,
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function sources_chart_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Source Chart');
  $block->content = t('Displays a chart showing all sources for event registrations.');

  return $block;
}

/**
 * Settings form.
 */
function sources_chart_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a legend'),
    '#description' => t('Check this box to include a legend in your chart.'),
    '#default_value' => !empty($conf['include_legend']) ? $conf['include_legend'] : 0,
    '#return_value' => 1,
  );

  // Display settings.
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
  );
  
  $form['include_tooltip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips'),
    '#description' => t('Check this box to display tooltips when hovering over a point in your chart.'),
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 0,
    '#return_value' => 1,
  );  

  // Height.
  $form['display_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Please enter the height of the chart, as an integer.'),
    '#default_value' => !empty($conf['height']) ? $conf['height'] : '',
  );

  $form['display_settings']['column_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Column width'),
    '#description' => t('Please enter the default width of columns, as a decimal. Example: 0.8.'),
    '#default_value' => !empty($conf['column_width']) ? $conf['column_width'] : '',
  );

  // Display labels.
  $form['display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels'),
    '#description' => t('Check this box to display labels for every region on your chart.'),
    '#default_value' => !empty($conf['display_labels']) ? $conf['display_labels'] : 0,
    '#return_value' => 1,
  );

  // Chart color.
  $form['display_settings']['chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['chart_color']) ? $conf['chart_color'] : '',
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
 * Validation handler for settings form.
 */
function sources_chart_form_validate($form, &$form_state) {

  // Height.
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * Submit handler.
 */
function sources_chart_form_submit($form, &$form_state) {
  // Legend.
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // Height.
  $form_state['conf']['height'] = $form_state['values']['height'];
  $form_state['conf']['column_width'] = $form_state['values']['column_width'];

  // Colors.
  $form_state['conf']['color_series'] = $form_state['values']['color_series'];

  // Display labels.
  $form_state['conf']['display_labels'] = $form_state['values']['display_labels'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
}

/**
 * Render callback.
 */
function sources_chart_render($subtype, $conf, $panel_args, $context = NULL) {

  // Create some default containers for data.
  $source_data = array();
  $source_labels = array();
  $count = 0;

  // Select all the sources for petition signatures.
  $sources = db_query("
  	SELECT ers.field_cmcev_source_value AS source,
COUNT(registration.activity_id) AS registrations
FROM {crm_core_activity} AS registration
LEFT JOIN field_data_field_cmcev_source AS ers ON ers.entity_id = registration.activity_id
WHERE registration.type = 'cmcev_event_registration'
GROUP BY source
ORDER BY source ASC;
");

  // Prepare the data.
  foreach ($sources as $source) {
    $source_label = isset($source->source) ? check_plain($source->source) : 'N/A';
    $item = new flotData(array(array($count, $source->registrations)));
    $source_labels[] = array($count, $source_label);
    $item->label = $source_label;
    $source_data[] = $item;
    $count++;
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_event_height', '200') : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_event_series_colors', '') : $conf['color_series'];
  $display_labels = empty($conf['display_labels']) ? variable_get('crm_core_event_display_labels', 0) : $conf['display_labels'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_event_chart_color', '') : $conf['chart_color'];
  $column_width = empty($conf['column_width']) ? variable_get('crm_core_event_column_width', '0.8') : $conf['column_width'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_event_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_event_include_tooltips', 0) : $conf['include_tooltip'];

  // Set up the chart.
  $options = new flotStyleBar();

  $options->legend = new stdClass();
  $options->legend->show = FALSE;
  $options->legend->position = 'bottom';
  $options->series->suffix = ' registrations';

  $options->xaxis = new stdClass();
  $options->xaxis->ticks = $source_labels;
  $options->xaxis->autoscaleMargin = .10;

  $options->bars = new stdClass();
  if (!empty($column_width)) {
    $options->bars->barWidth = (float) $column_width;
  }
  $options->bars->align = 'center';
  $options->bars->lineWidth = 2;

  $options->grid->labelMargin = 12;
  $options->grid->minBorderMargin = 12;
  $options->grid->borderWidth = 2;
  $options->grid->labelMargin = 8;
  if (!empty($chart_color)) {
    $options->grid->backgroundColor = $chart_color;
  }

  /* colors */
  if ($color_series) {
    $options->colors = explode("\n", $color_series);
  }

  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }  
  
  // Output the chart.
  $content = theme('flot_graph', array(
    'data' => $source_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  $block = new stdClass();
  $block->title = t('Registration sources');
  $block->content = $content;

  return $block;
}
