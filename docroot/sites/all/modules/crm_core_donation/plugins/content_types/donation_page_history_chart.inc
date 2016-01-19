<?php
/**
 * @file
 * Line chart.
 * Displays a chart containing donation totals by day for the selected
 * donation page.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Donations by Page: History Chart'),
  'description' => t('Displays a chart showing donation totals for each online donation page.'),
  'category' => t('CRM Core Donations'),
  'edit form' => 'crm_core_donation_page_history_chart_form',
  'render callback' => 'crm_core_donation_page_history_chart',
  'admin info' => 'crm_core_donation_page_history_chart_info',
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
function crm_core_donation_page_history_chart_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Donations by Page: History Chart');
  $block->content = t('Displays a chart showing donation totals for each online donation page.');

  return $block;
}

/**
 * Settings form
 */
function crm_core_donation_page_history_chart_form($form, &$form_state) {
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
 * validate handler for settings form
 */
function crm_core_donation_page_history_chart_form_validate($form, &$form_state) {
  // xaxis
  if (!empty($form_state['values']['xaxis_tickmarks']) && !is_numeric($form_state['values']['xaxis_tickmarks'])) {
    form_set_error('xaxis_tickmarks', t('The number of tickmarks for the xaxis must be entered as a number (i.e. 7).'));
  }

  // height
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * submit handler for settings form
 */
function crm_core_donation_page_history_chart_form_submit($form, &$form_state) {

  // legend
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

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
function crm_core_donation_page_history_chart($subtype, $conf, $panel_args, $context = NULL) {

  // this will display a line chart of activity for the current donation page
  // need to get a context from the page

  // set up the chart
  $source_data = array();
  $source_labels = array();
  $items = array();
  $count = 0;

  // okay, we are making a historical line chart
  // this will display all funding from this source from 
  $sql = "SELECT SUM(fda.field_cmcd_amount_value) AS amount, 
  	UNIX_TIMESTAMP(field_cmcd_receive_date_value) AS timestamp,
  	DATE_FORMAT(field_cmcd_receive_date_value, '%b %e') AS rdate,
  	DATE_FORMAT(field_cmcd_receive_date_value, '%j') AS doy
  	FROM {field_data_field_cmcd_amount} fda 
  	JOIN {field_data_field_cmcd_receive_date} fdd
  	ON fda.entity_id = fdd.entity_id 
  	JOIN {field_data_field_cmcd_page} fdp
  	ON fdd.entity_id = fdp.entity_id
  	WHERE fdd.field_cmcd_receive_date_value != ''
  	  AND fdd.field_cmcd_receive_date_value IS NOT NULL";

  if (isset($panel_args[0])) {
    $sql .= " AND fdp.field_cmcd_page_target_id = :page";
  }

  $sql .= " GROUP BY rdate, doy ORDER BY timestamp, rdate, doy DESC;";

  $args = array();
  if (isset($panel_args[0])) {
    $args = array(':page' => decode_entities($panel_args[0]));
  }

  $dates = db_query($sql, $args);

  // okay, we need to include some zero values using doy from the query
  $date_item = array();
  $last_date = '';
  $last_date_ts = '';
  $series_labels = array();
  foreach ($dates as $date) {
    // check if today is greater than lastdate
    if (($date->doy - $last_date) > 1 && $last_date != '') {
      // insert some zero values for yesterday and the day after lastdate
      $date_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
      $series_labels[] = '---';
      // add one for the day before the current record as well
      $date_item[] = array(($date->timestamp * 1000) - 86400 * 1000, 0);
      $series_labels[] = '---';
    }
    // add the date
    // $date_item[] = array($date->timestamp*1000, $date->amount);
    $date_item[] = array($date->timestamp * 1000, $date->amount);
    // record a label
    $series_labels[] = $date->rdate . '<br>' . $date->amount;
    // set lastdate
    $last_date = $date->doy;
    $last_date_ts = $date->timestamp;
  }

  // add the donations by date series as a new flotData item
  $date_data = new flotData($date_item);
  $date_data->label = t('Donations by Day');
  $data[] = $date_data;

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_donation_height', '200') : $conf['height'];
  $xaxis_tickmarks = empty($conf['xaxis_tickmarks']) ? variable_get('crm_core_donation_xaxis_tickmarks', '7') : $conf['xaxis_tickmarks'];
  $chart_color = empty($conf['chart_color']) ? variable_get('crm_core_donation_chart_color', '') : $conf['chart_color'];
  $line_color = empty($conf['line_color']) ? variable_get('crm_core_donation_line_color', '') : $conf['line_color'];
  $background_color = empty($conf['background_color']) ? variable_get('crm_core_donation_background_color', '') : $conf['background_color'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_donation_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_donation_include_tooltips', 0) : $conf['include_tooltip'];
  
  // get the currency symbol for this chart
  $cur = commerce_currency_load();
  $sym = $cur['symbol'];  

  // create a new line chart
  $options = new flotStyleLine();

  // add points to the chart
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;

  // add lines to the chart
  $options->series->lines = new stdClass();
  $options->series->lines->show = TRUE;
  $options->series->lines->fill = TRUE;
  $options->series->lines->lineWidth = 4;
  $options->series->prefix = $sym;

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
  $options->series->seriesLabels = $series_labels;

  // add labels to the chart
  $options->seriesLabels = $series_labels;
  
  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }

  // return the chart
  $block = new stdClass();
  $block->title = t('Donation Page History Chart');
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
