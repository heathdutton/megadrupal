<?php
/**
 * @file
 * Functions for manipulating xc_solr_field_type table.
 *
 * Field in the table: type_id, label, type, suffix
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

function xc_solr_field_type_fieldset() {
  return array(
    'label' => t('label'),
    'type'  => t('type'),
    'suffix' => t('suffix'),
  );
}

function xc_solr_field_type_options() {
  return array();
}

function xc_solr_field_type_view($record) {
  return xc_util_view($record,
    xc_solr_field_type_fieldset()
  );
}

function xc_solr_field_type_list() {
  $output = t('List the dynamic types in Solr. The Drupal Toolkit will index fields accoring to field types, so in Solr an original field name will be stored as [fieldname]_suffix, like author_s. An example from a Solr\'s schema.xml: %example. Here the *_i means, that every field name, suffixed by "_i" will be handled as sortable integer. Consult the schema.xml file\'s comments for more details. In another page you should map the metadata schema\'s fields to one or more type, and the field will be indexed into those Solr field(s). If you modify the schema.xml, you should keep this list syncronized, and remap the field names into types, otherwise the Drupal Toolkit will not know about your changes. After changes it is advisable reindex the whole content.',
    array('%example' => '<dynamicField name="*_i" type="sint" indexed="true" stored="true" />'));
  $headers = array(t('Label'), t('Solr base type'), t('Suffix'));
  $rows = array();

  $sql = 'SELECT * FROM {xc_solr_field_type} ORDER BY weight';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      $data->label,
      $data->type,
      l($data->suffix, 'admin/xc/solr/field_type/' . $data->type_id)
    );
  }
  //TODO from Tom about the theme output.
  //$output .= theme('table', $headers, $rows);
  $output .=  theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= '<p>' . l(t('Reorder this list'), 'admin/xc/solr/field_type/reorder') . '</p>';
  return $output;
}

/**
 * Returns a form for reordering dynamic Solr field types
 *
 * @see theme_xc_solr_field_type_reorder_form()
 *
 * @param $form_state (array)
 *   The FAPI form state
 *
 * @return (array)
 *   The FAPI form definition
 */
function xc_solr_field_type_reorder_form($form,&$form_state) {
  print('yesss');
  xc_log_info('tom13', '_metadata_type_to_node_type, pass in type: ');
  $weight_delta = db_query('SELECT count(*) FROM {xc_solr_field_type}')->fetchField();
  xc_log_info('tom14', '_metadata_type_to_node_type, pass in type: ');
  $form = array(
    '#tree' => TRUE,
  );

  $result = db_query('SELECT * FROM {xc_solr_field_type} ORDER BY weight');
  foreach ($result as $data) {
    $key = $data->type_id;
    $form['types'][$key]['label'] = array('#value' => $data->label);
    $form['types'][$key]['type'] = array('#value' => $data->type);
    $form['types'][$key]['suffix'] = array('#value' => $data->suffix);

    $form['types'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $data->weight,
      '#delta' => $weight_delta,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
  );
    static $cache;
xc_log_info('tom1222', '_metadata_type_to_node_type, pass in type: ' . $cache);
  return $form;
}

/**
 * Save the order of dynamic Solr types
 *
 * @param $form
 * @param $form_state
 */
function xc_solr_field_type_reorder_form_submit($form, &$form_state) {
  $types = $form_state['values']['types'];
  foreach ($types as $type_id => $type) {
    db_query("UPDATE {xc_solr_field_type} SET weight = :d1 WHERE type_id = :d2", array(':d1' => $type['weight'],':d2' => $type_id));
  }
  drupal_set_message(t('Order of dynamic Solr types has been successfully updated.'));
  $form_state['redirect'] = 'admin/xc/solr/field_type';
}

//TODO from Tom admin/xc/solr/field_type/reorder
//function theme_xc_solr_field_type_reorder_form($form, &$form_state) {
function theme_xc_solr_field_type_reorder_form($form, &$form_state) {

  $rows = array();
  drupal_add_tabledrag('type-sort', 'order', 'sibling', 'sort');

  foreach (element_children($form['types']) as $key) {
    $type = &$form['types'][$key];
    $type['weight']['#attributes']['class'] = 'sort';

    $cells = array(
      drupal_render($type['label']),
      drupal_render($type['type']),
      drupal_render($type['suffix']),
      drupal_render($type['weight'])
    );

    $rows[] = array(
      'data' => $cells,
      'class' => array('draggable')
    );
  }

  $output = theme('table', array(
    'header' => array(t('Label'), t('Type'), t('Suffix'), ''),
    'rows' => $rows,
    'attributes' => array('id' => 'type-sort')
  ));

  $output .= drupal_render($form);
  return $output;
}

function xc_solr_field_type_add() {
  drupal_set_title('Create a new field type');
  return drupal_get_form('xc_solr_field_type_form');
}

function xc_solr_field_type_form() {
  $schema = drupal_get_schema_unprocessed('xc_solr', 'xc_solr_field_type');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('type_id'),
    'hidden' => array(),
    'select' => xc_solr_field_type_options(),
    'label' => xc_solr_field_type_fieldset(),
  );

  $form = xc_util_build_autoform($schema, $options);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function xc_solr_field_type_edit_form(&$form_state, $record) {
  $form = xc_solr_field_type_form($form_state, $record);
  $form['type_id'] = array(
    '#type'  => 'hidden',
    '#value' => $record->type_id,
  );

  foreach (xc_solr_field_type_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $record->$name;
  }

  $form['submit']['#value'] = t('Save');
  $form['delete'] = array(
    '#type'  => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

function xc_solr_field_type_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_solr_field_type_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_solr_field_type', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added %type type!',
      array('%type' => $record->type)));
    $form_state['redirect'] = 'admin/xc/solr/field_type/' . $record->type_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new Solr field type.'));
  }
}

function xc_solr_field_type_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->type_id             = $values['type_id'];
  foreach (xc_solr_field_type_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_solr_field_type')
             ->condition('type_id', $record->type_id)
             ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%type removed', array('%type' => $record->type)));
      $form_state['redirect'] = 'admin/xc/solr/field_type/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count Solr field type.',
        array('%name' => $record->name)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_solr_field_type', $record, 'type_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%type" type!',
        array('%type' => $record->type)));
      $form_state['redirect'] = 'admin/xc/solr/field_type/' . $record->type_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify Solr field type.'));
    }
  }
}

/**
 * Return the field type of a given label
 *
 * @param $label (String)
 *   The label of a Solr field type (like: sortable integer, phrase, text, boolean,
 *   date, string facet, date facet....)
 *
 * @return (Object)
 *   The Solr field type record (fields: type_id, label, type, suffix)
 */
function xc_solr_field_type_by_label($label) {
  $sql = "SELECT * FROM {xc_solr_field_type} WHERE label = :label";
  $result = db_query($sql, array(':label' => $label));
  return $result->fetchObject();
}

