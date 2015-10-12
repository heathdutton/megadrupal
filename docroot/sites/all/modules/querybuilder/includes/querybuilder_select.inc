<?php

/**
 * @file
 * Demonstrates for dynamic select query.
 */

// Load querybuilder_common file.
module_load_include('inc', 'querybuilder', 'includes/querybuilder_common');

/**
 * Form builder; Return form for select.
 */
function querybuilder_select($form, &$form_state) {
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
  $sel_cond_fields = isset($form_state['values']['condition_fields']) ? $form_state['values']['condition_fields'] : '_none';
  if ($sel_qry_table != '_none') {
    if (!array_key_exists('_none', $sel_qry_table)) {
      // Get list of field name from table.
      $query_fields = _querybuilder_get_fields($sel_qry_table);
      $query_fields_list = array_combine($query_fields, $query_fields);
    }
  }
  $query_fields_list['_none'] = "Select Fields";
  $form['query_table'] = array(
    '#type' => 'select',
    '#title' => 'Select Table',
    '#multiple' => 'TRUE',
    '#options' => $tbl_list,
    '#default_value' => '_none',
    '#ajax' => array(
      'callback' => 'querybuilder_querytable_callback',
      'wrapper' => array('query-table-join',
        'query-table-fields',
        'query-table-fields-condition',
      ),
    ),
  );
  $form['placeholder_join'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="query-table-join">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($sel_qry_table) && isset($form_state['values']['query_table'])) {
    $num_table = count($sel_qry_table);
    if ($num_table > 1) {
      $form['placeholder_join']['#type'] = 'fieldset';
      $form['placeholder_join']['#title'] = 'Join';
      for ($limit = 1; $limit < $num_table; $limit++) {
        $form['placeholder_join']['joingrp_' . $limit] = array(
          '#type' => 'fieldset',
          '#tree' => TRUE,
        );
        $form['placeholder_join']['joingrp_' . $limit][$limit . "_L"] = array(
          '#type' => 'select',
          '#title' => 'Join Fields',
          '#options' => $query_fields_list,
        );
        $form['placeholder_join']['joingrp_' . $limit][$limit . "_R"] = array(
          '#type' => 'select',
          '#title' => 'Join Fields',
          '#options' => $query_fields_list,
        );
      }
    }
  }
  $form['query_fields'] = array(
    '#type' => 'select',
    '#title' => 'Fetch Fields',
    '#multiple' => TRUE,
    '#default_value' => '_none',
    '#options' => $query_fields_list,
    '#prefix' => '<div id="query-table-fields">',
    '#suffix' => '</div>',
  );
  $form['condition_fields'] = array(
    '#type' => 'select',
    '#title' => 'Condition Fields',
    '#multiple' => TRUE,
    '#default_value' => '_none',
    '#options' => $query_fields_list,
    '#prefix' => '<div id="query-table-fields-condition">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'querybuilder_condfield_callback',
      'wrapper' => 'placeholder-fields-condition',
    ),
  );
  $form['placeholder_cond'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="placeholder-fields-condition">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $map_cond_fields = array();
  if (!empty($sel_cond_fields) && isset($form_state['values']['condition_fields'])) {
    if (!array_key_exists('_none', $sel_cond_fields)) {
      $form['placeholder_cond']['#type'] = 'fieldset';
      foreach ($query_fields as $key => $value) {
        $fetch_key = array();
        $fetch_key = explode('%', $key);
        if (in_array($value, $sel_cond_fields)) {
          if ($fetch_key[1] == "blob" || $fetch_key[1] == "longblob" || $fetch_key[1] == "mediumblob") {
            $form['placeholder_cond'][$value] = array(
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
            $map_cond_fields[$value] = $fetch_key[1];
            $form['placeholder_cond'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#description' => t('Field Type-') . $description,
            );
            $form['placeholder_cond'][$value . "_operator"] = array(
              '#type' => 'select',
              '#title' => 'Operator for ' . $value,
              '#options' => $options,
            );
          }
        }
      }
      $form_state['cond_fields'] = $map_cond_fields;
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
      '#markup' => querybuilder_select_result($form, $form_state),
      '#prefix' => '<div id="query-output">',
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Process result from query builder select form.
 */
function querybuilder_select_result($form, $form_state) {
  $query_table_array = $form_state['values']['query_table'];
  $query_fetch_fields = $form_state['values']['query_fields'];
  $act_query = NULL;
  if (!array_key_exists('_none', $query_table_array)) {
    $i = 0;
    // Get table symbol.
    $query_tbl_list = querybuilder_table_key($query_table_array);
    $query_tbl_data = array_flip($query_tbl_list);
    foreach ($query_tbl_list as $key => $value) {
      if ($i < 1) {
        $act_query = '$query = db_select(\'' . $value . '\', \'' . $key . '\');';
      }
      else {
        $lhs = $form_state['values']['placeholder_join']['joingrp_' . $i][$i . '_L'];
        $rhs = $form_state['values']['placeholder_join']['joingrp_' . $i][$i . '_R'];
        $l_tbl_name = explode('.', $lhs);
        $r_tbl_name = explode('.', $rhs);
        $lkey = $l_tbl_name[0];
        $rkey = $r_tbl_name[0];
        $jn_l = $query_tbl_data[$lkey] . "." . $l_tbl_name[1];
        $jn_r = $query_tbl_data[$rkey] . "." . $r_tbl_name[1];
        $act_query .= "<br>" . '$query->join(\'' . $value . '\', \'' . $key . '\', \'' . $jn_l . ' = ' . $jn_r . '\');';
      }
      $i++;
    }
    $i = 0;
    $field_array = array();
    if (!array_key_exists('_none', $query_fetch_fields)) {
      foreach ($query_fetch_fields as $key => $value) {
        $split_name = explode('.', $key);
        $tbl = $split_name[0];
        $tbl_key = $query_tbl_data[$tbl];
        if (array_key_exists($tbl_key, $field_array)) {
          $i++;
        }
        else {
          $i = 1;
        }
        $field_array[$tbl_key][$i] = $split_name[1];
      }
      foreach ($field_array as $key => $value) {
        $fld_array = 'array(';
        $first = 0;
        foreach ($value as $in_val) {
          if ($first < 1) {
            $fld_array .= '\'' . $in_val . '\'';
          }
          else {
            $fld_array .= ', \'' . $in_val . '\'';
          }
          $first++;
        }
        $fld_array .= ')';
        $act_query .= "<br>" . '$query->fields(\'' . $key . '\', ' . $fld_array . ');';
      }
    }
    else {
      foreach ($query_tbl_list as $key => $val) {
        $act_query .= "<br>" . '$query->fields(\'' . $key . '\');';
      }
    }
    if (isset($form_state['cond_fields']) && !empty($form_state['cond_fields'])) {
      $query_cond_fields = $form_state['cond_fields'];
      foreach ($query_cond_fields as $key => $value) {
        if (isset($form_state['values']['placeholder_cond'][$key]) && !empty($form_state['values']['placeholder_cond'][$key])) {
          $cond_field_value = check_plain($form_state['values']['placeholder_cond'][$key]);
          $cond_field_value_operator = $form_state['values']['placeholder_cond'][$key . "_operator"];
          // Get operator.
          $cond_act_oper = querybuilder_actual_operator($cond_field_value_operator);
          // For multi values.
          $chk_multi_val = array();
          $chk_multi_val = explode('|', $cond_field_value);
          $validate_result = FALSE;
          foreach ($chk_multi_val as $val) {
            // Validate user-input for condition fields.
            $validate_result = querybuilder_validate_value($value, $val);
            if (!$validate_result) {
              break;
            }
          }
          if ($validate_result) {
            $cond_map_fields_data[$key] = $cond_field_value;
            $cond_map_fields_operator[$key] = $cond_act_oper;
          }
        }
      }
      if (isset($cond_map_fields_data) && !empty($cond_map_fields_data)) {
        $act_query .= "<br>" . '$query->condition(array(';
        foreach ($cond_map_fields_data as $key => $value) {
          $split_name = explode('.', $key);
          $tbl = $split_name[0];
          $tbl_key = $query_tbl_data[$tbl];
          $fld = $split_name[1];
          if ($cond_map_fields_operator[$key] == "LIKE_E") {
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', \'' . $value . '% \', \'LIKE\',';
          }
          elseif ($cond_map_fields_operator[$key] == "S_LIKE") {
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', \'%' . $value . '\', \'LIKE\',';
          }
          elseif ($cond_map_fields_operator[$key] == "NOT S_LIKE") {
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', \'%' . $value . '\', \'NOT LIKE\',';
          }
          elseif ($cond_map_fields_operator[$key] == "NOT LIKE_E") {
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', \'' . $value . '% \', \'NOT LIKE\',';
          }
          elseif ($cond_map_fields_operator[$key] == "IN") {
            $tem_val = NULL;
            $tem_val = str_replace('|', ',', $value);
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', array(' . $tem_val . '), \'IN\',';
          }
          elseif ($cond_map_fields_operator[$key] == "NOT IN") {
            $tem_val = NULL;
            $tem_val = str_replace('|', ',', $value);
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', array(' . $tem_val . '), \'NOT IN\',';
          }
          else {
            $act_query .= "<br>" . '\'' . $tbl_key . '.' . $fld . '\', \'' . $value . '\', ' . '\'' . $cond_map_fields_operator[$key] . '\',';
          }
        }
        $act_query .= "<br>" . '));';
      }
    }
    $act_query .= "<br>" . '$result = $query->execute();';
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

/**
 * Implements function for table symbol.
 *
 * @param array $query_table_array
 *   Array of tables name.
 *
 * @return array $tbl_list
 *   Character array that represent tables.
 */
function querybuilder_table_key($query_table_array = array()) {
  $tbl_list = array();
  if (!empty($query_table_array)) {
    foreach ($query_table_array as $key => $value) {
      $tbl_key = NULL;
      $tbl_key = substr($key, 0, 1);
      if (!array_key_exists($tbl_key, $tbl_list)) {
        $tbl_list[$tbl_key] = $value;
      }
      else {
        $tbl_key = substr($key, -1);
        if (!array_key_exists($tbl_key, $tbl_list)) {
          $tbl_list[$tbl_key] = $value;
        }
        else {
          $tbl_key = substr($key, 0, 2);
          if (!array_key_exists($tbl_key, $tbl_list)) {
            $tbl_list[$tbl_key] = $value;
          }
          else {
            $tbl_key = substr($key, 0, 3);
            if (!array_key_exists($tbl_key, $tbl_list)) {
              $tbl_list[$tbl_key] = $value;
            }
            else {
              $tbl_list[$key] = $value;
            }
          }
        }
      }
    }
  }
  return $tbl_list;
}
