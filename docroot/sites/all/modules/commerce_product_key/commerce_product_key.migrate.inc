<?php

/**
 * @file
 * Support for migrate module (product key beeing migrate destination).
 *
 * Based on node destination implementation (migrate/plugins/destinations/node.inc).
 */


/**
 * Destination class implementing migration into nodes.
 */
class MigrateDestinationCommerceProductKey extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'product_key_id' => array(
        'description' => 'The primary identifier for a product key, used internally only.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Return an options array for product key destinations.
   *
   * @param string $language
   *  Default language for product keys created via this destination class.
   */
  static public function options($language) {
    return compact('language');
  }

  /**
   * Basic initialization
   *
   * @param string $bundle
   *  The product key type.
   * @param array $options
+   *  Options applied to product keys.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct('commerce_product_key', $bundle, $options);
  }

  /**
   * Returns a list of fields available to be mapped for the product key type (bundle)
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    // First the core (commerce_product_key table) properties
    $fields['product_key_id'] = t('CommerceProductKey: Existing PK ID');
    $fields['order_id'] = t('CommerceProductKey: Order which this PK belongs to');
    $fields['is_new'] = t('CommerceProductKey: Boolean indicating wether this PK is new or existing');
    $fields['key_owner_id'] = t('CommerceProductKey: User which this PK belongs to');
    $fields['uid'] = t('CommerceProductKey: User which created this PK');
    $fields['code'] = t('CommerceProductKey: Premium key itself');
    $fields['activated'] = t('CommerceProductKey: Boolean indicating wether this PK was activated');
    $fields['revoked'] = t('CommerceProductKey: Boolean indicating wether this PK was revoked');
    $fields['status'] = t('CommerceProductKey: Boolean indicating wether this PK should be displayed to the user');
    $fields['language'] = t('CommerceProductKey: Language of this PK');
    $fields['created'] = t('CommerceProductKey: Date when PK was created');
    $fields['changed'] = t('CommerceProductKey: Date when PK was changed');
    $fields['data'] = t('CommerceProductKey: A serialized array of additional data');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle);
    $fields += migrate_handler_invoke_all('CommerceProductKey', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Delete a batch of product keys at once.
   *
   * @param $product_key_ids
   *  Array of product key IDs to be deleted.
   */
  public function bulkRollback(array $product_key_ids) {
    migrate_instrument_start('commerce_product_key_delete_multiple');
    $this->prepareRollback($nids);
    commerce_product_key_delete_multiple($product_key_ids);
    $this->completeRollback($nids);
    migrate_instrument_stop('commerce_product_key_delete_multiple');
  }

  /**
   * Import a single product key.
   *
   * @param $product_key
   *  Product key object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (product_key_id only in this case) of the product key that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $product_key, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      // Make sure is_new is off
      $product_key->is_new = FALSE;
      if (isset($product_key->product_key_id)) {
        if ($product_key->product_key_id != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming product_key_id !product_key_id and map destination product_key_id !destid1 don't match",
            array('!product_key_id' => $product_key->product_key_id, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $product_key->product_key_id = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($product_key->product_key_id)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination product_key_id provided'));
      }
      $old_product_key = commerce_product_key_load($product_key->product_key_id);
      if (!isset($product_key->created)) {
        $product_key->created = $old_product_key->created;
      }
      if (!isset($product_key->status)) {
        $product_key->status = $old_product_key->status;
      }
      if (!isset($product_key->activated)) {
        $product_key->activated = $old_product_key->activated;
      }
      if (!isset($product_key->revoked)) {
        $product_key->revoked = $old_product_key->revoked;
      }
      if (!isset($product_key->key_owner_id)) {
        $product_key->key_owner_id = $old_product_key->key_owner_id;
      }
      if (!isset($product_key->uid)) {
        $product_key->uid = $old_product_key->uid;
      }
    }

    // Set some required properties.
    // Set type before invoking prepare handlers - they may take type-dependent actions.
    $product_key->type = $this->bundle;

    if ($migration->getSystemOfRecord() == Migration::SOURCE) {
      // Apply defaults
      if (!isset($product_key->language)) {
        $product_key->language = $this->language;
      }

      if (isset($product_key->created)) {
        $product_key->created = MigrationBase::timestamp($product_key->created);
      }
      if (isset($node->changed)) {
        $changed = MigrationBase::timestamp($product_key->changed);
      }


    }

    // Invoke migration prepare handlers
    $this->prepare($product_key, $row);

    // Trying to update an existing product key
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      // Incoming data overrides existing data, so only copy non-existent fields
      foreach ($old_product_key as $field => $value) {
        // An explicit NULL in the source data means to wipe to old value (i.e.,
        // don't copy it over from $old_product_key)
        if (property_exists($product_key, $field) && $product_key->$field === NULL) {
          // Ignore this field
        }
        elseif (!isset($product_key->$field)) {
          $product_key->$field = $old_product_key->$field;
        }
      }
    }

    if (isset($product_key->product_key_id) && !(isset($product_key->is_new) && $product_key->is_new)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('commerce_product_key_save');
    commerce_product_key_save($product_key);
    migrate_instrument_stop('commerce_product_key_save');

    if (isset($product_key->product_key_id)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }

      // Update changed timestamp.
      if (isset($changed)) {
        db_update('commerce_product_key')
          ->fields(array('changed' => $changed))
          ->condition('product_key_id', $product_key->product_key_id)
          ->execute();
        $product_key->changed = $changed;
      }

      $return = array($product_key->product_key_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($product_key, $row);
    return $return;
  }
}
