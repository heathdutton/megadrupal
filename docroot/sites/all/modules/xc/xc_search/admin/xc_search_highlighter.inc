<?php
/**
 * @file
 * Highlighter functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_title($record) {
  return t('"!name" highlighter', array('!name' => $record->name));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_fieldset() {
  return array(
    'name' => t('name'),
    'description' => t('description'),
    'fieldlist' => t('field list'),
    'require_field_match' => t('require field match'),
    'use_phrase_highlighter' => t('use phrase highlighter'),
    'highlight_multiterm' => t('highlight multiterm'),
    'snippets' => t('snippets'),
    'fragsize' => t('fragment size'),
    'merge_contiguous' => t('merge contigous'),
    'max_analyzed_chars' => t('maximum analyzed characters'),
    'alternate_field' => t('alternative field'),
    'max_alternate_field_length' => t('maximum length of alternative field'),
    'formatter' => t('formatter'),
    'pre' => t('pre'),
    'post' => t('post'),
    'fragmenter' => t('fragmenter'),
    'regex_pattern' => t('regular expression pattern'),
    'regex_slop' => t('regex slop'),
    'increment' => t('increment'),
    'regex_max_analyzed_chars' => t('maximum analyzed characters with regex'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_options() {
  return array(
    'require_field_match' => xc_util_get_global_options('true_false'),
    'use_phrase_highlighter' => xc_util_get_global_options('true_false'),
    'highlight_multiterm' => xc_util_get_global_options('true_false'),
    'merge_contiguous' => xc_util_get_global_options('true_false'),
    'formatter' => array(
      'simple' => t('Simple formatter'),
      'regex' => t('Regular expression based formatter'),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_view($record) {
  drupal_set_title(xc_search_highlighter_title($record));
  return xc_util_view($record,
    xc_search_highlighter_fieldset(),
    xc_search_highlighter_options()
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_list() {
  $headers = array(t('Name'), t('Description'));
  $rows = array();

  $sql = 'SELECT * FROM {xc_search_highlighter}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      l($data->name, 'admin/xc/search/highlighter/' . $data->highlighter_id),
      $data->description,
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_list_values() {
  $sql = 'SELECT highlighter_id, name FROM {xc_search_highlighter}';
  $result = db_query($sql);
  $rows = array();
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[$data->highlighter_id] = $data->name;
  }
  return $rows;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_add() {
  drupal_set_title('Create a new hightlighter');
  return drupal_get_form('xc_search_highlighter_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_highlighter');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('highlighter_id'),
    'hidden' => array(),
    'select' => xc_search_highlighter_options(),
    'label' => xc_search_highlighter_fieldset(),
    'fieldset' => array(
      'regex' => array(
        '#title' => t('fields for regex formatter'),
        '#description' => t('You should fill these fields, if you chosed regular expression based formatter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        'fields' => array('regex_pattern', 'regex_slop', 'increment', 'regex_max_analyzed_chars'),
      ),
    ),
  );

  $form = xc_util_build_autoform($schema, $options);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_edit_form($form, &$form_state, $record) {
  $form = xc_search_highlighter_form($form_state, $record);
  $form['highlighter_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->highlighter_id,
  );

  $regex = array('regex_pattern', 'regex_slop', 'increment', 'regex_max_analyzed_chars');
  foreach (xc_search_highlighter_fieldset() as $name => $label) {
    if (in_array($name, $regex)) {
      $form['regex'][$name]['#default_value'] = $record->$name;
    }
    else {
      $form[$name]['#default_value'] = $record->$name;
    }
  }

  $form['submit']['#value'] = t('Save');
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_highlighter_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_search_highlighter', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added %name highlighter!',
                         array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/highlighter/' . $record->highlighter_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new highlighter.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_highlighter_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->highlighter_id             = $values['highlighter_id'];
  foreach (xc_search_highlighter_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_highlighter')
                ->condition('highlighter_id', $record->field_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/highlighter/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count highlighter.',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_search_highlighter', $record, 'highlighter_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated %name highlighter!',
        array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/highlighter/' .
        $record->highlighter_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify highlighter.'));
    }
  }
}

/**
 * Confirmation form for importing highlighter settings
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_highlighter_import_defaults_form($form, &$form_state) {
  $form['delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do you want to delete existing values?'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
  );
  return confirm_form(
    array($form),
    t('Are you sure, that you would like to import default highlighter settings?'),
    'admin/xc/search/highlighter', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing highlighter settings
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_highlighter_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_highlighter_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/highlighter';
}

/**
 * Importing default highlighter settings
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_highlighter_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default highlighter settings'));

  if ($delete) {
    // delete all records
    $sql = 'DELETE FROM {xc_search_highlighter}';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql) */
    $result = db_delete('xc_search_highlighter')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_highlighter} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_highlighter.csv';
  xc_util_bulk_insert('xc_search_highlighter', xc_util_csv2objects($filename));

  variable_set('xc_search_highlighter_defaults_installed', XC_LOADED);
}
