<?php
/**
 * @file
 * Sorting option functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 *
 * xc_search_sortoption is a table contain definitions of items in the sorting options
 * dropdown list on the user interface. The user can choose how to sort the search
 * result list.
 *
 * Field list: id, primary_field, primary_direction, secondary_field, secondary_direction, label,
 * weight
 */

/**
 * Return the field name -> human readable label pairs of the fields
 * @return (Array)
 */
function xc_search_sortoption_fieldset() {
  return array(
    'primary_field' => t('The primary sorting field'),
    'primary_direction' => t('The direction: descending or ascending'),
    'secondary_field' => t('The secondary sorting field'),
    'secondary_direction' => t('The direction: descending or ascending'),
    'label' => t('Label which the end user will see in the selection list'),
    'is_enabled' => t('Is this this sort option enabled or disabled'),
  );
}

/**
 * Get the enumerated field values
 *
 * @return (Array)
 */
function xc_search_sortoption_options() {

  $directions = array(
    0 => t('Descending'),
    1 => t('Ascending'),
  );

  $is_enabled = array(
    0 => t('Disabled'),
    1 => t('Enabled'),
  );

  return array(
    'primary_field' => xc_search_sortoption_fieldlist(),
    'secondary_field' => xc_search_sortoption_fieldlist(),
    'primary_direction' => $directions,
    'secondary_direction' => $directions,
    'is_enabled' => $is_enabled,
  );
}

/**
 * Get the list of fields available in Solr index
 */
function xc_search_sortoption_fieldlist() {
  static $fields;

  if (!isset($fields)) {
    $fields = xc_solr_list_fields();
    sort($fields);
    array_unshift($fields, t('[relevancy]'));
    array_unshift($fields, t('[none]'));
    $fields = array_combine($fields, $fields);
  }

  return $fields;
}

/**
 * Show the full display of the record
 *
 * @param $record (Object)
 *   The sorting option object
 *
 * @return (String)
 *   The rendered view of sorting option record
 */
function xc_search_sortoption_view($record) {
  drupal_set_title(xc_search_sortoption_title($record));

  return xc_util_view(
    $record,
    xc_search_sortoption_fieldset(),
    xc_search_sortoption_options()
  );
}

/**
 * List all sorting options
 *
 * @return (String)
 *   The rendered list of sorting options
 */
function xc_search_sortoption_list() {
  $headers = array(t('Label'), t('Primary sort'), t('Secondary sort'), t('Status'), t('trigger'), t('edit'), t('delete'));

  $rows = array();
  $sql = 'SELECT * FROM {xc_search_sortoption} ORDER BY weight';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      l($data->label, 'admin/xc/search/sortoption/' . $data->id),
      $data->primary_field . ', ' . ($data->primary_direction == 0 ? 'desc' : 'asc'),
      ($data->secondary_field == t('[none]')
        ? ''
        : $data->secondary_field . ', ' . ($data->secondary_direction == 0 ? 'desc' : 'asc')
      ),
      ($data->is_enabled ? t('Enabled') : t('Disabled')),
      ($data->is_enabled
        ? l(t('disable'), 'admin/xc/search/sortoption/' . $data->id . '/disable')
        : l(t('enable'), 'admin/xc/search/sortoption/' . $data->id . '/enable')
      ),
      l(t('edit'), 'admin/xc/search/sortoption/' . $data->id . '/edit'),
      l(t('delete'), 'admin/xc/search/sortoption/' . $data->id . '/delete'),
    );
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new sorting option
 *
 * @return (string)
 *   The add sorting option form
 */
function xc_search_sortoption_add() {
  return drupal_get_form('xc_search_sortoption_form');
}

/**
 * The form to edit sorting option record
 *
 * @return (array)
 *   The FAPI array
 */
function xc_search_sortoption_form($form, $set_title = TRUE) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_sortoption');

  if ($set_title) {
    drupal_set_title($schema['description']);
  }

  $options = array(
    'omit' => array('id'),
    'select' => xc_search_sortoption_options(),
    'label' => xc_search_sortoption_fieldset(),
  );

  $form = xc_util_build_autoform($schema, $options);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * The form to modify the sorting option record
 *
 * @param $form_state (Array)
 *   The FAPI form_state
 * @param $record (Object)
 *   The sorting option record
 *
 * @return (array)
 *   The FAPI form
 */
