<?php

/**
 * @file
 * The controller for the product entity containing the CRUD operations.
 */

/**
 * The controller class for products contains methods for the product CRUD
 * operations. The load method is inherited from the default controller.
 */
class CommerceProductKeyEntityController extends DrupalDefaultEntityController {

  /**
   * Create a default product.
   *
   * @param $type
   *   The machine-readable type of the product.
   *
   * @return
   *   A product object with all default fields initialized.
   */
  public function create($type = '') {
    return (object) array(
      'product_key_id' => '',
      'key_owner_id' => NULL,
      'order_id' => NULL,
      'is_new' => TRUE,
      'code' => '',
      'type' => $type,
      'activated' => 0,
      'revoked' => 0,
      'uid' => '',
      'status' => 1,
      'created' => '',
      'changed' => '',
    );
  }

  /**
   * Saves a product.
   *
   * @param $product_key
   *   The full product object to save.
   *
   * @return
   *   The saved product object.
   */
  public function save($product_key) {
    $transaction = db_transaction();

    try {
      $product_key->changed = REQUEST_TIME;

      // Give modules the opportunity to prepare field data for saving.
      rules_invoke_all('commerce_product_key_presave', $product_key);
      field_attach_presave('commerce_product_key', $product_key);

      // If this is a new product...
      if ((isset($product_key->is_new) && $product_key->is_new) || empty($product_key->product_key_id)) {
        // Set the creation timestamp if not set.
        if (!isset($product_key->created) || empty($product_key->created)) {
          $product_key->created = REQUEST_TIME;
        }

        // Save the new product and fields.
        drupal_write_record('commerce_product_key', $product_key);
        field_attach_insert('commerce_product_key', $product_key);

        $op = 'insert';
      }
      else {
        // Invoke presave to let modules react before the save.
        rules_invoke_all('commerce_product_key_presave', $product_key);
        // Save the updated product and fields.
        drupal_write_record('commerce_product_key', $product_key, 'product_key_id');
        field_attach_update('commerce_product_key', $product_key);

        $op = 'update';
      }

      module_invoke_all('commerce_product_key_' . $op, $product_key);
      module_invoke_all('entity_' . $op, $product_key, 'commerce_product_key');
      rules_invoke_event('commerce_product_key_' . $op, $product_key);

      // Ignore slave server temporarily to give time for the
      // saved product to be propagated to the slave.
      db_ignore_slave();

      return $product_key;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('commerce_product_key', $e);
      throw $e;
    }
  }

  /**
   * Unserializes the data property of loaded products.
   */
  public function attachLoad(&$queried_products, $revision_id = FALSE) {
    foreach ($queried_products as $product_key_id => &$product_key) {
      $product_key->data = unserialize($product_key->data);
    }

    // Call the default attachLoad() method. This will add fields and call
    // hook_commerce_product_key_load().
    parent::attachLoad($queried_products, $revision_id);
  }

  /**
   * Deletes multiple products by ID.
   *
   * @param $product_key_ids
   *   An array of product IDs to delete.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($product_key_ids) {
    if (!empty($product_key_ids)) {
      $product_keys = $this->load($product_key_ids, array());

      // Ensure the products can actually be deleted.
      foreach ((array) $product_keys as $product_key_id => $product_key) {
        if (in_array(FALSE, module_invoke_all('commerce_product_key_can_delete', $product_key))) {
          unset($product_keys[$product_key_id]);
        }
      }

      // If none of the specified products can be deleted, return FALSE.
      if (empty($product_keys)) {
        return FALSE;
      }

      $transaction = db_transaction();

      try {
        db_delete('commerce_product_key')
          ->condition('product_key_id', $product_key_ids, 'IN')
          ->execute();

        foreach ($product_keys as $product_key_id => $product_key) {
          module_invoke_all('commerce_product_key_delete', $product_key);
          field_attach_delete('commerce_product_key', $product_key);
          rules_invoke_event('commerce_product_key_delete', $product_key);
          module_invoke_all('entity_delete', $product_key, 'commerce_product_key');
        }

        // Ignore slave server temporarily to give time for the
        // saved product to be propagated to the slave.
        db_ignore_slave();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('commerce_product_key', $e);
        throw $e;
      }

      // Clear the page and block and product_key_load_multiple caches.
      cache_clear_all();
      $this->resetCache();
    }

    return TRUE;
  }
}
