<?php

/**
 * @file
 *   Form-building functions
 */

/**
 * Provide a simple form to change time periods
 */
function quant_time_form($form, &$form_state) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("
    jQuery(document).ready(function ($) {
      $('#edit-custom-to').datepicker();
      $('#edit-custom-from').datepicker();
     });
  ", 'inline');
  
  $period_options = array(
    '1_week' => t('1 week'),
    '2_weeks' => t('2 weeks'),
    '1_month' => t('1 month'),
    '3_months' => t('3 months'),
    '6_months' => t('6 months'),
    '1_year' => t('1 year'),
    '2_years' => t('2 years'),
  );
  
  $period = isset($_GET['period']) ? filter_xss($_GET['period']) : NULL;
  if (!$period || !array_key_exists($period, $period_options)) {
    $period = '1_month'; // Default value
  }
  
  $form['message'] = array(
    '#type' => 'item',
    '#value' => t('Select a timeframe and click Update to see what\'s happening on your site during that time, as well as a total for all the activity during that timeframe.'),
  );
  
  $form['option'] = array(
    '#type' => 'radios',
    '#options' => array(
      'period' => '',
      'custom' => '',
    ),
    '#default_value' => isset($_GET['option']) ? filter_xss($_GET['option']) : 'period',
  );
  
  $form['period'] = array(
    '#type' => 'select',
    '#options'=> $period_options,
    '#default_value' => $period, 
  ); 
  
  $form['custom_from'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => isset($_GET['from']) ? filter_xss($_GET['from']) : '',
  );
  
  $form['custom_to'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => isset($_GET['to']) ? filter_xss($_GET['to']) : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  
  return $form;
}

/**
 * Validate handler for quant_time_form()
 */
function quant_time_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  
  // Make sure a time option is checked
  if (!$values['option']) {
    form_set_error('error', t('An option must be selected'));
  }
  
  // If custom option, make sure we have both dates
  if ($values['option'] == 'custom' &&
    !($values['custom_from'] && $values['custom_to'])) {
    form_set_error('option', t('You must specify both dates'));
  }
  else if ($values['option'] == 'custom') {
    // Convert the times
    $now = time();
    $from = strtotime($values['custom_from']);
    $to = strtotime($values['custom_to']);
    
    // Make sure from date exists
    if (!$from) {
      form_set_error('custom_from', t('The from date must be formatted correctly %format.', array('%format' => '(MM/DD/YY)')));
    }
    
    // Make sure to date exists
    if (!$to) {
      form_set_error('custom_to', t('The to date must be formatted correctly %format.', array('%format' => '(MM/DD/YY)')));
    }
    
    // Make sure from is less than to
    if ($from > $to) {
      form_set_error('custom_from', t('The from date must be before the to date'));
    }
    
    // Make sure to date is not past current time
    if ($to > $now) {
      form_set_error('custom_to', t('The to date must not be past today')); 
    }
  }
}

/**
 * Submit handler for quant_time_form()
 */
function quant_time_form_submit(&$form, &$form_state) {
  // Add a query to the URL which will be used to determine the query
  switch ($form_state['values']['option']) {
    case 'period':
      drupal_goto(
        'analytics', 
        array(
          'query' => array(
            'option' => 'period',
            'period' => $form_state['values']['period']
          )
        )
      );
      break;
    
    case 'custom':
      drupal_goto(
        'analytics', 
        array(
          'query' => array(
            'option' => 'custom',
            'from' => $form_state['values']['custom_from'],
            'to' => $form_state['values']['custom_to']
          )
        )
      );
      break;
  }
}

/**
 * Provide admin settings form
 */
