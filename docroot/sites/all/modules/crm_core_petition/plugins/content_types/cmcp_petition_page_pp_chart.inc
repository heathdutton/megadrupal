<?php

/**
 * @file
 * Bar chart
 * Chart displaying signatures for a given page.
 */

// create the plugin
$plugin = array(
  'single' => TRUE,
  'title' => t('Petition Page: Performance Chart'),
  'description' => t('Displays a chart showing signature activity for a specific petition page.'),
  'category' => t('CRM Core Petition'),
  'admin info' => 'crm_core_petition_page_pp_chart_info',
  'edit form' => 'crm_core_petition_page_pp_chart_form',
  'render callback' => 'crm_core_petition_page_pp_chart',
  'defaults' => array(
    'height' => '',
    'xaxis_tickmarks' => '',
    'chart_color' => '',
    'line_color' => '',
    'background_color' => '',
    'include_legend' => 0,
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * admin info
 */
function crm_core_petition_page_pp_chart_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Petition Page: Performance Chart');
  $block->content = t('Displays a chart showing signature activity for a specific petition page.');

  return $block;
}

/**
 * Settings form
 */
function crm_core_petition_page_pp_chart_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['time_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Period'),
    '#description' => t('Enter the number of days to report back on for this widget. If no value is provided, defaults to 90. You can enter the word \'All\' to display all values.'),
    '#default_value' => !empty($conf['time_period']) ? $conf['time_period'] : '90',
  );

  $form['include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a legend'),
    '#description' => t('Check this box to include a legend in your chart.'),
    '#default_value' => !empty($conf['include_legend']) ? $conf['include_legend'] : 0,
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

  // line color
  $form['display_settings']['line_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Line color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['line_color']) ? $conf['line_color'] : '',
  );

  // chart color
  $form['display_settings']['chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['chart_color']) ? $conf['chart_color'] : '',
  );

  // series background color
  $form['display_settings']['background_color'] = array(
    '#type' => 'textarea',
    '#title' => t('Background color'),
    '#description' => t('Please enter the color to use for the background of the series in valid #RRGGBB or rgba format. Leave blank to use the default color. You can use gradients for this, separate each value with a new line.'),
    '#default_value' => !empty($conf['background_color']) ? $conf['background_color'] : '',
  );

  $form['display_settings']['xaxis'] = array(
    '#type' => 'fieldset',
    '#title' => t('X-axis settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['display_settings']['xaxis']['xaxis_tickmarks'] = array(
    '#type' => 'textfield',
    '#title' => t('Tick marks'),
    '#description' => t('Please enter the number of tickmarks to display on the xaxis.'),
    '#default_value' => !empty($conf['xaxis_tickmarks']) ? $conf['xaxis_tickmarks'] : '',
  );

  return $form;
}

/**
 * Validate handler for settings form.
 */
