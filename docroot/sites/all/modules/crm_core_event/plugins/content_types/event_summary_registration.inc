<?php

/**
 * @file
 * Line chart displaying signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Event summary registration'),
  'description' => t('Displays a line chart showing attendees registration history.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'event_summary_registration_form',
  'render callback' => 'event_summary_registration_render',
  'admin info' => 'event_summary_registration_admin_info',
  'defaults' => array(
    'time_period' => '90',
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
 * Administration info.
 */
function event_summary_registration_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Event summary registration');
  $block->content = t('Displays a line chart showing attendees registration history.');

  return $block;
}

/**
 * Settings form.
 */
function event_summary_registration_form($form, &$form_state) {

  // This widget should allow users to set the timeframe for performance.
  $conf = $form_state['conf'];

  // This widget should allow users to set the timeframe for performance.
  $description = "Enter the number of days to report back on for this widget.'
    . ' If no value is provided, defaults to 90. You can enter the word 'All' to display all values.";
  $form['time_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Period'),
    '#description' => t($description),
    '#default_value' => !empty($conf['time_period']) ? $conf['time_period'] : '90',
  );

  $form['include_tooltip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips'),
    '#description' => t('Check this box to display tooltips when hovering over a point in your chart.'),
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 0,
    '#return_value' => 1,
  );

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

  // Height.
  $form['display_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Please enter the height of the chart, as an integer.'),
    '#default_value' => !empty($conf['height']) ? $conf['height'] : '',
  );

  // Line color.
  $form['display_settings']['line_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Line color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['line_color']) ? $conf['line_color'] : '',
  );

  // Chart color.
  $form['display_settings']['chart_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart color'),
    '#description' => t('Please enter the color to use for the line in valid #RRGGBB or rgba format. Leave blank to use the default color.'),
    '#default_value' => !empty($conf['chart_color']) ? $conf['chart_color'] : '',
  );

  // Series background color.
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
 * Validation handler for settings form.
 */
function event_summary_registration_form_validate($form, &$form_state) {

  // Timeframe.
  if (!is_numeric($form_state['values']['time_period']) && $form_state['values']['time_period'] !== 'All') {
    $message = "The number of days must be entered as a number (i.e. 90) or as the word 'All'.";
    form_set_error('time_period', t($message));
  }

  // Settings for xaxis.
  if (!empty($form_state['values']['xaxis_tickmarks']) && !is_numeric($form_state['values']['xaxis_tickmarks'])) {
    form_set_error('xaxis_tickmarks', t('The number of tickmarks for the xaxis must be entered as a number (i.e. 7).'));
  }

  // Height.
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * Submit handler for settings form.
 */
function event_summary_registration_form_submit($form, &$form_state) {
  // Timeframe.
  $form_state['conf']['time_period'] = $form_state['values']['time_period'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // Legend.
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // Height.
  $form_state['conf']['height'] = $form_state['values']['height'];

  // Settings for xaxis.
  $form_state['conf']['xaxis_tickmarks'] = $form_state['values']['xaxis_tickmarks'];

  // Colors.
  $form_state['conf']['line_color'] = $form_state['values']['line_color'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
  $form_state['conf']['background_color'] = $form_state['values']['background_color'];
}

/**
 * Render callback.
 */
function event_summary_registration_render($subtype, $conf, $panel_args, $context = NULL) {

  // Get the number of days to report back.
  $days = $conf['time_period'];

  // Run the query.
  $dates = db_query("
  SELECT count(activity.activity_id) registrations,
  UNIX_TIMESTAMP(fad.field_cmcev_reg_date_value) AS ts,
  DATE_FORMAT(fad.field_cmcev_reg_date_value, '%b %e') AS rdate,
  DATE_FORMAT(fad.field_cmcev_reg_date_value, '%j') AS doy
  FROM {crm_core_activity} AS activity
  JOIN {field_data_field_cmcev_reg_date} AS fad ON fad.entity_id = activity.activity_id
  JOIN {field_data_field_cmcev_event} AS event ON event.entity_id = activity.activity_id
  WHERE activity.type = 'cmcev_event_registration'
  AND fad.field_cmcev_reg_date_value > SUBDATE(NOW(), INTERVAL $days DAY)
  AND fad.field_cmcev_reg_date_value <> ''
  AND fad.field_cmcev_reg_date_value IS NOT NULL
  AND event.field_cmcev_event_target_id = :nid
  GROUP BY rdate, doy
  ORDER BY ts, rdate, doy DESC;
	", array(':nid' => $panel_args[0]));

  // Normalize the query for zero values, avoid big arcs.
  $date_item = array();
  $last_date = 0;
  $last_date_ts = 0;
  $series_labels = array();

  foreach ($dates as $date) {
    // Check if today is greater than lastdate.
    if (($date->doy - $last_date) > 1 && $last_date != '') {
      // Insert some zero values for yesterday and the day after lastdate.
      $date_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
      $series_labels[] = '---';
    }

    // Add the date.
    $date_item[] = array($date->ts * 1000, $date->registrations);

    // Record a label.
    $series_labels[] = $date->rdate . '<br />' . $date->registrations;

    // Set lastdate.
    $last_date = $date->doy;
    $last_date_ts = $date->ts;
  }

  // Draw the chart.
  $date_data[] = new flotData($date_item);

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_event_height', '200') : $conf['height'];
  $xaxis_tickmarks = empty($conf['xaxis_tickmarks']) ? variable_get('crm_core_event_xaxis_tickmarks', '') : $conf['xaxis_tickmarks'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_event_chart_color', '') : $conf['chart_color'];
  $line_color = empty($conf['line_color']) ? variable_get('crm_core_event_line_color', '') : $conf['line_color'];
  $background_color = empty($conf['background_color']) ? variable_get('crm_core_event_background_color', '') : $conf['background_color'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_event_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_event_include_tooltips', 0) : $conf['include_tooltip'];

  // Set up the chart.
  $options = new flotStyleLine();

  // Add points to the chart.
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;
  $options->series->suffix = ' registrations';

  // Add lines to the chart.
  $options->series->lines = new stdClass();
  $options->series->lines->show = TRUE;
  $options->series->lines->fill = TRUE;
  $options->series->lines->lineWidth = 4;

  $options->xaxis = new stdClass();
  $options->xaxis->mode = "time";
  $options->xaxis->ticks = (int) $xaxis_tickmarks;
  $options->xaxis->minTickSize = array(1, "day");

  $options->yaxis = new stdClass();
  $options->yaxis->min = 0;
  $options->series->seriesLabels = $series_labels;
  $options->seriesLabels = $series_labels;

  // Add colors to the line.
  if (!empty($line_color)) {
    $options->colors = array(
      $line_color,
    );
  }

  // Add a background for the series.
  if (!empty($background_color)) {
    $options->series->lines->fillColor = array(
      'colors' => explode("\n", $background_color),
    );
  }

  // Add a grid.
  $options->grid->borderWidth = 2;
  $options->grid->labelMargin = 8;
  if (!empty($chart_color)) {
    $options->grid->backgroundColor = $chart_color;
  }
  
  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }
  
  $block = new stdClass();

  $block->title = t('Registration');
  $block->content = theme('flot_graph', array(
    'data' => $date_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  return $block;
}
