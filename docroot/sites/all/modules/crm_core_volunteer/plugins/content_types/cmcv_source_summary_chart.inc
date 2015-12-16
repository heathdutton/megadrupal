<?php
/**
 * @file
 * Line chart.
 * Displays a chart containing registration amount totals by month
 * for a specific source.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Registrations by Source: History Chart'),
  'description' => t('Displays a chart showing historical registration activity for the selected source.'),
  'category' => t('CRM Core Volunteer'),
  'edit form' => 'source_summary_chart_form',
  'render callback' => 'source_summary_chart_render',
  'admin info' => 'source_summary_chart_info',
  'defaults' => array(
    'height' => '',
    'xaxis_tickmarks' => '',
    'chart_color' => '',
    'line_color' => '',
    'background_color' => '',
    'include_legend' => '0',
		'include_tooltip' => 0,
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function source_summary_chart_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = 'Registrations by Source: History Chart';
  $block->content = t('Displays a chart showing historical registration activity for the selected source.');

  return $block;
}

/**
 * Settings form.
 */
function source_summary_chart_form($form, &$form_state) {
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
 * Validation handler.
 */
function source_summary_chart_form_validate($form, &$form_state) {
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
 * Submission handler.
 */
function source_summary_chart_form_submit($form, &$form_state) {
  // Legend.
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];
  
  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];  

  // Height.
  $form_state['conf']['height'] = $form_state['values']['height'];

  // Xaxis.
  $form_state['conf']['xaxis_tickmarks'] = $form_state['values']['xaxis_tickmarks'];

  // colors.
  $form_state['conf']['line_color'] = $form_state['values']['line_color'];
  $form_state['conf']['chart_color'] = $form_state['values']['chart_color'];
  $form_state['conf']['background_color'] = $form_state['values']['background_color'];
}

/**
 * Render callback.
 */
function source_summary_chart_render($subtype, $conf, $panel_args, $context = NULL) {
  // Get historical data on donations from the selected source
  // present data by day.
  $sql = "
  SELECT count(activity.activity_id) amount,
  UNIX_TIMESTAMP(fad.field_cmcv_reg_date_value) AS ts,
  DATE_FORMAT(fad.field_cmcv_reg_date_value, '%b') AS rdate,
  DATE_FORMAT(fad.field_cmcv_reg_date_value, '%m') AS m
  FROM {crm_core_activity} AS activity
  JOIN {field_data_field_cmcv_reg_date} AS fad ON fad.entity_id = activity.activity_id
  JOIN {field_data_field_cmcv_source} AS fds ON fds.entity_id = activity.activity_id
  WHERE activity.type = 'cmcv_volunteer_commitment'
  AND fad.field_cmcv_reg_date_value <> ''
  AND fad.field_cmcv_reg_date_value IS NOT NULL
  AND fds.field_cmcv_source_value = :source
  GROUP BY rdate, m
  ORDER BY ts, rdate, m DESC;
	";

  // Run the query.
  $dates = db_query($sql, array(':source' => $panel_args[0]));

  // Modify the values returned by the query. include zero values for days
  // when donations where not collected, in order to accurately represent
  // donation activity.
  $date_item = array();
  $last_date = 0;
  $last_date_ts = 0;
  $series_labels = array();
  foreach ($dates as $date) {
    // Check if today is greater than lastdate.
    if (($date->m - $last_date) > 1 && $last_date != '') {
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
    $last_date = $date->m;
    $last_date_ts = $date->ts;
  }

  $date_data = new flotData($date_item);
  $date_data->label = 'Donations by Day';
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
  $options->series->suffix = ' Registrations';
  
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
  $options->xaxis->minTickSize = array(1, 'month');

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
  $block->title = t('Source History');
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
