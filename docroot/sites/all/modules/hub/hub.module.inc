<?php

/******************************************************************************
 * Helper functions
 *****************************************************************************/

function _hub_save($op, $data, $notify) {
  $function = "_hub_save_$op";
  $return = FALSE;
  
  switch ($op) {
    case 'insert':
      if (is_object($data)) {
        if (isset($data->name) && is_string($data->name)) {
          $result = $function($data);
          // @todo db_insert return value doesn't represent success or failure,
          // figure out how relay that information here so we can return some
          // feedback to the user.
          $message = ($result) ? t('Fresh data, alright!') : FALSE;
        }
        else {
          watchdog('hub', '%function requires the "name" property $data to exist and be a string.', array('%function' => $function), WATCHDOG_ERROR);
        }
      }
      else {
        watchdog('hub', '%function expects first argument to be an object.', array('%function' => $function), WATCHDOG_ERROR);
      }
      break;
    
    
    case 'update':
      if (is_object($data)) {
        if (isset($data->name) && is_string($data->name)) {
          $result = $function($data);
          $message = ($result) ? t('Update was a success! High five!') : FALSE;
        }
        else {
          watchdog('hub', '%function requires the "name" property $data to exist and be a string.', array('%function' => $function), WATCHDOG_ERROR);
        }
      }
      else {
        watchdog('hub', '%function expects first argument to be an object.', array('%function' => $function), WATCHDOG_ERROR);
      }      
      break;


    case 'delete':
      if (is_string($data)) {
        $result = $function($data);
        $message = ($result) ? t('All values associated with %name have been deleted.', array('%name' => $data)) : FALSE;
      }
      else {
        watchdog('hub', '%function expects first argument to be a string.', array('%function' => $function), WATCHDOG_ERROR);
      }
      break;

    
    default:
      watchdog('hub', 'Operation %op is not supported.', array('%op' => $op), WATCHDOG_WARNING);
      break;
  }

  if ($notify && $message) {
    drupal_set_message($message);
  }
  
  return $result;
}


/******************************************************************************
 * CRUD functions
 *****************************************************************************/

function _hub_save_insert($data) {
  return db_insert('hub')
    ->fields(array('name' => $data->name, 'data' => $data->data))
    ->execute();
}

function _hub_save_update($data) {
  return db_update('hub')
    ->condition('name', $data->name)
    ->fields(array('name' => $data->name, 'data' => $data->data))
    ->execute();
}

function _hub_save_delete($name) {
  return db_delete('hub')
    ->condition('name', $name)
    ->execute();
}
