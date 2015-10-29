<?php

/**
 * @file
 * Hooks provided by cloud_monitoring module
 * This is a documentation file.
 */

/**
 * Implement this hook to define a new monitoring server.
 */
function hook_cloud_monitor() {
  //example definition of a new monitoring server
  $monitors = array();
  $monitors['collectd'] = array(
      'display_name' => t('NAME'),
      'path' => drupal_get_path('module', 'cloud_monitoring') . '/plugins',
      'file' => 'file.inc',
      'class' => 'class',
  );
  return $monitors;
}

/**
 * Implement this hook to let cloud_monitoring
 * know that a module will support installing
 * to a new OS.  Currently, cloud_monitoring
 * only supports scripts that installs to Ubuntu 
 * and Debian 
 */
function hook_cloud_monitor_installable() {
  //example returns
  return array('redhat', 'centos');
}