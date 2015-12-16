<?php
/**
 * @file
 * Pie chart.
 * Displays the other sources people have contributed to when they have contributed to the selected source.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Donations by Source: Other Sources'),
  'description' => t('Displays the other sources people have contributed to when they have contributed to the selected source.'),
  'category' => t('CRM Core Donations'),
  'edit form' => 'crm_core_donation_donors_by_source_form',
  'render callback' => 'crm_core_donation_donors_by_source',
  'admin info' => 'crm_core_donation_donors_by_source_info',
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
 * admin info
 */
function crm_core_donation_donors_by_source_info($subtype, $conf, $contexts) {
  $block = new stdClass;
  $block->title = t('Donations by Source: Other Sources');
  $block->content = t('Displays a list of other sources people have given to who have also contributed to this source.');
}

/**
 * Settings form
 */
function crm_core_donation_donors_by_source_form($form, &$form_state) {
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

  // display labels
  $form['display_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display labels'),
    '#description' => t('Check this box to display labels for every region on your chart.'),
    '#default_value' => !empty($conf['display_labels']) ? $conf['display_labels'] : '0',
    '#return_value' => 1,
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
 * Validation for settings form
 */
function crm_core_donation_donors_by_source_form_validate($form, &$form_state) {

  // height
  if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
    form_set_error('height', t('The height of the chart must be entered as a number (i.e. 200).'));
  }
}

/**
 * submit handler for settings form
 */
function crm_core_donation_donors_by_source_form_submit($form, &$form_state) {

  // legend
  $form_state['conf']['include_legend'] = $form_state['values']['include_legend'];

  // display tooltip
  $form_state['conf']['include_tooltip'] = $form_state['values']['include_tooltip'];

  // height
  $form_state['conf']['height'] = $form_state['values']['height'];

  // colors
  $form_state['conf']['color_series'] = $form_state['values']['color_series'];

  // display labels
  $form_state['conf']['display_labels'] = $form_state['values']['display_labels'];
}

/**
 * Render callback
 */
function crm_core_donation_donors_by_source($subtype, $conf, $panel_args, $context = NULL) {

  // set up a table for displaying raw data
  $header = array('Source', 'Donors', 'Percentage');
  $rows = array();
  $total_donors = 0;

  // storing information that will go into the chart
  $source_data = array();

  // get the total donors for this source
  $this_source = db_query("SELECT COUNT(DISTINCT field_activity_participants_target_id) AS donors
  	FROM {field_data_field_cmcd_source} fds
  		JOIN {field_data_field_activity_participants} fdp
  		ON fds.entity_id = fdp.entity_id 
  	WHERE fds.field_cmcd_source_value = :source",
    array(':source' => decode_entities($panel_args[0]))
  );
  foreach ($this_source as $item) {
    $total_donors = $item->donors;
  }

  // get the total number of donors for each source
  $sql = <<<SQL
  SELECT 
		fds.field_cmcd_source_value as source,
		COUNT(DISTINCT field_activity_participants_target_id) AS donors
  FROM {field_data_field_cmcd_source} fds
		JOIN {field_data_field_activity_participants} fdp
		ON fds.entity_id = fdp.entity_id 
	GROUP BY source
	ORDER BY donors
SQL;
  $args = array();

  $donors = db_query($sql, $args);

  foreach ($donors as $row) {

    $percent = floor(($row->donors / $total_donors) * 100) . '%';

    $temp = new flotData(array(array(0, $row->donors)));
    $temp->label = '<div class="flot-tooltip-label">' . $row->source . '</div>';
    $source_data[] = $temp;

    // output the rows for the table
    $rows[] = array(
      'data' => array(
        array(
          'data' => $row->source,
          'class' => 'table-field-source',
        ),
        array(
          'data' => $row->donors,
          'class' => 'table-field-donors',
        ),
        array(
          'data' => $percent,
          'class' => 'table-field-percent',
        ),
        
      ),
    );
  }

  // Configuration.
  // If setting not set for widget, we grabbing it from modules configuration.
  $height = empty($conf['height']) ? variable_get('crm_core_donation_height', '200') : $conf['height'];
  $color_series = empty($conf['color_series']) ? variable_get('crm_core_donation_series_colors', '') : $conf['color_series'];
  $display_labels = empty($conf['display_labels']) ? variable_get('crm_core_donation_display_labels', 1) : $conf['display_labels'];
  $include_legend = empty($conf['include_legend']) ? variable_get('crm_core_donation_include_legend', 0) : $conf['include_legend'];
  $include_tooltip = empty($conf['include_tooltip']) ? variable_get('crm_core_donation_include_tooltips', 0) : $conf['include_tooltip'];

  // create a new pie chart
  $options = new flotStylePie();

  // create a radius, make it a donut chart
  $options->series->pie->radius = 1;
  $options->series->pie->innerRadius = 0.5;
  $options->series->suffix = ' donors';
  $options->series->useLabel = 1;
  
  // add the color series
  if (!empty($color_series)) {
    $options->colors = explode("\n", $color_series);
  }

  // add the labels
  if (!empty($display_labels)) {
    $options->series->pie->label->show = TRUE;
    $options->series->pie->label->radius = 2 / 3;
    $options->series->pie->label->threshold = 0.01;
    $options->series->pie->label->backgroundOpacity = 1;
  }
  else {
    $options->series->pie->label->show = FALSE;
  }

  // add the tooltips
  if (!empty($include_tooltip)) {
    $options->series->show_tooltip = TRUE;
  }
  else {
    $options->series->show_tooltip = FALSE;
  }
  
  // display the chart
  $content = theme('flot_graph', array(
    'data' => $source_data,
    'options' => $options,
    'element' => array(
      'style' => 'width: 100%; height: ' . (int) $height . 'px;',
    ),
    'legend' => ($include_legend === 1) ? TRUE : FALSE,
  ));
  // output the table
  $content .= '<div class="table-container">' . theme('table', array('rows' => $rows, 'header' => $header, 'attributes' => array('class' => array('table-donation-source-other-sources')))) . '<div>';

  $block = new stdClass();
  $block->title = t('Other Sources');
  $block->content = $content;

  return $block;
}
