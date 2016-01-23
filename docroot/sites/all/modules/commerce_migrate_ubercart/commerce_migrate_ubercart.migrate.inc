<?php

/*
 * Implements hook_migrate_api().
 */
function commerce_migrate_ubercart_migrate_api() {

  $configured = variable_get('commerce_migrate_ubercart_source_database', FALSE);

  /**
   * Common arguments
   */
  $common_arguments = array(
    'group_name' => 'commerce_ubercart',
  );

  /**
   * Customer Billing Profile
   */
  $customer_billing_arguments = $common_arguments + array(
    'machine_name' => 'CommerceMigrateUbercartCustomerBillingProfile',
    'class_name' => 'CommerceMigrateUbercartCustomerBillingProfileMigration',
    'description' => t('Import customer billing profiles from Ubercart.'),
  );

  /**
   * Products
   */

  // Find out what types are present on the old site.
  $types = array();
  if ($configured != FALSE) {
    db_set_active(variable_get('commerce_migrate_ubercart_source_database', 'default'));
    $result = db_query("SELECT DISTINCT n.type AS type, ucp.pcid AS pcid, ucp.name AS name, ucp.description AS description FROM node n LEFT OUTER JOIN uc_product_classes ucp ON n.type = ucp.pcid WHERE ( (type IN  ('product')) OR (pcid IS NOT NULL ) )");
    db_set_active();
  }
  if (isset($result)) {
    foreach($result as $row) {
      $types[$row->type] = $row->type;
    }
  }

  // Find out what types are present in the new site.
  // Register product migrations, note which ones will have an associated node
  // migration.
  $target_types = commerce_product_types();
  foreach($types as $type) {
    if (isset($target_types[$type])) {
      $product_arguments[] = array(
        'machine_name' => 'CommerceMigrateUbercartProduct' . ucfirst($type),
        'class_name' => 'CommerceMigrateUbercartProductMigration',
        'description' => t('Import products from Ubercart.'),
        'type' => $type,
      );
      $products[] = 'CommerceMigrateUbercartProduct' . ucfirst($type);
      $migratable_types[] = ucfirst($type);
    }
    else {
      drupal_set_message(t("Product type :type does not exist. Create it to enable migrations of this type, then go to the configuration page and click 're-register statically-defined classes.'", array(':type' => $type)), 'warning');
    }
  }

  /**
   * Product Nodes
   */

  // Cycle through the product migrations that exist in both versions of the
  // site. Register only these.
  if (isset($products)) {
    foreach($migratable_types as $type) {
      $dependency_name = 'CommerceMigrateUbercartProduct' . ucfirst($type);
      $node_arguments[] = array(
        'machine_name' => 'CommerceMigrateUbercartNode' . ucfirst($type),
        'class_name' => 'CommerceMigrateUbercartNodeMigration',
        'description' => t('Import nodes from Ubercart.'),
        'dependencies' => array($dependency_name),
        'type' => $type,
        'products' => $products,
      );
    }
  }

  /**
   * Orders
   */
   $order_arguments = $common_arguments + array(
    'class_name' => 'CommerceMigrateUbercartOrderMigration',
    'description' => t('Import orders from Ubercart.'),
    'dependencies' => array('CommerceMigrateUbercartCustomerBillingProfile'),
   );

  /**
   * Transactions
   */
   $transaction_arguments = $common_arguments + array(
    'class_name' => 'CommerceMigrateUbercartTransaction',
    'description' => t('Import payment transactions from Ubercart.'),
    'dependencies' => array('CommerceMigrateUbercartOrder'),
   );

  /**
   * Line Items
   */
  if ($configured != FALSE) {
    if (isset($products)) {
    $line_item_arguments = $common_arguments + array(
      'class_name' => 'CommerceMigrateUbercartLineItem',
      'description' => t('Import order line items from Ubercart.'),
      'dependencies' => array('CommerceMigrateUbercartOrder'),
      'products' => $products,
     );
    }
  }

  /**
   * Shipping Line Items
   */
  if ($configured != FALSE) {
    $shipping_line_item_arguments = $common_arguments + array(
      'class_name' => 'CommerceMigrateUbercartShippingLineItem',
      'description' => t('Import shipping line items from Ubercart.'),
      'dependencies' => array('CommerceMigrateUbercartOrder'),
    );
  }

  /**
   * Register all the classes
   */
  $api = array(
    'api' => 2,
    'groups' => array(
    'commerce_ubercart' => array(
      'title' => t('Commerce Migrate Ubercart'),
      ),
    ),
  );

  if ($configured != FALSE) {
    $api['migrations'] = array(
      'CommerceMigrateUbercartCustomerBillingProfile' => $customer_billing_arguments,
      'CommerceMigrateUbercartOrder' => $order_arguments,
      'CommerceMigrateUbercartTransaction' => $transaction_arguments,
      'CommerceMigrateUbercartShippingLineItem' => $shipping_line_item_arguments,
    );

    if (isset($products)) {

      $api['migrations']['CommerceMigrateUbercartLineItem'] = $line_item_arguments;

      // Register all the product migrations
      foreach ($product_arguments as $arguments) {
        $arguments = array_merge_recursive($arguments, $common_arguments);
        $api['migrations'][$arguments['machine_name']] = $arguments;
      }

      // Register all the node migrations
      foreach ($node_arguments as $arguments) {
        $arguments = array_merge_recursive($arguments, $common_arguments);
        $api['migrations'][$arguments['machine_name']] = $arguments;
      }
    }

    // Only return if configured, migrate doesn't like empty migration groups.
    return $api;
  }
}
