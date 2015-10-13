<?php
/**
 * @file
 * Provides the class for managing user data sets.
 */

/**
 * Controller class for CRM integration.
 */
class PartyUserDataSet extends PartyDefaultDataSet {

  /**
   * Get the user label.
   */
  public function getLabel($delta) {
    $entity = $this->getEntity($delta);
    return format_username($entity);
  }

  /**
   * Act on party delete.
   */
  public function hook_party_delete() {
    foreach ($this->getEntities() as $user) {
      $this->detachEntity($user);

      $method = variable_get('party_user_party_delete_action', 'disallow');
      if ($method != 'disallow') {
        user_cancel(array(), $user->uid, $method);
      }
    }

    $this->clear();
  }

  /**
   * {@inheritdoc}
   */
  static function registerMergeProcesses($master_controller, $merger_controller) {
    // Merging of web accounts is not currently supported.
    return array();
  }
}
