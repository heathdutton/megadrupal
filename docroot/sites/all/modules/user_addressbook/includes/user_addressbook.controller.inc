<?php

/**
 * @file
 * The controller for the user address entity containing the CRUD operations.
 */

/**
 * The controller class for user addresses contains methods for the user address
 * CRUD operations. The load method is inherited from the default controller.
 */
class UserAddressEntityController  extends EntityAPIController {

  /**
   * Create a default user address.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A user address object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'address_id' => NULL,
      'name' => '',
      'uid' => '',
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  /**
   * Unserializes the data property of loaded user addresses.
   */
  public function attachLoad(&$queried_addresses, $revision_id = FALSE) {
    foreach ($queried_addresses as $address_id => &$user_address) {
      $user_address->data = unserialize($user_address->data);
    }

    // Call the default attachLoad() method. This will add fields and call
    // hook_user_addressbook_user_address_load().
    parent::attachLoad($queried_addresses, $revision_id);
  }

  /**
   * Saves an user address.
   *
   * When saving a user address without an ID, this function will create a new
   * user address at that time.
   *
   * @param $user_address
   *   The full user address object to save.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($user_address, DatabaseTransaction $transaction = NULL) {
    if (!isset($transaction)) {
      $transaction = db_transaction();
      $started_transaction = TRUE;
    }

    try {
      global $user;

      // Determine if we will be inserting a new profile.
      $user_address->is_new = empty($user_address->address_id);

      // Set the timestamp fields.
      if ($user_address->is_new && empty($user_address->created)) {
        $user_address->created = REQUEST_TIME;
      }
      else {
        // Otherwise if the profile is not new but comes from an entity_create()
        // or similar function call that initializes the created timestamp and
        // uid value to empty strings, unset them to prevent destroying existing
        // data in those properties on update.
        if ($user_address->created === '') {
          unset($user_address->created);
        }
        if ($user_address->uid === '') {
          unset($user_address->uid);
        }
      }

      $user_address->changed = REQUEST_TIME;

      return parent::save($user_address, $transaction);
    }
    catch (Exception $e) {
      if (!empty($started_transaction)) {
        $transaction->rollback();
        watchdog_exception($this->entityType, $e);
      }
      throw $e;
    }
  }
}
