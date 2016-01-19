<?php

/**
 * Provides a friend wrapper that allows us to access protected methods.
 *
 * EntityAPIController defines the saveRevision method as protected, meaning
 * it can't be called from outside. By creating a class that extends the
 * class, and then using a static method as a pass-through, we can access
 * the protected method safely.
 */
class CPSEntityAPIControllerFriend extends EntityAPIController {
  /**
   * Pass through to the protected saveRevision method.
   *
   * @param EntityAPIController $protected
   *   The controller to pass through to.
   *
   * @param Entity $entity
   *   The entity to save the revision for.
   *
   * @return int
   *   The success status of the revision save.
   *
   * @see EntityAPIController::saveRevision()
   */
  public static function protectedSaveRevision(EntityAPIController $protected, Entity $entity) {
    return $protected->saveRevision($entity);
  }
}
