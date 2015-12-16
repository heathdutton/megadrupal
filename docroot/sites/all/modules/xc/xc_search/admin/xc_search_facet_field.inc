<?php
/**
 * @file
 * Facet field funtions.
 *
 * If the attribute_set_id is 0, it means, that it should use the group's
 * default attribute
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_field_edit_load($field_id) {
  return xc_search_facet_field_load($field_id);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_field_fieldset() {
  return array(
    'name' => t('name'),
    'type' => t('facet type'),
    'facet_id' => t('facet'),
    'attribute_set_id' => t('attribute set'),
    'is_collapsed' => t('Default Facet State'),
    'weight' => t('weight'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_field_form($form, &$form_state, $facet) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_facet_field');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('field_id'),
    'label' => xc_search_facet_field_fieldset(),
    'hidden' => array(
      'facet_id' => $facet->facet_id,
      'type' => 'field',
    ),
    'select' => array(
      'attribute_set_id' => array('0' => t('Use the group\'s default')),
      'name' => array(),
      'is_collapsed' => array(
        1 => t('Collapsed'),
        0 => t('Expanded'),
      ),
    ),
  );
  $sql = 'SELECT set_id, name, description FROM {xc_search_field_facet_properties}';
  $result = db_query($sql);
  while ($data = db_fetch_object($result)) {
    $options['select']['attribute_set_id'][$data->set_id] = $data->name;
  }

  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';
  $options['select']['name'] = array_merge(
    xc_index_facet_mapping_get_facets($facet->metadata_schema, TRUE),
    xc_format_get_fields($facet->metadata_schema)
  );

  $form = xc_util_build_autoform($schema, $options);
  $form['is_collapsed']['#type'] = 'radios';
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
function xc_search_facet_field_edit_form($form, &$form_state, $record) {
  $facet = xc_index_facet_load($record->facet_id);
  $form = xc_search_facet_field_form($form_state, $facet);
  $form['field_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->field_id,
  );
  foreach (xc_search_facet_field_fieldset() as $name => $label) {
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
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_field_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_facet_field_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  $ret_val = drupal_write_record('xc_search_facet_field', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added facet field "%name"',
      array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/group/'
      . $record->facet_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet field.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_field_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->field_id         = $values['field_id'];
  foreach (xc_search_facet_field_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_facet_field')
                ->condition('field_id', $record->field_id)
                ->execute();
    if ($result == 1) { // facet field is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/group/'
        . $record->facet_id . '/view';
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
      $form_state['redirect'] = 'admin/xc/search/facet/group/'
        . $record->facet_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to create new facet.'));
    }
  }
}

/**
 * Remove a facet field from the facet group
 * @param $field (Object)
 *   A record from xc_search_facet_field table
 */
function xc_search_facet_field_remove($field) {
  $result = db_delete('xc_search_facet_field')
            ->condition('field_id', $field->field_id)
            ->execute();
  if ($result == 1) { // repository is deleted
    drupal_set_message(t('%name removed', array('%name' => $field->name)));
    drupal_goto('admin/xc/search/facet/group/' . $field->facet_id . '/view');
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count facet field(s).',
      array('%count' => $result)));
  }
}

/**
 * Confirmation form for importing default facet fields
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_facet_field_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default facet fields?'),
    'admin/xc/search/facet/field', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default facet fields
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_facet_field_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_facet_field_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/facet/field';
}

/**
 * Importing default facet fields
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_facet_field_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default facet fields'));

  if ($delete) {
    // delete all records 
    $result = db_delete('xc_search_facet_field')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_facet_field} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_facet_field.csv';
  xc_util_bulk_insert('xc_search_facet_field', xc_util_csv2objects($filename));

  variable_set('xc_search_facet_field_defaults_installed', XC_LOADED);
}
