<?php

/**
 * @file
 *   Page callbacks
 */
 
/**
 * The analytics page callback
 */
function quant_page() {
  quant_include('forms');
  $charts = array();
  
  // Get time period or interval
  $period = _quant_get_time_from_url();
  
  // Fetch all available quants
  if ($period) {
    $quants = quant_get_quants();
    foreach ($quants as $quant) {
      // Check to see if this quant should be shown
      if ($allowed = variable_get('quant_visible', array())) {
        if (isset($allowed[$quant->id]) && !$allowed[$quant->id]) {
          continue;
        }
      }
      // Process the quant
      $quant->execute($period);
      $charts[] = $quant->render();
    }
  }
  else {
    drupal_set_message(t('Invalid timeframe arguments provided.'), 'error');
  }

  // Add externals
  drupal_add_css(drupal_get_path('module', 'quant') . '/theme/quant.css');
  
  // Theme and return the page
  return theme(
    'quant_page', 
    array(
      'form' => drupal_render(drupal_get_form('quant_time_form')), 
      'charts' => $charts
    )
  );
}

/**
 * Retrieve the time period or interval from the URL
 * 
 * @param $period
 *   If the set option is a period, the return value will
 *   be a single timestamp representing how far to go back
 *   in time from the current. If the set option is custom,
 *   the return value will be an array representing a given
 *   time interval. The first value will be the "from" timestamp
 *   and the second value will be the "to" timestamp. If neither
 *   can be determined, FALSE will be returned. If no queries are
 *   in the URL, a timestamp for a 1 month period will be returned.
 */
function _quant_get_time_from_url() {
  // Get the option
  if (isset($_GET['option'])) {
    switch ($_GET['option']) {
      case 'custom':
        $from = strtotime($_GET['from']);
        $to = strtotime($_GET['to']);
        if (is_numeric($from) && is_numeric($to)) {
          // Move the 'to' date to 1 second before midnight
          return array($from, $to + 86399);
        }
        break;
    
      case 'period':
        $period = strtotime('-' . str_replace('_', ' ', filter_xss($_GET['period'])));
        if (is_numeric($period)) {
          return $period; 
        }
    }
  }
  else {
    return strtotime('-1 month');
  }
  
  return FALSE;
}
