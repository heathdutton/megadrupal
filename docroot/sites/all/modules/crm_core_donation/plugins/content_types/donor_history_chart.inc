<?php
/**
 * @file
 * Line chart.
 * Displays summary information about dates,
 * when new donors come into the system.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Donor: New Donors by Month chart'),
  'description' => t('Displays a chart showing when new donors have come into the system.'),
  'category' => t('CRM Core Donations'),
  'edit form' => 'crm_core_donation_donor_history_form',
  'render callback' => 'crm_core_donation_donor_history_chart',
  'admin info' => 'crm_core_donation_donor_history_info',
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
 * Admin info.
 */
function crm_core_donation_donor_history_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Donor: New Donors by Month chart');
  $block->content = t('Displays a chart showing when new donors have come into the system..');

  return $block;
}

/**
 * Settings form.
 */
function crm_core_donation_donor_history_form($form, &$form_state) {
  $conf = $form_state['conf'];

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
function crm_core_donation_donor_history_form_validate($form, &$form_state) {

  // X-axis.
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
function crm_core_donation_donor_history_form_submit($form, &$form_state) {

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
 * Render callback.
 */
function crm_core_donation_donor_history_chart($subtype, $conf, $panel_args, $context = NULL) {

  // set up the chart
  $source_data = array();
  $source_labels = array();
  $items = array();
  $count = 0;
  $args = array();

  // set up some other variables
  $first_donor_date = '';
  $last_donor = '';
  $last_donor_date = 0;
  $series_labels = array();

  // getting a list of the first time people donated to the site

  $sql = <<<SQL
  SELECT 
  	COUNT(DISTINCT fap.field_activity_participants_target_id) AS donors, 
  	DATE_FORMAT(field_cmcd_receive_date_value, '%b %y') AS month_year,
  	UNIX_TIMESTAMP(DATE_FORMAT(field_cmcd_receive_date_value, '%Y-%m-01 00:00:00')) AS ts
  FROM {field_data_field_cmcd_receive_date} fdrd
    JOIN {field_data_field_activity_participants} fap
    ON fdrd.entity_id = fap.entity_id
  WHERE fdrd.field_cmcd_receive_date_value = (
  	SELECT MIN(fd.field_cmcd_receive_date_value)
  	FROM {field_data_field_cmcd_receive_date} fd
  	  JOIN {field_data_field_activity_participants} fp
	    ON fd.entity_id = fp.entity_id
  	WHERE fap.field_activity_participants_target_id = fp.field_activity_participants_target_id
  ) 
  GROUP BY month_year, ts
  ORDER BY ts
SQL;

  $dates = db_query($sql, $args);

  foreach ($dates as $date) {

    // add a start date for the donor activity
    // start from nothing...
    if ($first_donor_date == '') {
      $item = array(($date->ts - (24 * 60 * 60 * 30)) * 1000, 0);
      $source_data[] = $item;
      // set labels for the x-axis
      $series_labels[] = '---';
      $first_donor_date = $item;
    }

    // check the last item. if there is a difference of more than 1 month,
    // we need to insert empty values
    $check = 60 * 60 * 24 * 31; // this should be a month in seconds
    if ($last_donor_date > 0) {

      $diff = $date->ts - $last_donor_date;

      if ($diff > $check) {

        $dry_spell = ceil($check / $diff);
        $dry_spell = ceil($diff / $check) - 1;

        // add the first date in between
        $new_date = $last_donor_date + $check;
        $item = array($new_date * 1000, 0);
        $source_data[] = $item;
        $series_labels[] = '---';

        // add the last date in between
        $new_date = $last_donor_date + ($check * $dry_spell);
        $item = array($new_date * 1000, 0);
        $source_data[] = $item;
        $series_labels[] = '---';
      }
    }

    // set up the date
    // remember that ts * 1000 is what you need for the chart
    $item = array($date->ts * 1000, $date->donors);
    $source_data[] = $item;

    // set labels for the x-axis
    $series_labels[] = $date->month_year . '<br>' . $date->donors . ' donors';

    // store the last donor date for comparison
    $last_donor_date = $date->ts;
  }

  // make the points into data for the chart
  $date_data[] = new flotData($source_data);

  // add the donations by date series as a new flotData item
  $date_data = new flotData($source_data);
  $date_data->label = t('New Donors by Day');
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
  
  // create a new line chart
  $options = new flotStyleLine();

  // add points to the chart
  $options->series->points = new stdClass();
  $options->series->points->show = TRUE;
  $options->series->points->fill = TRUE;
  $options->series->points->radius = 4;
  $options->series->points->lineWidth = 2;
  $options->series->suffix = ' donors';
  
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
  $block->title = t('New Donors by Month');
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
