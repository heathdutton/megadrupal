<?php
/**
 * @file
 * Date facet properties functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_fieldset() {
  return array(
    'name' => t('Name'),
    'description' => t('Description'),
    'start' => t('Start'),
    'end' => t('End'),
    'gap' => t('Gap'),
    'hardend' => t('Hardend'),
    'other' => t('Other'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_additional_fieldset() {
  return array(
    'query' => t('Query'),
    'gap' => t('Gap'),
    'label' => t('Label'),
    'position' => t('Position'),
    'weight' => t('Weight'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_options() {
  return array(
    'other' => array(
      'before' => t('before'),
      'after' => t('after'),
      'between' => t('between'),
      'none' => t('none'),
      'all' => t('all'),
    ),
    'hardend' => xc_util_get_global_options('true_false'),
    'position' => array(
      'before' => t('before'),
      'after' => t('after'),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_view($record) {
  drupal_set_title($record->name);
  $output = xc_util_view($record,
    xc_search_date_facet_properties_fieldset(),
    xc_search_date_facet_properties_options(),
    array('caption' => t('Main date facet'))
  );
  $additionals = xc_search_date_facet_properties_additionals_get_by_pid($record->set_id);
  if (!empty($additionals)) {
    foreach ($additionals as $additional) {
      $rows[] = array(
        $additional->query,
        $additional->gap,
        $additional->label,
        $additional->position,
      );
    }
    $labels = xc_search_date_facet_properties_additional_fieldset();
    $output .= theme('table', array('header' => array($labels['query'], $labels['gap'], $labels['label'], $labels['position']), 'rows' => $rows, 'attributes' => NULL, 'caption' => 'Additional date facet terms'));
  }

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_list() {

  $headers = array(t('Name'), t('Description'));
  $rows = array();

  $sql = 'SELECT set_id, name, description FROM {xc_search_date_facet_properties}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  //TODO from Tom, need to check the result. Could use foreach to loop for through the result.
  while ($data = $result->fetchObject()) {
    $rows[] = array(
      l($data->name, 'admin/xc/search/facet/datefacetset/' . $data->set_id),
      $data->description
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add a new server
 * @return string The add server page with a form
 */
