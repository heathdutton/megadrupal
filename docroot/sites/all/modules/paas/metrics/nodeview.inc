<?php

function paas_metric_nodeview_call($value) {
  $output = array();
  
  $teaser = FALSE;
  $page = FALSE;
  
  $node = node_load($value);
  if ($node && $node->nid) {
    init_theme();
    
    timer_start('node_view');
    
    $output['_type'] = $node->type;
    timer_start('hook_view');
    if (node_hook($node, 'view')) {
      $node = node_invoke($node, 'view', $teaser, $page);
    }
    else {
      $node = node_prepare($node, $teaser);
    }
    $output['_hook_view'] = round(timer_read('hook_view'), 2);
    $nodeapi = array();
    foreach (module_implements('nodeapi') as $name) {
      timer_start('nodeapi');
      $function = $name .'_nodeapi';
      $function($node, 'view', $teaser, $page);
      $output[$name] = round(timer_read('nodeapi'), 2);
    }
  }
  
  $output['_node_view'] = round(timer_read('node_view'), 2);
  
  header('Content-Type: text/plain');
  print serialize($output);
}
