<?php

/**
 * Implements hook_drush_command().
 */
function monitoring_munin_drush_command() {
  $items = array();

  $items['monitoring-munin'] = array(
    'callback' => 'monitoring_munin_drush_callback',
    'options' => array(
      'get-config' => 'Get munin plugin configuration options.',
    ),
    'description' => 'Run munin plugin.',
    'examples' => array(
      'drush monitoring-munin' => 'Runs all sensors and outputs results.',
    ),
    'drupal dependencies' => array('monitoring_munin'),
  );

  return $items;
}

/**
 * Command callback for munin integration.
 */
function monitoring_munin_drush_callback() {
  // We are requested to print munin configuration.
  if (drush_get_option('get-config', FALSE)) {
    $output = '';
    foreach (monitoring_munin_config() as $category => $graph) {
      $output .= "multigraph {$category}\n";
      foreach ($graph as $key => $val) {
        $output .= "$key $val\n";
      }
      $output .= "\n";
    }

    drush_print($output);
  }
  // We are requested to print values.
  else {
    drush_print(_monitoring_munin_result());
  }
}
