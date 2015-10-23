<?php
/**
 * @file
 * Search UI functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 *
 * xc_search_ui is a top level search interface definition element.
 * Now it connects facet group, and highlighter settings, later it will contain
 * a form definition link. The base concept here is that each search UI will be
 * defined by these elements.
 * Field list: sui_id, name, facet_id (->xc_search_facet_group),
 * highlighter_id (->xc_search_highlighter), do_prepopulate_facets, authors_count
 */


/**
 * Get title for search UI
 *
 * @param $record (Object)
 *   The search UI object
 *
 * @return (String)
 *   The name of search UI as it will be the title
 */
function xc_search_ui_title($record) {
  return $record->name;
}

/**
 * Return the field name -> human readable label pairs of the fields
 * @return (Array)
 */
function xc_search_ui_fieldset() {
  return array(
    'name' => t('Name'),
    'facet_id' => t('Facet group'),
    'highlighter_id' => t('Highlighter'),
    'do_prepopulate_facets' => t('Prepopulate facets?'),
    'authors_count' => t('The number of authors to display in search result list, if there are multiple'),
    'display_bookmark_action' => t('Whether or not to display bookmark action button?'),
    'display_email_action' => t('Whether or not to display email action button?'),
    'display_text_action' => t('Whether or not to display text action button?'),
    'display_print_action' => t('Whether or not to display print action button?'),
    'display_share_action' => t('Whether or not to display share action button?'),
    'use_distinct_search' => t('Use distinct Solr requests for search and facets?'),
    'handle_synonyms' => t('How to display synonyms?'),
    'handle_suggestions' => t('How to display suggestions?'),
    'use_post_method' => t('Which method should be used to send search form?'),
    // this field is added at load time, comes from xc_search_ui_boosting table, see xc_search_ui_get_boosting_by_ui
    'boost_factors' => t('Query time boosting factors'),
    'online_link_strategy' => t('Which record type to use as the source of online link?'),
    'display_abstract_strategy' => t('When to display the abstract on search result page?'),
  );
}

/**
 * Get the enumerated field values
 * @return (Array)
 */
function xc_search_ui_options() {
  require_once 'xc_search_facet_group.inc';
  require_once 'xc_search_highlighter.inc';
  return array(
    'facet_id' => xc_search_facet_group_list_values(),
    'highlighter_id' => xc_search_highlighter_list_values(),
    'do_prepopulate_facets' => xc_util_get_global_options('true_false'),
    'display_bookmark_action' => xc_util_get_global_options('true_false'),
    'display_email_action' => xc_util_get_global_options('true_false'),
    'display_text_action' => xc_util_get_global_options('true_false'),
    'display_print_action' => xc_util_get_global_options('true_false'),
    'display_share_action' => xc_util_get_global_options('true_false'),
    'use_distinct_search' => xc_util_get_global_options('true_false'),
    'handle_synonyms' => array(
      1 => t('Display synonyms during normal page load'),
      2 => t('Display synonyms after page loaded by Ajax calls'),
      3 => t('Do not display synonyms'),
    ),
    'handle_suggestions' => array(
      1 => t('Display suggestions during normal page load.'),
      2 => t('Display suggestions after page loaded by Ajax calls.'),
      3 => t('Do not display suggestions.'),
    ),
    'use_post_method' => array(
      1 => t('POST method'),
      0 => t('GET method'),
    ),
    'online_link_strategy' => array(
      'holdings' => t('Only from holdings records.'),
      'manifestation' => t('Only from manifestation record.'),
      'holdings_manifestation' => t('First try holdings, and if no link in holdings then try manifestation.'),
      'manifestation_holdings' => t('First try manifestation, and if no link in manifestation then try holdings.'),
    ),
    'display_abstract_strategy' => array(
      'always' => t('Always if abstract element is existing in the record'),
      'if_highlighted' => t('Only if abstract contains search term(s)'),
      'if_film' => t('Only if the document is a movie'),
      'if_film_highlighted' => t('Only if the document is a movie and abstract contains search term(s)'),
    ),
  );
}

/**
 * Show the full display of the record
 *
 * @param $record (Object)
 *   The search UI object
 * @return (String)
 *   The rendered view of search UI record
 */
function xc_search_ui_view($record) {
  // drupal_set_title(check_plain(xc_search_ui_title($record)));
  drupal_set_title(t('%name UI', array('%name' => $record->name)), PASS_THROUGH);
  $rows = array();
  foreach ($record->boost_factors as $id => $factor) {
    if ($factor['solr_field'] != '') {
      $rows[] = array($factor['solr_field'], $factor['factor']);
    }
  }
  $record->boost_factors = theme('table', array('header' => array(t('Solr field'), t('Boosting factor')), 'rows' => $rows));
  return xc_util_view($record, xc_search_ui_fieldset(), xc_search_ui_options());
}

/**
 * List all search UIs
 * @return (String)
 *   The rendered list of search UIs
 */
