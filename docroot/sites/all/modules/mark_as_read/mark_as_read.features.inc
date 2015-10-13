<?php
/**
 * @file
 * Everything that is required by features module to get data back and forth.
 */

/**
 * Implements hook_features_api().
 */
function mark_as_read_features_api() {
  return array(
    'mark_as_read' => array(
      'name' => 'mark_as_read',
      'default_hook' => 'default_mark_as_read_obj',
      'feature_source' => TRUE,
    ),
  );
}

/**
 * Implements hook_features_revert().
 */
function mark_as_read_features_revert($module) {
  mark_as_read_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 *
 * Everything in the code is regarded as the default value,
 * so features determines if its safe to replace the database
 * with the defaults (default_mark_as_read_obj).
 */
function mark_as_read_features_rebuild($module) {
  $default_list = features_get_default('mark_as_read', $module);

  foreach ($default_list as $list) {
    $list = (object) $list;
    // Update if data is modified in the database.
    // Check if the name exists or not.
    if (mark_as_read_db_get_list_by_id($list->list_id)) {
      mark_as_read_update_list_details_by_name($list->list_name, $list->css_selector, $list->attribute_name);
    }
    // Insert if default is not in the database.
    else {
      // While inserting the default value to the database, its important
      // to insert list id also as list_id field is auto_increment which
      // had new value which does not match with default value and always
      // forces features to be in the overridden state.
      mark_as_read_insert_list_details_features($list->list_id, $list->list_name, $list->css_selector, $list->attribute_name);
    }
  }
}

/**
 * Create default value for features import.
 *
 * Create the custom default Object in the features export which does not have
 * list id. As new list id gets created while inserted from the defaults which
 * always forces to be in the overridden state.
 */
function mark_as_read_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $lists = array();';
  foreach ($data as $name) {
    $code[] = "  \$lists['{$name}'] = " . features_var_export(mark_as_read_db_get_list_by_name($name)) . ";";
  }
  $code[] = "  return \$lists;";
  $code = implode("\n", $code);
  return array('default_mark_as_read_obj' => $code);
}

/**
 * Save the default value to the database.
 *
 * Insert all the list details including list id. This is used for
 * reverting the list.
 */
function mark_as_read_insert_list_details_features($list_id, $list_name, $css_selector, $attribute_name) {
  return db_insert('mark_as_read')
  ->fields(array(
    'list_id' => $list_id,
    'list_name' => $list_name,
    'css_selector' => $css_selector,
    'attribute_name' => $attribute_name,
  ))
  ->execute();
}
