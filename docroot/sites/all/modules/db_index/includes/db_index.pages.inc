<?php
/**
 * @file
 * Associated page callback and helper functions.
 */

/**
 * Callback function db_index_database_index_form().
 */
function db_index_database_index_form() {
  $form = array();
  $index_schema = db_index_get_schema_in_table_group('full');
  $form['clear_db_index'] = array(
    '#type' => 'submit',
    '#value' => t('Clear DB Index cache'),
    '#submit' => array('db_index_clear_cache'),
  );
  $form['table_index'] = array(
    '#markup' => $index_schema,
  );
  return $form;
}

/**
 * Callback function new_database_index_form().
 */
function db_index_new_database_index_form($form, &$form_state) {
  // drupal_set_title(t("New Database Indexes"));
  $form['index_new_name'] = array(
    '#type' => 'textfield',
    '#title' => t('New Index name'),
    '#required' => TRUE,
  );
  $table_to_index = db_index_get_schema_in_table_group('table');
  $form['select_table_to_index'] = array(
    '#type' => 'select',
    '#title' => t('Select the table'),
    '#options' => $table_to_index['tables'],
    '#ajax' => array(
      'callback' => 'db_index_select_fields_to_index_callback',
      'wrapper' => 'index-fieldset-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['#tree'] = TRUE;
  $form['index_fieldset'] = array(
    '#type' => 'fieldset',
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="index-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  if (empty($form_state['num_index'])) {
    $form_state['num_index'] = 1;
  }
  for ($i = 0; $i < $form_state['num_index']; $i++) {
    // Parent container.
    $form['index_fieldset'][$i] = array(
      '#prefix' => '<div class="two-col">',
      '#suffix' => '</div>',
    );
    if (empty($form_state['values']['select_table_to_index'])) {
      $options = $table_to_index[0];
    }
    else {
      $table_to_index = db_index_get_schema_in_table_group('table');
      $options = $table_to_index[$form_state['values']['select_table_to_index']];
    }
    $form['index_fieldset'][$i]['select_fields_to_index'] = array(
      '#type' => 'select',
      '#title' => t('Select the columns'),
      '#options' => $options,
      '#prefix' => '<div id="index-table-field-select">',
      '#suffix' => '</div>',
    );
  }
  $form['index_fieldset']['index_add_more'] = array(
    '#attributes' => array('class' => array('add-more-index')),
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('db_index_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'db_index_add_more_callback',
      'wrapper' => 'index-fieldset-wrapper',
    ),
  );
  if ($form_state['num_index'] > 1) {
    $form['index_fieldset']['remove_fieldset'] = array(
      '#attributes' => array('class' => array('remove-one-index')),
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('db_index_add_more_remove_one'),
      '#ajax' => array(
        'callback' => 'db_index_add_more_callback',
        'wrapper' => 'index-fieldset-wrapper',
      ),
    );
  }
  $form['add_new_db_index'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#validate'][] = 'db_index_validate_index_name_duplicate';
  return $form;
}

/**
 * Call back function to validates the index name is getting duplicated or not.
 */
function db_index_validate_index_name_duplicate($form, $form_state) {
  $tables = db_index_get_schema_in_table_group('table');
  $selected_table = $form_state['values']['select_table_to_index'];
  $table = $tables['tables'][$selected_table];
  $name = check_plain($form_state['values']['index_new_name']);
  if (db_index_exists($table, $name) == 1) {
    $error_message = t("The given index name is already exists for the table %table. Please give different name", array('%table' => $table));
    form_set_error('index_new_name', $error_message);
  }
  else {
    $columns = db_index_db_get_index($table, 'columns');
    $fields = array();
    foreach ($form_state['values']['index_fieldset'] as $field => $array) {
      if (is_numeric($field)) {
        if (in_array($tables[$selected_table][$array['select_fields_to_index']], $fields) && $form_state['clicked_button']['#value'] != 'Remove one') {
          form_set_error("index_fieldset", t('Column names are duplicted'));
        }
        else {
          $fields[] = $tables[$selected_table][$array['select_fields_to_index']];
        }
      }
    }
    if ($form_state['clicked_button']['#value'] == 'Save') {
      array_unshift($fields, "");
      unset($fields[0]);
      foreach ($columns as $index_name => $field) {
        if ($fields === $field) {
          $error_message = t("This set of columns are already index with the table %table index name as %index_name",
          array("%table" => $table, "%index_name" => $index_name));
          form_set_error("index_fieldset", $error_message);
        }
      }
    }
  }
}

/**
 * Callback for both ajax-enabled buttons.
 */
function db_index_add_more_callback($form, $form_state) {
  return $form['index_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 */
function db_index_add_more_add_one($form, &$form_state) {
  $form_state['num_index']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 */
function db_index_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_index'] > 1) {
    $form_state['num_index']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax Callback for drop down fields associated with the table name selected.
 */
function db_index_select_fields_to_index_callback($form, $form_state) {
  $table_to_index = db_index_get_schema_in_table_group('table');
  $option_select_field = $table_to_index[$form_state['values']['select_table_to_index']];
  $form['index_fieldset'][0]['select_fields_to_index']['#options'] = $option_select_field;
  return $form['index_fieldset'];
}

/**
 * Helper function db_index_get_schema_in_table_group().
 *
 * @param string $list
 *   It will decide what kind of return, 'full' list or only 'table'.
 *
 * @return array|string
 *   Here we have two types of returns, either array of list table fields or
 *   themed table with table name and fields.
 */
function db_index_get_schema_in_table_group($list) {
  if ($cached = cache_get('db_index', 'cache')) {
    $tables = $cached->data;
  }
  else {
    $tables = drupal_get_schema();
    // Setting the data to drupal cache.
    cache_set('db_index', $tables, 'cache', CACHE_TEMPORARY);
  }
  $index_label = "<ul><li>" . t("Index NAME") . "</li></ul>" . "<b>[" . t("Column NAMES are involved the Index NAME") . "]</b>";
  $header = array(t('Table'), $index_label);
  $rows = array();
  $tname = array();
  foreach ($tables as $table_name => $table) {
    // Database table names.
    $tname[] = $table_name;
    foreach ($table as $indexes => $index) {
      if ($indexes == 'indexes') {
        $list_index = '<ul>';
        foreach ($index as $index_name => $value) {
          // Indexes name to show the "db/index" page.
          $list_index .= '<li>' . $index_name . '</li>';
        }
        $list_index .= '</ul>';
      }
      if ($indexes == 'fields') {
        $list_fields_name = array();
        foreach ($index as $fields_name => $value) {
          // Storing the fields or columns name.
          $list_fields_name[] = $fields_name;
        }
        $table_fields[] = $list_fields_name;
      }
    }
    if ($list == 'full') {
      $cid_table = 'db_index' . $table_name;
      // Checking the data is there in the cache.
      if ($list_index_cashed = cache_get($cid_table, 'cache')) {
        $list_index_row = $list_index_cashed->data;
      }
      else {
        $list_index_row = db_index_db_get_index($table_name, 'list_index');
        $cid_table = 'db_index' . $table_name;
        // Setting the data to drupal cache.
        cache_set($cid_table, $list_index_row, 'cache', CACHE_TEMPORARY);
      }
    }
    else {
      $list_index_row = $list_index;
    }
    $rows[] = array($table_name, $list_index_row);
  }
  $table_fields['tables'] = $tname;
  switch ($list) {
    case 'full':
      $return = theme('table', array('header' => $header, 'rows' => $rows));
      break;

    case 'table':
      $return = $table_fields;
      break;
  }
  return $return;
}

/**
 * Callback function helps to show the indexes associated with columns.
 *
 * @param string $table
 *   The name the table.
 * @param string $type
 *   Define what should be the return 'columns' or 'list_index'.
 *
 * @return array|string
 *   $columns returns the array of columns.
 *   $list_index returns the string which have index names associated with its
 *   column names.
 */
function db_index_db_get_index($table = NULL, $type = NULL) {
  $index = array();
  $columns = array();
  if ($table) {
    $index_query = db_query("SHOW INDEX FROM {$table}");
    foreach ($index_query as $key) {
      if (!in_array($key->Key_name, $index)) {
        $index[] = $key->Key_name;
      }
    }
    foreach ($index as $key_name) {
      $index_columns_query = db_query("SHOW INDEX FROM {$table} WHERE Key_name =:key_name", array(':key_name' => $key_name));
      foreach ($index_columns_query as $column_key) {
        $columns[$key_name][$column_key->Seq_in_index] = $column_key->Column_name;
      }
    }
    $list_index = '<ul>';
    foreach ($columns as $index_name => $field) {
      $column_names = implode(",", $field);
      $list_index .= '<li>' . $index_name . '</li> <b>[' . $column_names . ']</b>';
    }
    $list_index .= '</ul>';
    switch ($type) {
      case 'columns':
        return $columns;

      case 'list_index';
        return $list_index;
    }
  }
}

/**
 * Helper function db_index_clear_cache().
 */
function db_index_clear_cache($form, &$form_state) {
  if ($form) {
    // Clearing the cached data with cid db_index_cache.
    cache_clear_all('db_index', 'cache', TRUE);
    drupal_set_message(t("Database Index cache cleared"));
  }
}

/**
 * Call back function that shows the stat of the use of the Indexes.
 */
function db_index_index_usage_statistics() {
  $mysql_version = db_query("SELECT VARIABLE_VALUE
    FROM information_schema.GLOBAL_VARIABLES
    WHERE VARIABLE_NAME = 'INNODB_VERSION'")->fetchCol();
  $form = array();
  if (version_compare($mysql_version[0], '5.6', 'ge')) {
    global $databases;
    $db_name = $databases['default']['default']['database'];
    $index_usage = db_query("select {*} from
    performance_schema.table_io_waits_summary_by_index_usage
    where object_schema = :db_name order by count_fetch desc",
      array(':db_name' => $db_name))->fetchALL();
    $header = array('INDEX_NAME', 'TABLE NAME', 'COUNT_FETCH');
    $rows = array();
    foreach ($index_usage as $usage) {
      if (!empty($usage->INDEX_NAME)) {
        $rows[] = array(
          $usage->INDEX_NAME,
          $usage->OBJECT_NAME,
          $usage->COUNT_FETCH,
        );
      }
    }
    $index_usage_stat = theme('table', array('header' => $header, 'rows' => $rows));
    $form['table_index'] = array(
      '#markup' => $index_usage_stat,
    );
  }
  else {
    $form['table_index'] = array(
      '#markup' => '<h6>Needs MySQL version of 5.6 or above to get Index Usages Statistics.</h6>',
    );
  }
  return $form;
}

/**
 * Callback function helps to drop the unused indexes.
 */
function db_index_drop_unused_index() {
  $form = array();
  $warning_message = '<div class="messages warning"git>
<h2 class="element-invisible">Warning message</h2>
Before deleting any indexes, have a backup of Database.</div>';
  $warning_message .= '<div class="messages warning">
<h2 class="element-invisible">Warning message</h2>
Deleting default indexes may cause problems.</div>';
  $table_to_index = db_index_get_schema_in_table_group('table');
  $form['warning_message'] = array(
    '#markup' => $warning_message,
  );
  $form['select_table_to_index'] = array(
    '#type' => 'select',
    '#title' => t('Select the table'),
    '#options' => $table_to_index['tables'],
    '#ajax' => array(
      // Ajax callback back for selecting the columns associated.
      'callback' => 'db_index_select_index_names_callback',
      'wrapper' => 'index-names-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#required' => TRUE,
  );
  $indexes = db_index_get_index_from_table($table_to_index['tables'][0]);
  $form['index_to_drop'] = array(
    '#type' => 'select',
    '#title' => t('Index'),
    '#options' => $indexes,
    '#prefix' => '<div id = "index-names-wrapper">',
    '#suffix' => '</div>',
  );
  $form['drop_index'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  return $form;
}

/**
 * Callback function that return $form['index_to_drop'] with options of indexes.
 */
function db_index_select_index_names_callback($form, $form_state) {
  $table_to_index = db_index_get_schema_in_table_group('table');
  $table = $table_to_index['tables'][$form_state['values']['select_table_to_index']];
  $index = db_index_get_index_from_table($table);
  $form['index_to_drop']['#options'] = $index;
  return $form['index_to_drop'];
}

/**
 * Helper function to get the index names with the associated table.
 *
 * @param string $table
 *   It will pass the name of the table.
 *
 * @return array
 *   $index returns the array index names.
 */
function db_index_get_index_from_table($table) {
  $index_query = db_query("SHOW INDEX FROM {$table}");
  $index = array();
  foreach ($index_query as $key) {
    if (!in_array($key->Key_name, $index)) {
      $index[] = $key->Key_name;
    }
  }
  return $index;
}
