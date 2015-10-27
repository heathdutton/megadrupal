<?php
/**
 * @file
 * Common function define.
 */

/**
 * Implements function for fetching list of table name from database.
 *
 * @return array $tables_list
 *   Returns array of all tables name.
 */
function querybuilder_get_tables() {
  // Fetch list of table name.
  $schema = drupal_get_schema(NULL, TRUE);
  // Sort ascending.
  ksort($schema);
  foreach ($schema as $table => $value) {
    $tables_list[$table] = $table;
  }
  $tables_list['_none'] = "Select table";
  return $tables_list;
}

/**
 * Implements helper function for fetching list of fields name from table(s).
 *
 * @param string|array $tables
 *   Array of tables name.
 *
 * @return array $field_list
 *   Returns array of all fields name.
 */
function _querybuilder_get_fields($tables) {
  // Fetch list of column names from table(s).
  if (!is_array($tables) && !empty($tables) && ($tables != '_none')) {
    $schema = drupal_get_schema($tables, TRUE);
    foreach ($schema['fields'] as $key => $value) {
      $type = $value['type'];
      $field_list[$key . "%" . $type] = $key;
    }
    return $field_list;
  }
  else {
    if (($tables != '_none') && (is_array($tables))) {
      if (!array_key_exists('_none', $tables)) {
        foreach ($tables as $value) {
          $schema = drupal_get_schema($value, TRUE);
          foreach ($schema['fields'] as $key => $val) {
            $type = $val['type'];
            $field_list[$value . "." . $key . "%" . $type] = $value . "." . $key;
          }
        }
      }
      return $field_list;
    }
  }
}

/**
 * Implements function for validate data type.
 *
 * @param string $data_type
 *   Data type name.
 * @param string $field_value
 *   Value of data type.
 *
 * @return bool
 *   Returns TRUE|FALSE.
 */
function querybuilder_validate_value($data_type, $field_value) {
  if (empty($field_value)) {
    return FALSE;
  }
  $result = FALSE;
  switch ($data_type) {
    case 'varchar':
      $result = TRUE;
      break;

    case 'longtext':
      $result = TRUE;
      break;

    case 'mediumtext':
      $result = TRUE;
      break;

    case 'text':
      $result = TRUE;
      break;

    case 'int':
      if (!is_numeric($field_value)) {
        $result = FALSE;
      }
      else {
        $result = TRUE;
      }
      break;

    case 'tinyint':
      if (!is_numeric($field_value)) {
        $result = FALSE;
      }
      else {
        $result = TRUE;
      }
      break;

    case 'serial':
      if (!is_numeric($field_value)) {
        $result = FALSE;
      }
      else {
        $result = TRUE;
      }
      break;

    case 'float':
      if (!is_numeric($field_value)) {
        $result = FALSE;
      }
      else {
        $result = TRUE;
      }
      break;
  }
  return $result;
}

/**
 * Implements function for getting operator.
 *
 * @param string $operator
 *   Operator name.
 *
 * @return string $sign
 *   Returns operator symbol.
 */
function querybuilder_actual_operator($operator) {
  switch ($operator) {
    case 'eq':
      $sign = '=';
      break;

    case 'neq':
      $sign = '<>';
      break;

    case 'sw':
      $sign = 'S_LIKE';
      break;

    case 'nsw':
      $sign = 'NOT S_LIKE';
      break;

    case 'ew':
      $sign = 'LIKE_E';
      break;

    case 'new':
      $sign = 'NOT LIKE_E';
      break;

    case 'gt':
      $sign = '>';
      break;

    case 'lt':
      $sign = '<';
      break;

    case 'elt':
      $sign = '<=';
      break;

    case 'egt':
      $sign = '>=';
      break;

    case 'in':
      $sign = 'IN';
      break;

    case 'nin':
      $sign = 'NOT IN';
      break;

    case 'em':
      $sign = 'isNull';
      break;

    case 'nem':
      $sign = 'isNotNull';
      break;

    default:
      $sign = '=';
      break;
  }
  return $sign;
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   According to form-ID return renderable array
 */
function querybuilder_querytable_callback($form, $form_state) {
  // Check form-ID.
  if ($form['form_id']['#value'] == "querybuilder_update") {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        ajax_command_replace("#query-table-fields", render($form['query_fields'])),
        ajax_command_replace("#query-table-fields-condition", render($form['condition_fields'])),
      ),
    );
  }
  elseif ($form['form_id']['#value'] == "querybuilder_select") {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        ajax_command_replace("#query-table-fields", render($form['query_fields'])),
        ajax_command_replace("#query-table-join", render($form['placeholder_join'])),
        ajax_command_replace("#query-table-fields-condition", render($form['condition_fields'])),
      ),
    );
  }
  elseif ($form['form_id']['#value'] == "querybuilder_merge") {
    return array(
      '#type' => 'ajax',
      '#commands' => array(
        ajax_command_replace("#query-table-fields", render($form['query_fields'])),
        ajax_command_replace("#query-table-fields-update", render($form['query_fields_update'])),
        ajax_command_replace("#query-table-fields-key", render($form['query_fields_key'])),
      ),
    );
  }
  else {
    return $form['query_fields'];
  }
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The placeholder element.
 */
function querybuilder_queryfield_callback($form, $form_state) {
  return $form['placeholder'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The placeholder_update element.
 */
function querybuilder_queryfield_update_callback($form, $form_state) {
  return $form['placeholder_update'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The placeholder_cond element.
 */
function querybuilder_condfield_callback($form, $form_state) {
  return $form['placeholder_cond'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The fieldsets element.
 */
function querybuilder_buildquery_callback($form, $form_state) {
  return $form['fieldsets'];
}
