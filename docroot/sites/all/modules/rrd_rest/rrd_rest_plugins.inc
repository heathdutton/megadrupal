<?php

/**
 * This file contains graph definition functions
 * @file
 */

/**
 * Function for swap graph
 */
function rrd_rest_swap_swap($config) {
  $obj = new Type_GenericStacked($config);
  $obj->order = array('free', 'cached', 'used');
  $obj->ds_names = array(
    'free' => t('Free'),
    'cached' => t('Cached'),
    'used' => t('Used'),
  );
  $obj->colors = array(
    'free' => '00e000',
    'cached' => '0000ff',
    'used' => 'ff0000',
  );
  $obj->rrd_title = t('Swap utilization');
  $obj->rrd_vertical = t('Bytes');
  $obj->rrd_format = '%5.1lf%s';
  return $obj;
}

/**
 * Function for swap_io graph
 */
function rrd_rest_swap_io($config) {
  $obj = new Type_GenericIO($config);
  $obj->order = array('out', 'in');
  $obj->ds_names = array(
    'out' => t('Out'),
    'in'  => t('In'),
  );
  $obj->colors = array(
    'out' => '0000ff',
    'in'  => '00b000',
  );
  $obj->rrd_title = t('Swapped I/O pages');
  $obj->rrd_vertical = t('Pages');
  $obj->rrd_format = '%5.1lf%s';
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to Processes
 */
function rrd_rest_plugin_processes($config) {
  $obj = new Type_GenericStacked($config);
  $obj->rrd_format = '%5.1lf%s';
  $obj->ds_names = array(
    'paging' => t('Paging'),
    'blocked' => t('Blocked'),
    'zombies' => t('Zombies'),
    'stopped' => t('Stopped'),
    'running' => t('Running'),
    'sleeping' => t('Sleeping'),
  );
  $obj->colors = array(
    'paging' => 'ffb000',
    'blocked' => 'ff00ff',
    'zombies' => 'ff0000',
    'stopped' => 'a000a0',
    'running' => '00e000',
    'sleeping' => '0000ff',
    'value' => 'f0a000',
  );
  $obj->rrd_title = t('Processes');
  $obj->rrd_vertical = t('Processes');
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to Disk
 */
function rrd_rest_plugin_disk($config) {
  $obj = new Type_GenericIO($config);
  $obj->data_sources = array('read', 'write');
  $obj->ds_names = array(
    'read' => t('Read'),
    'write' => t('Written'),
  );
  $obj->colors = array(
    'read' => '0000ff',
    'write' => '00b000',
  );

  switch ($obj->args['type']) {
    case 'disk_merged':
      $obj->rrd_title = t('Disk Merged Operations (@s)', array('@s' => $obj->args['plugin_instance']));
      $obj->rrd_vertical = t('Merged operations/s');
      $obj->rrd_format = '%5.1lf%s';
    break;
    case 'disk_octets':
      $obj->rrd_title = t('Disk Traffic (@s)', array('@s' => $obj->args['plugin_instance']));
      $obj->rrd_vertical = t('Bytes per second');
      $obj->rrd_format = '%5.1lf%s';
    break;
    case 'disk_ops':
      $obj->rrd_title = t('Disk Operations (@s)', array('@s' => $obj->args['plugin_instance']));
      $obj->rrd_vertical = t('Ops per second');
      $obj->rrd_format = '%5.1lf%s';
    break;
    case 'disk_time':
      $obj->rrd_title = t('Disk time per operation (@s)', array('@s' => $obj->args['plugin_instance']));
      $obj->rrd_vertical = t('Avg. Time/Op');
      $obj->rrd_format = '%5.1lf%s';
      $obj->scale = '0.001';
    break;
  }
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to Swap Space
 */
function rrd_rest_plugin_swap($config) {
  switch ($config['args']['type']) {
    case 'swap':
      return rrd_rest_swap_swap($config);
    case 'swap_io':
      return rrd_rest_swap_io($config);
  }
}

/**
 * Returns object with graphing parameters
 * related to Disk Usage
 */
function rrd_rest_plugin_df($config) {
  $obj = new Type_GenericStacked($config);
  $obj->data_sources = array('free', 'used');
  
  $obj->order = array(
    'reserved', 
    'free', 
    'used'
  );
  
  $obj->ds_names = array(
    'reserved' => t('Reserved'),
    'free' => t('Free'),
    'used' => t('Used'),
  );
  $obj->colors = array(
    'reserved' => 'aaaaaa',
    'free' => '00ff00',
    'used' => 'ff0000',
  );
  
  $obj->rrd_title = t('Free space (@s)', array('@s' => $obj->args['plugin_instance']));
  $obj->rrd_vertical = t('Bytes');
  $obj->rrd_format = '%5.1lf%sB';

  $obj->rrd_title = t('Free space (@s)', array('@s' => $obj->args['type_instance']));
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to network interface
 */
function rrd_rest_plugin_interface($config) {
  $obj = new Type_GenericIO($config);
  $obj->data_sources = array('rx', 'tx');
  $obj->ds_names = array(
    'rx' => t('Receive'),
    'tx' => t('Transmit'),
  );
  $obj->colors = array(
    'rx' => '0000ff',
    'tx' => '00b000',
  );

  $obj->rrd_format = '%5.1lf%s';
  
  switch ($obj->args['type']) {
    case 'if_errors':
      $obj->rrd_title = t('Interface Errors (@s)', array('@s' => $obj->args['type_instance']));
      $obj->rrd_vertical = t('Errors per second');
    break;
    case 'if_octets':
      $obj->rrd_title = t('Interface Traffic (@s)', array('@s' => $obj->args['type_instance']));
      $obj->rrd_vertical = t('Bytes per second');
    break;
    case 'if_packets':
      $obj->rrd_title = t('Interface Packets (@s)', array('@s' => $obj->args['type_instance']));
      $obj->rrd_vertical = t('Packets per second');
    break;
  }
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to cpu
 */
function rrd_rest_plugin_cpu($configs) {
  $obj = new Type_GenericStacked($configs);
  
  $obj->order = array(
    'idle', 
    'nice', 
    'user', 
    'wait', 
    'system', 
    'softirq', 
    'interrupt', 
    'steal'
  );
  
  $obj->ds_names = array(
    'idle' => t('Idle'),
    'nice' => t('Nice'),
    'user' => t('User'),
    'wait' => t('Wait-IO'),
    'system' => t('System'),
    'softirq' => t('SoftIRQ'),
    'interrupt' => t('IRQ'),
    'steal' => t('Steal'),
  );
  
  $obj->colors = array(
    'idle' => 'e8e8e8',
    'nice' => '00e000',
    'user' => '0000ff',
    'wait' => 'ffb000',
    'system' => 'ff0000',
    'softirq' => 'ff00ff',
    'interrupt' => 'a000a0',
    'steal' => '000000',
  );


  $obj->rrd_title = t('CPU-@s usage', array('@s' => $obj->args['plugin_instance']));
  $obj->rrd_vertical = t('Jiffies');
  $obj->rrd_format = '%5.2lf';
  
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to Memory
 */
function rrd_rest_plugin_memory($configs) {
  $obj = new Type_GenericStacked($configs);
  $obj->ds_names = array(
    'free' => t('Free'),
    'cached' => t('Cached'),
    'buffered' => t('Buffered'),
    'locked' => t('Locked'),
    'used' => t('Used'),
  );
  
  $obj->colors = array(
    'free' => '00e000',
    'cached' => '0000ff',
    'buffered' => 'ffb000',
    'locked' => 'ff00ff',
    'used' => 'ff0000',
  );
  $obj->order = array(
    'free',
    'buffered',
    'cached',
    'locked',
    'used',
  );
  
  $obj->rrd_title = t('Physical memory utilization');
  $obj->rrd_vertical = t('Bytes');
  $obj->rrd_format = '%5.1lf%s';
  return $obj;
}

/**
 * Returns object with graphing parameters
 * related to CPU Load
 */
function rrd_rest_plugin_load($configs) {
  $obj = new Type_Default($configs);
  $obj->data_sources = array('shortterm', 'midterm', 'longterm');
  $obj->ds_names = array(
    'shortterm' => t('1 min'),
    'midterm' => t('5 min'),
    'longterm' => t('15 min'),
  );
  
  $obj->colors = array(
    'shortterm' => '00ff00',
    'midterm' => '0000ff',
    'longterm' => 'ff0000',
  );
  
  $obj->rrd_title = t('System load');
  $obj->rrd_vertical = t('System load');
  $obj->rrd_format = '%.2lf';
  return $obj;
}