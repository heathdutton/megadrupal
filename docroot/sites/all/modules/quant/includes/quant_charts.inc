<?php

/**
 * Wrapper for hook_quant_charts().
 */
function _quant_quant_charts() {
  $path = drupal_get_path('module', 'quant');
  $charts = array();
  
  // Table charts
  $chart = new stdClass;
  $chart->id = 'table';
  $chart->name =  t('Table charts');
  $chart->description =  t('Output chart data in a plain-text table');
  $chart->plugin = array(
    'file' => 'QuantChartTable.inc',
    'path' =>  $path . '/plugins',
    'class' => 'QuantChartTable', 
  );
  $charts[$chart->id] = $chart;
  
  // Chart API charts
  if (module_exists('chart')) {
    $chart = new stdClass;
    $chart->id = 'chart';
    $chart->name =  t('Google charts');
    $chart->description =  t('Output chart data in a graphic chart provided by the Google Chart API.');
    $chart->plugin = array(
      'file' => 'QuantChartChartAPI.inc',
      'path' =>  $path . '/plugins',
      'class' => 'QuantChartChartAPI',
    );
    $charts[$chart->id] = $chart;
  }
  
  return $charts;
}
