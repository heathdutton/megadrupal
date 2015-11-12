<?php
/**
 * @file
 * Facet group functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_fieldset() {
  return array(
    'name' => t('Name'),
    'description' => t('Description'),
    'metadata_schema' => t('Metadata schema'),
    'attribute_set_id' => t('Default attribute set'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_view($facet) {
  global $_xc_search_server;

  drupal_set_title(filter_xss(xc_search_facet_group_title($facet)));
  $fields = xc_search_facets_by_group($facet->facet_id);
  $headers = array(t('Property'), t('Value'));
  $rows = array();
  $labels = xc_search_facet_group_fieldset();

  $attribute_set = xc_search_field_facet_properties_load($facet->attribute_set_id);
  $rows = array(
    array($labels['name'], $facet->name),
    array($labels['description'], $facet->description),
    array($labels['metadata_schema'], l($facet->metadata_schema, 'admin/xc/metadata/formats/' . $facet->metadata_schema)),
    array($labels['attribute_set_id'], l($attribute_set->name, 'admin/xc/search/facet/fieldfacetset/' . $facet->attribute_set_id)),
  );

  $output = theme('table', array('header' => array(t('Property'), t('Value')), 'rows' => $rows));

  if (count($fields) == 0) {
    $output .= t('No field connected to this facet group.');
    $output .= ' ';
    $output .= l(t('Please add field!'), 'admin/xc/search/facet/group/' . $facet->facet_id . '/add_field_facet');
  }
  else {
    $params = array(
      'search_type' => 'luke',
      'ft' => 'type',
      'numTerms' => 0,
    );
    $response = $_xc_search_server->search(NULL, 0, 0, $params, TRUE);
    $rows = array();
    foreach ($fields as $field) {
      $attribute_set = xc_search_get_attribute_set($field->type, $field->attribute_set_id);
      $type_path = ($field->type == 'field') ? '' : $field->type;
      $index_facet = xc_index_facet_load_by_name($field->name);
      $solr_field_name = $field->name . $index_facet->type;
      $rows[] = array(
        $field->label,
        $field->name,
        $field->type,
        ($field->is_collapsed ? t('Collapsed') : t('Expanded')),
        ($attribute_set
          ? l($attribute_set->name, 'admin/xc/search/facet/' . $field->type . 'facetset/' . $field->attribute_set_id . '/view')
          : ''
        ),
        (isset($response->fields->$solr_field_name) ? t('Yes') : t('No')),
        l(t('edit'), 'admin/xc/search/facet/' . $type_path . 'field/' . $field->field_id . '/edit') . ', ' .
        l(t('remove'), 'admin/xc/search/facet/field/' . $field->field_id . '/remove'),
      );
    }
    $output .= t('fields');
    $header = array(t('Label'), t('Name'), t('Type'), t('State'), t('Attribute set'), t('Has indexed data?'), t('Actions'));
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
    $output .= l(t('Reorder facets'),
      'admin/xc/search/facet/group/' . $facet->facet_id . '/reorder');
  }

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_reorder_form($form, &$form_state, $facet) {
  $fields = xc_search_facets_by_group($facet->facet_id);
  if (count($fields) == 0) {
    return;
  }
  $rows = array();
  $form = array(
    '#tree' => TRUE,
  );

  $weight_delta = count($fields);
  foreach ($fields as $field) {
    $key = $field->field_id;
    $form['facets'][$key] = array();
    $attribute_set = xc_search_get_attribute_set($field->type, $field->attribute_set_id);
    $type_path = ($field->type == 'field') ? '' : $field->type;

    $form['facets'][$key]['label'] = array('#value' => $field->label);
    $form['facets'][$key]['name'] = array('#value' => $field->name);
    $form['facets'][$key]['type'] = array('#value' => $field->type);

    $form['facets'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $field->weight,
      '#delta' => $weight_delta,
    );

    $form['facets'][$key]['attributes'] = array('#value' => l(
      $attribute_set->name,
      'admin/xc/search/facet/' . $field->type . 'facetset/' . $field->attribute_set_id . '/view'));
  }

  $form['facet_id'] = array(
    '#type' => 'hidden',
    '#value' => $facet->facet_id,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save facets\' order'),
  );

  return $form;
}

/**
 * Save the order of facets
 *
 * @param $form
 * @param $form_state
 */
