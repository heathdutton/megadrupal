<?php
/**
 * @file
 * Provides custom drush commands and callbacks.
 */

/**
 * Implements hook_drush_command().
 */
function bazaarvoice_products_drush_command() {
  $items = array();

  $items['bazaarvoice-products'] = array(
    'callback' => 'drush_bazaarvoice_products_bazaarvoice_products',
    'description' => "Prints a list of Entity type bundles and product id fields that are marked as Bazaarvoice products.",
    'aliases' => array('bv-products', 'bvp'),
    'options' => array(
      'type' => 'Entity types to limit list to',
    ),
    'drupal dependencies' => array('bazaarvoice_products'),
  );

  $items['bazaarvoice-products-add'] = array(
    'callback' => 'drush_bazaarvoice_products_bazaarvoice_products_add',
    'description' => "Add an entity bundle as a Bazaarvoice product type.",
    'aliases' => array('bv-products-add', 'bvpa'),
    'arguments' => array(
      'type' => 'Entity type name',
      'bundle' => 'Entity bundle name',
      'product_id_field' => 'Machine name of field to use as product id (defaults to title field)',
    ),
    'drupal dependencies' => array('bazaarvoice_products'),
  );

  $items['bazaarvoice-products-delete'] = array(
    'callback' => 'drush_bazaarvoice_products_bazaarvoice_products_delete',
    'description' => "Delete an entity bundle as a Bazaarvoice product type.",
    'aliases' => array('bv-products-delete', 'bvpd'),
    'arguments' => array(
      'type' => 'Entity type name',
      'bundle' => 'Entity bundle name',
    ),
    'drupal dependencies' => array('bazaarvoice_products'),
  );

  return $items;
}

/**
 * Drush callback for listing product entity types.
 */
function drush_bazaarvoice_products_bazaarvoice_products() {
  // Get list of product types.
  $products = bazaarvoice_products_get_product_entity_types();
  // Filtering by a particular type?
  if ($type = drush_get_option('type', '')) {
    // Is product type set?
    if (isset($products[$type])) {
      $products = array(
        $type => $products[$type],
      );
    }
    else {
      drupal_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('@type has no bundles marked as Bazaarvoice products.', array(
        '@type' => $type,
      )));
    }
  }
  // Have an array of products?
  if (is_array($products)) {
    // Build a table to display.
    $rows = array();
    $rows[] = array(dt('Type'), dt('Bundle'), dt('Product Id Field'));

    foreach ($products as $bundles) {
      foreach ($bundles as $data) {
        $rows[] = array_values($data);
      }
    }

    drush_print_table($rows, TRUE);
  }
  else {
    drush_set_error('BAZAARVOICE_PRODUCTS_INVALID', dt('No entity bundles marked as Bazaarvoice products.'));
  }
}

/**
 * Drush callback for setting an entity bundle as a Bazaarvoice product type.
 *
 * @param null|string $type
 *   The Entity type name.
 * @param null|string $bundle
 *   The Entity bundle name.
 * @param null|string $field
 *   The product id field machine name. (optional).
 */
function drush_bazaarvoice_products_bazaarvoice_products_add($type = NULL, $bundle = NULL, $field = NULL) {
  // Able to get entity info?
  if ($type && $bundle) {
    if ($entity_info = entity_get_info($type)) {
      // Bundle fieldable?
      if (field_info_instances($type, $bundle)) {
        // Able to get a list of valid id fields for this bundle?
        if ($id_fields = bazaarvoice_products_get_entity_product_id_fields($type, $bundle)) {
          // No field passed?
          if (!$field) {
            // Assume field used for label if it is present.
            $field = isset($entity_info['entity keys']['label']) ? $entity_info['entity keys']['label'] : 'label';
          }
          // Passed field a valid id field?
          if (isset($id_fields[$field])) {
            // Set the bundle as an entity type.
            bazaarvoice_products_set_entity_type_product_id($type, $bundle, $field);
            // Print message.
            $msg = dt('@type @bundle set as Bazaarvoice Product with @field as product_id.', array(
              '@type' => $type,
              '@bundle' => $bundle,
              '@field' => $field,
            ));
            drush_log($msg);
            drush_print($msg);
          }
          else {
            drush_set_error('BAZAARVOICE_PRODUCTS_INVALID', dt('@field is not a field that can be used as a product_id for @type @bundle.', array(
              '@field' => $field,
              '@type' => $type,
              '@bundle' => $bundle,
            )));
          }

        }
        else {
          drush_set_error('BAZAARVOICE_PRODUCTS_INVALID', dt('@type @bundle does not have any fields that can be used as a product_id.', array(
            '@type' => $type,
            '@bundle' => $bundle,
          )));
        }
      }
      else {
        drush_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('@bundle is not a fieldable bundle of @type', array(
          '@bundle' => $bundle,
          '@type' => $type,
        )));
      }
    }
    else {
      drush_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('@type is an invalid entity type.', array(
        '@type' => $type,
      )));
    }
  }
  else {
    drush_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('Must provide an entity type and bundle.'));
  }
}

/**
 * Drush callback for removing an entity bundle as a Bazaarvoice product type.
 *
 * @param null|string $type
 *   The Entity type.
 * @param null|string $bundle
 *   The Entity bundle.
 */
function drush_bazaarvoice_products_bazaarvoice_products_delete($type = NULL, $bundle = NULL) {
  // Able to get entity info?
  if ($type && $bundle) {
    // Is a bazaarvoice product type?
    if (bazaarvoice_products_is_product_type($type, $bundle)) {
      // Remove as a product type.
      bazaarvoice_products_delete_entity_type($type, $bundle);
      // Print drush message.
      $msg = dt('@type @bundle removed as Bazaarvoice Product.', array(
        '@type' => $type,
        '@bundle' => $bundle,
      ));
      drush_log($msg);
      drush_print($msg);
    }
    else {
      drush_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('@type @bundle is not a Bazaarvoice product type.', array(
        '@type' => $type,
      )));
    }
  }
  else {
    drush_set_error('BAZAARVOICE_PRODUCTS_INVALID_INPUT', dt('Must provide an entity type and bundle.'));
  }
}
