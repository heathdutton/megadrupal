<?php
/**
 * @file
 * Functions belong to the field/facet mapping
 *
 * The table (xc_index_field_to_facet) fields: ftof_id, field, facet
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_for_edit_load($xc_index_facet_for_edit) {
  static $cache = array();

  if (!isset($cache[$xc_index_facet_for_edit])) {
    $sql = "SELECT * FROM {xc_index_field_to_facet} WHERE facet = '%s'";
    $result = db_query("SELECT * FROM {xc_index_field_to_facet} WHERE facet = :facet", array(':facet' => $xc_index_facet_for_edit));
    $data = new stdClass;
    $data->type = 'FACET';
    $data->facet = array($xc_index_facet_for_edit);
    //while ($record = db_fetch_object($result)) {
    foreach ($result as $record) {
      $data->fields[] = $record->field;
    }
    $cache[$xc_index_facet_for_edit] = $data;
  }

  return $cache[$xc_index_facet_for_edit];
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_field_for_edit_load($xc_index_field_for_edit) {
  static $sql = "SELECT * FROM {xc_index_field_to_facet} WHERE field = '%s'";

  $result = db_query("SELECT * FROM {xc_index_field_to_facet} WHERE field = :field", array(':field' => $xc_index_field_for_edit));
  $data = new stdClass;
  $data->type = 'FIELD';
  $data->field = array($xc_index_field_for_edit);
  //while ($record = db_fetch_object($result)) {
  foreach ($result as $record) {
    $data->facet[] = $record->facet;
  }

  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_load($xc_index_facet_mapping) {
  $sql = 'SELECT * FROM {xc_index_field_to_facet} WHERE ftof_id = %d';
  $result = db_query('SELECT * FROM {xc_index_field_to_facet} WHERE ftof_id = :ftof_id', array(':ftof_id' => $xc_index_facet_mapping));
  $data = db_fetch_object($result);
  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_title($record) {
  return t('!field->!facet mapping', array(
    '!field' => $record->field,
    '!facet' => $record->facet,
  ));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_fieldset() {
  return array(
    'field' => t('field name'),
    'facet' => t('facet name'),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_options($record = NULL) {
  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';

  $fields = xc_format_get_fields();
  $fields_array = array();
  foreach ($fields as $field) {
    $fields_array[$field] = $field;
  }
  $options = array(
    'field' => $fields_array,
    'facet' => xc_index_facet_mapping_get_facets(NULL, TRUE),
  );
  return $options;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_view($record) {
  return xc_util_view($record,
    xc_index_facet_mapping_fieldset()
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_list() {
  $headers = array(t('Field name'), t('Facet alias name'));
  $rows = array();

  $sql = 'SELECT * FROM {xc_index_field_to_facet}';
  $result = db_query($sql);
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    $rows[] = array(
      $data->field,
      l($data->facet, 'admin/xc/index/facet_mapping/' . $data->ftof_id),
    );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_list_by_facets() {
  //drupal_set_title(t('List mappings by facets'));
  $output =  '<p>' . t('List of fields to facet mapping. Each facet contains the content of one or more fields, e.g. creators facet may cover the author, creator, photographer etc. fields. Each fields could be covered by one or more facets, e.g. creator field could be indexed as creators and authors facet, and it can be indexed as creator fields. Facets are special fields in Solr, we index them as phrase or as date, and we use them in displaying in faceted navigation. They helps the users to became familiar with the nature of the content.') . '</p>';

  $output .= '<p>' . t('On the left column you can see the facets cover one or more fields, and on the right column you can see the facets to which the actual field is indexed.') . '</p>';

  $headers = array(t('Facet'), t('Fields'));
  $rows = array();

  $sql = 'SELECT facet.facet_id, ftof.* FROM {xc_index_field_to_facet} as ftof
    LEFT JOIN {xc_index_facet} AS facet ON ftof.facet = facet.name
    ORDER BY ftof.facet';
  $result = db_query($sql);
  $key = '';
  $values = array();
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    if ($key != $data->facet) {
      if ($key != '') {
        $rows[] = array(
          l($key, 'admin/xc/index/facets/' . $id . '/edit'),
          join(', ', $values),
        );
        $values = array();
      }
      $key = $data->facet;
      $id  = $data->facet_id;
    }
    $values[] = $data->field; //, 'admin/xc/index/facet_mapping/field/' . $data->field . '/edit');
  }
  $rows[] = array(
    l($key, 'admin/xc/index/facets/' . $id . '/edit'),
    join(', ', $values),
  );
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_list_by_fields() {
  $output =  '<p>' . t('List of fields to facet mapping. Each facet contains the content of one or more fields, e.g. creators facet may cover the author, creator, photographer etc. fields. Each fields could be covered by one or more facets, e.g. creator field could be indexed as creators and authors facet, and it can be indexed as creator fields. Facets are special fields in Solr, we index them as phrase or as date, and we use them in displaying in faceted navigation. They helps the users to became familiar with the nature of the content.') . '</p>';

  $output .= '<p>' . t('On the left column you can see the fields mapped to one or more facet, and on the right column you can see the facets to which the actual field is indexed.') . '</p>';

  $headers = array(t('Field'), t('Facets'));
  $rows = array();

  $sql = 'SELECT facet.facet_id, ftof.* FROM {xc_index_field_to_facet} as ftof
    LEFT JOIN {xc_index_facet} AS facet ON ftof.facet = facet.name
    ORDER BY ftof.field';
  $result = db_query($sql);
  $field_name = '';
  $facets = array();
  //while ($data = db_fetch_object($result)) {
  foreach ($result as $data) {
    if ($field_name != $data->field) {
      if ($field_name != '') {
        $rows[] = array(
          $field_name, // 'admin/xc/index/facet_mapping/field/' . $field_name. '/edit'),
          join(', ', $facets),
        );
        $facets = array();
      }
      $field_name = $data->field;
    }
    $facets[] = l($data->facet, 'admin/xc/index/facets/' . $data->facet_id . '/edit');
  }
  $rows[] = array(
    $field_name, //'admin/xc/index/facet_mapping/field/' . $field_name. '/edit'),
    join(', ', $facets),
  );
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_add() {
  drupal_set_title('Create a field->facet mapping');
  return drupal_get_form('xc_index_facet_mapping_form');
}

/**
 * FAPI definition for the basic facet mapping form
 *
 * @param $form_state
 * @param $record
 *   The sdtClass containing the fields and facets
 *
 * @return
 *   The form definition
 *
 * @ingroup forms
 * @see xc_index_facet_for_edit_load()
 * @see xc_index_field_for_edit_load()
 * @see xc_index_facet_mapping_form_submit()
 */
