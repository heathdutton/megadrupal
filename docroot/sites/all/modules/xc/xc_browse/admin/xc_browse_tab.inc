<?php
/**
 * @file
 * Functions for xc_browse_tab table:
 * tab_id, ui_id, label, default_query, pagination_type, pagination_size, weight
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get the tabs belong to a browse UI
 *
 * @param $ui_id (int)
 *   The identifier of parent browse UI
 *
 * @return (array)
 *   The array of tab objects belonging to the UI
 */
function xc_browse_tab_get_by_ui($ui_id) {
  static $cache;
  if (!isset($cache)) {
    $cache = array();
  }

  if (!isset($cache[$ui_id])) {
    $result = db_query('SELECT * FROM {xc_browse_tab} WHERE ui_id = :ui_id ORDER BY weight, tab_id', array(':ui_id' => $ui_id));
    $val = array();
    foreach ($result as $data) {
      $val[] = $data;
    }
    $cache[$ui_id] = $val;
  }
  return $cache[$ui_id];
}

/**
 * Loads tab by it's ID
 *
 * @param $tab_id (int)
 *   The record identifier
 *
 * @return (Object)
 *   A xc_browse_tab table record object
 */
function xc_browse_tab_load($tab_id) {
  static $cache;
  if (!isset($cache[$tab_id])) {
    $result = db_query('SELECT * FROM {xc_browse_tab} WHERE tab_id = :tab_id', array(':tab_id' => $tab_id));
    $cache[$tab_id] = $result->fetchObject();
  }
  return $cache[$tab_id];
}

/**
 * Return the title
 *
 * @param $record (Object)
 */
function xc_browse_tab_title($record) {
  return t('Browse UI definition');
}

/**
 * Returns the field-human readable label pairs
 */
function xc_browse_tab_fieldset() {
  return array(
    'label' => t('The human readable label'),
    'ui_id' => t('Browse UI identifier'),
    'default_query' => t('Default query'),
    'pagination_type' => t('Pagination type'),
    'pagination_size' => t('Pagination size'),
    'weight' => t('Weight'),
  );
}

/**
 * Returns the options used in tab form
 *
 * @returns (Array)
 *   Associative array of options. The keys of array are the name of fields, and each field
 *   contains an assocuative array of key-value pairs.
 */
function xc_browse_tab_options() {
  return array(
    'pagination_type' => array(
      'none' => t('none'),
      'normal' => t('normal'),
      'logarithmic' => t('logarithmic'),
    ),
  );
}

/**
 * Displays the themed tab in admin screen
 *
 * @param $ui (int)
 *   The Browse UI identifier
 * @param $tab (int)
 *   The tab identifier
 *
 * @return (String)
 *   The themed tab view
 */
function xc_browse_tab_view($ui, $tab) {
  return theme('xc_browse_admin_tab', array('tab' => $tab, 'ui_id' => $ui->bid));
}

/**
 * Prepare data for displaying in admin UI
 *
 * @param $variables
 */
function template_preprocess_xc_browse_admin_tab(&$variables) {

  $variables['label'] = $variables['tab']->label;
  $variables['content'] = xc_util_view(
    $variables['tab'],
    xc_browse_tab_fieldset(),
    xc_browse_tab_options()
  );
  $path = 'admin/xc/browse/' . $variables['ui_id'] . '/' . $variables['tab']->tab_id;
  $variables['edit_link']   = url($path . '/edit');
  $variables['delete_link'] = url($path . '/delete');

  // form elements
  $variables['elements'] = array();
  $elements_data = xc_browse_element_get_by_tab($variables['tab']->tab_id);
  if (count($elements_data) == 0) {
    $variables['elements']['message'] = t('There is no any form element in this content pane.' .
      ' You can !add some form element to the content!',
      array('!add' => l(t('add'), $path . '/el_add')));
  }
  else {
    foreach ($elements_data as $element) {
      $variables['elements']['list'][] = theme('xc_browse_admin_element', array('tab' => $variables['tab'], 'element' => $element));
    }
    $variables['elements']['add_link'] = t('You can !add more form elements to the content!',
      array('!add' => l(t('add'), $path . '/el_add')));
  }

  // navigation lists
  $variables['bars'] = array();
  $bars_data = xc_browse_list_get_by_tab($variables['tab']->tab_id);
  if (count($bars_data) == 0) {
    $variables['bars']['message'] = t('There is no any navigation list in this content pane.' .
      ' You can !add some list to the content!', array('!add' => l(t('add'),
       $path . '/list_add')));
  }
  else {
    foreach ($bars_data as $bar) {
      $variables['bars']['list'][] = theme('xc_browse_admin_bar', array('tab' => $variables['tab'], 'bar' => $bar));
    }
    $variables['bars']['add_link'] = t('You can !add more lists to the content pane!',
      array('!add' => l(t('add'), $path . '/list_add')));
  }
}

