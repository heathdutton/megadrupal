<?php

/**
 * System settings page and a sample dashboard
 * @file
 */
 
 
/**
 * System Administration form
 */
function rrd_rest_admin_form($form) {
  $form['rrd_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('RRD Configuration'),
    '#description' => t('RRD Api related configurations'),
  );
  
  $form['rrd_config']['rrd_rest_data_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('RRD Data Directory'),
    '#default_value' => RRD_REST_DATA_DIR,
    '#description' => t('Location of the RRD Data Directory. Use an absolute file system path'),
  );
  
  $form['rrd_config']['rrd_rest_rrdtool'] = array(
    '#type' => 'textfield',
    '#title' => t('RRD Tool'),
    '#default_value' => RRD_REST_RRDTOOL,
    '#description' => t('Location of the RRD Tool. Use an absolute file system path'),
  );
  
  return system_settings_form($form);
}

/**
 * Dashboard
 */
function rrd_rest_dashboard() {
  $output = '';
  $host = isset($_GET['host']) ? $_GET['host'] : NULL;

  $hosts = _rrd_rest_collectd_hosts();
  $output .= '<h2>Hosts</h2>';
  if ($hosts != FALSE) {
    foreach ($hosts as $value) {
      $output .= l($value, 'monitor-server/dashboard', array('query' => array('host' => $value)));
    }
    $output .= '<hr>';
    if (!is_null($host)) {
      $plugins = rrd_rest_supported_plugins();
  
      foreach ($plugins as $plugin) {
        $output .= "<h3>" . $plugin . "</h3>";
        $output .= _rrd_rest_dashboard_graph($host, $plugin);
      }
    }
  }
  else {
    $output .= t('No hosts found. Make sure an RRD directory exists and Collectd is configured');
  }
  return $output;
}

/**
 * Detailed graph page
 */
function rrd_rest_dashboard_graph() {
  $output = '';
  $time_interval = array(
    '60' => t('1 Minute'),
    '300' => t('5 Minutes'),
    '3600' => t('1 Hour'),
    '7200' => t('2 Hours'),
    '86400' => t('1 Day'),
    '604800' => t('1 Week'),
    '31449600' => t('1 Year'),
  );
  
  $params = _rrd_rest_parse_params();
  $params['image_preset'] = 'large';
  
  //copy the params to use for the time interval links
  $link_params = $params;
  $time_links = array();
  foreach ($time_interval as $key => $value) {
    $link_params['seconds'] = $key;
    $time_links[] =  l($value, 'monitor-server/dashboard-graph', array('query' => $link_params));
  }
  if (preg_match('/^(60|300)$/', $params['seconds'])) {
    //add javascript to refresh the image every 15 seconds
    $script =  "
    (function ($) {
    jQuery(document).ready(function() {
      setInterval(function(){
      var source = $('.graph-image').attr('src');
      source += '&' + new Date().getTime();
      $('.graph-image').attr('src', source);
      }, 5000);}
    );
    }(jQuery));
    ";  
   drupal_add_js($script, array('type' => 'inline', 'scope' => 'footer'));
  }
  $output .= implode(' | ', $time_links);
  $output .= '<img class="graph-image" src="' . url('monitor-server/get-graph-by-plugin', array('query' => $params)) . '">';
  $output .= l(t('Back to Dashboard'), 'monitor-server/dashboard', array('query' => array('host' => $params['host'])));
  return $output;
}

/**
 * Loops the plugin and generate the graphs
 */
function _rrd_rest_dashboard_graph($host, $plugin) {
  $output = '<div>';
  $plugin_info = _rrd_rest_collectd_plugin_info($host, $plugin);
  
  $values = _rrd_rest_group_data($plugin_info);
  foreach ($values as $value) {
    $query = array(
      'seconds' => 86400,
      'plugin' => $plugin,
      'host' => $host,
      'image_preset' => 'medium',
    );
    if (isset($value['plugin_instance'])) {
      $query['plugin_instance'] = $value['plugin_instance'];
    }
    if (isset($value['type'])) {
      $query['type'] = $value['type'];
    }
    if (isset($value['type_instance'])) {
      $query['type_instance'] = $value['type_instance'];
    }
    $link = '<img src="' . url('monitor-server/get-graph-by-plugin', array('query' => $query)) . '">';
    
    //change the image_preset to large
    $query['image_preset'] = 'large';
    $output .= l($link, 'monitor-server/dashboard-graph', array('html' => TRUE, 'query' => $query));
  }
  $output .= '</div>';
  return $output;
}
