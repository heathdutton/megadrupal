<?php

/**
 * @file
 * Class that defines a Drupal User Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

class User extends Entity {

  /**
   * @inheritdoc
   */
  protected function createEntity($languageId, $entityId = NULL) {
    if ($userWrapper = parent::createEntity($languageId, $entityId)) {
      $roles = array(DRUPAL_AUTHENTICATED_RID);
      $userWrapper->roles->set(drupal_map_assoc($roles));
    }

    return $userWrapper;
  }

}
