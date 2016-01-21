<?php
/**
 * @file
 * Pie chart.
 * Displays a chart containing donation totals broken out by source.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Donations: Donations by Source Chart'),
  'description' => t('Displays a pie chart breaking down donations by source.'),
  'category' => t('CRM Core Donations'),
  'admin info' => 'crm_core_donation_donation_by_source_chart_info',
  'edit form' => 'crm_core_donation_donation_by_source_chart_form',
  'render callback' => 'crm_core_donation_donation_by_source_chart',
  'defaults' => array(
    'height' => '',
    'color_series' => '',
    'display_labels' => 0,
    'include_legend' => 0,
    'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function crm_core_donation_donation_by_source_chart_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Donations: Donations by Source Chart');
  $block->content = t('Displays a pie chart breaking down donations by source.');

  return $block;
}

/**
 * Settings form.
 */
function crm_core_donation_donation_by_source_chart_form($form, &$form_state) {
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
 * Submit handler for settings form.
 */
function crm_core_donation_donation_by_source_chart_form_validate($form, &$form_state) {

  // Height.
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function crm_core_donation_donation_by_source_chart_form_submit($form, &$form_state) {

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
function crm_core_donation_donation_by_source_chart($subtype, $conf, $panel_args, $context = NULL) {

  // Get a list of sources along with total amounts raised.
  $sources = db_query("SELECT SUM(fda.field_cmcd_amount_value) AS amount, fds.field_cmcd_source_value AS source
  	FROM {field_data_field_cmcd_amount} fda JOIN {field_data_field_cmcd_source} fds
  	ON fda.entity_id = fds.entity_id GROUP BY source ORDER BY source;");

  // Set up a structure for displaying the data.
  $source_data = array();
  foreach ($sources as $source) {
    $source_data[] = new flotData(array(array(0, $source->amount)));
    $source_data[count($source_data) - 1]->label = $source->source;
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_donation_height', '200') : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_donation_series_colors', '') : $conf['color_series'];
  $display_labels = $conf['display_labels'] == 0 ? variable_get('crm_core_donation_display_labels', 1) : $conf['display_labels'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_donation_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_donation_include_tooltips', 0) : $conf['include_tooltip'];
  
  // get the currency symbol for this chart
  $cur = commerce_currency_load();
  $sym = $cur['symbol'];
  
  // Create a new pie chart.
  $options = new flotStylePie();

  // Create a radius, make it a donut chart.
  $options->series->pie->radius = 1;
  $options->series->pie->innerRadius = 0.5;
  $options->series->prefix = $sym;
  
  // Add the color series.
  if (!empty($color_series)) {
    $options->colors = explode("\n", $color_series);
  }

  // Add the labels.
  if ($display_labels !== 0) {
    $options->series->pie->label->show = TRUE;
    $options->series->pie->label->radius = 2 / 3;
    $options->series->pie->label->threshold = 0.01;
    $options->series->pie->label->backgroundOpacity = 1;
  } else {
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
  $block->title = t('Donations by Source');
  $block->content = $content;

  return $block;
}
