<?php

/**
 * @file
 * Schema Kill administrative page callbacks.
 */

/**
 * Administrative form for removing extra tables.
 */
function schema_kill_kill_form() {

  $form['tables'] = array(
    '#type' => 'tableselect',
    '#header' => array(t('Table')),
    '#empty' => t('No extra tables in the schema.'),
  );

  $schema = drupal_get_schema(NULL, TRUE);
  $info = schema_compare_schemas($schema);

  // Ensure the data exists.
  $extra = isset($info['extra']) ? $info['extra'] : array();

  foreach ($extra as $table) {
    $form['tables']['#options'][$table] = array($table);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Do sum killin\'')
  );

  return $form;
}

/**
 * Submit handler for schema_kill_kill_form.
 */
function schema_kill_kill_form_submit(&$form, &$form_state) {
  $tables = array_filter($form_state['values']['tables']);
  foreach ($tables as $table) {
    db_drop_table($table);
    drupal_set_message(t('Dropped :table table.', array(':table' => $table)));
  }
}