function quant_admin_settings($form, &$form_state) {
  $form['view'] = array(
    '#type' => 'fieldset',
    '#title' => t('View analytics'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['view']['view_link'] = array(
    '#markup' => l(t('Click here to view the analytics page'), 'analytics'),
  );
  
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['display']['quant_visible'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show analytics for the following site items'),
    '#default_value' => _quant_quant_option_defaults(),
    '#options' => _quant_quant_options(),
    '#description' => t('The checked objects will be shown on the analytics page. If a quant object is missing from this list, try clearing the cache.'),
  );
  $form['display']['quant_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart width'),
    '#default_value' => variable_get('quant_width', 500),
    '#size' => 6,
    '#maxlength' => 4,
    '#required' => TRUE,
    '#description' => t('Specify the graph width in pixels. The Chart API will reject charts that are too wide.'),
  );
  $form['display']['quant_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Chart height'),
    '#default_value' => variable_get('quant_height', 200),
    '#size' => 6,
    '#maxlength' => 4,
    '#required' => TRUE,
    '#description' => t('Specify the graph height in pixels. The Chart API will reject charts that are too tall.'),
  );
  $form['display']['quant_chart'] = array(
    '#type' => 'radios',
    '#title' => t('Chart plugin'),
    '#options' => _quant_chart_options(),
    '#default_value' => variable_get('quant_chart', 'table'),
    '#description' => t('Choose the plugin to render the charts in.'),
  );
  
  // Add settings for all chart plugins
  foreach (quant_get_quant_charts() as $plugin) {
    if ($chart_form = $plugin->chart->adminSettings()) {
      $form['chart_plugin_options'][$plugin->id] = array(
        '#type' => 'fieldset',
        '#title' => $plugin->name,
        '#description' => $plugin->description,
      );
      $form['chart_plugin_options'][$plugin->id] += $chart_form;
    }
  }
  if (isset($form['chart_plugin_options'])) {
    $form['chart_plugin_options'] += array(
      '#type' => 'fieldset',
      '#title' => t('Chart plugin options'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );
  }
 
  return system_settings_form($form); 
}

/**
 * Validate the admin settings form
 */
function quant_admin_settings_validate(&$form, &$form_state) {
  // Check width
  if (!is_numeric($form_state['values']['quant_width']) ||
   strlen($form_state['values']['quant_width']) > 4 ||
   strlen($form_state['values']['quant_width']) < 2) {
    form_set_error('quant_width', t('The width must be a number that is between 2 and 4 digits'));
  }
  
  // Check height
  if (!is_numeric($form_state['values']['quant_height']) ||
   strlen($form_state['values']['quant_height']) > 4 ||
   strlen($form_state['values']['quant_height']) < 2) {
    form_set_error('quant_height', t('The height must be a number that is between 2 and 4 digits'));
  }
  
  // Allow chart plugins to validate
  foreach (quant_get_quant_charts() as $plugin) {
    $plugin->chart->adminSettingsValidate($form, $form_state);
  }
}

/**
 * Submit the admin settings form
 */
function quant_admin_settings_submit(&$form, &$form_state) {
  // Allow chart plugins to handle submission
  foreach (quant_get_quant_charts() as $plugin) {
    $plugin->chart->adminSettingsSubmit($form, $form_state);
  }
}

/**
 * Helper function to provide quant options
 */
function _quant_quant_options() {
  // Get available quants
  $options = array();
  $quants = quant_get_quants();
  foreach ($quants as $quant) {
    $options[$quant->id] = $quant->label;
  }
  return $options;
}

/**
 * Helper function to provide quant option defaults
 */
function _quant_quant_option_defaults() {
  $default = array();
  $options = _quant_quant_options();
  // See if visibility settings haven't been set yet
  if (!($default = variable_get('quant_visible', array()))) {
    return $options;
  }
  else {
    // Iterate the available options to see if there is one that is
    // not present in the visibility options. That can happen if a new
    // quant is available but the settings were set before it was
    foreach ($options as $id => $name) {
      if (!isset($default[$id])) {
        $default[$id] = $id;
      }
    }
  }
  return $default;
}

/**
 * Helper function to provide chart plugin options
 */
function _quant_chart_options() {
  $options = array();
  foreach (quant_get_quant_charts() as $id => $chart) {
    $options[$id] = $chart->name . '<br/>' . $chart->description;
  }
  return $options;    
}
