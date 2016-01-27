<?php

function paas_metric_nodeload_call($value) {
  $output = array();
  timer_start('node_load');
  
  $cond = 'n.nid = %d';
  $arguments = array($value);
  
  $fields = drupal_schema_fields_sql('node', 'n');
  $fields = array_merge($fields, drupal_schema_fields_sql('node_revision', 'r'));
  $fields = array_merge($fields, array('u.name', 'u.picture', 'u.data'));
  $fields = array_diff($fields, array('n.vid', 'n.title', 'r.nid'));
  $fields = implode(', ', $fields);
  $fields = str_replace('r.timestamp', 'r.timestamp AS revision_timestamp', $fields);
  $fields = str_replace('r.uid', 'r.uid AS revision_uid', $fields);
  $node = db_fetch_object(db_query('SELECT '. $fields .' FROM {node} n INNER JOIN {users} u ON u.uid = n.uid INNER JOIN {node_revisions} r ON r.vid = n.vid WHERE '. $cond, $arguments));
  
  if ($node && $node->nid) {
    $output['_type'] = $node->type;
    timer_start('hook_load');
    if ($extra = node_invoke($node, 'load')) {
      foreach ($extra as $key => $value) {
        $node->$key = $value;
      }
    }
    $output['_hook_load'] = round(timer_read('hook_load'), 2);
    $nodeapi = array();
    foreach (module_implements('nodeapi') as $name) {
      timer_start('nodeapi');
      $a3 = NULL;
      $a4 = NULL;
      $function = $name .'_nodeapi';
      $function($node, 'load', $a3, $a4);
      $output[$name] = round(timer_read('nodeapi'), 2);
    }
  }
  
  $output['_node_load'] = round(timer_read('node_load'), 2);
  
  header('Content-Type: text/plain');
  print serialize($output);
}
