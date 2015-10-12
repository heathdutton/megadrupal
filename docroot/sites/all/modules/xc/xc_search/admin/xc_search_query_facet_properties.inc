<?php
/**
 * @file
 * Handling functions for query type facet properties.
 *
 * Fields in xc_search_query_facet_properties table:
 * set_id (the identifier), name, description, query
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_query_facet_properties_fieldset() {
  return array(
    'name' => t('Name'),
    'description' => t('Description'),
    'query' => t('Query'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_query_facet_properties_view($record) {
  drupal_set_title($record->name);
  return xc_util_view(
    $record,
    xc_search_query_facet_properties_fieldset()
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_query_facet_properties_list() {

  $headers = array(t('Name'), t('Description'));
  $rows = array();

  $sql = 'SELECT set_id, name, description FROM {xc_search_query_facet_properties}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(l($data->name,
      'admin/xc/search/facet/queryfacetset/' . $data->set_id), $data->description);
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new server
 * @return string The add server page with a form
 */
function xc_search_query_facet_properties_add() {
  drupal_set_title('Facet attribute set');
  return drupal_get_form('xc_search_query_facet_properties_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_query_facet_properties_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search',
    'xc_search_query_facet_properties');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('set_id'),
    'label' => xc_search_query_facet_properties_fieldset(),
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
function xc_search_query_facet_properties_edit_form($form, &$form_state, $set) {
  $form = xc_search_query_facet_properties_form();
  $form['set_id'] = array(
    '#type' => 'hidden',
    '#value' => $set->set_id,
  );
  foreach (xc_search_query_facet_properties_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $set->name;
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
function xc_search_query_facet_properties_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_query_facet_properties_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  $ret_val = drupal_write_record('xc_search_query_facet_properties', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added facet settings %name',
                         array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/queryfacetset/' . $record->set_id;
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet settings.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_query_facet_properties_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->set_id      = $values['set_id'];
  $record->name        = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_query_facet_properties')
                ->condition('set_id', $record->set_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/queryfacetset/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count facet attribute set.',
        array('%count' => $result)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_query_facet_properties_fieldset() as $name => $label) {
      $record->$name = $values[$name];
    }
    $result = drupal_write_record('xc_search_query_facet_properties', $record, 'set_id');

    if ($result == SAVED_UPDATED) { // repository is updated
      drupal_set_message(t('%name modified', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/queryfacetset/' . $record->set_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify facet attribute set.'));
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
}
