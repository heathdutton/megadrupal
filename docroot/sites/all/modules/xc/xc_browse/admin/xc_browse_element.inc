<?php
/**
 * @file
 * Functions handling xc_browse_element table:
 * el_id, tab_id, type, facet_name, search_field, query_type, ui_effect, weight
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_get_by_tab($tab_id) {
  static $cache;
  if (!isset($cache)) {
    $cache = array();
  }
  if (!isset($cache[$tab_id])) {
    $sql = 'SELECT * FROM {xc_browse_element} WHERE tab_id = %d ORDER BY weight, el_id';
    $result = db_query('SELECT * FROM {xc_browse_element} WHERE tab_id = :tab_id ORDER BY weight, el_id', array(':tab_id' => $tab_id));
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
function xc_browse_element_load($el_id) {
  $sql = 'SELECT * FROM {xc_browse_element} WHERE el_id = %d';
  $result = db_query('SELECT * FROM {xc_browse_element} WHERE el_id = :el_id', array(':el_id' => $el_id));
  $data = $result->fetchObject();
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_title($record) {
  return t('Browse UI definition');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_fieldset() {
  return array(
    'el_id' => t('Form element identifier'),
    'tab_id' => t('Parent content pane identifier'),
    'label' => t('A label which displaying before the form element.'),
    'type' => t('Type of form element'),
    'facet_name' => t('The facet which populates the element'),
    'search_field' => t('The Solr field to search for'),
    'query_type' => t('A Solr query type'),
    'ui_effect' => t('JavaScript effect to apply on this element'),
    'sorting' => t('Sorting of values in the list'),
    'mincount' => t('Minimal occurrence'),
    'is_enabled' => t('Enable/disable the element'),
    'page_limit' => t('Limit'),
    'weight' => t('Weight'),
  );
}

/**
 * List of possible values for fields.
 */
function xc_browse_element_options() {
  return array(
    'type' => array(// , , ,
      'textfield' => t('text field'),
      'select' => t('dropdown select list'),
      'radios' => t('radio buttons'),
      'checkboxes' => t('check boxes'),
    ),
    'query_type' => array(
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
    'ui_effect' => array(),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_view($record) {
  $output = xc_util_view($record, xc_browse_element_fieldset(), xc_browse_element_options(), array());

  return $output;
}

/**
 * Prepare data for displaying in admin UI
 *
 * @param $variables
 */
function template_preprocess_xc_browse_admin_element(&$variables) {
  $path = sprintf('%s/%s/%s', $variables['tab']->ui_id, $variables['tab']->tab_id, $variables['element']->el_id);
  $variables += array(
    'label' => $variables['element']->label,
    'content' => xc_browse_element_view($variables['element']),
    'edit_link' => l(t('edit'),    'admin/xc/browse/' . $path . '/el_edit'),
    'delete_link' => l(t('delete'), 'admin/xc/browse/' . $path . '/el_delete'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_list($ui, $tab) {
  $headers = array(t('Label'), t('Name'));
  $rows = array();
  $options = xc_browse_element_options();
  $sql = 'SELECT * FROM {xc_browse_element}';
  $result = db_query($sql);
  foreach ($result as $data) {
    $rows[] = array(
      $data->label,
      $data->name,
      l(t('view'), 'admin/xc/browse/' . $data->el_id),
      l(t('edit'), 'admin/xc/browse/' . $data->el_id . '/edit'),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_add($ui, $tab) {
  drupal_set_title(t('Create a new browse UI'));
  return drupal_get_form('xc_browse_element_form', $ui, $tab);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_form($form, &$form_state, $ui, $tab) {
  $schema = drupal_get_schema_unprocessed('xc_browse', 'xc_browse_element');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('el_id'),
    'hidden' => array(
      'tab_id' => $tab->tab_id,
      'ui_id' => $ui->ui_id,
    ),
    'label' => xc_browse_element_fieldset(),
    'select' => xc_browse_element_options(),
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
function xc_browse_element_edit_form($form, &$form_state, $ui, $tab, $record) {
  $form = xc_browse_element_form($form_state, $ui, $tab, $record);
  $form['el_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->el_id,
  );

  foreach (xc_browse_element_fieldset() as $name => $label) {
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
function xc_browse_element_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_browse_element_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_browse_element', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "%label" browse UI!', array('%label' => $record->label)));
    $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'] . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new form element.'));
  }
  variable_del('xc_browse_stored_lists');
  menu_rebuild();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_browse_element_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  $record->el_id             = $values['el_id'];
  foreach (xc_browse_element_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_browse_element')
              ->condition('el_id', $record->el_id)
              ->execute();
    if ($result == 1) { // element is deleted
      drupal_set_message(t('%label removed', array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'];
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count element(s).',
        array('%count' => $result)));
    }
  }
  else {
    $ret_val = drupal_write_record('xc_browse_element', $record, 'el_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated "%label" form element!',
        array('%label' => $record->label)));
      $form_state['redirect'] = 'admin/xc/browse/' . $values['ui_id'] . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify form element.'));
    }
  }
  variable_del('xc_browse_stored_lists');
  menu_rebuild();
}

/**
 * Creates a form element deletion confirmation form
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
function xc_browse_element_delete_form($form, &$form_state, $ui, $tab, $element) {
  return confirm_form(
    array(
    'element_id' => array(
      '#type' => 'hidden',
      '#default_value' => $element->el_id,
    ),
    'label' => array(
      '#type' => 'hidden',
      '#default_value' => $element->label,
    ),
    'ui_id' => array(
      '#type' => 'hidden',
      '#default_value' => $ui->bid,
    ),
  ),
    t('Are you sure, that you would like to delete this form element?'),
    'admin/xc/browse/' . $ui->bid, // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Delete form element'),
    t('Cancel')
  );
}

/**
 * Process the form element deletion confirmation form values
 *
 * @param $form (Array)
 *   The FORM
 * @param $form_state (Array)
 *   The Form state
 */
function xc_browse_element_delete_form_submit($form, &$form_state) {
  $element_id = $form_state['values']['element_id'];
  $ui_id   = $form_state['values']['ui_id'];
  $label   = $form_state['values']['label'];

  xc_browse_element_delete($element_id, $label);
  variable_del('xc_browse_stored_lists');
  $form_state['redirect'] = 'admin/xc/browse/' . $ui_id;
}

/**
 * Delete a form element from the database
 *
 * @param $element_id (int)
 *   The ID of the form element record
 *
 * @return (boolean)
 *   TRUE if the record was deleted, FALSE otherwise
 */
function xc_browse_element_delete($element_id, $label = NULL) {
  $result = db_delete('xc_browse_element')
              ->condition('el_id', $element_id)
              ->execute();
  if ($result == 1) {
    drupal_set_message(t('%label removed',
      array('%label' => (!empty($label) ? $label : $element_id))));
    return TRUE;
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removes %count form element.',
      array('%count' => $result)));
    return FALSE;
  }
}