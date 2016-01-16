<?php
/**
 * @file
 * All functions to create, modify and delete predefined facets (xc_index_facet
 * table). Fields are: facet_id, name, label, type (= suffix for a Solr
 * dynamic field)
 * Another important table is the xc_index_field_to_facet table, which maps
 * schema fields to facet names. Its fields are: ftof_id (identifier), field
 * (the schema field name), facet (the name of facet).
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_fieldset() {
  return array(
    'name' => t('Machine name'),
    'label' => t('Display name'),
    'type' => t('Solr type'),
    'is_conditional' => t('Conditional facet?'),
    'conditions' => t('Conditions'),
    'enabled' => t('Enabled?'),
  );
}

/**
 * Returns option lists for the facet form.
 */
function xc_index_facet_options($record = NULL) {
  module_load_include('inc', 'xc_index', 'includes/xc_index_attribute');
  module_load_include('inc', 'xc_metadata', 'includes/includes/xc_metadata.format');

  // $fields = xc_format_get_fields();
  $fields = xc_format_get_fields() + xc_index_attribute_get_solr_fields();
  ksort($fields);

  $fields_array = array();
  foreach ($fields as $field => $display) {
    $fields_array[$field] = $field;
  }

  $options = array(
    'fields' => $fields_array,
    'type' => xc_solr_field_type_get_types(),
    'is_conditional' => array(
      1 => t('true'),
      0 => t('false'),
    ),
    'enabled' => array(
      1 => t('true'),
      0 => t('false'),
    ),
  );
  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_view($record) {
  $fields = array();
  $result = db_query('SELECT field FROM {xc_index_field_to_facet} WHERE facet = :facet ORDER BY field', array(':facet' => $record->name,));
  //while ($data = db_fetch_object($result)) {
  foreach($result as $data) {
    $fields[] = $data->field;
  }

  $record->aggregated_fields = join(', ', $fields);
  $fieldset = array_merge_recursive(xc_index_facet_fieldset(), array('aggregated_fields' => t('Fields')));

  return xc_util_view($record,
    $fieldset,
    xc_index_facet_options(),
    array('filters' => array('conditions' => array(array('highlight_string', TRUE))))
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_list() {
  $output = '<p>' . theme('image', array('path' =>
    'http://wiki.apache.org/moin_static184/modernized/img/alert.png', 'width' => '', 'height' => '', 'alt' =>
    t('Alert!'), 'title' => t('Alert!'), 'attributes' => NULL)) . ' ' . t('Alert! Advanced settings! Modify these settings very carefully, and only after you have read the manual!') . '</p>';
  $output .= '<p>' . t('The list of predefined facets. These facets should be defined before indexing. Each facet contain the content of one or more fields. We call the facet-field pairs to mapping. You can see the mappings !grouped_by_facets or !grouped_by_fields. You can restore default values comes with the module with click on !restore_defaults',
    array(
    '!grouped_by_facets' => l(t('grouped by facets'), 'admin/xc/index/facets/listfacets'),
    '!grouped_by_fields' => l(t('grouped by fields'), 'admin/xc/index/facets/listfields'),
    '!restore_defaults' => l(t('restore defaults'),   'admin/xc/index/facets/restore_defaults'),
  )) . '</p>';

  $types = xc_solr_field_type_get_all();
  $sql = 'SELECT * FROM {xc_index_facet} ORDER BY name';
  $result = db_query($sql);
  //while ($record = db_fetch_object($result)) {
  foreach($result as $record) {
    $enabled = !isset($record->enabled) || $record->enabled == 1;
    $rows[] = array(
      'data' => array(
        $record->label,
        $record->name,
        l($types[$record->type]->label,
          'admin/xc/solr/field_type/' . $types[$record->type]->type_id),
        ($record->is_conditional ? t('Yes') : t('False')),
        l(t('view'),    'admin/xc/index/facets/' . $record->facet_id . '/view'),
        l(t('edit'),    'admin/xc/index/facets/' . $record->facet_id . '/edit'),
        l(t('delete'), 'admin/xc/index/facets/' . $record->facet_id . '/delete'),
        l(($enabled ? t('disable') : t('enable')),
          'admin/xc/index/facets/' . $record->facet_id
          . ($enabled ? '/disable' : '/enable')),
      ),
      'style' => ((!isset($record->enabled) || $record->enabled == 1) ? '' : 'color:#ccc'),
    );
  }
  $labels = xc_index_facet_fieldset();
  $headers = array(
    $labels['label'],
    $labels['name'],
    $labels['type'],
    $labels['is_conditional'],
    t('view'),
    t('edit'),
    t('delete'),
    t('enable'),
  );
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
 * Get the conditional facets.
 *
 * @param $only_fulfilled_ones (Boolean)
 *   If TRUE returns only those facets, of which the 'conditions' field is fulfilled.
 *   Default is false (and thus returns all).
 *
 * @return (Array)
 *   Array of facets records, which are conditional ones. Properties:
 *   - name: machine name
 *   - label: display name
 *   - type: Solr type
 *   - fn: the function to run
 *   - is_conditional: whether the facet is a conditional one (should be always 1)
 *   - enabled: whether the facet is enabled to index (should be always 1)
 */
function xc_index_facet_list_conditionals($only_fulfilled_ones = FALSE) {
  static $facets;

  if (!isset($facets)) {
    $facets = array();
    $result = db_query('SELECT * FROM {xc_index_facet} WHERE is_conditional = :is_conditional AND enabled = :enabled', array(':is_conditional' => 1, ':enabled' => 1));
    foreach($result as $record) {
      if ($record->conditions == '') {
        if ($only_fulfilled_ones) {
          continue;
        }
        else {
          $record->fn = FALSE;
        }
      }
      else {
        $conditions = preg_replace('/(<\?php|\?>)/', '', $record->conditions);
        // $record->fn = create_function('$doc', $conditions);
        $record->fn = create_function('&$document,$metadata,$fields', $conditions);
      }
      $facets[] = $record;
    }
  }

  return $facets;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_add() {
  drupal_set_title('Create a new facet');
  return drupal_get_form('xc_index_facet_form');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_form($form, &$form_state, $record = NULL) {
  $schema = drupal_get_schema_unprocessed('xc_index', 'xc_index_facet');
  drupal_set_title($schema['description']);

  $options = array(
    'omit' => array('facet_id'),
    'hidden' => array(),
    'label' => xc_index_facet_fieldset(),
    'select' => xc_index_facet_options($record),
  );
  $form = xc_util_build_autoform($schema, $options);
  $form['is_conditional']['#type'] = 'radios';
  $form['is_conditional']['#post_render'] = array('xc_index_remove_div_around_is_conditional');

  $form['conditions']['#prefix'] = '<div id="conditions" style="display:none;">';
  $form['conditions']['#suffix'] = '</div>';
  $form['conditions']['#rows'] = 10;

  $form['fields'] = array(
    '#title' => t('Fields'),
    '#type' => 'select',
    '#options' => $options['select']['fields'],
    '#multiple' => TRUE,
    '#attributes' => array('size' => 18),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  drupal_add_js(drupal_get_path('module', 'xc_index') . '/indexing/facet.js');

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_remove_div_around_is_conditional($content, $elements) {
  $content = preg_replace(
    '/<div class="form-item" id="edit-is-conditional-\d+-wrapper">/', '',
    $content
  );
  $content = preg_replace('/<\/label>\s*<\/div>/', '', $content);
  return $content;
}

/**
 * Creating a two column layout for the adding/modifying facet form
 * @param $form Input 'raw' form structure
 * @return The layout
 */
function theme_xc_index_facet_form($variables) {
  $form = $variables['form'];
  $rows[] = array(
    array(
      'data' => drupal_render($form['name'])
              . drupal_render($form['label'])
              . drupal_render($form['type'])
              . drupal_render($form['is_conditional']),
      'width' => '50%',
      'valign' => 'top',
    ),
    array(
      'data' => drupal_render($form['fields']),
      'valign' => 'top',
    ),
  );
  $rows[] = array(array(
      'data' => drupal_render($form['conditions']),
      'colspan' => 2,
    ));
  $output = theme('table', array('header' => NULL, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_edit_form($form, &$form_state, $record = NULL) {
  include_once DRUPAL_ROOT . '/' . 'xc_index_facet_mapping.inc';

  $mapping = xc_index_facet_for_edit_load($record->name);
  $fields = $mapping->fields;
  $form = xc_index_facet_form($form_state, $record);
  drupal_set_title(t('Edit %facet facet', array('%facet' => $record->label)), PASS_THROUGH);
  $form['facet_id'] = array(
    '#type' => 'hidden',
    '#value' => $record->facet_id,
  );
  $form['original_fields'] = array(
    '#type' => 'hidden',
    '#value' => serialize($fields),
  );
  foreach (xc_index_facet_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $record->$name;
  }
  $form['fields']['#default_value'] = $fields;

  $form['submit']['#value'] = t('Modify');
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_index_facet_edit_form($variables) {
  $form = $variables['form'];
  return theme_xc_index_facet_form($form);
}

/**
 * Submit form
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function xc_index_facet_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = $values['fields'];

  // prepare data
  $record = new stdClass();
  foreach (xc_index_facet_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }

  // save new facet
  $ret_val = drupal_write_record('xc_index_facet', $record);
  if ($ret_val == SAVED_NEW) {
    drupal_set_message(t('Successfully created %name facet.', array('%name' => $record->label)));

    // save new field mapping to facet
    foreach ($fields as $field) {
      include_once DRUPAL_ROOT . '/' . 'xc_index_facet_mapping.inc';
      // create it only if it doesn't exist
      // TODO: something is missing here: where comes $facet???
      if (xc_index_facet_mapping_exists($field, $facet)) {
        continue;
      }
      $mapping_record = new stdClass();
      $mapping_record->field = $field;
      $mapping_record->facet = $record->name;
      $ret_val = drupal_write_record('xc_index_field_to_facet', $mapping_record);
      if ($ret_val == SAVED_NEW) {
        drupal_set_message(t('Successfully added %field field for %facet facet.',
          array('%facet' => $record->label, '%field' => $mapping_record->field)));
      }
      else {
        drupal_set_message(t('Unexpected error. Failed to create new field-facet mapping.') . db_error());
      }
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to create new facet.'));
  }
  variable_del('xc_solr_fields2index_list');
  $form_state['redirect'] = 'admin/xc/index/facets';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = $values['fields'];
  $original_fields = unserialize($values['original_fields']);
  if (!is_array($original_fields)) {
    $original_fields = array();
  }

  // prepare data
  $record = new stdClass();
  $record->facet_id = $values['facet_id'];
  foreach (xc_index_facet_fieldset() as $name => $label) {
    $record->$name = $values[$name];
  }
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  // $form_state['clicked_button']['#value']

  // delete
  if ($op == t('Delete')) {
    $result = db_delete('xc_index_facet')
              ->condition('facet_id', $record->facet_id)
              ->execute();
    if ($result == 1) { // facet is deleted
      drupal_set_message(t('Facet %name removed', array('%name' => $record->label)));
      // delete mappings
      $result = db_delete('xc_index_field_to_facet')
                ->condition('facet', $record->name)
                ->execute();
      if ($result == 1) { // facet is deleted
        drupal_set_message(t('All fields for %facet facet removed',
          array('%facet' => $record->label)));
      }
      else {
        drupal_set_message(t('Unexpected error. Deletion removed %count facet.',
          array('%count' => $result)));
      }
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count facet(s).',
        array('%count' => $result)));
    }
  }
  elseif ($op == t('Modify')) {
    $ret_val = drupal_write_record('xc_index_facet', $record, 'facet_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated %name facet', array('%name' => $record->label)));
      // TODO: update fields
      $deletables = array_diff($original_fields, $fields);
      $new_fields = array_diff($fields, $original_fields);
      foreach ($deletables as $field) {
        $result = db_delete('xc_index_field_to_facet')
                  ->condition('facet', $record->name)
                  ->condition('field', $field)
                  ->execute();
        if ($result == 1) { // facet is deleted
          drupal_set_message(
            t('Successfully removed %field field from %facet facet',
            array(
            '%facet' => $record->label,
            '%field' => $field,
          )));
        }
        else {
          drupal_set_message(t('Unexpected error. Deletion removed %count facet mapping(s).',
            array('%count' => $result)));
        }
      }
      foreach ($new_fields as $field) {
        $mapping_record = new stdClass();
        $mapping_record->field = $field;
        $mapping_record->facet = $record->name;
        $ret_val = drupal_write_record('xc_index_field_to_facet', $mapping_record);
        if ($ret_val == SAVED_NEW) {
          drupal_set_message(t('Successfully added %field field for %facet facet.',
            array('%facet' => $record->label, '%field' => $mapping_record->field)));
        }
        else {
          drupal_set_message(
            t('Unexpected error. Failed to create new field-facet mapping: %error.',
              array('%error' => db_error())),
            'error'
          );
        }
      }
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to create new facet.'));
    }
  }
  variable_set('xc_index_definition_modification_time', time());
  $form_state['redirect'] = 'admin/xc/index/facets';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_restore_defaults_form($form, &$form_state) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => t('Are you sure, that you would like to restore default values? You will lost all your settings and the module\'s default values will be reset.') . '<br />',
    '#value' => t('Yes'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_restore_defaults_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Yes')) {
    $result = db_delete('xc_index_facet')->execute();
    if ($result > 0) {
      drupal_set_message(t('All facets were removed'));
      $path = drupal_get_path('module', 'xc_index') . '/import/';
      $filename = $path . 'xc_index_facet.xml';
      xc_util_bulk_insert('xc_index_facet', xc_util_xmldump_to_records($filename));

      $result = db_delete('xc_index_field_to_facet')->execute();
      if ($result > 0) {
        drupal_set_message(t('All facet mapping were removed'));
        // insert the field to facet table records
        $table = 'xc_index_field_to_facet';
        xc_util_bulk_insert($table, xc_util_csv2objects($path . $table . '.csv'));
        // empty cache
        variable_del('xc_solr_fields2index_list');
      }
      else {
        drupal_set_message(t('Unexpected error. Deletion removed %count facet mappings.',
          array('%count' => $result)));
      }
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count facet.',
        array('%count' => $result)));
    }
  }
  variable_set('xc_index_definition_modification_time', time());
  $form_state['redirect'] = 'admin/xc/index/facets';
}

/**
 * A confirmation form for /enable and /disable facet
 *
 * @param $form_state
 * @param $facet (Object)
 *   The index facet object record
 * @param $facet_state (String)
 *   'enable' to enable the facet or 'disable' to disable the facet. Only enabled
 *   facets could be indexed.
 */
function xc_index_facet_enable_form($form, &$form_state, $facet, $facet_state) {
  // make sure about facet state's value
  $facet_state = empty($facet_state)
    ? 'enable'
    : (in_array($facet_state, array('enable', 'disable'))
      ? $facet_state
      : 'enable');

  $form['facet_id'] = array(
    '#type' => 'value',
    '#value' => $facet->facet_id,
  );
  $form['facet_state'] = array(
    '#type' => 'value',
    '#value' => $facet_state,
  );

  $action = t('!state facet \'!name\'', array(
    '!state' => $facet_state,
    '!name' => $facet->label,
  ));

  return confirm_form(
    array($form),
    t('Are you sure, that you would like to %state facet \'%name\'?',
      array('%state' => $facet_state, '%name' => $facet->label)),
    'admin/xc/index/facets', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    $action,
    t('Cancel')
  );
}

/**
 * Handling form submission for /enable and /disable facet
 * @param $form (Array)
 *   The form object
 * @param $form_state (Array)
 *   The state of form
 */
function xc_index_facet_enable_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $facet_state = 'enable';

  if (!empty($values['facet_state']) && in_array($values['facet_state'], array('enable', 'disable'))) {
    $facet_state = $values['facet_state'];
  }

  $facet = xc_index_facet_load($values['facet_id']);
  if (!empty($facet)) {
    $facet->enabled = ($facet_state == 'enable') ? 1 : 0;
    $ret_val = drupal_write_record('xc_index_facet', $facet, 'facet_id');
    if ($ret_val == SAVED_UPDATED) {
      drupal_set_message(t('Successfully updated %name facet', array('%name' => $facet->label)));
    }
    else {
      drupal_set_message(t('Unexpected error. Failed to %state facet.',
        array('%state' => $facet_state)));
    }
  }

  variable_set('xc_index_definition_modification_time', time());
  $form_state['redirect'] = 'admin/xc/index/facets';
}
