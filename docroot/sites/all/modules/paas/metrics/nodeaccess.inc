<?php

function paas_metric_nodeaccess_call($value) {
  switch ($value) {
    case 'realms':
      paas_dump_csv('SELECT DISTINCT realm FROM {node_access}');
      break;
    case 'modules':
      drupal_set_header('Content-Type: text/plain; charset=utf-8');
      $modules = module_implements('node_access_records');
      print '"module"';
      foreach ($modules as $module) {
        print "\n\"{$module}\"";
      }
      break;
  }
}
