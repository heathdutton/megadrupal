<?php

/**
 * @file commerce_migrate_magento.migrate.inc
 */

/*
 * Implements hook_migrate_api().
 */
function commerce_migrate_magento_migrate_api() {

  $configured = variable_get('commerce_migrate_magento_source_database', FALSE);

  /**
   * Stores
   */
  // Find out what stores are present on the old site.
  $migration_groups = $migrations_arguments = array();
  if ($configured != FALSE) {
    db_set_active(variable_get('commerce_migrate_magento_source_database', 'default'));
    $result = db_query("SELECT store_id, code, name FROM core_store");
    db_set_active();
  }

  // if the user sharing is Global use a Global class
  // 0 - Global, 1 - Site specific
  $user_sharing = FALSE;
  $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
  if ($user_sharing == 0) {
    // migration groups
    $group_name = 'commerce_magento_global';
    $migration_groups[$group_name] = array(
      'title' => t('Commerce Migrate Magento Store (@store_name)', array('@store_name' => 'Global')),
    );
    $common_arguments = array(
      'group_name' => $group_name,
    );
    $migrations_arguments[] = $common_arguments + array(
      'machine_name' => 'CommerceMagentoUserGlobal',
      'class_name' => 'CommerceMigrateMagentoUserMigration',
      'description' => t('Import users from Magento.'),
      'store_id' => 0,
    );
  }

  if (isset($result)) {
    foreach ($result as $row) {

      $store_id   = $row->store_id;
      $store_name = $row->name;
      $store_code = $row->code;
      $group_name = 'commerce_magento_' . $store_code;

      // migration groups
      $migration_groups[$group_name] = array(
        'title' => t('Commerce Migrate Magento Store (@store_name)', array('@store_name' => $store_name)),
      );

      /**
       * Common arguments
       */
      $common_arguments = array(
        'group_name' => $group_name,
      );

      /**
       * Users
       * customer/account_share/scope on core_config_data
       */
      if ($user_sharing) {
        $migrations_arguments[] = $common_arguments + array(
          'machine_name' => 'CommerceMagentoUser' . ucfirst($store_code),
          'class_name' => 'CommerceMigrateMagentoUserMigration',
          'description' => t('Import users from Magento.'),
          'store_id' => $store_id,
        );
      }

      /**
       * Customer Billing Profile
       */
      $customer_profile_migrations = array();
      $customer_profiles = array_filter(variable_get('commerce_migrate_magento_customer_profile_type', array()));
      if (!empty($customer_profiles)) {
        foreach ($customer_profiles as $machine_name => $customer_profile) {
          $migrations_arguments[] = $common_arguments + array(
            'machine_name' => 'CommerceMagentoCustomerProfile' . ucfirst($machine_name) . ucfirst($store_code),
            'class_name' => 'CommerceMigrateMagentoCustomerProfileMigration',
            'description' => t('Import customer @type profiles from Magento.', array('@type' => $machine_name)),
            'type' => $machine_name,
            'store_id' => $store_id,
            'store_code' => $store_code,
            'dependencies' => ($user_sharing == 1) ? array('CommerceMagentoUser' . ucfirst($store_code)) : array('CommerceMagentoUserGlobal'),
          );
          // @note : Removed the customer address dependency from orders since we have a clear relatioship with the following migrations with Orders
          // $customer_profile_migrations[] = 'CommerceMagentoCustomerProfile' . ucfirst($machine_name) . ucfirst($store_code);
          // there is a sales_flat_order_address table with address associate with orders, which doesn't exists as user accounts
          $migrations_arguments[] = $common_arguments + array(
            'machine_name' => 'CommerceMagentoOrderProfile' . ucfirst($machine_name) . ucfirst($store_code),
            'class_name' => 'CommerceMigrateMagentoOrderProfileMigration',
            'description' => t('Import customer @type profiles from Magento orders.', array('@type' => $machine_name)),
            'type' => $machine_name,
            'store_id' => $store_id,
            'store_code' => $store_code,
            'dependencies' => ($user_sharing == 1) ? array('CommerceMagentoUser' . ucfirst($store_code)) : array('CommerceMagentoUserGlobal'),
          );
          $customer_profile_migrations[] = 'CommerceMagentoOrderProfile' . ucfirst($machine_name) . ucfirst($store_code);
        }
      }

      /**
       * Products
       */
      $product_migrations = array();
      $products = array_filter(variable_get('commerce_migrate_magento_product_type', array()));
      if (!empty($products)) {
        foreach ($products as $machine_name => $product) {
          $migrations_arguments[] = $common_arguments + array(
            'machine_name' => 'CommerceMagentoProduct' . ucfirst($machine_name) . ucfirst($store_code),
            'class_name' => 'CommerceMigrateMagentoProductMigration',
            'description' => t('Import @type products from Magento.', array('@type' => $machine_name)),
            'type' => $machine_name,
            'store_id' => $store_id,
            'store_code' => $store_code,
          );
          $product_migrations[] = 'CommerceMagentoProduct' . ucfirst($machine_name) . ucfirst($store_code);
        }
      }

      /**
       * Product Display
       */
      $product_displays = array_filter(variable_get('commerce_migrate_magento_product_type', array()));
      if (!empty($product_displays)) {
        foreach ($product_displays as $machine_name => $product_display) {
          $migrations_arguments[] = $common_arguments + array(
            'machine_name' => 'CommerceMagentoProdDisp' . ucfirst($machine_name) . ucfirst($store_code),
            'class_name' => 'CommerceMigrateMagentoProductDisplayMigration',
            'description' => t('Import @type products from Magento.', array('@type' => $machine_name)),
            'dependencies' => $product_migrations,
            'type' => $machine_name,
            'store_id' => $store_id,
            'store_code' => $store_code,
            // @todo : may be we can use dependencies, but safe to keep this line too
            'products' => $product_migrations,
          );
        }
      }

      /**
       * Orders
       */
      $migrations_arguments[] = $common_arguments + array(
        'machine_name' => 'CommerceMagentoOrder' . ucfirst($store_code),
        'class_name' => 'CommerceMigrateMagentoOrderMigration',
        'description' => t('Import orders from Magento.'),
        'dependencies' => $customer_profile_migrations,
        'store_id' => $store_id,
        'store_code' => $store_code,
        // @todo : may be we can use dependencies, but safe to keep this line too
        'customer_profiles' => $customer_profile_migrations,
      );

      /**
       * Transactions
       */
      $migrations_arguments[] = $common_arguments + array(
        'machine_name' => 'CommerceMagentoTransaction' . ucfirst($store_code),
        'class_name' => 'CommerceMigrateMagentoTransactionMigration',
        'description' => t('Import payment transactions from Magento.'),
        'dependencies' => array('CommerceMagentoOrder' . ucfirst($store_code)),
        'store_id' => $store_id,
        'store_code' => $store_code,
      );

      /**
       * Line Items
       */
      if ($configured != FALSE) {
        if (isset($products)) {
          $migrations_arguments[] = $common_arguments + array(
            'machine_name' => 'CommerceMagentoLineItem' . ucfirst($store_code),
            'class_name' => 'CommerceMigrateMagentoLineItemMigration',
            'description' => t('Import order line items from Magento.'),
            'dependencies' => array('CommerceMagentoOrder' . ucfirst($store_code)) + $product_migrations,
            'store_id' => $store_id,
            'store_code' => $store_code,
            // @todo : may be we can use dependencies, but safe to keep this line too
            'products' => $product_migrations,
          );
        }
      }

      /**
       * Shipping Line Items
       */
      if ($configured != FALSE) {
        $migrations_arguments[] = $common_arguments + array(
          'machine_name' => 'CommerceMagentoShippingLineItem' . ucfirst($store_code),
          'class_name' => 'CommerceMigrateMagentoShippingLineItemMigration',
          'description' => t('Import shipping line items from Magento.'),
          'dependencies' => array('CommerceMagentoOrder' . ucfirst($store_code)),
          'store_id' => $store_id,
          'store_code' => $store_code,
        );
      }
    }
  }

  /**
   * Register all the classes
   */
  $api = array(
    'api' => 2,
    'groups' => $migration_groups,
  );

  if ($configured != FALSE) {
    foreach ($migrations_arguments as $migrations_argument) {
      $api['migrations'][$migrations_argument['machine_name']] = $migrations_argument;
    }
    // Only return if configured, migrate doesn't like empty migration groups.
    return $api;
  }
}

