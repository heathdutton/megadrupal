<?php
/**
 
(BEFORE YOUR START) ADMIN SETTINGS
--------------------------------------

Inside the admin settings, located at /admin/settings/quant, admins will be presented
with a list of every available quant (loaded from hook_quants). Admins can limit
which charts show on the analytics page. If the admin, at any time, restricts which
quants show up, newly added charts will not show up on the page until they are enabled
here. Quant objects are also cached, so the cache must be cleared before new quants
appear in this list.

API - PROVIDE YOUR OWN QUANT CHARTS
--------------------------------------

The real power of Quant lies in it's ability to generate these charts with very 
little provided information. Quant offers a simple, yet flexible API to allow 
developers to include quant charts with their modules.

*/

/**
 * Implements hook_quants().
 *
 * Provide a quant chart of comment creation over time
 */
function hook_quants() {
  $quants = array();

  $quant = new quant;
  $quant->id = 'comment_creation';	 // Unique ID
  $quant->label = t('Comment creation');	 // The title of the chart
  $quant->labelsum = TRUE;	 // Add the sum of items over time to the title
  $quant->table = 'comments';	 // The database table
  $quant->field = 'timestamp';	 // The column that stores the timestamp
  $quant->dataType = 'single';	 // Only one datapoint used
  $quant->chartType = 'line';	 // The type of chart
  $quants[$quant->id] = $quant;
  
  // See what else you can do with 
  // quant in the file includes/quants.inc.

  return $quants;
}

/**
 * Implements hook_quants_alter().
 * 
 * Alter the array of quants before they are cached
 */
function hook_quants_alter(&$quants) {
  $quants['comment_creation']->label = t('Comments added');
}

/**
 * Implements hook_quant_charts().
 * 
 * Provide a custom chart plugin used to render charts
 * 
 * See plugins/QuantChart.inc for how to create a chart plugin class
 */
function hook_quant_charts() {
  $charts = array();
  
  // Table charts
  $chart = new stdClass;
  $chart->id = 'table';
  $chart->name = t('Table charts');
  $chart->description = t('Output chart data in a plain-text table');
  $chart->plugin => array(
    'file' => 'QuantChartTable.inc',
    'path' => $path . '/plugins',
    'class' => 'QuantChartTable', 
  );
  $charts[$chart->id] = $chart;
  
  return $charts;
}

/**
 * Implements hook_quant_charts_alter().
 * 
 * Alter the plugins created in hook_quant_charts()
 */
function hook_quant_charts_alter(&$charts) {
  $charts['table']->description = t('Create table charts.');
}

/**

PRINT A QUANT ANYWHERE
--------------------------------------

Until better support comes for placing quants anywhere, you have the flexibility
to create quant charts in places like blocks, views, and anywhere where PHP is
available. Simply create your quant object then print it via:

$quant->execute('-1 week');
print $quant->render();

The period argument passed into execute() can take three possible variables:
  1) A UNIX timestamp. Data will be fetched from the current time
     back until the provided timestamp.
  2) A string compatable with strtotime(), ie, '-2 weeks'. Data will
     be fetched from the current time until the provided time.
  3) An array containing two of the previous options, where the first
     value will be the start time, and the last will be the end time.
*/
