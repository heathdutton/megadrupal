<?php

/**
 *  @file
 *  The main file that describes the "overrides" of the original views
 */

/**
 * Implementats of hook_views_data().
 */
function odata_views_data() {
  
  $data = array();
  $query = db_query('SELECT title, url, options FROM {odata_endpoints}');
  
  foreach ($query as $row) {
    $data[$row->title]['table']['group']  = t('Open Data');
  
    $data[$row->title]['table']['base'] = array(
      'field' => 'vsid', 'title' => $row->title, 'help' => t('Directly query test_endpoint to get some live views data.'), 'query class' => 'odata_query'
    );
    
    $data[$row->title]['table']['endpoint'] = $row->url . '/' . unserialize($row->options);
  }

  return $data;
}

/**
 * Implementats of hook_views_data_alter().
 */
function odata_views_data_alter(&$datas) {
  

  require('odata.admin.inc');
  $query = db_query('SELECT title,options,url FROM {odata_endpoints}');
  foreach ($query as $row) {
    $uri = $row->url . '/$metadata';
    $reply = drupal_http_request($uri);
    $xmltree=odata_xml2array2($reply->data, $row->url);
    $tree = odata_get_entity_properties($xmltree);
    $cat = unserialize($row->options);
  
  $complexes = odata_get_complex_properties($xmltree);
  
    if (!empty($tree[$cat]))
    foreach ($tree[$cat]['Values'] as $index => $value) {
      if (odata_get_type($value['Type']) !='' ) {
        $datas[$row->title][$value['Name']] = array(
          'title' => $value['Name'],
      'group' => $row->title,
          'field' => array(
            'handler' => _odata_get_field_handler($value['Type']), //'odata_handler_field',  //'views_handler_field_' . odata_get_type($value['Type']) . ' .inc', 'handler' =>_odata_get_field_handler($value['Type']),
            'bundle' => odata_get_type($value['Type']),
            'click sortable' => TRUE,
            'help' => $value['Name'],
          ), 'sort' => array(
            'help' => t('Order the results of the query as ' . $value['Name'] . '.'),
            'handler' => _odata_get_sort_handler($value['Type']),
          ), 'filter' => array(
            'handler' => _odata_get_filter_handler($value['Type']),
            'bundle' => odata_get_type($value['Type']),
      'help' => $value['Name'],
          ), 'argument' => array(
            'handler' => _odata_get_argument_handler($value['Type']),
      'bundle' => odata_get_type($value['Type']),
      'help' => $value['Name'],
          ),
        );
    }
    elseif ($value['Type'] != 'Null' ) {
      _odata_views_add_complex( $datas, $complexes, $row->title, $value['Type'], $value['Name']);
    }
    }
  }
}

/**
 *  Adds the Complex Types to the View
 */
function _odata_views_add_complex(&$datas, &$complexes, $table_name, $value_type, $name ='' ) {
  $complex_types = odata_get_complex_entity($complexes, $value_type);
  foreach ($complex_types as $i => $v ) {
    
    $datas[$table_name][ $name . '-' . $i] = array(
      'title' => $name . ': ' . $i,
    'group' => $table_name,
      'field' => array(
        'handler' => _odata_get_complex_field_handler($v['Type']),
        'bundle' => odata_get_type($v['Type']),
        'click sortable' => TRUE,
        'help' => $name . ':' . $i,
      ), 'sort' => array(
        'help' => t('Order the results of the query as ' . $name . ':' . $i . '.'),
        'handler' => _odata_get_sort_handler($v['Type']),
      ), 'filter' => array(
        'handler' => _odata_get_filter_handler($v['Type']),
        'bundle' => odata_get_type($v['Type']),
        'help' => $v['Name'],
      ), 'argument' => array(
        'handler' => _odata_get_argument_handler($v['Type']),
        'bundle' => odata_get_type($v['Type']),
        'help' => $v['Name'],
      ),
    );
  }
}

/**
 * Implementats of hook_views_plugins().
 */
function odata_views_plugins() {
  return array(
    'query' => array(
      'odata_query' => array(
        'title' => t('oData Query'), 'help' => t('Query will be generated and run using current oData endpoint.'),
        'handler' => 'odata_plugin_query_odata'
      ),
    ),
  );
}

/**
 *  It returns the correct filter handler for each type
 */
function _odata_get_filter_handler( $value_type ) {
  $type = odata_get_type($value_type);
  $ret = 'odata_handler_filter';
  if ($type=='numeric') {
    $ret = 'odata_handler_filter_numeric';
  }
  elseif ($type=='date') {
    $ret = 'odata_handler_filter_date';
  }
  elseif ($type=='boolean') {
    $ret = 'odata_handler_filter_boolean_operator';
  }
  elseif ($type=='markup') {
    $ret = 'odata_handler_filter_markup';
  }
  return $ret;
}

/**
 *  It returns the correct field handler for each type
 */
function _odata_get_field_handler( $value_type ) {
  $type = odata_get_type($value_type);
  $ret = 'odata_handler_field';
  if ($type=='numeric') {
    $ret = 'odata_handler_field_numeric';
  }
  elseif ($type=='date') {
    $ret = 'odata_handler_field_date';
  }
  elseif ($type=='boolean') {
    $ret = 'odata_handler_field_boolean';
  }
  elseif ($type=='markup') {
    $ret = 'odata_handler_field';
  }
  return $ret;
}

/**
 *  It returns the correct sort handler for each type
 */
function _odata_get_sort_handler( $value_type ) {
  $type = odata_get_type($value_type);
  if ($type=='date') {
    return 'odata_handler_sort_date';
  }
  else{
    return 'odata_handler_sort';
  }
}

/**
 *  It returns the correct field handler for each type
 */
function _odata_get_complex_field_handler( $value_type ) {
  $type = odata_get_type($value_type);
  $ret = 'odata_handler_field_complex';
  if ($type=='numeric') {
    $ret = 'odata_handler_field_complex_numeric';
  }
  elseif ($type=='date') {
    $ret = 'odata_handler_field_complex_date';
  }
  elseif ($type=='boolean') {
    $ret = 'odata_handler_field_complex_boolean';
  }
  elseif ($type=='markup') {
    $ret = 'odata_handler_field_complex';
  }
  return $ret;
}

/**
 *  It returns the correct argument handler for each type (Contextual Filters)
 */
function _odata_get_argument_handler( $value_type ) {
  $type = odata_get_type($value_type);
  $ret = 'odata_handler_argument_strings';
  if ($type=='numeric') {
    $ret = 'odata_handler_argument_numeric';
  }
  elseif ($type=='date') {
    $ret = 'odata_handler_argument_date';
  }
  elseif ($type=='boolean') {
    $ret = 'odata_handler_argument_boolean';
  }
  elseif ($type=='markup') {
    $ret = 'odata_handler_argument_string';
  }
  return $ret;
}

