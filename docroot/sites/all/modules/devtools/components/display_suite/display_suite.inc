<?php

/**
 * Registers a new Display Suite field.
 * @param [type] $entity_type [description]
 * @param [type] $data        [description]
 */
function add_ds_field($entity_type, $data) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $ds = \devtools\components\DisplaySuite::instance();

    try {
      $ds->addField($entity_type, $data);
    }
    catch(Exception $e) {
      drupal_set_message(t($e->getMessage()), 'error');

      return FALSE;
    }

    return TRUE;
  }
}

/**
 * Callback for all custom display suite fields. Used to check access & render
 * the output;
 */
function ds_field_access($field) {
  $output = '';

  if (is_array($field)) {
    // handle permissions
    $access_callback = FALSE;
    $access = TRUE;

    if (isset($field['access_callback'])) {
      $access_callback = $field['access_callback'];
    }
    else if (isset($field['formatter'])) {
      $access_callback = $field['formatter'] . '_access';
    }

    if ($access_callback && function_exists($access_callback)) {
      $access = $access_callback($field);
    }

    // render output
    if ($access && isset($field['formatter'])) {
      $output .= theme($field['formatter'], $field);
    }
  }

  return $output;
}