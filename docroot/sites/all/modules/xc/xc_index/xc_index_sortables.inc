<?php
/**
 * @file
 * Functions to make some admin selected Solr fields sortable.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 *
 * // TODO
 * - create table: xc_index_sortable(name char(255));
 * - xc_index_sortables_form_submit()
 * - xc_index_sortables_list()
 */

/**
 * Get all sortable fields.
 *
 * @return (Array)
 *   A list of sortable fields
 */
function xc_index_sortables_get_all() {
  $fields = array();
  $result = db_query('SELECT field FROM {xc_index_sortable_field}');
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $fields[] = $data->field;
  }
  return $fields;
}

/**
 * Form for selecting the sortable fields
 */
function xc_index_sortables_form($form, &$form_state) {
  $list = xc_solr_fields2index_list('simple');
  $options = array();
  foreach ($list as $item) {
    $options[$item] = $item;
  }
  $defaults = xc_index_sortables_get_all();

  $form['sortables'] = array(
    '#type' => 'select',
    '#title' => t('All Solr fields'),
    '#description' => t('Select the fields, you might want to make use in sorting search result list.'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#size' => 20,
    '#default_value' => $defaults,
    // '#required' => TRUE
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for sortable form
 *
 * Clears out the sortable fields from xc_index_sortable_field table, and populates
 * it with the selected fields.
 *
 * @param $form (Array)
 *   The FAPI form
 * @param $form_state (Array)
 *   The FAPI form state
 */
function xc_index_sortables_form_submit($form, &$form_state) {
  $sortables = $form_state['values']['sortables'];
  db_query('TRUNCATE TABLE {xc_index_sortable_field}');
  foreach ($sortables as $field) {
    $record = (object) array('field' => $field);
    $result = drupal_write_record('xc_index_sortable_field', $record);
    if ($result == SAVED_NEW) {
      drupal_set_message(t('Successfully registered %field field as sortable.', array('%field' => $field)));
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to register %field field as sortable.', array('%field' => $field)));
    }
  }
}