function crm_core_petition_page_pp_chart_form_validate($form, &$form_state) {
  // Timeframe.
  if (!is_numeric($form_state['values']['time_period']) && $form_state['values']['time_period'] !== 'All') {
    form_set_error('time_period', t('The number of days must be entered as a number (i.e. 90) or as the word \'All\'.'));
  }

  // X-axis
  if (!is_numeric($form_state['values']['xaxis_tickmarks']) || $form_state['values']['xaxis_tickmarks'] <= 0) {
    form_set_error('xaxis_tickmarks', t('The number of tickmarks for the xaxis must be entered as a positive number (i.e. 7).'));
  }

  // Height.
  if (!is_numeric($form_state['values']['height']) || $form_state['values']['height'] <= 0) {
    form_set_error('height', t('The height of the chart must be entered as a positive number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function crm_core_petition_page_pp_chart_form_submit($form, &$form_state) {
  // timeframe
  $form_state['conf']['time_period'] = $form_state['values']['time_period'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // legend
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // height
  $form_state['conf']['height'] = $form_state['values']['height'];

  // xaxis
  $form_state['conf']['xaxis_tickmarks'] = $form_state['values']['xaxis_tickmarks'];

  // colors
  $form_state['conf']['line_color'] = $form_state['values']['line_color'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
  $form_state['conf']['background_color'] = $form_state['values']['background_color'];
}

/**
 * Render callback
 */
function crm_core_petition_page_pp_chart($subtype, $conf, $panel_args, $context = NULL) {

  // this will display when donors have come into the system

  // this will display a line chart of activity for the current petition page
  // need to get a context from the page

  // set up the chart
  $sourceData = array();
  $sourceLabels = array();
  $items = array();
  $count = 0;

  // okay, we are making a historical line chart
  // this will display all funding from this source from 
  $sql = "
  SELECT 
  	COUNT(fdr.entity_id) AS amount, 
  	UNIX_TIMESTAMP(fdr.field_cmcp_received_date_value) AS timestamp,
  	DATE_FORMAT(fdr.field_cmcp_received_date_value, '%b %e') AS rdate,
  	DATE_FORMAT(fdr.field_cmcp_received_date_value, '%j') AS doy
	FROM {field_data_field_cmcp_received_date} fdr 
  	JOIN {field_data_field_cmcp_petition_page} fpp
  	ON fdr.entity_id = fpp.entity_id
	WHERE fdr.field_cmcp_received_date_value != ''
	  AND fdr.field_cmcp_received_date_value IS NOT NULL";

  if (isset($panel_args[0])) {
    $sql .= " AND fpp.field_cmcp_petition_page_target_id = :page";
  }

  $sql .= " GROUP BY rdate, doy ORDER BY timestamp, rdate, doy DESC;";

  $args = array();
  if (isset($panel_args[0])) {
    $args = array(':page' => $panel_args[0]);
  }

  $dates = db_query($sql, $args);

  // okay, we need to include some zero values using doy from the query
  $dateItem = array();
  $lastDate = '';
  $lastDateTS = '';
  $seriesLabels = array();
  foreach ($dates as $date) {
    // check if today is greater than lastdate
    if (($date->doy - $lastDate) > 1 && $lastDate != '') {
      // insert some zero values for yesterday and the day after lastdate
      $dateItem[] = array($lastDateTS * 1000 + 86400 * 1000, 0);
      $seriesLabels[] = '---';
      // add one for the day before the current record as well
      $dateItem[] = array(($date->timestamp * 1000) - 86400 * 1000, 0);
      $seriesLabels[] = '---';
    }
    // add the date
    // $dateItem[] = array($date->timestamp*1000, $date->amount);
    $dateItem[] = array($date->timestamp * 1000, $date->amount);
    // record a label
    $seriesLabels[] = $date->rdate . '<br>' . $date->amount;
    // set lastdate
    $lastDate = $date->doy;
    $lastDateTS = $date->timestamp;
  }
  $dateData[] = new flotData($dateItem);

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_petition_height', CMCP_CHART_HEIGHT) : $conf['height'];
  $xaxis_tickmarks = empty($conf['xaxis_tickmarks']) ? variable_get('crm_core_petition_xaxis_tickmarks', CMCP_XAXIS_TICKS) : $conf['xaxis_tickmarks'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_petition_chart_color', '') : $conf['chart_color'];
  $line_color = empty($conf['line_color']) ? variable_get('crm_core_petition_line_color', '') : $conf['line_color'];
  $background_color = empty($conf['background_color']) ? variable_get('crm_core_petition_background_color', '') : $conf['background_color'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_petition_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_petition_include_tooltips', 0) : $conf['include_tooltip'];
  
  
  $options = new flotStyleLine();

  // add points to the chart
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;
  $options->series->suffix = ' signatures';

  // add lines to the chart
  $options->series->lines = new stdClass();
  $options->series->lines->show = TRUE;
  $options->series->lines->fill = TRUE;
  $options->series->lines->lineWidth = 4;

  // add colors to the line
  if (!empty($line_color)) {
    $options->colors = array(
      $line_color,
    );
  }

  // add a background for the series
  if (!empty($background_color)) {
    $options->series->lines->fillColor = array(
      'colors' => explode("\n", $background_color),
    );
  }

  // add a grid
  $options->grid->borderWidth = 2;
  $options->grid->labelMargin = 8;
  if (!empty($chart_color)) {
    $options->grid->backgroundColor = $chart_color;
  }

  // add an x-axis
  $options->xaxis = new stdClass();
  $options->xaxis->mode = "time";
  $options->xaxis->ticks = (int) $xaxis_tickmarks;

  // add a y-axis
  $options->yaxis = new stdClass();
  $options->yaxis->min = 0;
  $options->series->seriesLabels = $seriesLabels;

  // add labels to the chart
  $options->seriesLabels = $seriesLabels;

  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }

  $content = theme('flot_graph', array(
    'data' => $dateData,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  $block = new stdClass();
  $block->title = t('Signature History');
  $block->content = $content;

  return $block;
}
