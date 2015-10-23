<?php
/**
 * @file
 * Display template element functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_title($record) {
  return t('Display template element');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_fieldset() {
  return array(
    'metadata_schema' => t('metadata schema'),
    'entity_type' => t('entity type'),
    'element_name' => t('element name'),
    'template' => t('element template'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_options() {
  $options = array('metadata_schema' => array());

  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';

  // metadata schema
  $options['metadata_schema'] = xc_format_get_schemas();

  // entities
  $options['entity_type'] = xc_format_get_entities();

  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_view($record) {
  drupal_set_title(xc_search_display_template_element_title($record));
  return xc_util_view($record,
    xc_search_display_template_element_fieldset(),
    xc_search_display_template_element_options(),
    array('filters' => array('template' => array('htmlentities', 'nl2br')))
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_list() {
  $headers = array(t('Schema'), t('Entity'), t('Element'), t('Template'));
  $rows = array();
  $options = xc_search_display_template_element_options();
  $sql = 'SELECT * FROM {xc_search_display_template_element}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      $data->metadata_schema,
      $data->entity_type,
      l($data->element_name, 'admin/xc/search/display_template_element/' . $data->element_id),
      nl2br(htmlentities($data->template)),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_add() {
  drupal_set_title('Create a new template element');
  return drupal_get_form('xc_search_display_template_element_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_display_template_element');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('element_id'),
    'hidden' => array(),
    'label' => xc_search_display_template_element_fieldset(),
    'select' => xc_search_display_template_element_options(),
    // TODO: metadata_schema!!!
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
function xc_search_display_template_element_edit_form($form, &$form_state, $record) {
  $form = xc_search_display_template_element_form($form_state, $record);
  $form['element_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->element_id,
  );

  foreach (xc_search_display_template_element_fieldset() as $name => $label) {
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
function xc_search_display_template_element_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_display_template_element_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_search_display_template_element', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%name" display template element!',
                         array('%name' => $record->element_name)));
    $form_state['redirect'] = 'admin/xc/search/display_template_element/' . $record->element_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new display template element.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_element_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->element_id             = $values['element_id'];
  foreach (xc_search_display_template_element_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_display_template_element')
              ->condition('element_id', $record->element_id)
              ->execute();
    if ($result == 1) { // element is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->element_name)));
      $form_state['redirect'] = 'admin/xc/search/display_template_element/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count display template.',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_search_display_template_element', $record,
      'element_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%name" display template element!',
                         array('%name' => $record->element_name)));
      $form_state['redirect'] = 'admin/xc/search/display_template_element/'
        . $record->element_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify display template element.'));
    }
  }
}

/**
 * Confirmation form for importing display template elements
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_display_template_element_import_defaults_form(&$form_state) {
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
    t('Are you sure, that you would like to import display template elements?'),
    'admin/xc/search/display_template_element', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Process'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing display template elements
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_display_template_element_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_display_template_element_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/display_template_element';
}

/**
 * Importing default display template elements
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_display_template_element_import_defaults($form, $delete = FALSE) {
  drupal_set_message(t('Importing default display display template elements'));

  if ($delete) {
    // delete all records
    $sql = 'DELETE FROM {xc_search_display_template_element}';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql) */
    $result = db_delete('xc_search_display_template_element')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_display_template_element} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $table = 'xc_search_display_template_element';
  $filename = drupal_get_path('module', 'xc_search') . '/import/' . $table . '.xml';
  xc_util_bulk_insert($table, xc_util_xmldump_to_records($filename));
  variable_set('xc_search_display_template_element_defaults_installed', XC_LOADED);
}
