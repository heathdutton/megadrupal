<?php

class BookitItemEntityController extends EntityAPIController {

  /**
   * Creates a new bookable item entity object.
   */
  public function create(array $values = array()) {
    $values += array(
      'item_id' => NULL,
      'revision_id' => NULL,
      'title' => '',
      'uid' => '',
      'status' => 1,
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  /**
   * Saves a bookable item entity.
   */
  public function save($bookit_item, DatabaseTransaction $transaction = NULL) {
    global $user;

    // Determine if we will be inserting a new bookable item.
    if (!isset($bookit_item->is_new)) {
      $bookit_item->is_new = empty($bookit_item->item_id);
    }

    // Set the created time if not set.
    if (empty($bookit_item->created)) {
      $bookit_item->created = REQUEST_TIME;
    }

    // Set the changed time.
    $bookit_item->changed = REQUEST_TIME;

    // Set the revision time and uid.
    $bookit_item->revision_timestamp = REQUEST_TIME;
    $bookit_item->revision_uid = $user->uid;

    if ($bookit_item->is_new || !empty($bookit_item->revision)) {
      // When inserting either a new bookable item or revision, $entity->log must be set
      // because {bookit_item_revision}.log is a text column and therefore
      // cannot have a default value. However, it might not be set at this
      // point, so we ensure that it is at least an empty string in that case.
      if (!isset($bookit_item->log)) {
        $bookit_item->log = '';
      }
    }
    else if (empty($bookit_item->log)) {
      // If we are updating an existing bookable item without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($bookit_item->log);
    }

    parent::save($bookit_item, $transaction);
  }
}
