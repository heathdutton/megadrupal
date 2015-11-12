<?php
/**
 * @file
 * Functions for manipulating xc_index_field_type table.
 * Fields are: field_type_id, metadata_schema, field, type (= the suffix for that Solr field)
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_field_type_view() {
  require_once drupal_get_path('module', 'xc_metadata') . '/includes/xc_metadata.format.inc';

  $output = t('Select a metadata schema to list its fields, and the connected Solr data types.');
  $schemas = xc_format_get_schemas();

  $items = array();
  foreach ($schemas as $key => $value) {
    $items[] = l($value, 'admin/xc/index/field_type/' . $key);
  }
  $output .= theme('item_list', array('items' => $items));
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_field_type_get_field_types() {
  $sql = 'SELECT * FROM {xc_index_field_type}';
  $result = db_query($sql);
  $values = array();
  foreach ($result as $data) {
    $values[$data->field] = array(
      'id' => $data->field_type_id,
      'type' => $data->type,
    );
  }
  return $values;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_field_type_form($form, &$form_state, $metadata_schema) {
  module_load_include('inc', 'xc_index', 'includes/xc_index_attribute');
  $schema_name = $metadata_schema['metadata_schema'];

  drupal_set_title(t('Map fields of %schema schema to Solr dynamic types',
    array('%schema' => $schema_name)), PASS_THROUGH);

  $fields = $metadata_schema['fields']
          + xc_index_attribute_get_solr_fields($schema_name);
  ksort($fields);

  $existing_field_types = xc_index_field_type_get_all_by_schema($schema_name);
  $types = xc_solr_field_type_get_all();
  $form['metadata_schema'] = array(
    '#type' => 'hidden',
    '#value' => $schema_name,
  );

  foreach ($fields as $field => $label) {
    $default_types = array('_t');
    if (isset($existing_field_types[$field])) {
      $default_types = $existing_field_types[$field];
    }
    foreach ($types as $suffix => $type) {
      $key = base64_encode('type---' . $field . '---' . $suffix);
      $form['types'][$field][$suffix][$key] = array(
        '#title' => check_plain($suffix),
        '#type' => 'checkbox',
        '#return_value' => $suffix,
      );
      if (in_array($suffix, $default_types)) {
        $form['types'][$field][$suffix][$key]['#attributes']['checked'] = 'checked';
      }
    }
    $form['types'][$field]['#TREE'] = TRUE;
  }
  $form['types']['#TREE'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['restore_defaults'] = array(
    '#type' => 'submit',
    '#value' => t('Restore defaults'),
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
function theme_xc_index_field_type_form($variables) {
  $form = $variables['form'];
  $output = '<p>' . theme('image', array('path' => 'http://wiki.apache.org/moin_static184/modernized/img/alert.png', 'width' => '', 'height' => '', 'alt' =>
    t('Alert!'), 'title' => t('Alert!'), 'attributes' => NULL)) . ' '
    . t('Alert! Advanced settings! Modify these settings very carefully, and only after you have read the manual!')
    . '</p>';

  $output .= '<p>' . t('There are lots of different dynamic Solr field types. If you want to add more, or remove uneccessary ones, first you should change the Solr\'s schema.xml then register the changes in !solr_field_type form, because the Toolkit can not read them out directly from Solr. This table always shows the actually registered Solr field types. Here you can map one or more Solr field types to a schema field or attribute. We set the default type to text, which tokenizes and store the input values &mdash; the best choose for searching for terms. Those fields which still has not mapped to a type are displayed in <font color="red">red</font>.',
    array('!solr_field_type' => l(t('Solr field types'), 'admin/xc/solr/field_type'))
  ) . '</p>';
  $output .= '<p>' . t('If you do not check any type, the field will not be indexed as distinct field. For example if this is place of publication, the user can not make distinction between the London as a place of publication or London as a term mentioned elsewhere in the bibliographical description. Even in that case the content of that field will remain searchable as part of the content of the whole description.') . '</p>';

  $metadata_schema = $form['metadata_schema']['#value'];
  $existing_field_types = xc_index_field_type_get_all_by_schema($metadata_schema);

  $rows = array();
  $row = array();
  foreach (element_children($form['types']) as $field) {
    $row = array(
      (isset($existing_field_types[$field])
        ? $field
        : array(
        'data' => $field,
        'style' => 'color:red',
      )),
    );
    foreach (element_children($form['types'][$field]) as $type) {
      $row[] = array('data' => drupal_render($form['types'][$field][$type]));
    }
    $rows[] = $row;
  }
  $header = array(t('field'));
  foreach (xc_solr_field_type_get_all() as $suffix => $type) {
    $header[] = $type->label;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_field_type_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $metadata_schema = $values['metadata_schema'];
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $solr_types = xc_solr_field_type_get_all();
  if ($op == t('Save')) {
    $existing_field_types = xc_index_field_type_get_all_by_schema($metadata_schema);

    // build a same array from user data
    $actual_field_types = array();
    foreach ($values as $key => $value) {
      if (preg_match('/^type---(.*?)---(.*?)$/', base64_decode($key), $matches)) {
        list($drop, $field, $suffix) = $matches;
        if (!isset($actual_field_types[$field])) {
          $actual_field_types[$field] = array();
        }

        if ($value != '0') {
          $actual_field_types[$field][] = $value;
        }
      }
    }

    $fields_to_ignore = array();
    foreach ($actual_field_types as $field => $value) {
      if (empty($value)) {
        $actual_field_types[$field][] = 'n/a';
        // array_push($fields_to_ignore, $field);
      }
    }

    // comparing the differences between stored values and actual ones
    $fields_to_delete = array_diff(array_keys($existing_field_types), array_keys($actual_field_types));
    $fields_to_create = array_diff(array_keys($actual_field_types), array_keys($existing_field_types));
    $common_fields = array_intersect(array_keys($existing_field_types), array_keys($actual_field_types));
    $types_to_delete = array();
    $types_to_create = array();
    $skip_delete_pattern = array('n/a');
    foreach ($common_fields as $field) {
      $del_types = array_diff($existing_field_types[$field], $actual_field_types[$field]);
      if (count($del_types)) {
        $types_to_delete[$field] = $del_types;
      }
      $new_types = array_diff($actual_field_types[$field], $existing_field_types[$field]);
      if (count($new_types)) {
        $types_to_create[$field] = $new_types;
      }
    }


    foreach ($fields_to_create as $field) {
      foreach ($actual_field_types[$field] as $type) {
        $types_to_create[$field][] = $type;
      }
    }

    // delete old fields
    if (!empty($fields_to_delete)) {
      foreach ($fields_to_delete as $field) {
        $result = db_delete('xc_index_field_type')
                  ->condition('metadata_schema', $metadata_schema)
                  ->condition('field', $field)
                  ->execute();
        if ($result == 1) { // repository is deleted
          drupal_set_message(t('Field %field removed', array('%field' => $field)));
        }
        else {
          drupal_set_message(t('Unexpected error. Deleted removes %count Solr field type(s).',
            array('%count' => $result)));
        }
      }
    }

    // delete old types
    if (!empty($types_to_delete)) {
      foreach ($types_to_delete as $field => $types) {
        foreach ($types as $type) {
          $result = db_delete('xc_index_field_type')
                    ->condition('metadata_schema', $metadata_schema)
                    ->condition('field', $field)
                    ->condition('type', $type)
                    ->execute();
          if ($result == 1) { // repository is deleted
            drupal_set_message(t('%field->%type removed', array(
              '%field' => $field,
              '%type' => ($type == 'n/a' ? t('ignored') : $solr_types[$type]->type),
            )));
          }
          else {
            drupal_set_message(t('Unexpected error. Deletion removed %count Solr field type.',
              array('%count' => $result)));
          }
        }
      }
    }

    // create new types
    if (!empty($types_to_create)) {
      foreach ($types_to_create as $field => $types) {
        foreach ($types as $type) {
          $record = (object) array(
            'metadata_schema' => $metadata_schema,
            'field' => $field,
            'type' => $type,
          );
          $ret_val = drupal_write_record('xc_index_field_type', $record);
          if ($ret_val == SAVED_NEW) {
            drupal_set_message(t('Successfully registered %field field as %type type.',
              array(
              '%field' => $record->field,
              '%type' => ($type == 'n/a' ? t('ignored') : $solr_types[$type]->type),
            )));
          }
          else {
            drupal_set_message(t('Unexpected error. Failed to register field type.'));
          }
        }
      }
    }
  } // save
  elseif ($op == t('Restore defaults')) {
    // TODO: delete all values belonging to the actual schema
    $result = db_delete('xc_index_field_type')
              ->condition('metadata_schema', $metadata_schema)
              ->execute();
    if ($result > 0) { // types are deleted
      drupal_set_message(t('All types are removed from %schema schema',
         array('%schema' => $metadata_schema)));
    }
    else {
      drupal_set_message(t('Unexpected error. Deletion removed %count Solr field type.',
        array('%count' => $result)));
    }
    // insert values from defaults
    $path = drupal_get_path('module', 'xc_index') . '/import/';
    $table = 'xc_index_field_type';
    $records = xc_util_csv2objects($path . $table . '.csv');
    foreach ($records as $record) {
      if ($record->metadata_schema == $metadata_schema) {
        $ret_val = drupal_write_record('xc_index_field_type', $record);
        if ($ret_val == SAVED_NEW) {
          drupal_set_message(t('Successfully registered %field field as %type type.',
            array(
            '%field' => $record->field,
            '%type' => $solr_types[$record->type]->label,
          )));
        }
        else {
          drupal_set_message(t('Unexpected error. Failed to register field type.'));
        }
      }
    }
  }
  variable_del('xc_solr_fields2index_list');
  variable_set('xc_index_definition_modification_time', time());
  $form_state['redirect'] = 'admin/xc/index/field_type';
}

/**
 * Get the list of field-type mappings in the form of an array.
 *
 * Each element of the array
 * has an 'id' and a 'type' element. The keys are the field names.
 *
 */
function xc_index_field_type_get_all_by_schema($metadata_schema) {
  $sql = 'SELECT * FROM {xc_index_field_type} WHERE metadata_schema = \'%s\'';
  $result = db_query('SELECT * FROM {xc_index_field_type} WHERE metadata_schema = :metadata_schema', array(':metadata_schema' => $metadata_schema,));
  $ret_val = array();
  foreach ($result as $data) {
    $ret_val[$data->field][] = $data->type;
  }
  return $ret_val;
}