function xc_search_sortoption_edit_form($form, &$form_state, $record) {
  $form = xc_search_sortoption_form(FALSE);
  drupal_set_title(t('Editing'));

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $record->id,
  );
  foreach (xc_search_sortoption_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $record->$name;
  }

  $form['submit']['#value'] = t('Save');
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * The handling of form submission
 *
 * @param $form (Array)
 *   The FAPI form definition
 * @param $form_state (Array)
 *   The FAPI form_state
 */
function xc_search_sortoption_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $record = new stdClass();
  foreach (xc_search_sortoption_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_search_sortoption', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added sorting option "%label"',
      array('%label' => $record->label)));
    $form_state['redirect'] = 'admin/xc/search/sortoption/' . $record->id;
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new sorting option.'));
  }

  variable_del('xc_search_sortoption');
}

/**
 * Handling of modification/deletion
 *
 * @param $form (Array)
 *   The FAPI form definition
 * @param $form_state (Array)
 *   The FAPI form_state
 */
function xc_search_sortoption_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->id      = $values['id'];
  $record->label   = $values['label'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_sortoption')
                ->condition('id', $record->id)
                ->execute();
    if ($result == 1) { // sorting option is deleted
      drupal_set_message(t('%label removed', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/search/sortoption/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to remove %name.',
        array('%name' => $record->label)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_sortoption_fieldset() as $name => $label) {
      $record->$name = $values[$name];
    }

    $result = drupal_write_record('xc_search_sortoption', $record, 'id');
    if ($result == SAVED_UPDATED) {
      drupal_set_message(t('%label modified', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/search/sortoption/' . $record->id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify %label.',
        array('%label' => $record->label)));
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
  variable_del('xc_search_sortoption');
}

/**
 * Gets the confirmation form for enabling/disabling a sort option
 *
 * @param $form_state (Array)
 *   The FAPI form state array
 * @param $record (Object)
 *   An xc_search_sortoption record
 * @param $do_enable (boolean)
 *   Flag for enable or disable the sort option
 *
 * @return (Array)
 *   The FAPI Form array
 */
function xc_search_sortoption_trigger_form($form, &$form_state, $record, $do_enable = TRUE) {

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $record->id,
  );
  $form['do_enable'] = array(
    '#type' => 'hidden',
    '#value' => $do_enable,
  );

  $question = ($do_enable)
    ? t('Are you sure, that you would like to enable !label option?', array('!label' => $record->label))
    : t('Are you sure, that you would like to disable !label option?', array('!label' => $record->label));

  $button_text = ($do_enable) ? t('Enable') : t('Disable');

  return confirm_form(
    $form,
    $question,
    'admin/xc/search/sortoption', // path to go if user click on 'cancel'
    '',
    $button_text,
    t('Cancel')
  );
}

/**
 * Processes the enable/disable form.
 *
 * Enables or disables a sort option.
 *
 * @param $form (Array)
 *   The FAPI form definition
 * @param $form_state (Array)
 *   The FAPI form state array
 */
function xc_search_sortoption_trigger_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $do_enable = (int) (boolean) $form_state['values']['do_enable'];

  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query('UPDATE {xc_search_sortoption} SET is_enabled = %d WHERE id = %d', $do_enable, $id) */
  db_update('xc_search_sortoption')
    ->fields(array(
      'is_enabled' => $do_enable,
    ))
    ->condition('id', $id)
    ->execute();

  // remove cached sortoptions
  variable_del('xc_search_sortoption');
  $form_state['redirect'] = 'admin/xc/search/sortoption';
}

/**
 * Confirmation form for importing default sorting options
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_sortoption_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default sorting options?'),
    'admin/xc/search/sortoption', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default sorting options
 *
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_sortoption_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_sortoption_restore_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/sortoption';
}

/**
 * Deletes all sorting option definitions.
 */
function xc_search_sortoption_delete_all() {
  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }
  $result = db_delete('xc_search_sortoption')->execute();
  if ($result > 0) { // sorting options are deleted
    drupal_set_message(t('All sorting options were removed'));
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count sorting options.',
      array('%count' => $result)));
  }
  variable_del('xc_search_sortoption');
}

/**
 * Imports default sort options.
 */
function xc_search_sortoption_import_defaults() {
  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }
  xc_search_sortoption_restore_defaults(FALSE);
}

/**
 * Restores sorting options default values.
 *
 * @param $delete_all (Boolean)
 *   If TRUE (default value) it first deletes all records. Otherwise it skips
 *   deletions.
 */
