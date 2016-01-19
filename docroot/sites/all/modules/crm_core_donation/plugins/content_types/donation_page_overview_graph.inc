<?php
/**
 * @file
 * Bar chart.
 * Displays a chart, detailing amounts raised for each donation page.
 */
// create the ctools plugin
$plugin = array(
  'single' => TRUE,
  'title' => t('Donations by Page: Donation Page Overview graph'),
  'description' => t('Displays a chart showing a breakdown of donation amounts for each online donation page.'),
  'category' => t('CRM Core Donations'),
  'edit form' => 'crm_core_donation_page_overview_chart_form',
  'render callback' => 'crm_core_donation_page_overview_chart',
  'admin info' => 'crm_core_donation_page_overview_chart_info',
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
 * Information for the panels administrative screen.
 */
function crm_core_donation_page_overview_chart_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Donations by Page: Donation Page Overview graph');
  $block->content = t('Displays a chart showing a breakdown of donation amounts for each online donation page.');

  return $block;
}

/**
 * Settings form for this plug in
 */
function crm_core_donation_page_overview_chart_form($form, &$form_state) {
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
 * submit handler for settings form
 */
function crm_core_donation_page_overview_chart_form_validate($form, &$form_state) {

  // height
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * submit handler for settings form
 */
function crm_core_donation_page_overview_chart_form_submit($form, &$form_state) {

  // legend
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // height
  $form_state['conf']['height'] = $form_state['values']['height'];
  $form_state['conf']['column_width'] = $form_state['values']['column_width'];

  // colors
  $form_state['conf']['color_series'] = $form_state['values']['color_series'];

  // Chart color.
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
}

/**
 * Render callback
 */
function crm_core_donation_page_overview_chart($subtype, $conf, $panel_args, $context = NULL) {

  // set up the chart
  $source_data = array();
  $source_labels = array();
  $items = array();
  $count = 0;

  // get information about the pages
  $pages = db_query("
  	SELECT nid, title,
    	COALESCE(SUM(fda.field_cmcd_amount_value), 0) amount
    FROM {node} 
    LEFT JOIN {field_data_field_cmcd_page} fdp
      ON fdp.field_cmcd_page_target_id = node.nid
    LEFT JOIN {field_data_field_cmcd_amount} fda
      ON fda.entity_id = fdp.entity_id 
    WHERE type = 'cmcd_page'
      GROUP BY nid, title");

  // populate the chart
  foreach ($pages as $page) {
    $item = new flotData(array(array($count, $page->amount)));
    $item->label = $page->title;
    $source_data[] = $item;
    $source_labels[] = array($count, $page->title);
    $count++;
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_donation_height', '200') : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_donation_series_colors', '') : $conf['color_series'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_donation_chart_color', '') : $conf['chart_color'];
  $column_width = empty($conf['column_width']) ? variable_get('crm_core_donation_column_width', '0.8') : $conf['column_width'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_donation_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_donation_include_tooltips', 0) : $conf['include_tooltip'];
  
  // get the currency symbol for this chart
  $cur = commerce_currency_load();
  $sym = $cur['symbol'];

  // create a new bar chart
  $options = new flotStyleBar();

  // add labels to the x-axis
  $options->xaxis = new stdClass();
  $options->xaxis->ticks = $source_labels;
  $options->xaxis->autoscaleMargin = .10;
  $options->series->prefix = $sym;
  
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

  // output the chart
  $content = theme('flot_graph', array(
    'data' => $source_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  $block = new stdClass();
  $block->title = '';
  $block->content = $content;

  return $block;
}
