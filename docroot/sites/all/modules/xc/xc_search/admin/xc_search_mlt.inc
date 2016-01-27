<?php
/**
 * @file
 * More like this functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get title for 'MoreLikeThis'
 * @param $mlt (Object)
 *   A MoreLikeThis record
 * @return (String)
 *   Title string
 */
function xc_search_mlt_title($mlt) {
  return $mlt->name;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_fieldset() {
  return array(
    'name' => t('Name'),
    'fl' => t('Fields'),
    'mintf' => t('Minimum Term Frequency'),
    'mindf' => t('Minimum Document Frequency'),
    'minwl' => t('Minimum word length'),
    'maxwl' => t('Maximum word length'),
    'maxqt' => t('Maximum query terms'),
    'maxntp' => t('Maximum number of tokens to parse'),
    'boost' => t('Boosting'),
    'qf' => t('Boosting factors'),
    'count' => t('Limit'),
    'match_include' => t('Include matched document?'),
    'match_offset' => t('Similarity of which result?'),
    'interestingTerms' => t('Show the "interesting" terms'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_options() {
  return array(
    'boost' => xc_util_get_global_options('true_false_solr'),
    'match_include' => xc_util_get_global_options('true_false_solr'),
    'interestingTerms' => array(
      'list' => t('list'),
      'details' => t('details'),
      'none' => t('none'),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_view($record) {
  drupal_set_title(xc_search_mlt_title($record));
  return xc_util_view($record,
    xc_search_mlt_fieldset(),
    xc_search_mlt_options()
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_list() {

  $headers = array(t('Name'), t('Similar fields'));
  $rows = array();

  $sql = 'SELECT mlt_id, name, fl FROM {xc_search_mlt}';
  $result = db_query($sql); 
  foreach ($result as $data) {
    $rows[] = array(
      l($data->name, 'admin/xc/search/mlt/' . $data->mlt_id),
      $data->fl,
      l(t('edit'), 'admin/xc/search/mlt/' . $data->mlt_id . '/edit'),
      l(t('delete'), 'admin/xc/search/mlt/' . $data->mlt_id . '/delete'),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new server
 * @return string The add server page with a form
 */
function xc_search_mlt_add() {
  return drupal_get_form('xc_search_mlt_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_mlt');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('mlt_id'),
    'select' => xc_search_mlt_options(),
    'label' => xc_search_mlt_fieldset(),
  );
  $form = xc_util_build_autoform($schema, $options);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_edit_form($form, &$form_state, $set) {
  $form = xc_search_mlt_form();
  $form['mlt_id'] = array(
    '#type' => 'hidden',
    '#value' => $set->mlt_id,
  );
  foreach (xc_search_mlt_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $set->$name;
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
function xc_search_mlt_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_mlt_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  $ret_val = drupal_write_record('xc_search_mlt', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added "More like this" settings "%name"',
                         array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/mlt/' . $record->mlt_id;
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new "More like this" settings.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_mlt_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->mlt_id      = $values['mlt_id'];
  $record->name        = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_mlt')
                ->condition('mlt_id', $record->mlt_id)
                ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/mlt/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to remove %name.',
        array('%name' => $record->name)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_mlt_fieldset() as $name => $label) {
      $record->$name = $values[$name];
    }

    $result = drupal_write_record('xc_search_mlt', $record, 'mlt_id');
    if ($result == SAVED_UPDATED) { // repository is updated
      drupal_set_message(t('%name modified', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/mlt/' . $record->mlt_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify %name.',
        array('%name' => $record->name)));
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
}

/**
 * Confirmation form for importing more like this settings
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_mlt_import_defaults_form(&$form_state) {
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
    t('Are you sure, that you would like to import default more like this settings?'),
    'admin/xc/search/mlt', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing more like this settings
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_mlt_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_mlt_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/mlt';
}

/**
 * Importing default more like this settings
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_mlt_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default more like this settings'));

  if ($delete) {
    // delete all records 
    $result = db_delete('xc_search_mlt')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_mlt} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_mlt.csv';
  xc_util_bulk_insert('xc_search_mlt', xc_util_csv2objects($filename));

  variable_set('xc_search_mlt_defaults_installed', XC_LOADED);
}
