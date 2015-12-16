<?php
// $Id: simple_page_title.db.inc,v 1.1 2011/02/02 22:13:28 jeebsuk Exp $

/**
 * @file
 * Simple Page Title - Database Helper Functions
 *
 * @author
 * Jamie Giberti <jagiberti@gmail.com>
 */

/**
 * Simple DB Function to insert a row into the simple_page_title DB table.
 * @param string $path
 * @param string $page_title
 * @return bool
 */
function simple_page_title_add_page_title_to_db($path, $page_title) {
  $sql = 'INSERT INTO {simple_page_title} (path, page_title) VALUES ("%s", "%s")';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $path, $page_title) */
  $result = $id = db_insert('simple_page_title')
  ->fields(array(
    'path' => $path,
    'page_title' => $page_title,
  ))
  ->execute();
  return $result;
}

/**
 * Simple DB Function to update a row in the simple_page_title DB table.
 * @param string $path
 * @param string $page_title
 * @return unknown_type
 */
function simple_page_title_update_page_title_for_path($path, $page_title) {
  $sql = 'UPDATE {simple_page_title} SET page_title = "%s" WHERE path = "%s"';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $page_title, $path) */
  $result = db_update('simple_page_title')
  ->fields(array(
    'page_title' => $page_title,
  ))
  ->condition('path', $path)
  ->execute();
  return $result;
}

/**
 * Simple DB Function to delete a row in the simple_page_title DB table.
 * @param string $path
 * @return bool
 */
function simple_page_title_delete_page_title_for_path($path) {
  $sql = 'DELETE FROM {simple_page_title} WHERE path = "%s"';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $path) */
  $result = db_delete('simple_page_title')
  ->condition('path', $path)
  ->execute();
  return $result;
}

/**
 * Function to return a count of the number of records for a particular path (should never be greater than one)
 * @param string $path
 * @param string $path_alias
 * @return int
 */
function simple_page_title_get_page_title_for_path_count($path, $path_alias = NULL) {
  $sql_d7 = 'SELECT COUNT(*) as path_count FROM {simple_page_title} WHERE path = :path_1';
  if (empty($path_alias)) {
    // TODO Please convert this statement to the D7 database API syntax.
    $path_count = db_query($sql_d7, array(':path_1' => $path))->fetchField();
  }
  else {
    $sql_d7 .= ' OR path = :path_2';
    $path_count = db_query($sql_d7, array(':path_1' => $path, ':path_2' => $path_alias))->fetchField();
  }

  return $path_count;
}

/**
 * Function to return an object for a given id
 * @param string $path
 * @param string $path_alias
 * @return object
 */
function simple_page_title_get_page_title_object_for_id($id) {  
  $sql_d7 = 'SELECT * FROM {simple_page_title} WHERE pt_id = :pt_id';
  
  $result = db_query($sql_d7, array(':pt_id' => $id))->fetchObject();
  
  return $result;
//  foreach ($result as $row) {
//  	//should only be one row
//  	return $row;
//  }
  
//  $select = db_select('simple_page_title');
//  $select->condition('pt_id', $id);
//  $result = $select->execute();
//  return $result->fetchObject();

}

/**
 * Function to return an object for a particular path (should never be greater than one match)
 * @param string $path
 * @param string $path_alias
 * @return object
 */
function simple_page_title_get_page_title_object_for_path($path, $path_alias = NULL) {  
  $sql_d7 = 'SELECT * FROM {simple_page_title} WHERE path = :path_1';
  if (empty($path_alias)) {
    $result = db_query($sql_d7, array(':path_1' => $path))->fetchObject();
  }
  else {
    $sql_d7 .= ' OR path = :path_2';
    $result = db_query($sql_d7, array(':path_1' => $path, ':path_2' => $path_alias))->fetchObject();
  }
  
  return $result;
  //print_r($result) . '<br />';
//  foreach ($result as $row) {
//  	//print_r($row);
//  }

//  if (!empty($path_alias)) {
//    $result_object = db_fetch_object($result);
//  }
//  $result_object = '';
//  return $result_object;
}

/**
 * Function to retrieve the weight of a given module from the system table
 * @param string $module_name
 * @return int
 */
function simple_page_title_get_module_weight($module_name) {
  $sql = 'SELECT weight FROM {system} WHERE name = :name';
  $module_weight = (int) db_query($sql, array(':name' => $module_name))->fetchField();
  return $module_weight;
}

/**
 * Function to update the weight of a module in the system table
 * @param string $module_name
 * @param int $module_weight
 * @return bool
 */
function simple_page_title_update_module_weight($module_name, $module_weight) {
  $sql = 'UPDATE {system} SET weight = %d WHERE name = "%s"';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $module_weight, $module_name) */
  $result = db_update('system')
  ->fields(array(
    'weight' => $module_weight,
  ))
  ->condition('name', $module_name)
  ->execute();
  return $result;
}
