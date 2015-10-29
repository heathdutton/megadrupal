<?php
/**
 * @file
 * XC Metadata format functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get metadata format array
 *
 * @see xc_format_get()
 */
function xc_format_load($format) {
  return xc_format_get($format);
}

/**
 * Get metadata format array
 *
 * @param $format
 *    Machine name of format to get, if necessary, fetches all formats by
 *    default
 * @param $update
 *    TRUE to refresh the metadata format cache
 * @return
 *    Metadata format array
 */
function xc_format_get($format = 'all', $update = FALSE) {
  static $formats;
  if (!is_array($formats) || $update) {
    $formats = array();
    $formats = module_invoke_all('xc_format');
    xc_format_get_types(TRUE);
  }
  if ($format != 'all'
       && !empty($formats[$format])
       && is_array($formats[$format])) {
    return $formats[$format];
  }
  else {
    return $formats;
  }
}

function xc_format_get_types($update = FALSE) {
  static $types;
  if (!is_array($types) || $update) {
    $formats = xc_format_get();
    $types = array();
    foreach ($formats as $name => $format) {
      $types[] = $name;
    }
  }
  return $types;
}

/**
 * List all metadata formats
 *
 * @return
 *    Themed HTML output
 */
function xc_format_list() {
  $header = array(t('Format'), t('Type'), t('Description'), array(
      'data' => t('Operations'),
      'colspan' => 2,
    ));
  $rows = array();
  $formats = xc_format_get();
  if (empty($formats)) {
    drupal_set_message(
      t('No formats are available on this system. You must first !install_link a schema module',
        array('!install_link' => l(t('install'), 'admin/modules'))),
      'warning'
    );
  }
  foreach ($formats as $type => $format) {
    $enable_disable = xc_format_is_enabled($type)
      ? l(t('disable'), 'admin/xc/metadata/format/' . $type . '/disable')
      : l(t('enable'), 'admin/xc/metadata/format/' . $type . '/enable');
    $rows[] = array(
      $format['name'],
      $type,
      $format['description'],
      l(t('view'), 'admin/xc/metadata/format/' . $type),
      $enable_disable,
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * View a metadata format
 *
 * @param $type
 *    Metadata format to view
 * @return
 *    Themed HTML output
 */
function xc_format_view($type) {
  $format = xc_format_get($type);
  $header = array(t('Property'), t('Value'));
  $rows = array();
  $output = '';

  $rows[] = array(t('Type'), $type);
  $properties = array(
    'description' => t('Description'),
    'content type' => t('Content type'),
    'root element' => t('Root element'),
    'namespaceURI' => t('namespaceURI'),
    'schemaLocation' => t('schemaLocation'),
  );

  foreach ($properties as $property => $label) {
    if (!empty($format[$property])) {
      $value = check_plain($format[$property]);
      if ($property == 'schemaLocation') {
        $value = str_replace(' ', '<br />', $value);
      }
      $rows[] = array(
        'data' => array($label, $value),
        'valign' => 'top',
      );
    }
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  if (!empty($format['entities']) && xc_format_is_enabled($type)) {
    $_header = array('Entity', 'Fields');
    $_rows = array();


    foreach ($format['entities'] as $entity) {
      $_entity = xc_metadata_entity_get($entity);

      $_entity_fields = $_entity->get_fields();
      $_entity_field_names = array();
      foreach ($_entity_fields as $_field) {
        $field = _array_to_qualified_name($_field, 'field');
        if (!empty($_field->attributes)) {
          $_attributes = $_field->get_attributes();

          $attributes = array();
          foreach ($_attributes as $_attribute) { // XCMetadataAttribute
            $attribute = $_attribute->get_qualified_name();
            if (!empty($_attribute->possible_values)) {
              $attribute .= " ['" . join("', '", $_attribute->possible_values) . "']";
            }
            $attributes[] = $attribute;
          }
          if (!empty($attributes)) {
            $field .= ' (' . t('attributes: @attributes', array('@attributes' => join(', ', $attributes))) . ')';
          }
        }
        $_entity_field_names[] = $field;
      }

      $_fields = empty($format['fields'])
        ? $_entity_field_names :
        array_intersect($format['fields'], $_entity_field_names);
      $_rows[] = array(
        data => array(
          '<strong>' . (empty($_entity->title) ? $entity : $_entity->title) . '</strong>',
          theme('item_list', array('items' => $_fields)),
        ),
        'valign' => 'top',
      );
    }

    $output .= theme('table', array('header' => $_header, 'rows' => $_rows));
  }

  return $output;
}

/**
 * Check if a metadata format is enabled
 *
 * @param $type
 *   Machine name of metadata format to check
 * @return
 *   TRUE if metadata format is enabled
 */
function xc_format_is_enabled($type) {
  return in_array($type, variable_get('xc_enabled_formats', array()));
}

/**
 * Enable a metadata format
 *
 * @param $type
 *   Machine name of metadata format to enable
 */
function xc_format_enable($type) {
  $format = xc_format_get($type);
  $enabled = variable_get('xc_enabled_formats', array());
  $enabled = array_merge($enabled, array($type));
  variable_set('xc_enabled_formats', $enabled);

  if (!isset($format['entities'])) {
    drupal_set_message('Unloaded format: ' . $type, 'error');
  }
  else {
    $schema = xc_load_schema($format['entities']);
    xc_install_schema($schema);
  }
}

/**
 * Enable metadata format form
 */
function xc_format_enable_form(&$form_state, $type) {
  $format = xc_format_get($type);
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['format'] = array(
    '#type' => 'value',
    '#value' => $format,
  );
  return confirm_form($form,
    t('Are you sure you want to enable the %name format?', array('%name' => $format['name'])),
    'admin/xc/metadata/format', '', t('Enable'), t('Cancel'));
}

/**
 * Enable metadata format form submit handler
 */
function xc_format_enable_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $format = $form_state['values']['format'];
  xc_format_enable($type);
  drupal_set_message(t('%name format enabled. You may need to clear the cache or rebuild the menu structure.',
    array('%name' => $format['name'])));
  $form_state['redirect'] = 'admin/xc/metadata/format';
}

/**
 * Disable a metadata format
 *
 * @param $type
 *   Machine name of metadata format to disable
 */
function xc_format_disable($type) {
  $format = xc_format_get($type);
  $enabled = variable_get('xc_enabled_formats', array());
  $enabled = array_diff($enabled, array($type));
  variable_set('xc_enabled_formats', $enabled);

  $schema = xc_load_schema($format['entities']);
  xc_uninstall_schema($schema);
}

/**
 * Disable metadata format form
 */
function xc_format_disable_form(&$form_state, $type) {
  $format = xc_format_get($type);
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['format'] = array(
    '#type' => 'value',
    '#value' => $format,
  );

  return confirm_form($form,
    t('Are you sure you want to disable the %name format?',
    array('%name' => $format['name'])),
    'admin/xc/metadata/format',
    'This will destroy all metadata associated with that format',
    t('Disable'),
    t('Cancel')
  );
}

/**
 * Disable metadata format form submit handler
 */
function xc_format_disable_form_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $format = $form_state['values']['format'];
  xc_format_disable($type);
  drupal_set_message(t('%name format disabled. You may need to clear the cache or rebuild the menu structure.',
    array('%name' => $format['name'])));
  $form_state['redirect'] = 'admin/xc/metadata/format';
}

/**
 * Display metadata format name for page title
 *
 * @param $format
 *    Metadata format array
 * @return
 *    Title string
 */
function xc_format_title($format) {
  if (!is_array($format)) {
    $format =  xc_format_get($format);
  }
  return $format['name'];
}

/**
 * Get all installed metadata schemas
 */
function xc_format_get_schemas() {
  $values = array();
  $formats = xc_format_get();
  foreach ($formats as $type => $format) {
    $values[$type] = $format['name'];
  }
  return $values;
}

/**
 * Get all metadata schema's entities
 */
function xc_format_get_entities() {
  $entities = array();
  $formats = xc_format_get();
  foreach ($formats as $format) {
    foreach ($format['entities'] as $entity) {
      $entities[$entity] = $entity;
    }
  }
  return $entities;
}

/**
 * Get the schema of a given entity
 *
 * @param $entity The name of entity
 * @return unknown_type
 */
function xc_format_get_schema_by_entity($entity) {
  $entities = array();
  $formats = xc_format_get();
  foreach ($formats as $format) {
    foreach ($format['entities'] as $_entity) {
      if ($entity == $_entity) {
        return $format['module'];
      }
    }
  }
  return '';
}

/**
 * Get fields from schema(s)
 *
 * If schema parameter is null of 'all', it gives all fields of all schemas,
 * if schema is a name of an existing schema, it gives all fields of that schema.
 *
 * @param $schema
 *    The name of the schema. 'all' or NULL means all schemas.
 *
 * @return
 *    Array of fields
 */
function xc_format_get_fields($schema = 'all') {
  $fields = array();
  if (is_null($schema) || empty($schema)) {
    $schema = 'all';
  }
  $formats = xc_format_get($schema);
  if ($schema == 'all') {
    foreach ($formats as $key => $format) {
      foreach ($format['entities'] as $entity) {
        $_entity = xc_metadata_entity_get($entity);
        $_entity_fields = $_entity->get_fields();
        foreach ($_entity_fields as $_entity_field) {
          $name = $_entity_field->namespace . ':' . $_entity_field->name;
          $fields[$name] = $name;
        }
      }
    }
  }
  else {
    foreach ($formats['entities'] as $entity) {
      $_entity = xc_metadata_entity_get($entity);
      if (is_object($_entity)) {
        $_entity_fields = $_entity->get_fields();
        foreach ($_entity_fields as $_entity_field) {
          $name = $_entity_field->namespace . ':' . $_entity_field->name;
          $fields[$name] = $name;
        }
      }
    }
  }
  $sorted_fields = array();
  sort($fields);
  foreach ($fields as $k => $field_name) {
    $sorted_fields[$field_name] = $field_name;
  }
  return $sorted_fields;
}

/**
 * Get all fields in a schema which has at least one attribute
 *
 * @param $schema
 * @return unknown_type
 */
function xc_format_get_fields_with_attributes($schema = 'all') {
  $fields = array();
  if (is_null($schema) || empty($schema)) {
    $schema = 'all';
  }
  $formats = xc_format_get($schema);
  if ($schema == 'all') {
    foreach ($formats as $key => $format) {
      foreach ($format['entities'] as $entity) {
        $_entity = xc_metadata_entity_get($entity);
        $_entity_fields = $_entity->get_fields();
        foreach ($_entity_fields as $_entity_field) {
          $name = $_entity_field->namespace . ':' . $_entity_field->name;
          $fields[$name] = array(
            'name' => $name,
            'attributes' => $_entity_field->attributes,
          );
        }
      }
    }
  }
  else {
    foreach ($formats['entities'] as $entity) {
      $_entity = xc_metadata_entity_get($entity);
      $_entity_fields = $_entity->get_fields();
      foreach ($_entity_fields as $_entity_field) {
        $name = $_entity_field->namespace . ':' . $_entity_field->name;
        $fields[$name] = array(
          'name' => $name,
          'attributes' => $_entity_field->attributes,
        );
      }
    }
  }
  $sorted_fields = array();
  sort($fields);
  foreach ($fields as $i => $field) {
    $sorted_fields[$field['name']] = $field;
  }
  return $sorted_fields;
}

