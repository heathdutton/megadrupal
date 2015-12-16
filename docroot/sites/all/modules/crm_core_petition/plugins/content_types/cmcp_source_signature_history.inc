<?php

/**
 * @file
 * Line chart displaying signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Petitions: Source Summary Signature History'),
  'description' => t('Displays a line chart showing signature history.'),
  'category' => t('CRM Core Petition'),
  'edit form' => 'cmcp_source_signature_history_form',
  'render callback' => 'cmcp_source_signature_history_render',
  'admin info' => 'cmcp_source_signature_history_admin_info',
  'defaults' => array(
    'text' => '',
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * admin info
 */
function cmcp_source_signature_history_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Petitions: Source Summary Signature History');
  $block->content = t('Displays a line chart showing signature history over a specified period. Defaults to 90 days in the past.');
  return $block;
}

/**
 * Settings form.
 */
function cmcp_source_signature_history_form($form, &$form_state) {

  // this widget should allow users to set the timeframe for performance
  $conf = $form_state['conf'];
  $form['time_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Period'),
    '#description' => t('Enter the number of days to report back on for this widget. If no value is provided, defaults to 90.'),
    '#default_value' => !empty($conf['time_period']) ? $conf['time_period'] : '90',
  );
  
  $form['include_tooltip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips'),
    '#description' => t('Check this box to display tooltips when hovering over a point in your chart.'),
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 0,
    '#return_value' => 1,
  );
    
  return $form;
}

/**
 * Submit handler for settings form.
 */
function cmcp_source_signature_history_form_submit($form, &$form_state) {
  $form_state['conf']['time_period'] = $form_state['values']['time_period'];
  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

}

/**
 * Render callback.
 */
function cmcp_source_signature_history_render($subtype, $conf, $panel_args, $context = NULL) {

  // get the number of days to report back
  $days = $conf['time_period'];

  $source = $panel_args[0];

  // run the query
  $dates = db_query("
  SELECT COUNT(fdd.entity_id) AS amount,
  	UNIX_TIMESTAMP(field_cmcp_received_date_value) AS timestamp,
  	DATE_FORMAT(field_cmcp_received_date_value, '%b %e') AS rdate,
  	DATE_FORMAT(field_cmcp_received_date_value, '%j') AS doy
  	FROM {field_data_field_cmcp_received_date} fdd
  	JOIN {field_data_field_cmcp_source} fds
  	  ON fds.entity_id = fdd.entity_id
  	WHERE fdd.field_cmcp_received_date_value > SUBDATE(NOW(), INTERVAL $days DAY)
  	  AND fdd.field_cmcp_received_date_value != ''
  	  AND fdd.field_cmcp_received_date_value IS NOT NULL
  	  AND fds.field_cmcp_source_value = :source
  	GROUP BY rdate, doy 
  	ORDER BY timestamp, rdate, doy DESC;
	", array(':source' => $source));

  // normalize the query for zero values, avoid big arcs
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
  
  // draw the chart
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
  $options->series->seriesLabels = $seriesLabels;
  $options->series->lines = new stdClass();
  $options->series->lines->show = true;
  $options->series->lines->fill = TRUE;
  $options->series->lines->lineWidth = 4;
  $options->series->suffix = ' signatures';  
  
  // add a background for the series
  if (!empty($background_color)) {
    $options->series->lines->fillColor = array(
      'colors' => explode("\n", $background_color),
    );
  }
  
  /* points */
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;

  /* grid */
  $options->grid->borderWidth = 2;
  $options->grid->labelMargin = 8;
  if (!empty($chart_color)) {
    $options->grid->backgroundColor = $chart_color;
  }
  
  /* xaxis */
  $options->xaxis = new stdClass();
  $options->xaxis->mode = "time";
  $options->xaxis->ticks = (int) $xaxis_tickmarks;
  
  /* yaxis */
  $options->yaxis = new stdClass();
  $options->yaxis->min = 0;

  // add colors to the line
  if (!empty($line_color)) {
    $options->colors = array(
      $line_color,
    );
  }
  
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }
    
  $content = theme('flot_graph', array(
  	'data' => $dateData, 
  	'options'	=> $options,
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