function xc_search_facet_group_reorder_form_submit($form, &$form_state) {
  $id = $form_state['values']['facet_id'];
  $facets = $form_state['values']['facets'];
  foreach ($facets as $key => $facet) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("UPDATE {xc_search_facet_field} SET weight = %d WHERE name = '%s' AND facet_id = %d", $facet['weight'], $form['facets'][$key]['name']['#value'], $id) */
    db_update('xc_search_facet_field')
      ->fields(array(
          'weight' => $facet['weight'],
        ))
      ->condition('name', $form['facets'][$key]['name']['#value'])
      ->condition('facet_id', $id)
      ->execute();
  }
  drupal_set_message(t('Order of facets has been successfully updated.'));
  $form_state['redirect'] = 'admin/xc/search/facet/group/' . $id . '/view';
}

/**
 * Theming the facet reordering form
 *
 * @param $form
 *
 * @return unknown_type
 */
function theme_xc_search_facet_group_reorder_form($variables) {
  $form = $variables['form'];
  $rows = array();
  drupal_add_tabledrag('facet-sort', 'order', 'sibling', 'sort');

  foreach (element_children($form['facets']) as $key) {
    $facet = &$form['facets'][$key];
    $facet['weight']['#attributes']['class'] = 'sort';

    $cells = array(
      drupal_render($facet['label']),
      drupal_render($facet['name']),
      drupal_render($facet['type']),
      drupal_render($facet['attributes']),
      drupal_render($facet['weight']),
    );

    $rows[] = array(
      'data' => $cells,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array('header' => array(t('Label'), t('Name'), t('Type'), t('Attribute set'), ''), 'rows' => $rows, 'attributes' => array('id' => 'facet-sort')));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_list() {
  $headers = array(t('Name'), t('Description'));
  $rows = array();

  $sql = 'SELECT * FROM {xc_search_facet_group}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      l($data->name, 'admin/xc/search/facet/group/' . $data->facet_id),
      $data->description,
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_list_values() {
  $sql = 'SELECT * FROM {xc_search_facet_group}';
  $result = db_query($sql);
  $rows = array();
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[$data->facet_id] = $data->name;
  }
  return $rows;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_add() {
  drupal_set_title('Create a new facet navigation');
  return drupal_get_form('xc_search_facet_group_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_form($form) {
  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';

  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_facet_group');
  drupal_set_title($schema['description']);

  $options = array(
    'omit' => array('facet_id'),
    'hidden' => array(),
    'label' => xc_search_facet_group_fieldset(),
    'select' => array(
      'metadata_schema' => xc_format_get_schemas(),
      'attribute_set_id' => array(),
    ),
  );
  // attribute sets
  $sql = 'SELECT set_id, name, description FROM {xc_search_field_facet_properties}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $options['select']['attribute_set_id'][$data->set_id] = $data->name;
  }

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
function xc_search_facet_group_edit_form($form, &$form_state, $facet) {

  $form = xc_search_facet_group_form();
  $form['facet_id'] = array(
    '#type' => 'hidden',
    '#value' => $facet->facet_id,
  );
  $form['name']['#default_value']            = $facet->name;
  $form['description']['#default_value']     = $facet->description;
  $form['metadata_schema']['#default_value'] = $facet->metadata_schema;
  $form['attibute_set_id']['#default_value'] = $facet->attibute_set_id;

  $form['submit']['#value']      = t('Save');
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
function xc_search_facet_group_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_facet_group_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  $ret_val = drupal_write_record('xc_search_facet_group', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully added facet group "%name"',
      array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/group/'
      . $record->facet_id . '/view';
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet group.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_facet_group_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->facet_id      = $values['facet_id'];
  $record->name          = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_facet_group')
              ->condition('facet_id', $record->facet_id)
              ->execute();
    // TODO: delete connected fields
    if ($result == 1) { // repository is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count facet group(s).',
        array('%count' => $result)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_facet_group_fieldset() as $name => $label) {
      $record->$name = $values[$name];
    }
    $result = drupal_write_record('xc_search_facet_group', $record, 'facet_id');

    if ($result == SAVED_UPDATED) { // repository is updated
      drupal_set_message(t('%name modified', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/group/'
        . $record->facet_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify facet group.'));
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
}

/**
 * Confirmation form for importing default facet groups
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_facet_group_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default facet groups?'),
    'admin/xc/search/facet/group', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default facet groups
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_facet_group_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_facet_group_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/facet/group';
}

/**
 * Importing default facet groups
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_facet_group_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default facet groups'));

  if ($delete) {
    // delete all records
    $sql = 'DELETE FROM {xc_search_facet_group}';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql) */
    $result = db_delete('xc_search_facet_group')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_facet_group} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_facet_group.csv';
  xc_util_bulk_insert('xc_search_facet_group', xc_util_csv2objects($filename));

  variable_set('xc_search_facet_group_defaults_installed', XC_LOADED);
}
