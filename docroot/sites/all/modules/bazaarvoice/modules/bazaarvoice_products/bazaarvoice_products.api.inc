<?php
/**
 * @file
 * bazaarvoice_products.api.inc
 *
 * Defines bazaarvoice_products hooks that can be used by other modules.
 */

/**
 * Alter the array of fields that can be used as a product_id.
 *
 * Invoked from bazaarvoice_products_get_entity_product_id_fields() after field
 * options have been retrieved.
 *
 * @param array $fields
 *   Associative array of fields that can be used as the product_id. Array keys
 *   are the field machine names and values are the display labels.
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_products_alter_hooks
 */
function hook_bazaarvoice_products_entity_product_id_fields_alter(array &$fields, $type, $bundle) {
  // For article nodes remove the field field_article_author
  if ($type == 'node' && $bundle == 'article') {
    unset($fields['field_article_author']);
  }
}

/**
 * Alter the information about a product entity.
 *
 * Invoked from bazaarvoice_products_get_product_info() after product entity
 * info has been retrieved.
 *
 * @param array $product_info
 *   Associative array of product entity info. Contains the following elements:
 *    - type: the entity type
 *    - bundle: the entity bundle
 *    - eid: the entity id
 *    - vid: the entity revision id
 *    - product_id : The Bazaarvoice product id
 *    - entity: The Drupal entity object.
 *
 * @ingroup bazaarvoice_products_alter_hooks
 */
function hook_bazaarvoice_product_info_alter(array &$product_info) {
  // For article entities set the revision id to 1.
  if ($product_info['type'] == 'node' && $product_info['bundle'] == 'Article') {
    $product_info['vid'] = 1;
  }
}

/**
 * Alter the entity types that can be used as a Bazaarvoice product.
 *
 * Invoked from bazaarvoice_products_get_product_entity_types() after fetching
 * types from the database.
 *
 * @param array $entity_types
 *   Structured Associative array of entity types. Key is the entity type. Each
 *   type is an associative array of entity bundles, keyed by bundle name. Each
 *   bundle is an associative array with the following elements:
 *    - type: The entity type.
 *    - bundle: The entity bundle.
 *    - product_id_field: Machine name of field mapped as the product id.
 *
 * @ingroup bazaarvoice_products_alter_hooks
 */
function hook_bazaarvoice_products_product_types_alter(array &$entity_types) {
  // Remove the node article bundle from being used as a product type.
  if (isset($entity_types['node']['article'])) {
    unset($entity_types['node']['article']);
  }
}

/**
 * Alter the boolean flag that indicates if an entity type and bundle is a
 * product type.
 *
 * Invoked from bazaarvoice_products_is_product_type() after checking if type
 * bundle is set as a product type.
 *
 * @param bool &$is_type
 *   Boolean TRUE or FALSE indicating is a product type.
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_products_alter_hooks
 */
function hook_bazaarvoice_products_is_product_type_alter(&$is_type, $type, $bundle) {
  // Force articles nodes not to be product types.
  if ($type == 'node' && $bundle == 'article') {
    $is_type = FALSE;
  }
}

/**
 * Alter the statistics data for a particular product.
 *
 * Invoked from bazaarvoice_products_get_product_statistics() after retrieving
 * statistics from Bazaarvoice.
 *
 * @param array &$statistics
 *   Associative of statistics data for a product from Bazaarvoice. Elements
 *   vary depending on Bazaarvoice account, but will contain at minimum:
 *    - TotalReviewCount: the number of reviews for this product
 *    - AverageOverallRating: the average rating of product
 *    - OverallRatingRange: the rating range (default is 5).
 * @param string $product_id
 *   The product id.
 */
function hook_bazaarvoice_products_product_statistics_alter(array &$statistics, $product_id) {
  // Force rating range to be out of 6 instead of default of 5.
  $statistics['OverallRatingRange'] = 6;
}

/**
 * Allows modules to define fields that can be used as a product_id field.
 *
 * Invoked from bazaarvoice_products_get_entity_product_id_fields().
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @return array
 *   Associative array of fields. Key is the machine name of the field and value
 *   is the display label.
 *
 * @ingroup bazaarvoice_products_hooks
 */
function hook_bazaarvoice_products_entity_product_id_fields($type, $bundle) {
  $field_options = array();
  // Get entity info.
  $entity_info = entity_get_info($type);
  // Support label?
  if (isset($entity_info['entity keys']['label'])) {
    // Add the label as a field option.
    $field_options[$entity_info['entity keys']['label']] = t('@label (label)', array('@label' => ucwords($entity_info['entity keys']['label'])));
  }
  return $field_options;
}

/**
 * Perform an action after an entity type and bundle is set as a product type.
 *
 * Invoked from bazaarvoice_products_set_entity_type_product_id() after data
 * is saved to the database.
 *
 * @param string $type
 *   The Entity type.
 * @param string $bundle
 *   The entity bundle.
 * @param string $product_id_field
 *   The machine_name of the field used as the product_id.
 *
 * @ingroup bazaarvoice_products_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_products_set_type($type, $bundle, $product_id_field) {
  // Log action.
  watchdog('bazaarvoice_products', '!type !bundle set as product type with !field as product_id', array(
    '!type' => $type,
    '!bundle' => $bundle,
    '!field' => $product_id_field,
  ));
}

/**
 * Perform an action when an entity type bundle is removed as a product type.
 *
 * Invoked from bazaarvoice_products_delete_entity_type() after data is deleted
 * from the database.
 *
 * @param string $type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 *
 * @ingroup bazaarvoice_products_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_products_delete_type($type, $bundle) {
  // Log action.
  watchdog('bazaarvoice_products', '!type !bundle removed as a product type.', array(
    '!type' => $type,
    '!bundle' => $bundle,
  ));
}
