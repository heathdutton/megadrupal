<?php

/**
 * Node class for the CPS Entity management system.
 */
class CPSEntityNode extends CPSEntityPublishableBase {
  /**
   * @{inheritdoc}
   */
  public function unpublish($entity) {
    $entity->status = NODE_NOT_PUBLISHED;
  }

  /**
   * @{inheritdoc}
   */
  public function publish($entity) {
    // Entities which have a published flag must implement this specifically.
    $entity->status = NODE_PUBLISHED;
  }

  /**
   * @{inheritdoc}
   */
  public function isPublished($entity) {
    return $entity->status != NODE_NOT_PUBLISHED;
  }
}
