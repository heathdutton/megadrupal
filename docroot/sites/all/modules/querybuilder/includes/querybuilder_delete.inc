<?php

/**
 * @file
 * Demonstrates for dynamic delete query.
 */

// Load querybuilder_common file.
module_load_include('inc', 'querybuilder', 'includes/querybuilder_common');

/**
 * Form builder; Return form for delete.
 */
function querybuilder_delete($form, &$form_state) {
  $options_char = array(
    'eq' => 'Is equal to',
    'neq' => 'Is not equal to',
    'sw' => 'Start with',
    'nsw' => 'Does not start with',
    'ew' => 'End with',
    'new' => 'Does not end with',
    'em' => 'Is Empty',
    'nem' => 'Not Empty',
  );
  $options_num = array(
    'eq' => 'equal to',
    'neq' => 'Is not equal to',
    'lt' => 'Less than',
    'elt' => 'Less than or equal to',
    'egt' => 'Greater than or equal to',
    'in' => 'IN',
    'nin' => 'Not IN',
    'em' => 'Is Empty',
    'nem' => 'Not Empty',
  );
  // Get list of tables name from database.
  $tbl_list = querybuilder_get_tables();

  // Get value from $form_state['values']
  $sel_qry_table = isset($form_state['values']['query_table']) ? $form_state['values']['query_table'] : '_none';
  $sel_qry_fields = isset($form_state['values']['query_fields']) ? $form_state['values']['query_fields'] : '_all';
  if (!empty($sel_qry_table) && $sel_qry_table != '_none') {
    // Get list of field name from table.
    $query_fields = _querybuilder_get_fields($sel_qry_table);
    $query_fields_list = array_combine($query_fields, $query_fields);
  }
  $query_fields_list['_none'] = "Select Fields";
  $form['query_table'] = array(
    '#type' => 'select',
    '#title' => 'Select Table',
    '#options' => $tbl_list,
    '#default_value' => '_none',
    '#ajax' => array(
      'callback' => 'querybuilder_querytable_callback',
      'wrapper' => 'query-table-fields',
    ),
  );
  $form['query_fields'] = array(
    '#type' => 'select',
    '#title' => 'Select Fields',
    '#multiple' => TRUE,
    '#default_value' => '_none',
    '#options' => $query_fields_list,
    '#prefix' => '<div id="query-table-fields">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'querybuilder_queryfield_callback',
      'wrapper' => 'placeholder-fields',
    ),
  );
  $form['placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="placeholder-fields">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $map_fields = array();
  if (!empty($sel_qry_fields) && isset($form_state['values']['query_fields'])) {
    if (!array_key_exists('_none', $sel_qry_fields)) {
      $form['placeholder']['#type'] = 'fieldset';
      foreach ($query_fields as $key => $value) {
        $fetch_key = array();
        $fetch_key = explode('%', $key);
        if (in_array($value, $sel_qry_fields)) {
          if ($fetch_key[1] == "blob" || $fetch_key[1] == "longblob" || $fetch_key[1] == "mediumblob") {
            $form['placeholder'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#attributes' => array('disabled' => TRUE),
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
          else {
            if (strpos($fetch_key[1], 'int') !== FALSE || strpos($fetch_key[1], 'float') !== FALSE || strpos($fetch_key[1], 'decimal') !== FALSE) {
              $options = $options_num;
              $description = $fetch_key[1];
              $description .= "<br>" . t("For 'between' operator value can separate by '|'");
            }
            else {
              $options = $options_char;
              $description = $fetch_key[1];
            }
            $map_fields[$value] = $fetch_key[1];
            $form['placeholder'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#description' => t('Field Type-') . $description,
            );
            $form['placeholder'][$value . "_operator"] = array(
              '#type' => 'select',
              '#title' => 'Operator for ' . $value,
              '#options' => $options,
            );
          }
        }
      }
      $form_state['fields'] = $map_fields;
    }
  }
  $form['build_query'] = array(
    '#type' => 'checkbox',
    '#title' => t('Build Query'),
    '#ajax' => array(
      'callback' => 'querybuilder_buildquery_callback',
      'wrapper' => 'textfields',
      'effect' => 'fade',
    ),
  );
  $form['fieldsets'] = array(
    '#title' => t("Output"),
    '#prefix' => '<div id="textfields">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );
  if (!empty($form_state['values']['build_query']) && $form_state['values']['build_query']) {
    $form['fieldsets']['query_outputs'] = array(
      '#title' => 'Query and Its output',
      '#markup' => querybuilder_delete_result($form, $form_state),
      '#prefix' => '<div id="query-output">',
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Process result from query builder delete form.
 */
function querybuilder_delete_result($form, $form_state) {
  $query_table = $form_state['values']['query_table'];
  $act_query = NULL;
  if (isset($form_state['fields']) && !empty($form_state['fields'])) {
    $query_fields = $form_state['fields'];
    foreach ($query_fields as $key => $value) {
      $validate_result = FALSE;
      if (isset($form_state['values']['placeholder'][$key]) && !empty($form_state['values']['placeholder'][$key])) {
        $field_value = check_plain($form_state['values']['placeholder'][$key]);
        $chk_multi_val = array();
        $chk_multi_val = explode('|', $field_value);
        foreach ($chk_multi_val as $val) {
          // Validate user-input.
          $validate_result = querybuilder_validate_value($value, $val);
          if (!$validate_result) {
            break;
          }
        }
      }
      if (isset($form_state['values']['placeholder'][$key . "_operator"]) && !empty($form_state['values']['placeholder'][$key . "_operator"])) {
        $field_value_operator = $form_state['values']['placeholder'][$key . "_operator"];
        // Get operator.
        $act_oper = querybuilder_actual_operator($field_value_operator);
        if ($validate_result) {
          $map_fields_data[$key] = $field_value;
          $map_fields_operator[$key] = $act_oper;
        }
      }
    }
  }
  if (isset($map_fields_data) && !empty($map_fields_data)) {
    $act_query = "<br>" . '$transaction = db_transaction();';
    $act_query .= "<br>" . 'try {' . "<br>";
		$act_query .= '$query = db_delete(\'' . $query_table . '\')';
    foreach ($map_fields_data as $key => $value) {
      if ($map_fields_operator[$key] == "LIKE_E") {
         $act_query .= "<br>";
				 $act_query .= '->condition(\'' . $key  . '\', \'' . $value . '% \', \'LIKE\')';
      }
      elseif ($map_fields_operator[$key] == "S_LIKE") {
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', \'%' . $value . '\', \'LIKE\')';
      }
      elseif ($map_fields_operator[$key] == "NOT S_LIKE") {
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', \'%' . $value . '\', \'NOT LIKE\')';
      }
      elseif ($map_fields_operator[$key] == "NOT LIKE_E") {
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', \'' . $value . '% \', \'NOT LIKE\')';
      }
      elseif ($map_fields_operator[$key] == "IN") {
        $tem_val = NULL;
        $tem_val = str_replace('|', ',', $value);
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', array(' . $tem_val . '), \'IN\')';
      }
      elseif ($map_fields_operator[$key] == "NOT IN") {
        $tem_val = NULL;
        $tem_val = str_replace('|', ',', $value);
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', array(' . $tem_val . '), \'NOT IN\')';
      }
      else {
        $act_query .= "<br>";
				$act_query .= '->condition(\'' . $key . '\', \'' . $value . '\', ' . '\'' . $map_fields_operator[$key] . '\')';
      }
    }
    $act_query .= "<br>" . '->execute();';
		$act_query .= "<br>" . '}';
    $act_query .= "<br>" . 'catch (Exception $e) {';
    $act_query .= "<br>" . '$transaction->rollback();';
    $act_query .= "<br>" . 'watchdog_exception(\'Query_Issue\', $e);';
    $act_query .= "<br>" . '}';
  }
  else {
    $act_query = 'Insufficient data!';
  }
  $header = array(array('data' => "Query"));
  $rows = array();
  $rows['query'] = array($act_query);
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
