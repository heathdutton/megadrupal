<?php

/**
 * @file
 * Contains functions around product attributes.
 */

/**
 * Retrieve all Productfeed attributes from DB and modules.
 *
 * @return array
 *   product feed attributes.
 */
function bazaarvoice_productfeed_get_attributes() {
  $attributes = &drupal_static(__FUNCTION__);
  // Have attributes not been retrieved already?
  if (!isset($attributes)) {
    // Get default attributes.
    $attributes = bazaarvoice_productfeed_default_attributes();
    // Get attributes from modules.
    if ($add_attributes = module_invoke_all('bazaarvoice_productfeed_attributes')) {
      $attributes = array_merge($attributes, $add_attributes);
    }
    // Provide attribute name as an element for each attribute.
    foreach ($attributes as $name => &$data) {
      $data['attribute'] = $name;
    }
    // Get custom attributes from database.
    if ($db_attributes = bazaarvoice_productfeed_get_custom_attributes()) {
      // Add to attributes array. Potential for collision?
      $attributes += $db_attributes;
    }
    // Allow other modules to alter.
    drupal_alter('bazaarvoice_productfeed_attributes', $attributes);
  }
  // Return array of attributes.
  return $attributes;
}

/**
 * Retrieve all custom attributes stored in the database.
 *
 * @param mixed $attribute_names
 *   Array of attributes to retrieve.
 *
 * @return mixed
 *   Array of custom productfeed attributes.
 */
function bazaarvoice_productfeed_get_custom_attributes($attribute_names = FALSE) {
  // @TODO: static?
  $query = db_select('bazaarvoice_productfeed_custom_attributes', 'bpfa')
    ->fields('bpfa', array('attribute', 'label', 'field_types'));
  // Passed attributes to retrieve?
  if ($attribute_names) {
    // If passed only a single attribute, make it an array.
    if (!is_array($attribute_names)) {
      $attribute_names = array($attribute_names);
    }
    // Add condition to query.
    $query->condition('attribute', $attribute_names, 'IN');
  }
  // Get attributes from database.
  $attributes = $query->execute()->fetchAllAssoc('attribute', PDO::FETCH_ASSOC);
  // Attributes retrieved?
  if ($attributes) {
    // Universalize the field types.
    foreach ($attributes as &$data) {
      $data['field_types'] = unserialize($data['field_types']);
    }
  }
  // Return the retrieved attributes.
  return $attributes;
}

/**
 * Saves a custom attribute into the database.
 *
 * @param string $attribute_id
 *   The unique attribute id.
 * @param array $attribute
 *   The attribute data values to store.
 */
function bazaarvoice_productfeed_set_custom_attribute($attribute_id, array $attribute) {
  // Serialize field types.
  $attribute['field_types'] = serialize($attribute['field_types']);

  // Run a merge query.
  db_merge('bazaarvoice_productfeed_custom_attributes')
    ->key(array('attribute' => $attribute_id))
    ->fields($attribute)
    ->execute();
}

/**
 * Default list of productfeed attributes.
 *
 * @return array
 *   Array of productfeed attributes.
 */
