<?php

/**
 * Load the saved information from the db.
 */
function closeblock_db_load($params) {
  $query = db_select('closeblock', 'closeblock')
    ->fields('closeblock');
    
  foreach ($params as $param=>$value) {
    $query->condition($param, $value, '=');
  }
  
  $result = $query->execute();
  $result = $result->fetchAll();
  
  return $result;
}

/**
 * Delete the saved information from the db.
 */
function closeblock_db_delete($params) {
  $query = db_delete('closeblock');
    
  if ($params) {
    foreach ($params as $param => $value) {
      if ($param == 'theme' && ($value == 'global' || empty($value))) {
        continue;
      }
      $query->condition($param, $value, '=');
    }
  }  
  
  $result = $query->execute();
  
  return $result;
}
