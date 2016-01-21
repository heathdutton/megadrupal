<?php
/**
 * @file
 * bazaarvoice_productfeed.api.inc
 *
 * Defines bazaarvoice_productfeed hooks that can be used by other modules.
 */

/**
 * Alter the attributes available when mapping entity type fields.
 *
 * This is invoked from bazaarvoice_productfeed_form_node_type_form_alter().
 * when building the table of attributes for mapping.
 *
 * @param array &$attributes
 *   Associative array of Productfeed attributes that can be mapped to entity
 *   fields. Keyed by the attribute name, each attribute consists of:
 *    - label : Human readable label.
 *    - multiple: Attribute name for multiple values.
 *    - translatable: Boolean if attribute is translatable.
 *    - field_types: Array of field types this attribute accepts.
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_entityform_attributes_alter(array &$attributes, $type, $bundle) {
  // Add entityreference as an accepted field type to the Author attribute.
  if (isset($attributes['Author'])) {
    $attributes['Author']['field_types'][] = 'entityreference';
  }
}

/**
 * Alter the products and their attributes being fetched for the product feed.
 *
 * This is invoked from bazaarvoice_productfeed_get_products() after the
 * attribute values have been retrieved.
 *
 * @param array &$products
 *   Array of products and their attributes. Product keys are the entity id and
 *   each product is an associative array of attributes.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_products_alter(array &$products) {
  // Foreach product add site name to.
  foreach ($products as &$product) {
    $product['SiteName'] = variable_get('site_name');
  }
}

/**
 * Alter the subfields available for a given field. Allows modules that offer
 * complicated field types (entityreferences for example) to include the fields
 * of the referenced data from the initial field as sub options.
 *
 * This is invoked from bazaarvoice_productfeed_get_subfield_fields() after the
 * initial array of subfields have been retrieved.
 *
 * @param array &$sub_fields
 *   Array of subfield already retrieved for the given field.
 * @param array $attribute_data
 *   Array of data about the current attribute, such as supported field types.
 * @param string $field_name
 *   Name of the field being retrieved.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_subfield_fields_alter(array &$sub_fields, array $attribute_data, $field_name) {
  // Get field info.
  $field_info = field_info_field($field_name);
  // Field is an entity reference?
  if ($field_info['type'] == 'entityreference') {
    // Remove the field_author field as a sub field.
    if (isset($sub_fields['field_author'])) {
      unset($sub_fields['field_author']);
    }
  }
}

/**
 * Alter the value retrieved for a field.
 *
 * This is invoked from bazaarvoice_productfeed_get_field_value() after the
 * value has already been retrieved.
 *
 * @param mixed &$field_value
 *   The retrieved field value, either an array or string.
 * @param array $field_data
 *   Array of field data fetched from the product entity.
 * @param array $field_info
 *   Array of info about the field and subfield data was retrieved from.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_field_value_alter(&$field_value, array $field_data, array $field_info) {
  $field_info = field_info_field($field_info['field']);
  // Is it an entity reference field?
  if ($field_info['type'] == 'entityreference') {
    // Force the value to be foobar.
    $field_value = 'foobar';
  }
}

/**
 * Alter Bazaarvoice productfeed attributes.
 *
 * Invoked from bazaarvoice_productfeed_get_attributes() after all attributes
 * have been retrieved from database and other modules. Includes custom
 * attributes.
 *
 * @param array &$attributes
 *   Associative array of productfeed attributes. Each attribute is an
 *   associative array with the following attributes:
 *    - label : Human readable label.
 *    - multiple: Attribute name for multiple values.
 *    - translatable: Boolean if attribute is translatable.
 *    - field_types: Array of field types this attribute accepts.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_attributes_alter(array &$attributes) {
  // To each attribute add support for entityreference fields.
  foreach ($attributes as &$attribute) {
    $attribute['field_types'][] = 'entityreference';
  }
}

/**
 * Alter Bazaarvoice productfeed attributes for a given entity type and bundle.
 *
 * Invoked from bazaarvoice_productfeed_get_entity_attributes() after all
 * attributes and field mapping for given type and bundle.
 *
 * @param array &$attributes
 *   Associative array of bazaarvoice productfeed attributes.
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_entity_attributes_alter(array &$attributes, $type, $bundle) {
  // For article nodes remove all entityreference field support.
  if ($type == 'node' && $bundle == 'article') {
    foreach ($attributes as &$attribute) {
      foreach ($attribute['fields'] as $field_name => $field_type) {
        if ($field_type == 'entityreference') {
          unset($attribute['fields'][$field_name]);
          unset($attribute['field_options'][$field_name]);
        }
      }
    }
  }
}

/**
 * Alter the attribute field mappings for a particular entity type.
 *
 * Invoked from bazaarvoice_productfeed_get_entity_mappings() after mappings
 * have been retrieved from database and other modules.
 *
 * @param array &$mappings
 *   Associative array of productfeed attributes and field mappings. The key for
 *   each is the attribute name. Each attribute contains:
 *   - field : field_name of field mapped to.
 *   - sub_field: field_name of sub field mapped to.
 * @param array $entity_info
 *   Associative array of entity info includes the entity type and bundle.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarovice_productfeed_entity_mappings_alter(array &$mappings, array $entity_info) {
  // For article nodes.
  if ($entity_info['type'] == 'node' && $entity_info['bundle'] == 'article') {
    // Force author to point to field_author.
    $mappings['Author']['field'] = 'field_author';
  }
}

/**
 * Alter the value being used in the product feed for an attribute.
 *
 * Invoked from bazaarvoice_productfeed_format_attribute_value() when building
 * the productfeed.
 *
 * @param mixed $value
 *   The value to be formatted and included in productfeed.
 * @param string $attribute_name
 *   The name of the attribute being built.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_format_attribute_value_alter(&$value, $attribute_name) {
  // Force the value of the Author attribute to be all caps.
  if ($attribute_name == 'Author') {
    $value = strtoupper($value);
  }
}

/**
 * Alter the element to be used for building the XML productfeed.
 *
 * Invoked from bazaarvoice_productfeed_build_feed_element() after all sub
 * elements have been built.
 *
 * @param array &$element
 *   Associative array to be used by bazaarvoice_productfeed_build_feed_xml()
 *   to build XML document. The array structure mimics that of a render array
 *   The element contains the following by default:
 *    - #attributes : associative array of attributes for the root XML element.
 *    - deltas : Additional sub arrays which represent child XML elements. By
 *      default these are the Brands, Categories and Products.
 * @param string $locale
 *   The Bazaarvoice locale productfeed is being built for.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_feed_element_alter(array &$element, $locale) {
  // Add a new Foo attribute and set to Bar.
  $element['#attributes']['Foo'] = 'Bar';
}

/**
 * Alter the Brands element that will be used in building the Productfeed XML.
 *
 * Invoked from bazaarvoice_productfeed_build_feed_brands() after the brands
 * array has been built.
 *
 * @param array &$brands
 *   Associative array to be used by bazaarvoice_productfeed_build_feed_xml()
 *   to build the Brand XML elements. The array structure mimics that of a
 *   render array. Each brand is an array of sub elements. Each sub element is
 *   an associative array with the following keys:
 *    - #name : The name of the child element, creates the XML tag.
 *    - #value: The value to place into the XML tag.
 * @param array $products
 *   Array of products being used to build the productfeed. Brands are retrieved
 *   from the $product['brand'] array item.
 * @param string $locale
 *   The Bazaarvoice locale code being used to build the productfeed.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_feed_brands_alter(array &$brands, array $products, $locale) {
  // Force all brand names to be caps.
  foreach ($brands as &$brand) {
    foreach ($brand as &$element) {
      if ($element['#name'] == 'Name') {
        $element['#value'] = strtoupper($element['#value']);
      }
    }
  }
}

/**
 * Alter the Categories element used in building the Productfeed XML.
 *
 * Invoked from bazaarvoice_productfeed_build_feed_categories() after the
 * categories array has been built.
 *
 * @param array &$categories
 *   Associative array to be used by bazaarvoice_productfeed_build_feed_xml()
 *   to build the Categories XML elements. The array structure mimics that of a
 *   render array. Each category is an array of sub elements. Each sub element
 *   is an associative array with the following keys:
 *    - #name : The name of the child element, creates the XML tag.
 *    - #value: The value to place into the XML tag.
 * @param array $products
 *   Array of products being used to build the productfeed. Categories are
 *   retrieved from the $product['Category'] array item.
 * @param string $locale
 *   The Bazaarvoice locale code being used to build the productfeed.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_feed_categories_alter(array &$categories, array $products, $locale) {
  // Force all category names to be caps.
  foreach ($categories as &$category) {
    foreach ($category as &$element) {
      if ($element['#name'] == 'Name') {
        $element['#value'] = strtoupper($element['#value']);
      }
    }
  }
}

/**
 * Alter the Products element used in building the Productfeed XML.
 *
 * Invoked from bazaarvoice_productfeed_build_feed_products() after the
 * products_element array has been built.
 *
 * @param array &$products_element
 *   Associative array to be used by bazaarvoice_productfeed_build_feed_xml()
 *   to build the Products XML elements. The array structure mimics that of a
 *   render array. Each product is an array of sub elements. Each sub element
 *   is a bazaarvoice attribute structured with the following keys:
 *    - #name : The name of the attribute element, creates the XML tag.
 *    - #value: The value to place into the XML tag.
 * @param array $products
 *   Array of products being used to build the productfeed.
 * @param string $locale
 *   The Bazaarvoice locale code being used to build the productfeed.
 *
 * @ingroup bazaarvoice_productfeed_alter_hooks
 */