function bazaarvoice_productfeed_default_attributes() {
  $attributes = array();
  $attributes['Name'] = array(
    'label' => t('Product Name'),
    'multiple' => 'Names',
    'translatable' => TRUE,
    'field_types' => array(
      'text',
    ),
  );
  $attributes['Description'] = array(
    'label' => t('Product Description'),
    'multiple' => 'Descriptions',
    'translatable' => TRUE,
    'field_types' => array(
      'text',
      'text_long',
      'text_with_summary',
    ),
  );
  $attributes['ImageUrl'] = array(
    'label' => t('Product Image'),
    'multiple' => 'ImageUrls',
    'translatable' => TRUE,
    'field_types' => array(
      'image',
    ),
  );
  $attributes['Category'] = array(
    'label' => t('Product Category'),
    'translatable' => TRUE,
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  $attributes['CategoryExternalId'] = array(
    'label' => t('Product Category ID'),
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  $attributes['Brand'] = array(
    'label' => t('Product Brand'),
    'translatable' => TRUE,
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  $attributes['BrandExternalId'] = array(
    'label' => t('Product Brand ID'),
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  $attributes['ModelNumber'] = array(
    'label' => t('Model Number'),
    'multiple' => 'ModelNumbers',
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  $attributes['ManufacturerPartNumber'] = array(
    'label' => t('Manufacturer Part Number'),
    'multiple' => 'ManufacturePartNumbers',
    'field_types' => array(
      'text',
      'taxonomy_term_reference',
    ),
  );
  return $attributes;
}

/**
 * Retrieve product attribute for a particular entity type.
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 * @param bool $attributes
 *   Array of attributes to retrieve, or boolean false.
 *
 * @return array|bool
 *   Array of attributes and field mappings or boolean FALSE.
 */
function bazaarvoice_productfeed_get_entity_attributes($type, $bundle, $attributes = FALSE) {
  // If not passed specific attributes, get them all.
  if (!$attributes) {
    $attributes = bazaarvoice_productfeed_get_attributes();
  }
  // Get Entity info.
  $entity_info = entity_get_info($type);
  // Fieldable entity and have attributes?
  if ($entity_info['fieldable'] && $attributes) {
    // Get entity fields.
    $fields = field_info_instances($type, $bundle);
    // Previous field mappings for this entity type?
    if ($mappings = bazaarvoice_productfeed_get_entity_mappings($type, $bundle, $attributes)) {
      // Merge the mapping settings into the attributes array.
      $attributes = array_merge_recursive($attributes, $mappings);
    }
    // Loop through all of the attributes.
    foreach ($attributes as &$attribute) {
      if (!isset($attribute['field_types'])) {
        continue;
      }
      // If this attribute supports text and this entity has a label.
      if (in_array('text', $attribute['field_types']) && isset($entity_info['entity keys']['label'])) {
        // Add the label as a field option.
        $attribute['field_options'][$entity_info['entity keys']['label']] = t('@label (label)', array('@label' => ucwords($entity_info['entity keys']['label'])));
      }
      // Entity have fields?
      if ($fields) {
        // Loop through the entity fields.
        foreach ($fields as $field_name => $field) {
          // Get field info.
          $field_info = field_info_field($field_name);
          // This field supported by this attribute?
          if (in_array($field_info['type'], $attribute['field_types'])) {
            // Add this field as an option for this attribute.
            $attribute['fields'][$field_name] = $field_info['type'];
            $attribute['field_options'][$field_name] = $field['label'];
          }
        }
      }
    }
  }
  // Allow other modules to modify these attributes.
  drupal_alter('bazaarvoice_productfeed_entity_attributes', $attributes, $type, $bundle);
  return $attributes;
}

/**
 * Retrieve field mappings for attributes for a particular entity type.
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 * @param array $attributes
 *   Array of productfeed attributes.
 *
 * @return array
 *   Array of attribute field mappings.
 */
function bazaarvoice_productfeed_get_entity_mappings($type, $bundle, array $attributes) {
  $entity_mappings = &drupal_static(__FUNCTION__);
  $mappings = array();
  // Only care about the attribute names.
  $attributes = array_keys($attributes);
  // See if attributes have already been retrieved in static cache.
  foreach ($attributes as $attribute) {
    // Has attribute already been retrieved?
    if (isset($entity_mappings[$type][$bundle][$attribute])) {
      // Add to mappings.
      $mappings[$attribute] = $entity_mappings[$type][$bundle][$attribute];
      // Unset from list of attributes to retrieve.
      unset($attributes[array_search($attribute, $attributes)]);
    }
  }
  // Any attributes that still need to be retrieved?
  if (count($attributes) > 0) {
    // DB query to fetch attributes.
    $results = db_select('bazaarvoice_productfeed_entity_mappings', 'bpfem')
      ->fields('bpfem', array('attribute', 'field_name', 'sub_field_name'))
      ->condition('attribute', $attributes, 'IN')
      ->condition('entity_type', $type)
      ->condition('entity_bundle', $bundle)
      ->execute();
    // For each returned attribute, build a field mapping array.
    foreach ($results as $result) {
      $mappings[$result->attribute] = array(
        'field' => $result->field_name,
        'sub_field' => $result->sub_field_name,
      );
      // Remove from array of attributes to retrieve.
      unset($attributes[array_search($result->attribute, $attributes)]);
    }
    // Save to entity mappings.
    $entity_mappings[$type][$bundle] = $mappings;
    // Any leftover attributes?
    if (count($attributes) > 0) {
      // Build an empty array.
      foreach ($attributes as $attribute) {
        $mappings[$attribute] = array(
          'field' => '',
          'sub_field' => '',
        );
      }
    }
  }
  // Allow other modules to add mappings.
  $entity_info = array('type' => $type, 'bundle' => $bundle);
  $additional_mappings = module_invoke_all('bazaarvoice_productfeed_entity_mappings', $entity_info);
  $mappings = array_merge($mappings, $additional_mappings);
  // Allow other modules to alter mappings.
  drupal_alter('bazaarovice_productfeed_entity_mappings', $mappings, $entity_info);
  // Return entity mappings.
  return $mappings;
}

/**
 * Format value for an attribute.
 *
 * @param mixed &$value
 *   String or array of values.
 * @param string $attribute_name
 *   Name of the attribute the value is for.
 */
function bazaarvoice_productfeed_format_attribute_value(&$value, $attribute_name) {
  // Passed an array of values?
  if (is_array($value)) {
    // Process each value.
    foreach ($value as &$sub_value) {
      bazaarvoice_productfeed_format_attribute_value($sub_value, $attribute_name);
    }
  }
  // Is a string, single value.
  else {
    // Format known attributes.
    switch ($attribute_name) {
      case 'CategoryExternalId':
      case 'BrandExternalId':
        // Use bazaarovice_products formatter.
        $value = bazaarvoice_products_format_product_id($value);
        break;
    }
    // Allow other modules to alter.
    drupal_alter('bazaarvoice_productfeed_format_attribute_value', $value, $attribute_name);
  }
}

/**
 * Check if attribute supports sub-fields.
 *
 * @param array $attribute
 *   The attribute data.
 *
 * @return bool
 *   Return Boolean TRUE or FALSE.
 */
function bazaarvoice_prouductfeed_attribute_supports_subfield(array $attribute) {
  // Term reference?
  if (in_array('taxonomy_term_reference', $attribute['field_types'])) {
    $support = TRUE;
  }
  // Entity reference?
  elseif (module_exists('entity_reference') && in_array('entityreference', $attribute['field_types'])) {
    $support = TRUE;
  }
  // Allow other modules to provide info for this attributes.
  else {
    $support = module_invoke_all('bazaarvoice_productfeed_supports_subfield', $attribute);
  }
  // If returned an array and sum is greater then 0, then supports subfields.
  if (is_array($support)) {
    $support = (array_sum($support) > 0);
  }
  return $support ? TRUE : FALSE;
}
