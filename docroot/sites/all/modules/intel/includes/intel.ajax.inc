<?php
/**
 * @file
 * Supports ajax callbacks
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

function intel_ajax($endpoint) {  
  watchdog('intel_ajax', print_r($_GET, 1));
  if ($endpoint == 'visitor/save') {
    $visitor = intel_visitor_load('user', 0);
    $visitor->save();
  }
}