function hook_bazaarvoice_productfeed_feed_products_alter(array &$products_element, array $products, $locale) {
  // Add an XML 'locale' attribute to each product.
  foreach ($products_element as &$product_element) {
    $product_element['#attributes']['locale'] = $locale;
  }
}

/**
 * Perform an action after productfeed attribute/field mappings have been
 * updated for an entity bundle.
 *
 * Invoked from bazaarvoice_productfeed_save_entity_attributes() after saving
 * the mappings.
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 * @param array $mappings
 *   Associative array of bazaarvoice productfeed / field mappings. Keys are the
 *   names of the attributes. Each mapping is an associative array of:
 *    - field: the machine name of the mapped field
 *    - sub_field: the machine name of the mapped sub field or NULL.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_save_entity_attributes($type, $bundle, array $mappings) {
  // Log the update of mappings.
  watchdog('bazaarvoice_productfeed', 'Productfeed mappings updated for !type !bundle', array(
    '!type' => $type,
    '!bundle' => $bundle,
  ));
}

/**
 * Perform an action after productfeed attribute/field mappings have been
 * deleted for an entity bundle.
 *
 * Invoked from bazaarvoice_productfeed_delete_entity_attributes() after
 * deleting the mappings.
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarovice_productfeed_delete_entity_attributes($type, $bundle) {
  // Log the removal of mappings.
  watchdog('bazaarvoice_productfeed', 'Productfeed mappings removed for !type !bundle', array(
    '!type' => $type,
    '!bundle' => $bundle,
  ));
}

/**
 * Perform actions after a productfeed attribute has been deleted.
 *
 * Invoked from bazaarvoice_productfeed_delete_attributes() after the attributes
 * have been deleted from the database and the productfeed has been triggered
 * for rebuild.
 *
 * @param array $attributes
 *   Array of attribute names deleted from database.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_delete_attributes(array $attributes) {
  // Log the deletion of attributes.
  watchdog('bazaarvoice_productfeed', '!count custom attributes have been deleted: <em>!attributes</em>', array(
    '!count' => count($attributes),
    '!attributes' => implode(', ', $attributes),
  ));
}

/**
 * Perform an action when fetching the available subfields for a particular
 * field.
 *
 * Invoked from bazaarvoice_productfeed_get_subfield_fields() if the field type
 * is unknown to the productfeed module. Suitable for modules that add new field
 * types to the system (such as entityreference).
 *
 * @param string $attribute_name
 *   Name of the bazaarvoice attribute.
 * @param array $attribute_data
 *   Array of data about the current attribute, such as supported field types.
 * @param string $field_name
 *   Name of the field being retrieved.
 *
 * @return array
 *   Array of available sub fields.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_subfield_fields($attribute_name, array $attribute_data, $field_name) {
  $sub_fields = array();
  // Get field info.
  $field_info = field_info_field($field_name);
  // Is the field an entityreference?
  if ($field_info['type'] == 'entityreference') {
    // Foreach entity bundle.
    foreach ($field_info['settings']['handler_settings']['target_bundles'] as $bundle) {
      // Get the attributes supported by this entity.
      $attributes = bazaarvoice_productfeed_get_entity_attributes($field_info['settings']['target_type'], $bundle, array($attribute_name => $attribute_data));
      // Field options returned?
      if (isset($product_fields[$attribute_name]['field_options'])) {
        // Add to list of sub_fields.
        $sub_fields = array_merge($sub_fields, $attributes[$attribute_name]['field_options']);
      }
    }
  }

  return $sub_fields;
}

/**
 * Perform action when fetching value for a field.
 *
 * Invoked from bazaarvoice_productfeed_get_field_value() if the field type
 * is unknown to the productfeed module. Suitable for modules that add new field
 * types to the system (such as entityreference).
 *
 * @param mixed $data
 *   The data fetched from the entity field.
 * @param string $field_name
 *   The machine name of the field fetching data for.
 * @param string $sub_field
 *   The name of the sub_field to fetch data for or NULL.
 *
 * @return mixed
 *   The fetched value.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_field_value($data, $field_name, $sub_field) {
  $value = $data;
  $field_info = field_info_field($field_name);
  if ($field_info['type'] == 'entityreference') {
    $value = array();
    // Get the entity type.
    $entity_type = $field_info['settings']['target_type'];
    // Loop through each entity.
    foreach ($data as $entity) {
      // Get the entity id fields.
      list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
      // Have a sub_field ?
      if ($sub_field) {
        // Get the metadata.
        $metadata = entity_metadata_wrapper($entity_type, $id);
        // Get subfield value.
        $value[] = $metadata->$sub_field->value();
      }
      else {
        // Use the entity id.
        $value[] = $id;
      }
    }
  }
  return $value;
}

/**
 * Allow other modules to provide attributes for the productfeeed.
 *
 * Invoked from bazaarvoice_productfeed_get_attributes() after fetching default
 * attributes provided by the bazaarvoice_productfeed module.
 *
 * @return array
 *   An associative array of productfeed attributes. The key is the name of the
 *   attribute.  Each attribute should have the following elements:
 *    - label : Human readable label.
 *    - multiple: Attribute name for multiple values.
 *    - translatable: Boolean if attribute is translatable.
 *    - field_types: Array of field types this attribute accepts.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_attributes() {
  // Return an Author attribute.
  return array(
    'Author' => array(
      'label' => t('Author'),
      'multiple' => 'Authors',
      'translatable' => TRUE,
      'field_types' => array(
        'text',
      ),
    ),
  );
}

/**
 * Allow modules to provide additional attribute field mappings.
 *
 * Invoked from bazaarvoice_productfeed_get_entity_mappings() after fetching
 * mappings stored in the database.
 *
 * @param array $entity_info
 *   Array containng the entity type and bundle fetching mappings for.
 *
 * @return array
 *   Associative array of custom attribute field mappings.Key is the name of the
 *   attribute and contains the following elements:
 *    - field: machine name of the field to be mapped to.
 *    - sub_field (optional): machine name of the sub field to be mapped to.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_entity_mappings(array $entity_info) {
  $mappings = array();
  // For article nodes, map Author to the field field_author
  if ($entity_info['type'] == 'node' && $entity_info['bundle'] == 'article') {
    $mappings['Author'] = array(
      'field' => 'field_author',
    );
  }

  return $mappings;
}

/**
 * Allow modules to indicate if an attribute supports sub fields.
 *
 * Invoked from bazaarvoice_productfeed_get_subfield_fields() if the field type
 * is unknown to the productfeed module. Suitable for modules that add new field
 * types to the system (such as entityreference).
 *
 * @param array $attribute
 *   Array of data about the attribute. Has the following elements:
 *    - label : Human readable label.
 *    - multiple: Attribute name for multiple values.
 *    - translatable: Boolean if attribute is translatable.
 *    - field_types: Array of field types this attribute accepts.
 *
 * @return bool
 *   Return boolean TRUE or FALSE is sub field is supported.
 *
 * @ingroup bazaarvoice_productfeed_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_productfeed_supports_subfield(array $attribute) {
  // If field_types include entity reference return TRUE.
  if (module_exists('entity_reference') && in_array('entityreference', $attribute['field_types'])) {
    return TRUE;
  }
  return FALSE;
}
