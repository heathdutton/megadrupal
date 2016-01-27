<?php

function paas_metric_drupalinfo_call($value) {
  switch ($value) {
    case 'variable':
      $name = $_GET['name'];
      $value = variable_get($name, '::default::');
      if ($value === '::default::') {
        drupal_not_found();
        return;
      }
      drupal_add_http_header('Content-Type', 'application/octet-stream');
      echo serialize($value);
      break;
    case 'system':
      drupal_add_http_header('Content-Type', 'text/plain');

      $res = db_select('system', 's')
      ->fields('s', array('name', 'type', 'status', 'info'))
      ->condition('type', array('module', 'theme'), 'IN')
      ->execute()
      ->fetchAll();
      
      echo "name,type,status,title,version";

      foreach ($res as $system) {
        $system->info = unserialize($system->info);
        $title = isset($system->info['name']) ? $system->info['name'] : '';
        $version = isset($system->info['version']) ? $system->info['version'] : '';
        $title = str_replace(array('\\', '"'), array('\\\\', '\\"'), $title);
        echo "\n{$system->name},{$system->type},{$system->status},$title,$version";
      }
      break;
    case 'blocks':
      paas_dump_csv('SELECT bid, module, delta, theme, visibility, pages, cache FROM {block}');
      break;
    case 'filters':
      paas_dump_csv('SELECT format, name, cache FROM {filter_format}');
      break;
  }
}