function xc_search_date_facet_properties_add() {
  drupal_set_title('Facet attribute set');
  return drupal_get_form('xc_search_date_facet_properties_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_form($form) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_search_date_facet_properties');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('set_id'),
    'select' => xc_search_date_facet_properties_options(),
    'label' => xc_search_date_facet_properties_fieldset(),
  );
  $form = xc_util_build_autoform($schema, $options);
  $form['#tree'] = TRUE;

  for ($i = 0; $i < 4; $i++) {
    $form['additionals'][$i] = array(
      'id' => array(
        '#type' => 'hidden',
        '#value' => $object['id'],
      ),
      'query' => array(
        '#type' => 'textfield',
        '#title' => t('Initial query'),
        '#size' => 40,
        '#default_value' => $object['foo'],
        '#description' => t('The initial date query'),
      ),
      'gap' => array(
        '#type' => 'textfield',
        '#title' => t('Gap'),
        '#size' => 40,
        '#default_value' => $object['foo'],
        '#description' => t('The gap between dates if the user clicks on this facet term. Examples: +10YEARS, +5MONTHS, +3WEEK, +5DAYS, +1DAY'),
      ),
      'label' => array(
        '#type' => 'textfield',
        '#title' => t('Label'),
        '#size' => 40,
        '#default_value' => $object['foo'],
        '#description' => t('Date label to show in facet block'),
      ),
      'position' => array(
        '#type' => 'select',
        '#title' => t('Position'),
        '#options' => $options['select']['position'],
        '#default_value' => $object['foo'],
        '#description' => t('Relative to main dates'),
      ),
    );
  }

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
function xc_search_date_facet_properties_edit_form($form, &$form_state, $set) {
  $form = xc_search_date_facet_properties_form();
  $form['set_id'] = array(
    '#type' => 'hidden',
    '#value' => $set->set_id,
  );

  foreach (xc_search_date_facet_properties_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $set->$name;
  }
  // fill additionals value
  $additionals = xc_search_date_facet_properties_additionals_get_by_pid($set->set_id);
  if (!empty($additionals)) {
    $i = 0;
    foreach ($additionals as $additional) {
      $form['additionals'][$i]['id']['#value'] = $additional->id;
      foreach (xc_search_date_facet_properties_additional_fieldset()
           as $name => $label) {
        $form['additionals'][$i][$name]['#default_value'] = $additional->$name;
      }
      $i++;
    }
  }
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
function theme_xc_search_date_facet_properties_form($variables) {
  $form = $variables['form'];
  $output = drupal_render($form['name']);
  $output .= drupal_render($form['description']);
  $rows[] = array(drupal_render($form['start']), drupal_render($form['end']));
  $rows[] = array(
    'data' => array(
      drupal_render($form['gap']),
      drupal_render($form['hardend']) . drupal_render($form['other']),
    ),
    'valign' => 'top',
  );
  $output .= theme('table', array('header' => NULL, 'rows' => $rows, 'attributes' => array(), 'caption' => t('Main dates')));
  $rows = array();
  foreach (element_children($form['additionals']) as $key) {
    $item = &$form['additionals'][$key];
    if (count($rows) == 0) {
      $header = array(
        $item['query']['#title'],
        $item['gap']['#title'],
        $item['label']['#title'],
        $item['position']['#title'],
      );
      $rows[] = array(
        'data' => array(
          $item['query']['#description'],
          $item['gap']['#description'],
          $item['label']['#description'],
          $item['position']['#description'],
        ),
        'valign' => 'top',
      );
    }
    unset($item['query']['#description'], $item['query']['#title'],
     $item['gap']['#description'], $item['gap']['#title'],
     $item['label']['#description'], $item['label']['#title'],
     $item['position']['#description'], $item['position']['#title']);
    $rows[] = array(
      drupal_render($item['query']),
      drupal_render($item['gap']),
      drupal_render($item['label']),
      drupal_render($item['position']),
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array(), 'caption' => t('Additional dates')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_search_date_facet_properties_edit_form($variables) {
  $form = $variables['form'];
  return theme('xc_search_date_facet_properties_form', array('form' => $form));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $record = new stdClass();
  foreach (xc_search_date_facet_properties_fieldset() as $name => $label) {
    if (isset($values[$name])) {
      $record->$name = $values[$name];
    }
  }

  $ret_val = drupal_write_record('xc_search_date_facet_properties', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Thank you for adding date facet settings "%name"',
      array('%name' => $record->name)));
    $form_state['redirect'] = 'admin/xc/search/facet/datefacetset/' . $record->set_id;
    foreach ($values['additionals'] as $key => $item) {
      if (!empty($item['query'])) {
        $item += array(
          'weight' => $key,
          'pid' => $record->set_id,
        );
        $additional = (Object) $item;
        drupal_write_record('xc_search_date_facet_properties_additional', $additional);
      }
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet settings.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_edit_form_submit($form, &$form_state) {
  $values = $form_state['clicked_button']['#post'];
  $record = new stdClass();
  $record->set_id = $values['set_id'];
  $record->name   = $values['name'];

  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $result = db_delete('xc_search_date_facet_properties')
              ->condition('set_id', $record->set_id)
              ->execute();
    if ($result == 1) { // date facet property is deleted
      drupal_set_message(t('%name removed', array('%name' => $record->name)));
      $form_state['redirect'] = 'admin/xc/search/facet/datefacetset/list';
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count date facet properties.',
        array('%count' => $result)));
    }
  }
  elseif ($form_state['clicked_button']['#value'] == t('Save')) {
    foreach (xc_search_date_facet_properties_fieldset() as $name => $label) {
      if (isset($values[$name])) {
        $record->$name = $values[$name];
      }
    }

    $result = drupal_write_record('xc_search_date_facet_properties', $record,
      'set_id');
    if ($result == SAVED_UPDATED) { // repository is updated
      drupal_set_message(t('%name modified', array('%name' => $record->name)));
      foreach ($values['additionals'] as $key => $item) {
        if (!empty($item['query'])) {
          $item += array(
            'weight' => $key,
            'pid' => $record->set_id,
          );
          $additional = (Object) $item;
          if (!empty($additional->id)) {
            $result = drupal_write_record('xc_search_date_facet_properties_additional',
              $additional, 'id');
          }
          else {
            $result = drupal_write_record('xc_search_date_facet_properties_additional', $additional);
          }
        }
        elseif (!empty($item['id'])) {
          $sql = 'DELETE FROM {xc_search_date_facet_properties_additional} WHERE id = %d';
          // TODO Please review the conversion of this statement to the D7 database API syntax.
          /* db_query($sql, $item['id']) */
          $result = db_delete('xc_search_date_facet_properties_additional')
                      ->condition('id', $item['id'])
                      ->execute();
        }
      }
      $form_state['redirect'] = 'admin/xc/search/facet/datefacetset/' . $record->set_id . '/view';
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to modify facet attribute set.'));
    }

  }
  else {
    drupal_set_message(t('Unexpected error. Something else happened.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_test_form($form, $form_state, $set) {
  $form = array(
    'set_id' => array(
      '#type' => 'value',
      '#value' => $set->set_id,
    ),
    'query' => array(
      '#type' => 'textfield',
      '#title' => t('Initial query'),
      '#size' => 40,
      '#default_value' => $form_state['input']['query'],
      '#description' => t('The query phrase to search against.'),
    ),
    'field' => array(
      '#type' => 'textfield',
      '#title' => t('field'),
      '#size' => 40,
      '#default_value' => $form_state['input']['field'],
      '#description' => t('Please enter a field name.'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    ),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_test_form_submit($form, &$form_state) {
  $query = $form_state['clicked_button']['#post']['query'];
  $field = $form_state['clicked_button']['#post']['field'];
  $set_id = $form_state['values']['set_id'];
  $form_state['redirect'] = 'admin/xc/search/facet/datefacetset/' . $set_id . '/test/'
    . urlencode($query) . '||' . urlencode($field);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_test_result($set, $params) {
  global $_xc_search_server;

  list($query, $field) = explode('||', $params, 2);
  $params = array(
    'facet' => 'true',
    'facet.mincount' => 1,
  );

  // get set's parameters
  include_once drupal_get_path('module', 'xc_search') . '/searching/xc_search_properties2solr_params.inc';
  $params += xc_search_date_facet_properties2solr_params($set, $field);

  // get the additional query facet parameter's
  $additionals = xc_search_date_facet_properties_additionals_get_by_pid($set->set_id);
  $display_map = array();
  foreach ($additionals as $additional) {
    $query = $field . ':' . $additional->query;
    $query_map[$query] = $additional;
    $display_map[$additional->position][$additional->weight] = $query;
    $params['facet.query'][] = $query;
  }

  // request Solr
  $response = $_xc_search_server->search($query, 0, 0, $params, FALSE);

  ksort($display_map['before']);
  foreach ($display_map['before'] as $weight => $query) {
    $items[] = $query_map[$query]->label . ': ' . $response->facet_counts->facet_queries->$query;
  }

  foreach ($response->facet_counts->facet_dates->$field as $term => $count) {
    if ($count > 0 && preg_match('/^\d/', $term)) {
      $items[] = $term . ': ' . $count;
    }
  }

  ksort($display_map['after']);
  foreach ($display_map['after'] as $weight => $query) {
    $items[] = $query_map[$query]->label . ': ' . $response->facet_counts->facet_queries->$query;
  }
  $output = theme('item_list', array('items' => $items));
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_search_date_facet_properties_additionals_get_by_pid($pid) {
  $sql = 'SELECT * FROM {xc_search_date_facet_properties_additional}
          WHERE pid = %d
          ORDER BY weight';
  $result = db_query('SELECT * FROM {xc_search_date_facet_properties_additional}
          WHERE pid = :pid
          ORDER BY weight', array(':pid' => $pid));
  $rows = array();
  //while ($data = db_fetch_object($result)) {
  while ($data = $result->fetchObject()) {
    $rows[] = $data;
  }
  return $rows;
}

/**
 * Confirmation form for importing default date facet properties
 *
 * @param $form_state (array)
 *   The FAPI form_state value object
 */
function xc_search_date_facet_properties_import_defaults_form($form, &$form_state) {
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
    t('Are you sure, that you would like to import default date facet properties?'),
    'admin/xc/search/facet/datefacetset', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Import'),
    t('Cancel')
  );
}

/**
 * Handling submission for importing default date facet properties
 * @param $form (Array)
 *   The FAPI form object
 * @param $form_state
 *   The FAPI form_state value object
 */
function xc_search_date_facet_properties_import_defaults_form_submit($form, &$form_state) {
  $delete = (boolean) $form_state['values']['delete'];
  xc_search_date_facet_properties_import_defaults($delete);
  $form_state['redirect'] = 'admin/xc/search/facet/datefacetset';
}

/**
 * Importing default date facet properties
 *
 * @param $delete (Boolean)
 *   Whether or not delete the existing values (default: FALSE)
 */
function xc_search_date_facet_properties_import_defaults($delete = FALSE) {
  drupal_set_message(t('Importing default date facet properties'));

  if ($delete) {
    // delete all records
    $sql = 'DELETE FROM {xc_search_date_facet_properties}';
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql) */
    $result = db_delete('xc_search_date_facet_properties')
                ->execute();

    // set autoincrement to 1
    $sql = 'ALTER TABLE {xc_search_date_facet_properties} AUTO_INCREMENT = 1';
    $result = db_query($sql);
  }

  $filename = drupal_get_path('module', 'xc_search') . '/import/xc_search_date_facet_properties.csv';
  xc_util_bulk_insert('xc_search_date_facet_properties', xc_util_csv2objects($filename));

  variable_set('xc_search_date_facet_properties_defaults_installed', XC_LOADED);
}
