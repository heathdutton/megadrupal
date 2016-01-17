<?php
/**
 * Experimental Zebra generator.
 * 
 * @param  [type] $id Unique ID per Zebra items set
 * @return [type]     [description]
 */
function zebra($id) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $data = &drupal_static(__FUNCTION__);
    
    if (is_null($data)) {
      $data = array();
    }
    
    $zebra = array(
      'odd' => 'even',
      'even' => 'odd'
    );
    
    if (!isset($data[$id])) {
      $data[$id] = 'even';
    }
    
    return $data[$id] = $zebra[$data[$id]];
  }
}
