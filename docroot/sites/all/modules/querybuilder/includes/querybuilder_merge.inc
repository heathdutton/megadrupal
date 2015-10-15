<?php
/**
 * @file
 * Demonstrates for dynamic merge query.
 */

// Load querybuilder_common file.
module_load_include('inc', 'querybuilder', 'includes/querybuilder_common');

/**
 * Form builder; Return form for merge.
 */
function querybuilder_merge($form, &$form_state) {
  // Get list of tables name from database.
  $tbl_list = querybuilder_get_tables();
  // Get value from $form_state['values']
  $sel_qry_table = isset($form_state['values']['query_table']) ? $form_state['values']['query_table'] : '_none';
  $sel_qry_fields = isset($form_state['values']['query_fields']) ? $form_state['values']['query_fields'] : '_none';
  $sel_upd_fields = isset($form_state['values']['query_fields_update']) ? $form_state['values']['query_fields_update'] : '_none';
  $sel_key_fields = isset($form_state['values']['query_fields_key']) ? $form_state['values']['query_fields_key'] : '_all';

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
      'wrapper' => array(
        'query-table-fields',
        'query-table-fields-update',
        'query-table-fields-key',
      ),
    ),
  );
  $form['query_fields'] = array(
    '#type' => 'select',
    '#title' => 'Insert Fields',
    '#multiple' => TRUE,
    '#options' => $query_fields_list,
    '#default_value' => '_none',
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
            $map_fields[$value] = $fetch_key[1];
            $form['placeholder'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
        }
      }
      $form_state['fields'] = $map_fields;
    }
  }

  $form['query_fields_update'] = array(
    '#type' => 'select',
    '#title' => 'Update Fields',
    '#multiple' => TRUE,
    '#options' => $query_fields_list,
    '#default_value' => '_none',
    '#prefix' => '<div id="query-table-fields-update">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'querybuilder_queryfield_update_callback',
      'wrapper' => 'placeholder-update-fields',
    ),
  );
  $form['placeholder_update'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="placeholder-update-fields">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  $map_update_fields = array();
  if (!empty($sel_upd_fields) && isset($form_state['values']['query_fields_update'])) {
    if (!array_key_exists('_none', $sel_upd_fields)) {
      $form['placeholder_update']['#type'] = 'fieldset';
      foreach ($query_fields as $key => $value) {
        $fetch_key = array();
        $fetch_key = explode('%', $key);
        if (in_array($value, $sel_upd_fields)) {
          if ($fetch_key[1] == "blob" || $fetch_key[1] == "longblob" || $fetch_key[1] == "mediumblob") {
            $form['placeholder_update'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#attributes' => array('disabled' => TRUE),
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
          else {
            $map_update_fields[$value] = $fetch_key[1];
            $form['placeholder_update'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
        }
      }
      $form_state['update_fields'] = $map_update_fields;
    }
  }
  $form['query_fields_key'] = array(
    '#type' => 'select',
    '#title' => 'Key Fields',
    '#multiple' => TRUE,
    '#default_value' => '_none',
    '#options' => $query_fields_list,
    '#prefix' => '<div id="query-table-fields-key">',
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
  if (!empty($sel_key_fields) && isset($form_state['values']['query_fields_key'])) {
    if (!array_key_exists('_none', $sel_key_fields)) {
      $form['placeholder_cond']['#type'] = 'fieldset';
      foreach ($query_fields as $key => $value) {
        $fetch_key = array();
        $fetch_key = explode('%', $key);
        if (in_array($value, $sel_key_fields)) {
          if ($fetch_key[1] == "blob" || $fetch_key[1] == "longblob" || $fetch_key[1] == "mediumblob") {
            $form['placeholder_cond'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#attributes' => array('disabled' => TRUE),
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
          else {
            $map_cond_fields[$value] = $fetch_key[1];
            $form['placeholder_cond'][$value] = array(
              '#type' => 'textfield',
              '#title' => 'Value for ' . $value,
              '#description' => t('Field Type-') . $fetch_key[1],
            );
          }
        }
      }
      $form_state['key_fields'] = $map_cond_fields;
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
      '#markup' => querybuilder_merge_result($form, $form_state),
      '#prefix' => '<div id="query-output">',
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Process result from query builder merge form.
 */
function querybuilder_merge_result($form, $form_state) {
  $query_table = $form_state['values']['query_table'];
  $act_query = NULL;
  $map_insert_fields = $map_update_fields = $map_key_fields = NULL;
  if (isset($form_state['fields']) && !empty($form_state['fields'])) {
    $query_insert_fields = $form_state['fields'];
    foreach ($query_insert_fields as $key => $value) {
      if (isset($form_state['values']['placeholder'][$key]) && !empty($form_state['values']['placeholder'][$key])) {
        $field_value = check_plain($form_state['values']['placeholder'][$key]);
        $validate_result = FALSE;
        // Validate user-input.
        $validate_result = querybuilder_validate_value($value, $field_value);
        if ($validate_result) {
          $map_insert_fields[$key] = $field_value;
        }
      }
    }
  }
  if (isset($form_state['update_fields']) && !empty($form_state['update_fields'])) {
    $query_update_fields = $form_state['update_fields'];
    foreach ($query_update_fields as $key => $value) {
      if (isset($form_state['values']['placeholder_update'][$key]) && !empty($form_state['values']['placeholder_update'][$key])) {
        $field_up_value = check_plain($form_state['values']['placeholder_update'][$key]);
        $validate_result = FALSE;
        // Validate user-input for condition fields.
        $validate_result = querybuilder_validate_value($value, $field_up_value);
        if ($validate_result) {
          $map_update_fields[$key] = $field_up_value;
        }
      }
    }
  }
  if (isset($form_state['key_fields']) && !empty($form_state['key_fields'])) {
    $query_key_fields = $form_state['key_fields'];
    foreach ($query_key_fields as $key => $value) {
      if (isset($form_state['values']['placeholder_cond'][$key]) && !empty($form_state['values']['placeholder_cond'][$key])) {
        $field_key_value = check_plain($form_state['values']['placeholder_cond'][$key]);
        $validate_result = FALSE;
        // Validate user-input for key fields.
        $validate_result = querybuilder_validate_value($value, $field_key_value);
        if ($validate_result) {
          $map_key_fields[$key] = $field_key_value;
        }
      }
    }
  }
  if (!empty($map_insert_fields) && !empty($map_update_fields) && !empty($map_key_fields)) {
    $act_query = '$transaction = db_transaction();';
    $act_query .= "<br>" . 'try {';
    $act_query .= "<br>" . '$query = db_merge(\'' . $query_table . '\')' . "<br>";
    $act_query .= '->insertFields(array(';
    foreach ($map_insert_fields as $key => $value) {
      $act_query .= "<br>" . '\'' . $key . '\' => \'' . $value . '\',';
    }
    $act_query .= "<br>" . '))' . "<br>";
    $act_query .= '->updateFields(array(';
    foreach ($map_update_fields as $key => $value) {
      $act_query .= "<br>" . '\'' . $key . '\' => \'' . $value . '\',';
    }
    $act_query .= "<br>" . '))' . "<br>";
    $act_query .= '->key(array(';
    foreach ($map_key_fields as $key => $value) {
      $act_query .= "<br>" . '\'' . $key . '\' => \'' . $value . '\',';
    }
    $act_query .= "<br>" . '))' . "<br>";
    $act_query .= '->execute();';
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
