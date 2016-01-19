<?php
/**
 * @file
 * Functions to manipulate xc_index_attribute_to_field table.
 *
 * The table's structure:
 * @code
 * CREATE TABLE xc_index_attribute_to_field (
 *   atof_id         int(10) unsigned NOT NULL auto_increment,
 *   metadata_schema varchar(255) NOT NULL default '',
 *   entity          varchar(255) NOT NULL default '',
 *   schema_field    varchar(255) NOT NULL default '',
 *   attribute       varchar(255) NOT NULL default '',
 *   type            varchar(20)  NOT NULL default '',
 *   solr_field      varchar(255) NOT NULL default '',
 *   PRIMARY KEY  (`atof_id`),
 *   KEY `field_type_id` (`atof_id`)
 * );
 * @endcode
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_attribute_view() {
  $output = '<p>' . t('Select a metadata record type (entity) to list and modify how to index its fields\' attributes.') . '</p>';

  $items = array();
  $schemas = module_invoke_all('xc_schema');

  foreach ($schemas as $key => $value) {
    $items[] = l($value['title'], 'admin/xc/index/attribute/' . $key);
  }

  $output .= theme('item_list', array('items' => $items, 'title' => t('Available record types:')));
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_attribute_get_solr_fields($metadata_schema = NULL) {
  static $schemas;
  if (!isset($schemas)) {
    $schemas = module_invoke_all('xc_schema');
  }

  $sql = 'SELECT schema_field, attribute, solr_field, type, entity
            FROM {xc_index_attribute_to_field}
           WHERE type != \'ignore\'';
  if (!is_null($metadata_schema)) {
    $sql .= ' AND metadata_schema = :metadata_schema';
  }

  $result = db_query($sql, array(':metadata_schema' => $metadata_schema));
  $values = array();

  //while ($data = db_fetch_object($result)) {
  foreach($result as $data) {

    if ($data->type == 'value_as_key' || $data->type == 'as_key_exclusively') {
      $o_xc_attribute = xc_metadata_attribute_get($data->attribute, NULL, $data->schema_field);
      if (is_null($o_xc_attribute)) {
        drupal_set_message(filter_xss('not registered: ' . $data->schema_field . '@' . $data->attribute));
      }

      if (!empty($o_xc_attribute->possible_values)) {
        $possible_values = $o_xc_attribute->possible_values;
      }
      elseif (isset($schemas[$data->entity]['fields'][$data->schema_field]['attributes'][$data->attribute]['possible values'])) {
        $possible_values = $schemas[$data->entity]['fields'][$data->schema_field]['attributes'][$data->attribute]['possible values'];
      }
      else {
        $possible_values = '';
      }

      if (is_array($possible_values)) {
        // TODO: warning: Invalid argument supplied for foreach () in ...
        foreach ($possible_values as $value) {
          $values[$data->schema_field . '_' . $value] = $data->schema_field . '@' . $data->attribute;
        }
      }
      /*
       else {
       drupal_set_message(filter_xss('possible_values is not array: ' . $data->schema_field . '@' . $data->attribute));
       }
       */
    }
    else {
      $values[$data->solr_field] = $data->schema_field . '@' . $data->attribute;
    }
  }

  return $values;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_attribute_form($form, &$form_state, $metadata_schema) {
  drupal_set_title(t('Handling %entity entity\'s fields\' attributes',
    array('%entity' => $metadata_schema['entity_name'])), PASS_THROUGH);

  $existing_attributes = xc_index_attribute_get_all_by_schema(
    $metadata_schema['metadata_schema'],
    $metadata_schema['entity_name']
  );

  //$schema = module_invoke($schema_name, 'xc_schema');
  $types = xc_index_attribute_get_handler_types();
  foreach ($metadata_schema['fields'] as $field => $obj) {
    if (isset($obj['attributes'])) {
      foreach ($obj['attributes'] as $attribute_name => $attr_object) {
        $default_value = 'ignore';
        $default_exclusive = 0;

        if (isset($existing_attributes[$field])
           && isset($existing_attributes[$field][$attribute_name])) {
          $default_value = $existing_attributes[$field][$attribute_name]->type;
        }

        $form['types'][$field . '---' . $attribute_name] = array(
          '#type' => 'radios',
          '#options' => $types,
          '#default_value' => $default_value,
        );
      }
    }
  }

  $form['metadata_schema'] = array(
    '#type' => 'hidden',
    '#value' => $metadata_schema['metadata_schema'],
  );

  $form['schema'] = array(
    '#type' => 'value',
    '#value' => $metadata_schema,
  );

  $form['entity_name'] = array(
    '#type' => 'hidden',
    '#value' => $metadata_schema['entity_name'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['restore'] = array(
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
function theme_xc_index_attribute_form($variables) {
  $form = $variables['form'];
  $types = xc_index_attribute_get_handler_types();
  $output = '<p>' . theme('image', array('path' =>
    'http://wiki.apache.org/moin_static184/modernized/img/alert.png', 'width' => '', 'height' => '', 'alt' =>
    t('Alert!'), 'title' => t('Alert!'), 'attributes' => NULL)) . ' '
    . t('Alert! Advanced settings! Modify these settings very carefully, and only after you have read the manual!') . '</p>';
  $output .= t('We can handle the attributes in different ways:');
  $output .= theme('item_list', array('items' => array_values($types)));

  $output .= '<p>' . t('<strong>ignore</strong> means, that we do not index that attribute at all.') . '</p>';
  $output .= '<p>' . t('Otherwise we will index the attribute somehow. But there are some cases, when the the attribute somehow qualifies the parent elements name, and other cases, when it contains brand new informations. An example for the first case is a subject field, where the type attribute localize the source of the subject term. An exemple for the second case is when an author record contain not only the name of the author, but the author\'s ID in an attibute.') . '</p>';
  $output .= '<p>' . t('<strong>as key</strong> means, that the attribute\'s value will be the part of the field, and we value of the parent element will be the value of this new field.Imagine this: &lt;subject type="LCSH"&gt;History&lt;/subject&gt;. It means, that the History term comes from LCSH subject headings. Here the value could be use as key, and we can store the term History as subject, and as LCSH subject. The Drupal Toolkit will create a new field for this, joining the field name and the attribute name together, like this: subject_LCSH, so finally we will have two fields: subject = "History" and subject_LCSH = "History".') . '</p>';
  $output .= '<p>' . t('<strong>as key excusively</strong> means, that when the <em>as key</em> option was selected only the new field (created from the attribute value) is indexed, and not the general main one. So &mdash; if we continue the previous example &mdash; only subject_LCSH = "History" will be indexed, and not the general subject = "History".') . '</p>';
  $output .= '<p>' . t('<strong>as value</strong> means the second case, we use the attribute\'s name as the part of the new field name, and its value as the value of that field (and not the element\'s value). Example: &lt;author agentID="123"&gt;Shakespeare&lt;/subject&gt; becames author = "Shakespeare" and author_agentID = "123".') . '</p>';
  $output .= '<p>' . t('You can map the field types created from attrubutes in the !search_field_type form. Those fields which still has not mapped to a type are displayed in <font color="red">red</font>.',
    array('!search_field_type' => l(t('field types'), 'admin/xc/index/field_type')))
    . '</p>';

  // $metadata_schema['fields']['xc:recordID']['attributes']
  $metadata_schema = $form['schema']['#value'];
  $metadata_schema_name = $form['metadata_schema']['#value'];
  $entity_name = $form['entity_name']['#value'];
  $existing_attributes = xc_index_attribute_get_all_by_schema($metadata_schema_name, $entity_name);

  $rows = array();
  $row = array();
  $prev_field = "";
  foreach (element_children($form['types']) as $key) {
    list($field, $attribute) = explode('---', $key);
    $o_xc_attribute = xc_metadata_attribute_get($attribute, NULL, $field);

    if (!empty($o_xc_attribute->possible_values)) {
      $possible_values = $o_xc_attribute->possible_values;
    }
    elseif (isset($metadata_schema['fields'][$field]['attributes'][$attribute]['possible values'])) {
      $possible_values = $metadata_schema['fields'][$field]['attributes'][$attribute]['possible values'];
    }
    else {
      $possible_values = '';
    }

    if (is_array($possible_values)) {
      $possible_values = '<br /><em>' . t('possible values') . ': ' . join(', ', $possible_values) . '</em>';
    }

    $row = array($field . $possible_values);
    $row[] = isset($existing_attributes[$field])
      ? $attribute
      : array(
      'data' => $attribute,
      'style' => 'color:red',
    );
    foreach (element_children($form['types'][$key]) as $type) {
      $row[] = array('data' => drupal_render($form['types'][$key][$type]));
    }

    $rows[] = array(
      'data' => $row,
      'valign' => 'top',
    );
    $prev_field = $field;
  }

  $header = array(t('field'), t('attribute'));
  foreach ($types as $suffix => $type) {
    $header[] = $type;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_index_attribute_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $values      = $form_state['values'];
  $entity_name = $values['entity_name'];

  if ($op == t('Save')) {
    xc_index_attribute_save_form($values);
  }
  elseif ($op == t('Restore defaults')) {
    xc_index_attribute_restore_defaults($values);
  }

  $form_state['redirect'] = 'admin/xc/index/attribute/' . $entity_name;
}

/**
 * Restore default values for an entities' attribute handling
 * @param $values
 * @return unknown_type
 */
function xc_index_attribute_restore_defaults($values) {
  $metadata_schema = $values['metadata_schema'];
  $entity_name = $values['entity_name'];

  // delete old records
  $result = db_delete('xc_index_attribute_to_field')
              ->condition('metadata_schema', $metadata_schema)
              ->condition('entity', $entity_name)
              ->execute();
  if ($result == 1) { // attribute is deleted
    drupal_set_message(t('All attribute removed from %schema/%entity',
      array('%schema' => $metadata_schema, '%entity' => $entity_name)));

    // insert default records
    $data = xc_index_attribute_get_raw_defaults();
    foreach ($data as $record) {
      if ($record->metadata_schema == $metadata_schema
           && $record->entity == $entity_name) {
        $ret_val = drupal_write_record('xc_index_attribute_to_field', $record);
        if ($ret_val == SAVED_NEW) {
          drupal_set_message(t('Successfully restored %mt/%ent/%field@%attr as %type type.',
            array(
            '%mt' => $metadata_schema,
            '%ent' => $entity_name,
            '%field' => $record->schema_field,
            '%attr' => $record->attribute,
            '%type' => $record->type,
          )));
        }
        else {
          drupal_set_message(t('Unexpected error. Failed to register attribute mapping.'));
        }
      }
    }
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removes %count attribute handling(s).',
      array('%count' => $result)));
  }
}

/**
 * Save the corrent values to database
 * @param $values
 * @return unknown_type
 */
function xc_index_attribute_save_form($values) {
  $metadata_schema = $values['metadata_schema'];
  $entity_name     = $values['entity_name'];
  $existing_attributes = xc_index_attribute_get_all_by_schema($metadata_schema, $entity_name);
  $handling_types = xc_index_attribute_get_handler_types();

  // build a same array from user data
  $actual_attributes = array();
  foreach ($values as $key => $value) {
    if ($value != '0' && preg_match('/^(.*?)---(.*?)$/', $key, $matches)) {
      $actual_attributes[$matches[1]][$matches[2]] = $value;
    }
  }

  // delete those attribute settings, which are not inside the actual values
  foreach ($existing_attributes as $field_name => $field) {
    foreach ($field as $attribute_name => $data) {
      if (!isset($actual_attributes[$field_name][$attribute_name])) {
        $result = db_delete('xc_index_attribute_to_field')
                    ->condition('metadata_schema', $metadata_schema)
                    ->condition('entity', $entity_name)
                    ->condition('schema_field', $field_name)
                    ->condition('attribute', $attribute_name)
                    ->execute();
        if ($result == 1) { // repository is deleted
          drupal_set_message(t('%field@%attribute removed',
            array('%field' => $field_name, '%attribute' => $attribute_name)));
          // TODO: fieldtype: delete
        }
        else {
          drupal_set_message(t('Unexpected error. Deletion removes %count Solr field type(s).',
            array('%count' => $result)));
        }
      }
    }
  }

  // modify the existing settings, and insert the non existing ones
  foreach ($actual_attributes as $field_name => $field) {
    foreach ($field as $attribute_name => $type) {
      if (isset($existing_attributes[$field_name][$attribute_name])) {
        // skip where type was not changed
        if ($existing_attributes[$field_name][$attribute_name]->type == $type) {
          continue;
        }
        $o_attribute = $existing_attributes[$field_name][$attribute_name];
        $record = new stdClass();
        $record->atof_id = $o_attribute->atof_id;
        $record->type = $type;
        if ($record->type == 'ignore') {
          $record->solr_field = '';
        }
        elseif ($record->type == 'value_as_key'
             || $record->type == 'as_key_exclusively') {
          $record->solr_field = $field_name . '_xxxxxx';
        }
        else {
          $record->solr_field = $field_name . '_' . $attribute_name;
        }
        $ret_val = drupal_write_record('xc_index_attribute_to_field', $record, 'atof_id');
        if ($ret_val == SAVED_UPDATED) {
          drupal_set_message(t('Successfully updated %field@%attr facet as %type type.',
            array(
            '%field' => $field_name,
            '%attr' => $attribute_name,
            '%type' => $record->type,
          )));
        }
        else {
          drupal_set_message(t('Unexpected error. Failed to update attribute mapping.'));
        }
      }
      else { // new record should be created
        $record = new stdClass();
        $record->metadata_schema = $metadata_schema;
        $record->entity          = $entity_name;
        $record->schema_field    = $field_name;
        $record->attribute       = $attribute_name;
        $record->type            = $type;
        if ($record->type == 'ignore') {
          $record->solr_field = '';
        }
        elseif ($record->type == 'value_as_key'
             || $record->type == 'as_key_exclusively') {
          $record->solr_field = $field_name . '_xxxxxx';
        }
        else {
          $record->solr_field = $field_name . '_' . $attribute_name;
        }
        //$record->solr_field      = $field_name . '_' . $attribute_name;
        $ret_val = drupal_write_record('xc_index_attribute_to_field', $record);
        if ($ret_val == SAVED_NEW) {
          drupal_set_message(t('Successfully registered %mt/%ent/%field@%attr facet as %type type.',
            array(
            '%mt' => $metadata_schema,
            '%ent' => $entity_name,
            '%field' => $field_name,
            '%attr' => $attribute_name,
            '%type' => $record->type,
          )));
        }
        else {
          drupal_set_message(t('Unexpected error. Failed to register attribute mapping.'));
        }
      }
    }
  }
}

/**
 * Get the list of field-type mappings in the form of an array.
 *
 * Each element of the array has an 'id' and a 'type' element. The keys are the
 * field names.
 *
 * @param $metadata_schema (String)
 *   The name of metadata schema
 *
 * @param $entity_name (String)
 *   The name of entity
 *
 * @return (Array)
 *   The attributes belonging to the given schema and entity
 */
function xc_index_attribute_get_all_by_schema($metadata_schema, $entity_name) {
  $result = db_query('SELECT * FROM {xc_index_attribute_to_field}
           WHERE metadata_schema = :metadata_schema
             AND entity = :entity', array(':metadata_schema' => $metadata_schema, ':entity' => $entity_name));

  $attributes = array();
  //while ($data = db_fetch_object($result)) {
  foreach($result as $data) {
    $attributes[$data->schema_field][$data->attribute] = $data;
  }

  return $attributes;
}

/**
 * Get attribute handler types.
 * The following handlers exist: 'ignore', 'values_as_key',
 * 'as_key_exclusively', 'value_as_value'.
 * @return (Array)
 *   The array of keys - label pairs
 */
function xc_index_attribute_get_handler_types() {
  $types = array(
    'ignore' => t('ignore'),
    'value_as_key' => t('as key'),
    'as_key_exclusively' => t('as key exclusively'),
    'value_as_value' => t('as value'),
  );

  return $types;
}

/**
 * Return an array of stdClass of xc_index_attribute_to_fields records
 * @return unknown_type
 */
function xc_index_attribute_get_raw_defaults() {
  $path = drupal_get_path('module', 'xc_index') . '/import/';
  $table = 'xc_index_attribute_to_field';
  return xc_util_csv2objects($path . $table . '.csv');
}
