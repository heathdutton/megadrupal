<?php

function paas_metric_treemap_boot() {
  global $_paas_metric_treemap;
  static $called = FALSE;
  if ($called) {
    return;
  }
  $called = TRUE;
  
  $_paas_metric_treemap = array();
  
  $_paas_metric_treemap[] = 'bootstrap,conf_init,' . timer_read('page');
  
  timer_start('hook_boot');
  bootstrap_invoke_all('boot');
  $_paas_metric_treemap[] = 'bootstrap,hook_boot,' . timer_read('hook_boot');
  
  timer_start('booted');
}

function paas_metric_treemap_init() {
  global $_paas_metric_treemap;
  static $called = FALSE;
  if ($called) {
    return;
  }
  $called = TRUE;
  
  $_paas_metric_treemap[] = 'bootstrap,load modules,' . timer_read('booted');
  
  timer_start('hook_init');
  module_invoke_all('init');
  $_paas_metric_treemap[] = 'bootstrap,hook_init,' . timer_read('hook_init');
  
  timer_start('page callback');
  $return = node_page_default();
  $_paas_metric_treemap[] = 'page callback,,' . timer_read('page callback');
  
  $regions = system_region_list(variable_get('theme_default', 'garland'));
  timer_start('blocks');
  foreach (array_keys($regions) as $region) {
    theme('blocks', $region);
  }
  $_paas_metric_treemap[] = 'theming,blocks,' . timer_read('blocks');
  
  timer_start('theming');
  theme('page', $return);
  $_paas_metric_treemap[] = 'theming,page,' . timer_read('theming');
}

function paas_metric_treemap_call($value) {
  global $_paas_metric_treemap;
  
  drupal_set_header('Content-Type: text/plain; charset=utf-8');
  echo "level1,level2,time\n";
  echo implode("\n", $_paas_metric_treemap);
  exit;
}
