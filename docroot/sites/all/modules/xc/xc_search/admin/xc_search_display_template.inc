<?php
/**
 * @file
 * Display template functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_get($entity_type, $display_type) {
  static $template_cache;
  if (!isset($template_cache)) {
    $template_cache = array();
  }

  if (!isset($template_cache[$entity_type])
       || !isset($template_cache[$entity_type][$display_type])) {
    $sql = 'SELECT template FROM {xc_search_display_template}
      WHERE entity_type = \'%s\'
        AND display_type = \'%s\'';
    $result = db_query('SELECT template FROM {xc_search_display_template}
      WHERE entity_type = :entity_type
        AND display_type = :display_type', array(':entity_type' => $entity_type, ':display_type' => $display_type));
    $data = $result->fetchObject();
    $template_cache[$entity_type][$display_type] = $data->template;
  }
  return $template_cache[$entity_type][$display_type];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_title($record) {
  return t('Display template');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_fieldset() {
  return array(
    'metadata_schema' => t('metadata schema'),
    'entity_type' => t('entity type'),
    'display_type' => t('display type'),
    'template' => t('the displayed template'),
    'highlighter_id' => t('the connected highlighter'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_options() {
  $options = array('metadata_schema' => array());

  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';

  // metadata schema
  $options['metadata_schema'] = xc_format_get_schemas();

  // entities
  $options['entity_type'] = xc_format_get_entities();

  // display types
  $options['display_type'] = array(
    'title' => t('result\'s title'),
    'snippet' => t('result\'s body'),
    'full' => t('full display'),
  );

  // highlighters
  $options['highlighter_id'] = array(
    '' => '-- select optionally --',
  );
  // $options['highlighter_id'] += xc_search_highlighter_list_values();

  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_view($record) {
  drupal_set_message(check_plain(xc_search_display_template_title($record)));
  return xc_util_view($record,
    xc_search_display_template_fieldset(),
    xc_search_display_template_options(),
    array('filters' => array('template' => array(array('highlight_string', TRUE))))
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_list() {
  $headers = array(t('Schema'), t('Entity'), t('Display type'), t('Operations'));
  $rows = array();
  $options = xc_search_display_template_options();
  $sql = 'SELECT * FROM {xc_search_display_template}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      $data->metadata_schema,
      $data->entity_type,
      $options['display_type'][$data->display_type],
      //nl2br(htmlentities($data->template)),
      join(' &mdash; ', array(
        l(t('view'),
          'admin/xc/search/display_template/' . $data->template_id),
        l(t('edit'),
          'admin/xc/search/display_template/' . $data->template_id . '/edit'),
      )),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_add() {
  drupal_set_title('Create a new template');
  return drupal_get_form('xc_search_display_template_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_display_template');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('template_id'),
    'hidden' => array(),
    'label' => xc_search_display_template_fieldset(),
    'select' => xc_search_display_template_options(),
    // TODO: metadata_schema!!!
  );

  $form = xc_util_build_autoform($schema, $options);
  $form['template']['#rows'] = 25;
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
function xc_search_display_template_edit_form($form, &$form_state, $record) {
  $form = xc_search_display_template_form($form_state, $record);
  $form['template_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->template_id,
  );

  foreach (xc_search_display_template_fieldset() as $name => $label) {
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
function xc_search_display_template_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_display_template_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_search_display_template', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%name" display_template!',
      array('%name' => $record->entity_type . '/' . $record->display_type)));
    xc_search_display_template_save_to_file($record);
    $form_state['redirect'] = 'admin/xc/search/display_template/'
      . $record->template_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new display template.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_display_template_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->template_id             = $values['template_id'];
  foreach (xc_search_display_template_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_display_template')
                ->condition('template_id', $record->template_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->template_id)));
      $form_state['redirect'] = 'admin/xc/search/display_template/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count display template.',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_search_display_template', $record, 'template_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%name" display template!',
        array('%name' => $record->entity_type . '/' . $record->display_type)));
      xc_search_display_template_save_to_file($record);
      $form_state['redirect'] = 'admin/xc/search/display_template/'
        . $record->template_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify display template.'));
    }
  }
}

/**
 * Confirmation form for importing default display templates
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_display_template_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import display templates?'),
    'admin/xc/search/display_template', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Process'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default display templates
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_display_template_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_display_template_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/display_template';
}

/**
 * Importing default display templates
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_display_template_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default display templates'));

  if ($delete) {
    // delete all records
    $sql = 'DELETE FROM {xc_search_display_template}';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql) */
    $result = db_delete('xc_search_display_template')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_display_template} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $table = 'xc_search_display_template';
  $filename = drupal_get_path('module', 'xc_search') . '/import/' . $table . '.xml';
  xc_util_bulk_insert($table, xc_util_xmldump_to_records($filename));
  variable_set('xc_search_display_template_defaults_installed', XC_LOADED);
}

/**
 * Save template content to template file.
 * @param $record
 */
function xc_search_display_template_save_to_file($record) {
  $dirname = drupal_get_path('module', 'xc_search') . '/' . XC_SEARCH_TEMPLATE_DIR;
  //TODO from Tom, need to check dir name
  $filename = $dirname . '/' . $record->entity_type . '-' . $record->display_type . '.tpl.php';
  $options = array('ftp' => array('overwrite' => TRUE));
  $stream = stream_context_create($options);
  if (file_put_contents($filename, $record->template)) {
    drupal_set_message(t('The template were saved as %filename', array('%filename' => $filename)));
  }
  else {
    drupal_set_message(t('Can not save template to file. Please check file permissions at the %dir directory', array('%dir' => $dirname)));
  }
}
