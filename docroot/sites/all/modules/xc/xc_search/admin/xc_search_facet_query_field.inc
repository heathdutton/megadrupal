<?php
/**
 * @file
 * Facet query field functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_query_field_form($form, &$form_state, $facet) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_facet_field');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('field_id', 'name'),
    'hidden' => array(
      'facet_id' => $facet->facet_id,
      'type' => 'query',
    ),
    'select' => array(
      'attribute_set_id' => array(),
    ),
  );
  $sql = 'SELECT set_id, name, description FROM {xc_search_query_facet_properties}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $options['select']['attribute_set_id'][$data->set_id] = $data->name;
  }
  $form = xc_util_build_autoform($schema, $options);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (count($options['select']['attribute_set_id']) == 0) {
    $form['attribute_set_id']['#description'] = l(
      t('There is no query facet defined. Please create a new query facet first!'),
      'admin/xc/search/facet/queryfacetset/add'
    );
  }

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_query_field_edit_form($form, &$form_state, $field) {
  $facet = xc_index_facet_load($field->facet_id);
  $form = xc_search_facet_query_field_form($form_state, $facet);
  $form['field_id'] = array(
    '#type' => 'hidden',
    '#value' => $field->field_id,
  );
  $form['attibute_set_id']['#default_value'] = $field->attibute_set_id;

  $form['submit']['#value']      = t('Save');
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
function xc_search_facet_query_field_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->type             = $values['type'];
  $record->facet_id         = $values['facet_id'];
  $record->attribute_set_id = $values['attribute_set_id'];
  $query_facet = xc_search_query_facet_properties_load($record->attribute_set_id);
  $record->name             = $query_facet->name;

  $ret_val = drupal_write_record('xc_search_facet_field', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added facet field %name',
                         array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/' . $record->facet_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_query_field_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->field_id         = $values['field_id'];
  $record->type             = $values['type'];
  $record->facet_id         = $values['facet_id'];
  $record->attribute_set_id = $values['attribute_set_id'];

  $query_facet = xc_search_query_facet_properties_load($record->attribute_set_id);
  $record->name             = $query_facet->name;

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_facet_field')
                ->condition('field_id', $record->field_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/' . $record->facet_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count repository.',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_search_facet_field', $record, 'field_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated facet field "%name"',
        array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/' . $record->facet_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to create new facet.'));
    }
  }
}