function xc_search_ui_list() {
  $headers = array(
    t('Name'),
    t('Facet group'),
    t('Highlighter'),
    t('edit'),
    t('delete'),
  );
  $rows = array();
  $sql = 'SELECT ui.*, grp.name AS facet, hl.name AS hlt'
       . ' FROM {xc_search_ui} AS ui'
       . ' LEFT JOIN {xc_search_facet_group} AS grp ON '
       . '   grp.facet_id = ui.facet_id'
       . ' LEFT JOIN {xc_search_highlighter} AS hl ON '
       . '   hl.highlighter_id = ui.highlighter_id';
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      l($data->name, 'admin/xc/search/ui/' . $data->sui_id),
      t('!name facet group',
        array('!name' => l($data->facet,
        'admin/xc/search/facet/group/' . $data->facet_id))),
      t('!name highlighter',
        array('!name' => l($data->hlt,
        'admin/xc/search/highlighter/' . $data->highlighter_id))),
      l(t('edit'), 'admin/xc/search/ui/' . $data->sui_id . '/edit'),
      l(t('delete'), 'admin/xc/search/ui/' . $data->sui_id . '/delete'),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new server
 * @return string The add server page with a form
 */
function xc_search_ui_add() {
  return drupal_get_form('xc_search_ui_form');
}

/**
 * The form to edit search UI record
 * @return (array)
 *   The FAPI array
 */
function xc_search_ui_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_ui');
  drupal_set_title($schema['description']);

  $options = array(
    'omit' => array('sui_id'),
    'select' => xc_search_ui_options(),
    'label' => xc_search_ui_fieldset(),
  );

  if (empty($options['select']['facet_id'])) {
    drupal_set_message(t('There is no facet group defined. Please define it first!'), 'error');
  }

  $form = xc_util_build_autoform($schema, $options);
  $form['#tree'] = TRUE;
  $form['authors_count']['#description'] = t('Possible values: -1 = ALL, 0 = NONE, any positive number = the number of authors to display.');
  $form['authors_count']['#size'] = 2;

  $form['handle_synonyms']['#type'] = 'radios';
  $form['handle_suggestions']['#type'] = 'radios';
  $form['use_post_method']['#type'] = 'radios';
  $form['online_link_strategy']['#type'] = 'radios';
  $form['display_abstract_strategy']['#type'] = 'radios';

  for ($i = 0; $i < 3; $i++) {
    $form['boost_factors'][$i] = xc_search_ui_get_boost_row();
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * The form to modify the search UI record
 * @param $form_state (Array)
 *   The FAPI form_state
 * @param $record (Object)
 *   The search UI record
 * @return (array)
 *   The FAPI form
 */
function xc_search_ui_edit_form($form, &$form_state, $record) {
  $form = xc_search_ui_form($form);
  drupal_set_title(t('Editing %name UI', array('%name' => $record->name)), PASS_THROUGH);
  $form['sui_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->sui_id,
  );

  foreach (xc_search_ui_fieldset() as $name => $label) {
    if ($name != 'boost_factors') {
      $form[$name]['#default_value'] = $record->$name;
    }
  }

  // handling boost factor (boost_factors)
  $i = 0;
  foreach ($record->boost_factors as $id => $boost_factor) {
    if ($boost_factor['solr_field'] != '') {
      if (!isset($form['boost_factors'][$i])) {
        $form['boost_factors'][$i] = xc_search_ui_get_boost_row();
      }
      $form['boost_factors'][$i]['id']['#default_value'] = $id;
      $form['boost_factors'][$i]['solr_field']['#default_value'] = $boost_factor['solr_field'];
      $form['boost_factors'][$i]['factor']['#default_value'] = $boost_factor['factor'];
      $i++;
    }
  }
  if ($i > 0) {
    for ($j = $i; $j < $i + 3; $j++) {
      $form['boost_factors'][$j] = xc_search_ui_get_boost_row();
    }
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
function xc_search_ui_get_boost_row() {
  return array(
    'id' => array(
      '#type' => 'hidden',
    ),
    'solr_field' => array(
      '#type' => 'textfield',
      '#size' => 40,
    ),
    'factor' => array(
      '#type' => 'textfield',
      '#size' => 20,
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_search_ui_edit_form($variables) {
  $form = $variables['form'];
  return theme_xc_search_ui_form($form);
}

/**
 * Theming the search UI form
 *
 * @param unknown_type $form
 */
function theme_xc_search_ui_form($variables) {
  $form = $variables['form'];
  foreach (xc_search_ui_fieldset() as $key => $value) {
    if ($key == 'boost_factors') {
      $rows = array();
      foreach (element_children($form['boost_factors']) as $i) {
        $row = array();
        foreach (element_children($form['boost_factors'][$i]) as $field) {
          if ($form['boost_factors'][$i][$field]['#type'] != 'hidden') {
            $row[] = drupal_render($form['boost_factors'][$i][$field]);
          }
        }
        $rows[] = $row;
      }
      $output .= theme('table', array('header' => array(t('Solr field'), t('Factor')), 'rows' => $rows, 'attributes' => array(), 'caption' => t('Boosting factors')));
    }
    else {
      $output .= drupal_render($form[$key]);
    }
  }

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * The handling of form submission
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function xc_search_ui_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_ui_fieldset() as $name => $label) {
    if ($name != 'boost_factors') {
      $record->$name = $values[$name];
    }
  }
  $ret_val = drupal_write_record('xc_search_ui', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added search UI settings "%name"',
      array('%name' => $record->name)));

    foreach ($values['boost_factors'] as $key => $boost_factor) {
      if ($boost_factor['solr_field'] == '') {
        continue;
      }
      $factor = (object) array(
        'sui_id' => $record->sui_id,
        'solr_field' => $boost_factor['solr_field'],
        'factor' => $boost_factor['factor'],
      );
      if (!empty($boost_factor['id'])) {
        $factor->id = $boost_factor['id'];
        drupal_write_record('xc_search_ui_boosting', $factor, 'id');
      }
      else {
        drupal_write_record('xc_search_ui_boosting', $factor);
      }
    }

    $form_state['redirect'] = 'admin/xc/search/ui/' . $record->sui_id;
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new search UI settings.'));
  }
}

/**
 * Handling of modification/deletion
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function xc_search_ui_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->sui_id      = $values['sui_id'];
  $record->name        = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $sql = 'DELETE FROM {xc_search_ui} WHERE sui_id = %d';
    $result = db_delete('xc_search_ui')
              ->condition('sui_id', $record->sui_id)
              ->execute();
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/ui/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to remove %name.',
        array('%name' => $record->name)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_ui_fieldset() as $name => $label) {
      if ($name != 'boost_factors') {
        $record->$name = $values[$name];
      }
    }

    $result = drupal_write_record('xc_search_ui', $record, 'sui_id');
    if ($result == SAVED_UPDATED) {
      drupal_set_message(t('%name modified', array('%name' => $record->name)));

      foreach ($values['boost_factors'] as $key => $boost_factor) {
        if ($boost_factor['solr_field'] == '' || $boost_factor['factor'] == '') {
          if (!empty($boost_factor['id'])) {
            // TODO Please review the conversion of this statement to the D7 database API syntax.
            /* db_query('DELETE FROM {xc_search_ui_boosting} WHERE id = %d', $boost_factor['id']) */
            db_delete('xc_search_ui_boosting')
              ->condition('id', $boost_factor['id'])
              ->execute();
          }
          continue;
        }

        $factor = (object) array(
          'sui_id' => $record->sui_id,
          'solr_field' => $boost_factor['solr_field'],
          'factor' => $boost_factor['factor'],
        );
        if (!empty($boost_factor['id'])) {
          $factor->id = $boost_factor['id'];
          drupal_write_record('xc_search_ui_boosting', $factor, 'id');
        }
        else {
          drupal_write_record('xc_search_ui_boosting', $factor);
        }
      }

      $form_state['redirect'] = 'admin/xc/search/ui/' . $record->sui_id . '/view';
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
 * Confirmation form for importing default user interface
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_ui_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default user interface?'),
    'admin/xc/search/ui', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default user interface
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_ui_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_ui_restore_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/ui';
}