function xc_search_sortoption_restore_defaults($delete_all = TRUE) {
  drupal_set_message(t('Importing search user interface default values.'));

  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_sortoption.csv';
  if (!file_exists($filename)) {
    drupal_set_message(t('The CSV file %filename is inexistent', array('%filename' => $filename)));
  }
  else {
    if ($delete_all) {
      // delete all records
      xc_search_sortoption_delete_all();

      // set autoincrement to 1
      $sql = 'ALTER TABLE {xc_search_sortoption} AUTO_INCREMENT = 1';
      $result = db_query($sql);
    }
    xc_util_bulk_insert('xc_search_sortoption', xc_util_csv2objects($filename));
    variable_set('xc_search_sortoption_defaults_installed', XC_LOADED);
    drupal_set_message(t('The default sorting options were successfully imported.'));
  }
  variable_del('xc_search_sortoption');
}

/**
 * Return a list of options used in search result set ordering selection dropdown.
 *
 * @return (Array)
 *   An associative array of sort options, where key is the Solr search component, and
 *   value is the displayable title.
 */
function xc_search_sortoption_get_options($only_enabled = FALSE) {
  if ($only_enabled) {
    $sql = 'SELECT * FROM {xc_search_sortoption} WHERE is_enabled = 1 ORDER BY weight';
  }
  else {
    $sql = 'SELECT * FROM {xc_search_sortoption} ORDER BY weight';
  }
  $result = db_query($sql);
  $options = array();

  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $key = sprintf(
      '%s %s',
      ($data->primary_field == t('[relevancy]') ? 'score' : $data->primary_field),
      ($data->primary_direction == 0 ? 'desc' : 'asc')
    );
    if ($data->secondary_field != t('[none]')) {
      $key .= sprintf(
        ',%s %s',
        ($data->secondary_field == t('[relevancy]') ? 'score' : $data->secondary_field),
        ($data->secondary_direction == 0 ? 'desc' : 'asc')
      );
    }
    $options[$key] = t($data->label);
  }

  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_sortoption_reorder_form($form, &$form_state) {
  $weight_delta = db_query('SELECT count(*) FROM {xc_search_sortoption}')->fetchField();
  $result = db_query('SELECT * FROM {xc_search_sortoption} ORDER BY weight');

  $rows = array();
  $form = array(
    '#tree' => TRUE,
  );

  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $key = $data->id;
    $form['options'][$key] = array();
    $form['options'][$key]['label'] = array('#value' => check_plain($data->label));
    $form['options'][$key]['primary'] = array(
      '#value' => check_plain($data->primary_field . ', ' . ($data->primary_direction == 0 ? 'desc' : 'asc')),
    );
    $form['options'][$key]['secondary'] = array(
      '#value' => check_plain(($data->secondary_field == t('[none]') ? ''
        : $data->secondary_field . ', ' . ($data->secondary_direction == 0 ? 'desc' : 'asc')
      )),
    );
    $form['options'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $data->weight,
      '#delta' => $weight_delta,
    );
    $form['options'][$key]['actions'] = array(
      '#value' => l(
        t('view'),
        'admin/xc/search/sortorder/' . $field->id . '/view'
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
  );

  return $form;
}

/**
 * Themeing reordering sorting options form.
 *
 * @param $form (Array)
 *   The FAPI form array
 *
 * @return (String)
 *   The themed form
 */
function theme_xc_search_sortoption_reorder_form($variables) {
  $form = $variables['form'];
  $rows = array();
  drupal_add_tabledrag('facet-sort', 'order', 'sibling', 'sort');

  foreach (element_children($form['options']) as $key) {
    $facet = &$form['options'][$key];
    $facet['weight']['#attributes']['class'] = 'sort';

    $cells = array(
      drupal_render($facet['label']),
      drupal_render($facet['primary']),
      drupal_render($facet['secondary']),
      drupal_render($facet['actions']),
      drupal_render($facet['weight']),
    );

    $rows[] = array(
      'data' => $cells,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => array(
      t('Label'),
      t('Primary sort'),
      t('Secondary sort'),
      t('View'),
      ''
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'facet-sort')
  ));

  // add the rest of the form
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Save the order of facets
 *
 * @param $form
 * @param $form_state
 */
function xc_search_sortoption_reorder_form_submit($form, &$form_state) {
  $options = $form_state['values']['options'];
  foreach ($options as $id => $option) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query('UPDATE {xc_search_sortoption} SET weight = %d WHERE id = %d', $option['weight'], $id) */
    db_update('xc_search_sortoption')
      ->fields(array(
          'weight' => $option['weight'],
        ))
      ->condition('id', $id)
      ->execute();
  }
  drupal_set_message(t('Order of sorting options has been successfully updated.'));
  $form_state['redirect'] = 'admin/xc/search/sortoption';
}