/**
 * Lists the available tabs
 */
function xc_browse_tab_list() {
  $headers = array(t('Label'), t('Name'));
  $rows = array();
  $options = xc_browse_tab_options();
  $sql = 'SELECT * FROM {xc_browse_tab}';
  $result = db_query($sql);
  foreach ($result as $data) {
    $rows[] = array(
      $data->label,
      $data->name,
      l(t('view'), 'admin/xc/browse/' . $data->tab_id),
      l(t('edit'), 'admin/xc/browse/' . $data->tab_id . '/edit'),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Creates a page for the add new content page form
 * @param $ui
 */
function xc_browse_tab_add($ui) {
  drupal_set_title(t('Add a new content pane'));
  return drupal_get_form('xc_browse_tab_form', $ui);
}

/**
 * Returns the definition of tab form
 *
 * @param $form_state (Array)
 *   The form state
 * @param $ui (int)
 *   The Browse UI identifier
 * @param $tab (int)
 *   The tab's identifier
 *
 * @return (Array)
 *   The FAPI definition
 */
function xc_browse_tab_form($form, &$form_state, $ui, $tab = NULL) {
  $schema = drupal_get_schema_unprocessed('xc_browse', 'xc_browse_tab');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('tab_id'),
    'hidden' => array(
      'ui_id' => $ui->bid,
    ),
    'label' => xc_browse_tab_fieldset(),
    'select' => xc_browse_tab_options(),
  );

  $form = xc_util_build_autoform($schema, $options);
  //$form['ui_id']['#default_value'] = ;
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
function xc_browse_tab_edit_form($form, $ui, $tab) {
  $form = xc_browse_tab_form($form, $ui, $tab);
  $form['tab_id'] = array(
    '#type' => 'hidden',
    '#value' => $tab->tab_id,
  );

  foreach (xc_browse_tab_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $tab->$name;
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
function xc_browse_tab_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_browse_tab_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_browse_tab', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%label" tab!',
      array('%label' => $record->label)));
    $form_state['redirect'] = 'admin/xc/browse/' . $record->ui_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new tab.'));
  }
  menu_rebuild();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_tab_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->tab_id             = $values['tab_id'];
  foreach (xc_browse_tab_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    if ($result == 1) { // element is deleted
      drupal_set_message(t('%label removed', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $record->ui_id;
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removes %count tab(s).',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_browse_tab', $record, 'tab_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%label" tab!',
        array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $record->ui_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify tab.'));
    }
  }
  menu_rebuild();
}

/**
 * Creates a tab deletion confirmation form
 *
 * @param $ui (Object)
 *   The browse UI object
 * @param $tab (Object)
 *   The tab object
 *
 * @return (Array)
 *   The confirmation form
 */
function xc_browse_tab_delete_form($form, &$form_state, $ui, $tab) {
  return confirm_form(
    array(
    'tab_id' => array(
      '#type' => 'hidden',
      '#default_value' => $tab->tab_id,
    ),
    'label' => array(
      '#type' => 'hidden',
      '#default_value' => $tab->label,
    ),
    'ui_id' => array(
      '#type' => 'hidden',
      '#default_value' => $ui->bid,
    ),
  ),
    t('Are you sure, that you would like to delete this tab?'),
    'admin/xc/browse/' . $ui->bid, // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Delete tab'),
    t('Cancel')
  );
}

/**
 * Process the tab deletion confirmation form values
 *
 * @param $form (Array)
 *   The FORM
 * @param $form_state (Array)
 *   The Form state
 */
function xc_browse_tab_delete_form_submit($form, &$form_state) {
  $tab_id = $form_state['values']['tab_id'];
  $ui_id   = $form_state['values']['ui_id'];
  $label   = $form_state['values']['label'];

  xc_browse_tab_delete($tab_id, $label);
  $form_state['redirect'] = 'admin/xc/browse/' . $ui_id;
}

/**
 * Delete a tab from the database
 *
 * @param $tab_id (int)
 *   The ID of the tab record
 *
 * @return (boolean)
 *   TRUE if the record was deleted, FALSE otherwise
 */
function xc_browse_tab_delete($tab_id, $label = NULL) {
  $result = db_delete('xc_browse_tab')
            ->condition('tab_id', $tab_id)
            ->execute();
  if ($result == 1) {
    drupal_set_message(t('%label removed',
      array('%label' => (!empty($label) ? $label : $tab_id))));
    return TRUE;
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count tab(s).',
      array('%count' => $result)));
    return FALSE;
  }
}