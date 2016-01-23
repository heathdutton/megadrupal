<?php
/**
 * @file
 * Functions for xc_browse_list table: lid, tab_id, label, type, field, query_type, weight
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_get_by_tab($tab_id) {
  static $cache;
  if (!isset($cache)) {
    $cache = array();
  }
  if (!isset($cache[$tab_id])) {
    $result = db_query('SELECT * FROM {xc_browse_list} WHERE tab_id = :tab_id ORDER BY weight, lid', array(':tab_id' => $tab_id));
    $val = array();
    foreach ($result as $data) {
      $val[] = $data;
    }
    $cache[$tab_id] = $val;
  }
  return $cache[$tab_id];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_load($lid) {
  $result = db_query('SELECT * FROM {xc_browse_list} WHERE lid = :lid', array(':lid' => $lid));
  $data = $result->fetchObject();
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_title($record) {
  return t('Browse UI definition');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_fieldset() {
  return array(
    'tab_id' => t('The parent content pane identifier'),
    'label' => t('The label above the list'),
    'type' => t('The type of the navigation list'),
    'field' => t('The Solr field to search for when the user clicks on an item'),
    'query_type' => t('A Solr query type'),
    'sorting' => t('Sorting of values in the list'),
    'mincount' => t('Minimal occurrence'),
    'is_enabled' => t('Enable/disable the list'),
    'page_limit' => t('Limit'),
    'weight' => t('Weight'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_options() {
  return array(
    'type' => array(
      'ABC' => t('ABC'),
      'dates' => t('dates'),
      'facet' => t('facet'),
    ),
    'query_type' => array(//, ,
      'keyword' => t('keyword'),
      'phrase' => t('phrase'),
      'proximity' => t('proximity'),
      'truncated' => t('truncated'),
    ),
    'sorting' => array(
      'true' => 'sort by count',
      'false' => 'sort alphabetically',
    ),
    'is_enabled' => array(
      0 => t('False'),
      1 => t('True'),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_view($record) {
  $output = xc_util_view($record,
    xc_browse_list_fieldset(),
    xc_browse_list_options()
  );
  return $output;
}

/**
 * Prepare data for displaying in admin UI
 *
 * @param $variables
 */
function template_preprocess_xc_browse_admin_bar(&$variables) { // $tab, $bar
  $path = $variables['tab']->ui_id . '/' . $variables['tab']->tab_id
    . '/' . $variables['bar']->lid;
  $label = $variables['bar']->label;
  if (empty($label)) {
    $label = '(no title)';
  }
  $variables += array(
    'label' => $label,
    'content' => xc_browse_list_view($variables['bar']),
    'edit_link' => l(t('edit'),    'admin/xc/browse/' . $path . '/list_edit'),
    'delete_link' => l(t('delete'), 'admin/xc/browse/' . $path . '/list_delete'),
  );
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_list() {
  $headers = array(t('Label'), t('Name'));
  $rows = array();
  $options = xc_browse_list_options();
  $sql = 'SELECT * FROM {xc_browse_list}';
  $result = db_query($sql);
  foreach ($result as $data) {
    $rows[] = array(
      $data->label,
      $data->name,
      l(t('view'), 'admin/xc/browse/' . $data->lid),
      l(t('edit'), 'admin/xc/browse/' . $data->lid . '/edit'),
    );
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_add($ui, $tab) {
  drupal_set_title(t('Create a new browse UI'));
  return drupal_get_form('xc_browse_list_form', $ui, $tab);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_form($form, &$form_state, $ui, $tab) {
  $schema = drupal_get_schema_unprocessed('xc_browse', 'xc_browse_list');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('lid'),
    'hidden' => array(
      'tab_id' => $tab->tab_id,
      'ui_id' => $ui->ui_id,
    ),
    'label' => xc_browse_list_fieldset(),
    'select' => xc_browse_list_options(),
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
function xc_browse_list_edit_form($form, &$form_state, $ui, $tab, $record) {
  $form = xc_browse_list_form($form_state, $ui, $tab, $record);
  $form['lid'] = array(
    '#type' => 'hidden',
    '#value' => $record->lid,
  );

  foreach (xc_browse_list_fieldset() as $name => $label) {
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
function xc_browse_list_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_browse_list_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_browse_list', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%label" browse UI!',
      array('%label' => $record->label)));
    $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'] . '/view';
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
function xc_browse_list_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->lid = $values['lid'];
  foreach (xc_browse_list_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_browse_list')
                ->condition('lid', $record->lid)
                ->execute();
    if ($result == 1) { // element is deleted
      drupal_set_message(t('%label removed', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'];
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count list(s).',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_browse_list', $record, 'lid');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%label" list!',
        array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'] . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed modify list.'));
    }
  }
  menu_rebuild();
}

/**
 * Creates a list deletion confirmation form
 *
 * @param $ui (Object)
 *   The browse UI object
 * @param $tab (Object)
 *   The tab object
 * @param $list (Object)
 *   The list (bar) object
 *
 * @return (Array)
 *   The confirmation form
 */
function xc_browse_list_delete_form($form, &$form_state, $ui, $tab, $list) {
  return confirm_form(
    array(
    'list_id' => array(
      '#type' => 'hidden',
      '#default_value' => $list->lid,
    ),
    'label' => array(
      '#type' => 'hidden',
      '#default_value' => $list->label,
    ),
    'ui_id' => array(
      '#type' => 'hidden',
      '#default_value' => $ui->bid,
    ),
  ),
    t('Are you sure, that you would like to delete this list?'),
    'admin/xc/browse/' . $ui->bid, // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Delete list'),
    t('Cancel')
  );
}

/**
 * Process the list deletion confirmation form values
 *
 * @param $form (Array)
 *   The FORM
 * @param $form_state (Array)
 *   The Form state
 */
function xc_browse_list_delete_form_submit($form, &$form_state) {
  $list_id = $form_state['values']['list_id'];
  $ui_id   = $form_state['values']['ui_id'];
  $label   = $form_state['values']['label'];

  xc_browse_list_delete($list_id, $label);
  $form_state['redirect'] = 'admin/xc/browse/' . $ui_id;
}

/**
 * Delete a list from the database
 *
 * @param $list_id (int)
 *   The ID of the list record
 *
 * @return (boolean)
 *   TRUE if the record was deleted, FALSE otherwise
 */
function xc_browse_list_delete($list_id, $label = NULL) {
  $result = db_delete('xc_browse_list')
              ->condition('lid', $list_id)
              ->execute();
  if ($result == 1) {
    drupal_set_message(t('%label removed', array('%label' => (!empty($label) ? $label : $list_id))));
    return TRUE;
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count list(s).',
      array('%count' => $result)));
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_list_get_ABC() {
  static $cache;
  if (empty($cache)) {
    $cache = range('A', 'Z');
    $cache[] = '0-9';
  }
  return $cache;
}

