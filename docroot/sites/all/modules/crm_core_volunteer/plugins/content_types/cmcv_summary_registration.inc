<?php
/**
 * @file
 * Line chart.
 * Displays a chart showing when people volunteered for this opportunity.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Opportunities: Registration History'),
  'description' => t('Displays a line chart showing volunteer registration history for the selected opportunity.'),
  'category' => t('CRM Core Volunteer'),
  'edit form' => 'cmcv_summary_registration_form',
  'render callback' => 'cmcv_summary_registration_render',
  'admin info' => 'cmcv_summary_registration_admin_info',
  'defaults' => array(
    'time_period' => '90',
    'height' => '',
    'xaxis_tickmarks' => '',
    'chart_color' => '',
    'line_color' => '',
    'background_color' => '',
    'include_legend' => '',
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function cmcv_summary_registration_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Opportunities: Registration History');
  $block->content = t('Displays a line chart showing volunteer registration history for the selected opportunity.');

  return $block;
}

/**
 * Settings form.
 */
function cmcv_summary_registration_form($form, &$form_state) {

  $conf = $form_state['conf'];

  // This widget should allow users to set the timeframe for performance.
  $form['time_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Period'),
    '#description' => t("Enter the number of days to report back on for this widget. If no value is provided, defaults to 90. You can enter the word 'All' to display all values."),
    '#default_value' => !empty($conf['time_period']) ? $conf['time_period'] : '90',
  );

  $form['include_tooltip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display tooltips'),
    '#description' => t('Check this box to display tooltips when hovering over a point in your chart.'),
    '#default_value' => !empty($conf['include_tooltip']) ? $conf['include_tooltip'] : 1,
    '#return_value' => 1,
  );

  $form['include_legend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a legend'),
    '#description' => t('Check this box to include a legend in your chart.'),
    '#default_value' => !empty($conf['include_legend']) ? $conf['include_legend'] : '0',
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
function cmcv_summary_registration_form_validate($form, &$form_state) {

  // Timeframe.
  if (!is_numeric($form_state['values']['time_period']) && $form_state['values']['time_period'] !== 'All') {
    form_set_error('time_period', t("The number of days must be entered as a number (i.e. 90) or as the word 'All'."));
  }

  // X-axis.
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
function cmcv_summary_registration_form_submit($form, &$form_state) {
  // Timeframe.
  $form_state['conf']['time_period'] = $form_state['values']['time_period'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // Legend.
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // Height.
  $form_state['conf']['height'] = $form_state['values']['height'];

  // Xaxis.
  $form_state['conf']['xaxis_tickmarks'] = $form_state['values']['xaxis_tickmarks'];

  // Colors.
  $form_state['conf']['line_color'] = $form_state['values']['line_color'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
  $form_state['conf']['background_color'] = $form_state['values']['background_color'];
}

/**
 * Render callback.
 */
function cmcv_summary_registration_render($subtype, $conf, $panel_args, $context = NULL) {

  // Get a list of registration totals by day.
  $sql = <<<SSQL
  SELECT count(activity.activity_id) amount,
  UNIX_TIMESTAMP(fad.field_cmcv_reg_date_value) AS ts,
  DATE_FORMAT(fad.field_cmcv_reg_date_value, '%b %e') AS rdate,
  DATE_FORMAT(fad.field_cmcv_reg_date_value, '%j') AS doy
  FROM {crm_core_activity} AS activity
  JOIN {field_data_field_cmcv_reg_date} AS fad ON fad.entity_id = activity.activity_id
  JOIN {field_data_field_cmcv_vo_reference} fdr ON fdr.entity_id = activity.activity_id
  WHERE activity.type = 'cmcv_volunteer_commitment'
SSQL;

  if (is_numeric($conf['time_period'])) {
    $sql .= ' AND fad.field_cmcv_reg_date_value > SUBDATE(NOW(), INTERVAL ' . $conf['time_period'] . ' DAY) ';
  }

  $sql .= <<<ESQL
  AND fad.field_cmcv_reg_date_value <> ''
  AND fad.field_cmcv_reg_date_value IS NOT NULL
  AND fdr.field_cmcv_vo_reference_target_id = :nid
  GROUP BY rdate, doy
  ORDER BY ts, rdate, doy DESC;
ESQL;

  // Run the query.
  $dates = db_query($sql, array(':nid' => $panel_args[0]));

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
      // Add one for the day before the current record as well.
      $date_item[] = array(($date->ts * 1000) - 86400 * 1000, 0);
      $series_labels[] = '---';
    }

    // Add the date.
    $date_item[] = array($date->ts * 1000, $date->amount);

    // Record a label.
    $series_labels[] = $date->rdate . '<br />' . $date->amount;

    // Set lastdate.
    $last_date = $date->doy;
    $last_date_ts = $date->ts;
  }

  // Add the registrations by date series as a new flotData item.
  $date_data = new flotData($date_item);
  $date_data->label = t('Registration by Day');
  $data[] = $date_data;

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_volunteer_chart_height', CMCV_CHART_HEIGHT) : $conf['height'];
  $xaxis_tickmarks = empty($conf['xaxis_tickmarks']) ? variable_get('crm_core_volunteer_xaxis_tickmarks', CMCV_XAXIS_TICKS) : $conf['xaxis_tickmarks'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_volunteer_chart_color', '') : $conf['chart_color'];
  $line_color = empty($conf['line_color']) ? variable_get('crm_core_volunteer_line_color', '') : $conf['line_color'];
  $background_color = empty($conf['background_color']) ? variable_get('crm_core_volunteer_background_color', '') : $conf['background_color'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_volunteer_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_volunteer_include_tooltips', 0) : $conf['include_tooltip'];

  // Create a new line chart.
  $options = new flotStyleLine();
  $options->series->suffix = ' Volunteers';
  
  // Add points to the chart.
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;

  // Add lines to the chart.
  $options->series->lines = new stdClass();
  $options->series->lines->show = TRUE;
  $options->series->lines->fill = TRUE;
  $options->series->lines->lineWidth = 4;

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

  // Add an x-axis.
  $options->xaxis = new stdClass();
  $options->xaxis->mode = "time";
  $options->xaxis->ticks = (int) $xaxis_tickmarks;

  // Add a y-axis.
  $options->yaxis = new stdClass();
  $options->yaxis->min = 0;
  $options->series->seriesLabels = $series_labels;

  // Add labels to the chart.
  $options->seriesLabels = $series_labels;

  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }

  // Return the chart.
  $block = new stdClass();
  if ($conf['time_period'] == 'All') {
    $block->title = t('Registration History');
  }
  else {
    $block->title = t('@days-Day Registration History', array('@days' => $conf['time_period']));
  }
  $block->content = theme('flot_graph', array(
    'data' => $data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));

  return $block;
}
