<?php

/**
 * The controller class used for user notifications state entities
 */
class UserNotificationsStateController extends EntityAPIControllerExportable {

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    if ($entity->default_state == 1) {
      $query = db_update('user_notifications_state')
        ->fields(array('default_state' => 0))
        ->condition('user_notifications_state_id', $entity->user_notifications_state_id, '<>');
      $query->execute();
    }
  }

  /**
   * Permanently deletes the entity.
   *
   * TODO: We need the seen and unseen entity types so for now don't allow them
   * to be deleted, recognizing this isn't the proper way to implement this but
   * for now we'll do it...
   *
   * @see entity_delete()
   */
  public function delete($ids){
    if ($this->name == 'seen' || $this->name == 'unseen') {
      //TODO: NEEDS TO BE FIXED
      drupal_set_message(t('Cannot delete default states.'), 'error');
    } else {
      parent::delete($ids, $transaction);
    }
  }

}
