<?php

/**
 * @file
 * The controller for the return entity containing the CRUD operations.
 */

/**
 * The controller class for returns contains methods for the return CRUD
 * operations. The load method is inherited from the default controller.
 */
class CommerceReturnEntityController extends EntityAPIController {

  /**
   * Create a default return.
   *
   * @param array $values
   *   An array of values to set, keyed by property name. A fully loaded
   *   commerce_order object has to be passed into values array associated with
   *   the `order` key.
   *
   * @return CommerceReturn|bool
   *   A CommerceReturn object with all default fields initialized. False
   *   otherwise.
   */
  public function create(array $values = array()) {
    global $user;
    if (
      !isset($values['order'])
      &&
      entity_metadata_wrapper('commerce_order', $values['order'])->type() !== 'commerce_order'
    ) {
      watchdog_exception(
        $this->entityType,
        new Exception('Cannot create an instance of CommerceReturn without a passed commerce_order entity type.')
      );

      return FALSE;
    }

    $values += array(
      'return_id' => NULL,
      'return_number' => NULL,
      'revision_id' => NULL,
      'mail' => (!empty($values['uid']) && ($account = user_load($values['uid']))) ? $account->mail : $user->mail,
      'data' => array(),
      'created' => '',
      'changed' => '',
      'hostname' => '',
      'commerce_order_reference' => array(
        LANGUAGE_NONE => array(
          '0' => array(
            'target_id' => $values['order']->order_id
          )
        )
      ),
    );

    // Seeking for product line items to be attached to return.
    $line_items = commerce_return_new_configure_line_items($values['order']);
    if (!empty($line_items)) {
      $data = array();
      foreach (array_keys($line_items) as $delta => $line_item_id) {
        $data[$delta] = array('line_item_id' => $line_item_id);
      }

      $values += array(
        'commerce_return_line_items' => array(
          LANGUAGE_NONE => $data,
        )
      );
    }

    // Try to attach a RMA customer profile on return.
    $profile = commerce_return_new_configure_customer_profile($values['order']);
    if (!empty($profile)) {
      $values += array(
        'commerce_customer_rma' => array(
          LANGUAGE_NONE => array(
            '0' => array(
              'profile_id' => $profile->profile_id,
            )
          )
        )
      );
    }

    // Remove the order object since we have attached the reference to the order
    // on commerce_order_reference field.
    unset($values['order']);

    return parent::create($values);
  }

  /**
   * Saves a return.
   *
   * When saving a return without a return ID, this function will create a new
   * return at that time. For new returns, it will also determine and save the
   * return number and then save the initial revision of the return. Subsequent
   * returns that should be saved as new revisions should set $return->revision
   * to TRUE and include a log string in $return->log.
   *
   * @param object $return
   *   The full return object to save.
   * @param DatabaseTransaction $transaction
   *   An optional transaction object.
   *
   * @throws Exception
   *   Throw a new PHP exception if the query has failed.
   *
   * @return int
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($return, DatabaseTransaction $transaction = NULL) {
    if (!isset($transaction)) {
      $transaction = db_transaction();
      $started_transaction = TRUE;
    }

    try {
      global $user;

      // Determine if we will be inserting a new return.
      $return->is_new = empty($return->return_id);

      // Set the timestamp fields.
      if ($return->is_new && empty($return->created)) {
        $return->created = REQUEST_TIME;
        $return->hostname = ip_address();
      }
      else {
        // Otherwise if the return is not new but comes from an entity_create()
        // or similar function call that initializes the created timestamp, uid,
        // and hostname values to empty strings, unset them to prevent
        // destroying existing data in those properties on update.
        if ($return->created === '') {
          unset($return->created);
        }
        if ($return->uid === '') {
          unset($return->uid);
        }
        if (property_exists($return, 'hostname') && $return->hostname === '') {
          unset($return->hostname);
        }
      }

      $return->changed = REQUEST_TIME;

      $return->revision_timestamp = REQUEST_TIME;
      $return->revision_hostname = ip_address();
      $return->revision_uid = $user->uid;

      // Recalculate the return total using the current line item data.
      commerce_return_calculate_total($return);

      if ($return->is_new || !empty($return->revision)) {
        // When inserting either a new return or revision, $return->log must be set
        // because {commerce_return_revision}.log is a text column and therefore
        // cannot have a default value. However, it might not be set at this
        // point, so we ensure that it is at least an empty string in that case.
        if (!isset($return->log)) {
          $return->log = '';
        }
      }
      elseif (empty($return->log)) {
        // If we are updating an existing return without adding a new revision,
        // we need to make sure $return->log is unset whenever it is empty. As
        // long as $return->log is unset, drupal_write_record() will not attempt
        // to update the existing database column when re-saving the revision.
        unset($return->log);
      }

      return parent::save($return, $transaction);
    } catch (Exception $e) {
      if (!empty($started_transaction)) {
        $transaction->rollback();
        watchdog_exception($this->entityType, $e);
      }
      throw $e;
    }
  }

  /**
   * Unserializes the data property of loaded returns.
   */
  public function attachLoad(&$queried_returns, $revision_id = FALSE) {
    foreach ($queried_returns as $return_id => &$return) {
      $return->data = is_string($return->data) ? unserialize($return->data) : FALSE;
    }

    // Call the default attachLoad() method.
    parent::attachLoad($queried_returns, $revision_id);
  }
}