function xc_index_facet_mapping_form($form, &$form_state, $record = NULL) {
  $schema = drupal_get_schema_unprocessed('xc_search', 'xc_index_field_to_facet');
  drupal_set_title($schema['description']);
  $options = array(
    'omit' => array('ftof_id'),
    'hidden' => array(),
    'label' => xc_index_facet_mapping_fieldset(),
    'select' => xc_index_facet_mapping_options($record),
  );
  $form = xc_util_build_autoform($schema, $options);
  $form['field']['#multiple'] = TRUE;
  $form['facet']['#multiple'] = TRUE;
  $form['stored'] = array(
    '#type' => 'hidden',
    '#value' => serialize($record),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
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
function xc_index_facet_mapping_edit_form(&$form_state, $record) {
  $form = xc_index_facet_mapping_form($form_state, $record);

  foreach (xc_index_facet_mapping_fieldset() as $name => $label) {
    $form[$name]['#default_value'] = $record->$name;
  }

  $form['submit']['#value'] = t('Modify');
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
function theme_xc_index_facet_mapping_form($variables) {
  $form = $variables['form'];
  $rows[] = array(
    array(
      'data' => drupal_render($form['field']),
      'width' => '50%',
      'valign' => 'top',
    ),
    array(
      'data' => drupal_render($form['facet']),
      'valign' => 'top',
    ),
  );
  $output .= theme('table', array('header' => NULL, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_xc_index_facet_mapping_edit_form($variables) {
  $form = $variables['form'];
  return theme_xc_index_facet_mapping_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = $values['field'];
  $facets = $values['facet'];
  foreach ($facets as $facet) {
    foreach ($fields as $field) {
      // create it only if it doesn't exist
      if (xc_index_facet_mapping_exists($field, $facet)) {
        continue;
      }
      $record = new stdClass();
      $record->field = $field;
      $record->facet = $facet;
      $ret_val = drupal_write_record('xc_index_field_to_facet', $record);
      if ($ret_val == SAVED_NEW) {
        drupal_set_message(t('Successfully added %facet facet for %field!',
          array('%facet' => $record->facet, '%field' => $record->field)));
      }
      else {
        drupal_set_message(t('Unexpected error. Failed to create new field-facet mapping.') . db_error());
      }
    }
  }
  $form_state['redirect'] = 'admin/xc/index/facet_mapping/listfacets';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_edit_form_submit($form, &$form_state) {
  global $active_db;

  $values = $form_state['values'];
  $record = unserialize($values['stored']);
  $fields = $values['field'];
  $facets = $values['facet'];

  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Delete')) {
    foreach ($facets as $facet) {
      foreach ($fields as $field) {
        $result = db_delete('xc_index_field_to_facet')
                  ->condition('facet', $facet)
                  ->condition('field', $field)
                  ->execute();
        if ($result == 1) { // facet mapping deleted
          drupal_set_message(t('%field->%facet facet mapping removed',
            array('%field' => $field, '%facet' => $facet)));
        }
        else {
          drupal_set_message(t('Unexpected error. Deletion removes %count facet mapping(s).',
            array('%count' => $result)));
        }
      }
    }
  }
  elseif ($op == t('Modify')) {
    $deletable_fields = array_diff($record->field, $fields);
    $deletable_facets = array_diff($record->facet, $facets);
    $deletable_pairs = array();
    foreach ($record->facet as $facet) {
      foreach ($deletable_fields as $field) {
        $deletable_pairs[] = array($facet, $field);
      }
    }
    foreach ($deletable_facets as $facet) {
      foreach ($record->field as $field) {
        $deletable_pairs[] = array($facet, $field);
      }
    }
    foreach ($deletable_pairs as $pair) {
      $result = db_delete('xc_index_field_to_facet')
                ->condition('facet', $pair[0])
                ->condition('field', $pair[1])
                ->execute();
      if ($result == 1) { // facet mapping deleted
        drupal_set_message(t('%field->%facet facet mapping removed', array(
          '%field' => $pair[1],
          '%facet' => $pair[0],
        )));
      }
      else {
        drupal_set_message(t('Unexpected error. Deletion removes %count facet mapping(s).',
          array('%count' => $result)));
      }
    }

    foreach ($facets as $facet) {
      foreach ($fields as $field) {
        // create it only if it doesn't exist
        if (xc_index_facet_mapping_exists($field, $facet)) {
          continue;
        }
        $record = new stdClass();
        $record->field = $field;
        $record->facet = $facet;
        $ret_val = drupal_write_record('xc_index_field_to_facet', $record);
        if ($ret_val == SAVED_NEW) {
          drupal_set_message(t('%field->%facet facet mapping created!',
            array('%facet' => $record->facet, '%field' => $record->field)));
        }
        else {
          drupal_set_message(t('Unexpected error. Failed to create new facet mapping. %error',
            array('%error' => mysql_error($active_db))));
        }
      }
    }
  }
  $form_state['redirect'] = 'admin/xc/index/facet_mapping/listfacets';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_restore_defaults_form() {
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => t('Are you sure, that you would like to restore default values?') . '<br />',
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
function xc_index_facet_mapping_restore_defaults_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Yes')) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query('DELETE FROM {xc_index_field_to_facet}') */
    $result = db_delete('xc_index_field_to_facet')->execute();
    if ($result > 0) {
      drupal_set_message(t('All facets were removed'));
      $path = drupal_get_path('module', 'xc_index') . '/import/';
      $table = 'xc_index_field_to_facet';
      xc_util_bulk_insert($table, xc_util_csv2objects($path . $table . '.csv'));
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count facet mapping(s).',
        array('%count' => $result)));
    }
  }
  $form_state['redirect'] = 'admin/xc/index/facet_mapping/listfacets';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_facet_mapping_exists($field, $facet) {
  $sql = 'SELECT COUNT(*) FROM {xc_index_field_to_facet}
            WHERE field = \'%s\'
              AND facet = \'%s\'';
  $count = db_query('SELECT COUNT(*) FROM {xc_index_field_to_facet}
            WHERE field = :field
              AND facet = :facet', array(':field' => $field, ':facet' => $facet))->fetchField();
  return ($count == 0 ? FALSE : TRUE);
}

