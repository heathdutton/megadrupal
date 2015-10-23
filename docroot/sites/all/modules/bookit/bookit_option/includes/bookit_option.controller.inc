<?php

class BookitOptionEntityController extends EntityAPIController {

  /**
   * Create a new booking option.
   */
  public function create(array $values = array()) {
    $values += array(
      'title' => '',
      'uid' => '',
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  /**
   * Saves a booking option.
   */
  public function save($bookit_option, DatabaseTransaction $transaction = NULL) {
    global $user;

    // Set the created time if not set.
    if (empty($bookit_option->created)) {
      $bookit_option->created = REQUEST_TIME;
    }

    // Set the uid if not set.
    if (empty($bookit_option->uid)) {
      $bookit_option->uid = $user->uid;
    }

    // Set the changed time.
    $bookit_option->changed = REQUEST_TIME;

    parent::save($bookit_option, $transaction);
  }
}
