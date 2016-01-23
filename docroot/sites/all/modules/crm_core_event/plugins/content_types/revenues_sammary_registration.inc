<?php

/**
 * @file
 * Line chart displaying signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Revenues summary registration'),
  'description' => t('Displays a line chart showing events registration history and revenues amount.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'revenues_sammary_registration_form',
  'render callback' => 'revenues_sammary_registration_render',
  'admin info' => 'revenues_sammary_registration_admin_info',
  'defaults' => array(
    'text' => '',
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
function revenues_sammary_registration_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Revenues summary registration');
  $block->content = t('Displays a line chart showing events registration history and revenues amount.');

  return $block;
}

/**
 * Settings form.
 */
function revenues_sammary_registration_form($form, &$form_state) {

  // Stored widget configuration.
  // This widget should allow users to set the timeframe for performance.
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
 * Submit handler for settings form.
 */
function revenues_sammary_registration_form_validate($form, &$form_state) {
  
  // Timeframe.
  if (!is_numeric($form_state['values']['time_period']) && $form_state['values']['time_period'] !== 'All') {
    form_set_error('time_period', t('The number of days must be entered as a number (i.e. 90) or as the word "All".'));
  }

  // Xaxis.
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
function revenues_sammary_registration_form_submit($form, &$form_state) {
  
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
function revenues_sammary_registration_render($subtype, $conf, $panel_args, $context = NULL) {

  $content = '';

  if (is_numeric($panel_args[0])) {

    // Run the query.
    $data = db_query("
  SELECT COUNT( DISTINCT registration.activity_id) registrations,
  SUM(era.field_cmcev_amount_value) as revenue,
  ere.field_cmcev_event_target_id as event_nid,
  ed.field_cmcev_date_value as event_start_date,
  UNIX_TIMESTAMP(erd.field_cmcev_reg_date_value) AS ts,
  DATE_FORMAT(erd.field_cmcev_reg_date_value, '%b %e') AS rdate,
  DATE_FORMAT(erd.field_cmcev_reg_date_value, '%j') AS doy
  FROM {crm_core_activity} AS registration
  LEFT JOIN {field_data_field_cmcev_reg_date} AS erd ON erd.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_amount} AS era ON era.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_event} AS ere ON ere.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_date} AS ed ON ed.entity_id = ere.field_cmcev_event_target_id
  WHERE registration.type = 'cmcev_event_registration'
  AND erd.field_cmcev_reg_date_value IS NOT NULL
  AND erd.field_cmcev_reg_date_value <> ''
  AND ( (DATE_FORMAT(ADDTIME(ed.field_cmcev_date_value2, SEC_TO_TIME(10800)), '%Y%m') >= :yearmonth
AND DATE_FORMAT(ADDTIME(ed.field_cmcev_date_value, SEC_TO_TIME(10800)), '%Y%m') <= :yearmonth) )
  GROUP BY rdate, doy
  ORDER BY ts, rdate, doy DESC;
	", array(':yearmonth' => $panel_args[0]));

    // Normalize the query for zero values, avoid big arcs.
    $reg_item = array();
    $reg_series_label = array();

    $revenue_item = array();
    $revenue_series_label = array();

    $last_date = 0;
    $last_date_ts = 0;

    foreach ($data as $item) {
      // Check if today is greater than lastdate.
      if (($item->doy - $last_date) > 1 && $last_date != 0) {
        // Insert some zero values for yesterday and the day after lastdate.
        $reg_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
        $reg_series_label[] = '---';
        $revenue_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
        $revenue_series_label[] = '---';
        if (($item->doy - $last_date) > 2) {
          $reg_item[] = array($last_date_ts * 1000 + 86400 * 1000 * 2, 0);
          $reg_series_label[] = '---';
          $revenue_item[] = array($last_date_ts * 1000 + 86400 * 1000 * 2, 0);
          $revenue_series_label[] = '---';
        }
      }

      // Add the date.
      $reg_item[] = array($item->ts * 1000, $item->registrations);
      $revenue_item[] = array(
        $item->ts * 1000,
        isset($item->revenue) ? $item->revenue : 0,
      );

      // Record a label.
      $reg_series_label[] = format_plural($item->registrations, '@reg registration', '@reg registrations', array('@reg' => $item->registrations));
      $revenue_series_label[] = '$' . check_plain($item->revenue);

      // Set lastdate.
      $last_date = $item->doy;
      $last_date_ts = $item->ts;
    }
    
    // Configuration.
    // If setting not set for widget, we grabbing it from modules configuration.
    $height = empty($conf['height']) ? variable_get('crm_core_event_height', '200') : $conf['height'];
    $xaxis_tickmarks = empty($conf['xaxis_tickmarks']) ? variable_get('crm_core_event_xaxis_tickmarks', '') : $conf['xaxis_tickmarks'];
    $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_event_chart_color', '') : $conf['chart_color'];
    $line_color = empty($conf['line_color']) ? variable_get('crm_core_event_line_color', '') : $conf['line_color'];
    $background_color = empty($conf['background_color']) ? variable_get('crm_core_event_background_color', '') : $conf['background_color'];
    $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_event_include_legend', 0) : $conf['include_legend'];
    $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_event_include_tooltips', 0) : $conf['include_tooltip'];

    // Draw the chart.
    $chart_data[] = new flotData($reg_item);

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

    // Add colors to the line.
    if (!empty($line_color)) {
      $options->colors = array(
        $line_color,
      );
    }
  
    // Add a background for the series.
    if ($background_color) {
      $options->series->lines->fillColor = array(
        'colors' => explode("\n", $background_color),
      );
    }

    // Add a grid.
    $options->grid->borderWidth = 2;
    $options->grid->labelMargin = 8;
    if ($chart_color) {
      $options->grid->backgroundColor = $chart_color;
    }
      
    // Add an x-axis.
    $options->xaxis = new stdClass();
    $options->xaxis->mode = "time";
    $options->xaxis->ticks = (int) $xaxis_tickmarks;


    // add the tooltips
    if (!empty($include_tooltip)) {
      $options->series->show_tooltip = TRUE;
    }
    else {
      $options->series->show_tooltip = FALSE;
    }    
    
    $content = theme('flot_graph', array(
      'data' => $chart_data,
      'options' => $options,
    ));
  }

  $block = new stdClass();

  $block->title = t('Registration');
  $block->content = $content;

  return $block;
}