/**
 * Deletes all search user interface definitions.
 */
function xc_search_ui_delete_all() {
  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }
  $result = db_delete('xc_search_ui')->execute();
  if ($result > 0) { // repository is deleted
    drupal_set_message(t('All search UI were removed'));
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count search UIs.',
      array('%count' => $result)));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_ui_import_defaults() {
  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }
  xc_search_ui_restore_defaults(FALSE);
}

/**
 * Restores search user interface default values.
 *
 * @param $delete_all (Boolean)
 *   If TRUE (default value) it first deletes all records. Otherwise it skips
 *   deletions.
 */
function xc_search_ui_restore_defaults($delete_all = TRUE) {
  drupal_set_message(t('Importing search user interface default values.'));

  if (!user_access(ADMINISTER_XC_SEARCH)) {
    return;
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_ui.csv';
  if (!file_exists($filename)) {
    drupal_set_message(t('The CSV file %filename is inexistent', array('%filename' => $filename)));
  }
  else {
    if ($delete_all) {
      // delete all records
      xc_search_ui_delete_all();

      // set autoincrement to 1
      $sql = 'ALTER TABLE {xc_search_ui} AUTO_INCREMENT = 1';
      $result = db_query($sql);
    }
    xc_util_bulk_insert('xc_search_ui', xc_util_csv2objects($filename));
    variable_set('xc_search_ui_defaults_installed', XC_LOADED);
    drupal_set_message(t('The default search UIs were successfully imported.'));
  }
}
