<?php
/**
 * @file
 * Bar chart displaying information about sources
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Petitions: Source Chart'),
  'description' => t('Displays a chart showing all sources for petition signatures.'),
  'category' => t('CRM Core Petition'),
  'admin info' => 'crm_core_petition_source_chart_info',
  'edit form' => 'crm_core_petition_source_chart_form',
  'render callback' => 'crm_core_petition_source_chart',
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
 * admin info
 */
function crm_core_petition_source_chart_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Petitions: Source Chart');
  $block->content = t('Displays a chart showing sources for all signature activity.');

  return $block;
}

/**
 * Settings form
 */
function crm_core_petition_source_chart_form($form, &$form_state) {
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
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 0,
    '#return_value' => 1,
  );
  
  // display settings
  $form['display_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
  );

  // height
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

  // display labels
  $form['display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels'),
    '#description' => t('Check this box to display labels for every region on your chart.'),
    '#default_value' => !empty($conf['display_labels']) ? $conf['display_labels'] : 0,
    '#return_value' => 1,
  );

  // chart color
  $form['display_settings']['chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['chart_color']) ? $conf['chart_color'] : '',
  );

  // color series
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
function crm_core_petition_source_chart_form_validate($form, &$form_state) {
  // Height.
  if (!is_numeric($form_state['values']['height']) || $form_state['values']['height'] <= 0) {
    form_set_error('height', t('The height of the chart must be entered as a positive number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function crm_core_petition_source_chart_form_submit($form, &$form_state) {
  // legend
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // height
  $form_state['conf']['height'] = $form_state['values']['height'];
  $form_state['conf']['column_width'] = $form_state['values']['column_width'];

  // colors
  $form_state['conf']['color_series'] = $form_state['values']['color_series'];

  // display labels
  $form_state['conf']['display_labels'] = $form_state['values']['display_labels'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
}

/**
 * Render callback
 */
function crm_core_petition_source_chart($subtype, $conf, $panel_args, $context = NULL) {

  // create some default containers for data
  $sourceData = array();
  $sourceLabels = array();
  $items = array();
  $count = 0;

  // select all the sources for petition signatures
  $sources = db_query("
  	SELECT 
  		COUNT(entity_id) AS amount, 
  		fds.field_cmcp_source_value AS source
  	FROM {field_data_field_cmcp_source} fds
		GROUP BY source 
		ORDER BY source;");

  // prepare the data
  foreach ($sources as $source) {
    $item = new flotData(array(array($count, $source->amount)));
    $sourceLabels[] = array($count, $source->source);
    $item->label = $source->source;
    $sourceData[] = $item;
    $count++;
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_petition_height', CMCP_CHART_HEIGHT) : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_petition_series_colors', '') : $conf['color_series'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_petition_chart_color', '') : $conf['chart_color'];
  $column_width = empty($conf['column_width']) ? variable_get('crm_core_petition_column_width', CMCP_COLUMN_WIDTH) : $conf['column_width'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_petition_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_petition_include_tooltips', 0) : $conf['include_tooltip'];

  // create a new bar chart
  $options = new flotStyleBar();

  // add labels to the x-axis
  $options->xaxis = new stdClass();
  $options->xaxis->ticks = $sourceLabels;
  $options->xaxis->autoscaleMargin = .10;
  $options->series->suffix = ' signatures';

  // need to add a bar width
  $options->bars = new stdClass();
  if (!empty($column_width)) {
    $options->bars->barWidth = (float) $column_width;
  }
  $options->bars->align = 'center';
  $options->bars->lineWidth = 2;

  /* grid */
  $options->grid->borderWidth = 2;
  $options->grid->labelMargin = 8;
  if (!empty($chart_color)) {
    $options->grid->backgroundColor = $chart_color;
  }
  $options->grid->minBorderMargin = 12;

  /* colors */
  if (!empty($color_series)) {
    $options->colors = explode("\n", $color_series);
  }
  
  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }
  
  $content = theme('flot_graph', array(
    'data' => $sourceData,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  $block = new stdClass();
  $block->title = 'Signature Sources';
  $block->content = $content;

  return $block;
}
