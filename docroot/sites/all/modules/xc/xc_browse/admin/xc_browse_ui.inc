<?php
/**
 * @file
 * Functions for xc_browse_ui table: bid, label, name
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get all browse UIs
 *
 * @param $only_enabled (Boolean)
 *   If TRUE it gets only the enabled UIs
 *
 * @return (array)
 *   The list of UI objects. The object's keys are:
 *   - bid (int) identifier
 *   - label (string) human readable name
 *   - name (string) machine name
 */
function xc_browse_ui_get_all($only_enabled = FALSE) {
  $sql = 'SELECT * FROM {xc_browse_ui} WHERE is_enabled = 1';

  $result = db_query($sql);
  $rows = array();
  foreach ($result as $data) {
    $rows[] = $data;
  }
  return $rows;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_get_by_name($name) {
  $sql = 'SELECT * FROM {xc_browse_ui} WHERE name = \'%s\'';
  $result = db_query('SELECT * FROM {xc_browse_ui} WHERE name = :name', array(':name' => $name));
  $data = $result->fetchObject();
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_load($bid) {
  $sql = 'SELECT * FROM {xc_browse_ui} WHERE bid = %d';
  $result = db_query('SELECT * FROM {xc_browse_ui} WHERE bid = :bid', array(':bid' => $bid));
  $data = $result->fetchObject();
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_title($record) {
  return t($record->label);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_fieldset() {
  return array(
    'label' => t('The human readable label'),
    'name' => t('Machine name'),
    'is_enabled' => t('Is enabled'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_options() {
  return array(
    'is_enabled' => xc_util_get_global_options('true_false'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_admin_ui_view($ui) {
  // TODO Please change this theme call to use an associative array for the $variables parameter.
  return theme('xc_browse_admin_ui', array('ui' => $ui, 'options' => xc_browse_ui_options()));
}

function template_preprocess_xc_browse_admin_ui(&$variables) {
  drupal_add_js('misc/drupal.js', array('type' => 'file', 'weight' => JS_LIBRARY));
  drupal_add_js('misc/collapse.js', array('type' => 'file', 'weight' => JS_LIBRARY));

  $variables['label'] = $variables['ui']->label;
  $variables['content'] = xc_util_view(
    $variables['ui'],
    xc_browse_ui_fieldset(),
    xc_browse_ui_options()
  );

  $tabs = xc_browse_tab_get_by_ui($variables['ui']->bid);
  $variables['message'] = NULL;
  $variables['tabs'] = array();
  if (count($tabs) == 0) {
    $variables['message'] = t('Please !add_content_pane to this browse user interface!',
      array('!add_content_pane' => l(t('add a content pane'),
      'admin/xc/browse/' . $variables['ui']->bid . '/tab_add')));
  }
  else {
    foreach ($tabs as $tab) {
      $variables['tabs'][] = theme('xc_browse_admin_tab', array('tab' => $tab, 'ui_id' => $variables['ui']->bid));
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_list() {
  $headers = array(t('Label'), t('Name'), t('Is enabled?'), array(
      'data' => t('Operations'),
      'colspan' => 2,
    ));
  $rows = array();
  $options = xc_browse_ui_options();
  $sql = 'SELECT * FROM {xc_browse_ui}';
  $result = db_query($sql);
  foreach ($result as $data) {
    $rows[] = array(
      $data->label,
      $data->name,
      ($data->is_enabled ? t('enabled') : t('disabled')),
      l(t('view'), 'admin/xc/browse/' . $data->bid),
      l(t('edit'), 'admin/xc/browse/' . $data->bid . '/edit'),
    );
  }

  if (empty($rows)) {
    drupal_set_message(
      t('No browse UI is defined on this system. You must first !create a browse UI.',
       array('!create' => l(t('create'), 'admin/xc/browse/add'))),
     'warning');
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_add() {
  drupal_set_title(t('Create a new browse UI'));
  return drupal_get_form('xc_browse_ui_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_browse', 'xc_browse_ui');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('bid'),
    'hidden' => array(),
    'label' => xc_browse_ui_fieldset(),
    'select' => xc_browse_ui_options(),
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
function xc_browse_ui_edit_form($form, &$form_state, $record) {
  $form = xc_browse_ui_form($form_state, $record);
  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => $record->bid,
  );

  foreach (xc_browse_ui_fieldset() as $name => $label) {
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
function xc_browse_ui_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_browse_ui_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_browse_ui', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%label" browse UI!',
      array('%label' => $record->label)));
    $form_state['redirect'] = 'admin/xc/browse/' . $record->bid . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new browse UI.'));
  }
  menu_rebuild();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_ui_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->bid             = $values['bid'];
  foreach (xc_browse_ui_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_browse_ui')
              ->condition('bid', $record->bid)
              ->execute();
    if ($result == 1) { // element is deleted
      drupal_set_message(t('%label removed', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count display template(s).',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_browse_ui', $record, 'bid');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%label" browse UI!',
        array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $record->bid . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify browse UI.'));
    }
  }
  menu_rebuild();
}
