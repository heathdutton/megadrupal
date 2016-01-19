<?php
/**
 * @file
 * Field facet properties functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * TODO: handling facet.enum.cache.minDF
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_title($record) {
  return $record->name;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_fieldset() {
  return array(
    'name' => t('name'),
    'description' => t('description'),
    'prefix' => t('prefix'),
    'sort' => t('Sort'),
    'limitation' => t('Limitation'),
    'offset' => t('Offset'),
    'mincount' => t('Mincount'),
    'missing' => t('Missing'),
    'method' => t('Method'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_options() {
  return array(
    'sort' => array(
      'count' => t('sort the constraints by count'),
      'index' => t('sorted in their index order (lexicographic by indexed term)'),
    ),
    'missing' => xc_util_get_global_options('true_false'),
    'method' => array(
      'enum' => t('count on all documents'),
      'fc' => t('count on documents in cache'),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_view($record) {
  drupal_set_title(filter_xss(xc_search_field_facet_properties_title($record)));
  return xc_util_view($record,
    xc_search_field_facet_properties_fieldset(),
    xc_search_field_facet_properties_options()
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_list() {

  $headers = array(t('Name'), t('Description'));
  $rows = array();

  $sql = 'SELECT set_id, name, description FROM {xc_search_field_facet_properties}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(l($data->name, 'admin/xc/search/facet/fieldfacetset/' .
      $data->set_id), $data->description);
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new server
 * @return string The add server page with a form
 */
function xc_search_field_facet_properties_add() {
  drupal_set_title('Facet attribute set');
  return drupal_get_form('xc_search_field_facet_properties_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search',
    'xc_search_field_facet_properties');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('set_id'),
    'select' => xc_search_field_facet_properties_options(),
    'label' => xc_search_field_facet_properties_fieldset(),
  );
  $form = xc_util_build_autoform($schema, $options);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_edit_form($form, &$form_state, $set) {
  $form = xc_search_field_facet_properties_form();
  $form['set_id'] = array(
    '#type' => 'hidden',
    '#value' => $set->set_id,
  );
  foreach (xc_search_field_facet_properties_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $set->$name;
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
function xc_search_field_facet_properties_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_field_facet_properties_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  $ret_val = drupal_write_record('xc_search_field_facet_properties', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added facet settings "%name"',
      array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/fieldfacetset/'
      . $record->set_id;
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet settings.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_field_facet_properties_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->set_id      = $values['set_id'];
  $record->name        = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_field_facet_properties')
                ->condition('set_id', $record->set_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/fieldfacetset/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count properties.',
        array('%count' => $result)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_field_facet_properties_fieldset() as $name => $label) {
      $record->$name = $values[$name];
    }

    $result = drupal_write_record('xc_search_field_facet_properties', $record,
      'set_id');
    if ($result == SAVED_UPDATED) { // repository is updated
      drupal_set_message(t('%name modified', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/fieldfacetset/' . $record->set_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify facet attribute set.'));
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
}

/**
 * Confirmation form for importing field facet properties
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_field_facet_properties_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default field facet properties?'),
    'admin/xc/search/facet/fieldfacetset', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing field facet properties
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_field_facet_properties_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_field_facet_properties_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/facet/fieldfacetset';
}

/**
 * Importing default field facet properties
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_field_facet_properties_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default field facet properties'));

  if ($delete) {
    // delete all records 
    $result = db_delete('xc_search_field_facet_properties')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_field_facet_properties} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_field_facet_properties.csv';
  xc_util_bulk_insert('xc_search_field_facet_properties', xc_util_csv2objects($filename));

  variable_set('xc_search_field_facet_properties_defaults_installed', XC_LOADED);
}
