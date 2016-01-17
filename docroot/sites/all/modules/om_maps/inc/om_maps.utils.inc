<?php
// $Id$

/**
 * @file
 * OM Maps Admin Utilities
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
define('OM_MAPS_PATH', drupal_get_path('module', 'om_maps')); 


/**
 * Implementation of hook_init().
 *
 */
function om_maps_init() {
  global $_om_maximenu_variable;
  
  $_om_maximenu_variable  = variable_get('om_maximenu', array());
  
  // checking on OM Maximenu variable if the maps are loaded
  $codes = array_keys(om_maps_loaded_maps_get());

  // getting available maps, codes only
  $maps = om_maps_get(); 
  
  foreach ($maps as $file_name => $map) { 
    $enabled = in_array($map['code'], $codes) ? 1: 0;
    // this will now load necessary styling
    if ($enabled) drupal_add_css(OM_MAPS_PATH . '/maps/' . $file_name . '/' . $file_name . '.css');
  }  
}  


/**
 * OM Maps get
 * - reads directory names and get the map menu properties
 * 
 * returns $maps array ($maps['map_name'] = 'code';
 *
 */
function om_maps_get() {
  module_load_include('inc', 'om_maximenu', 'inc/om_maximenu.utils'); 
   
  $files = array();
  $dir = OM_MAPS_PATH . '/maps/';
  $folders = scandir($dir);
  $excluded_files = array('.', '..', '.cvs', '.svn', '.git');

  foreach ($folders as $key => $val) {
    if (!in_array($val, $excluded_files)) (is_dir($dir . $val)) ? $map_files[$val] = om_maps_content_get(om_string_name($val)): '';     
  }
  
  $maps = array();
  foreach ($map_files as $file_name => $map) {
    $maps[$file_name] = array();
    foreach ($map as $code => $map_name) {  
      $maps[$file_name]['map_name'] = $map_name;
      $maps[$file_name]['code']     = $code;      
    }
  }
  return $maps;
}


/**
 * OM Maps content
 * - This will actually load the links defined in the file
 * 
 * @params
 * - $file_name
 * - $code_only
 *
 * returns $map_content, code only or full
 *
 */
function om_maps_content_get($file_name = NULL, $code_only = TRUE) {

  $map_content = array();
  $code = array();
  
  if ($file_name) {
    $file = OM_MAPS_PATH . '/maps/' . $file_name . '/' . $file_name . '.inc';
    $map_load = file_get_contents($file);
    ob_start();
    eval($map_load);
    //$output = ob_get_contents();
    ob_end_clean(); 
    // $map_name should match what's in the variable
    $map_content = $$file_name; 
    $code[$map_content['code']] = trim($map_content['title']);      
  } 
  
  return ($code_only) ? $code: $map_content;
}
  
  
/**
 * OM Maps getting the titles from OM Maximenu
 * - check if there are current maps
 *   loaded in OM Maximenu variable
 *
 * returns $map_titles ($map_titles['code'] = 'title';
 * 
 */
function om_maps_loaded_maps_get() {
  global $_om_maximenu_variable;
  $maximenu = $_om_maximenu_variable;
  
  $map_titles = array();
  if (!empty($maximenu)) {
    foreach ($maximenu as $menu_key => $content) {
      // code reference, ex. cc001 for USA Map
      // cc001 means country code 1
      $code = $content['code'];
      $map_titles[$code] = $content['title'];
    }
  }
  return $map_titles;
}  
  
  

  
