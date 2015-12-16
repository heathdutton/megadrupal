<?php

/**
 * @file
 * Hooks for Views integration.
 * Provides field handler for Gelocate, along with some open-ended code for defining plugins. 
 * There's also a lot of boilerplate here which is not really needed. 
 */

/**
 * Implements hook_views_data().
 */

function geolocate_views_data() {
  
  //$data = field_views_field_default_views_data($field);
  //$field_name = $field['field_name'];  
  //$table_name = 'geolocate_proximity';  
    
  $data['geolocate']['table']['group'] = t('Global');
  $data['geolocate']['table']['join'] = array(
    // Exist in all views.
    '#global' => array(),
  );
 
  $data['geolocate']['geolocate_proximity'] = array(
    
    'title' => t('Geolocate user - proximity'),
    'title short' => t('User\'s lat-long'),
    'help' => t('Field containing the current users location. (Real time, not a field on their profile.)'),
    
    'field' => array(
      'handler' => 'views_handler_geolocate_proximity',
      'field' => 'field_geolocate_proximity',
      'field_name' => 'Field geolocate proximity',
      'float' => TRUE,
      'click sortable' => TRUE,
      //'table' => $table_name,
      //'real_field' => $table_name,
    ),

    'filter' => array(
      'field' => 'field_geolocate_distance',
      'handler' => 'views_handler_filter_numeric',
      'field_name' => 'Field geolocate proximity',
      //'table' => $table_name,
      //'real_field' => $table_name,
      ),
          
    'sort' => array(
      'handler' => 'views_handler_sort',
/*
      'field' => 'field_geolocate_distance',
      'field_name' => 'Field geolocate distance',
      'table' => $table_name,
      'real_field' => $table_name,
*/
      ),
      
    'argument' => array(
      'handler' => 'geolocate_handler_argument_proximity',
      'field_name' => 'Field geolocate proximity',
      ),      
    );
  
  return $data;
}




