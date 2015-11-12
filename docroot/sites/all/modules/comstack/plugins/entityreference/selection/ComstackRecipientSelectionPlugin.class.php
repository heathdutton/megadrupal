<?php

/**
 * Comstack recipient selection handler.
 */
class ComstackRecipientSelectionPlugin extends EntityReference_SelectionHandler_Generic_user {
  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new ComstackRecipientSelectionPlugin($field, $instance, $entity_type, $entity);
  }

  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    global $user;
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    // Exclude the current user.
    $query->entityCondition('entity_id', $user->uid, '!=');

    // The user entity doesn't have a label column.
    if (isset($match)) {
      $query->propertyCondition('name', $match, $match_operator);
    }

    // Prevent in-active/blocked users from appearing in this list.
    $query->propertyCondition('status', 1);

    // Add a tag to the query so that other modules can alter it later.
    // Use cases for this could be to restrict the list of users by role, or by
    // a friends list etc.
    $query->addTag('comstack_recipients');
    $query->addMetaData('entityreference_selection_handler', $this);

    return $query;
  }

  public function entityFieldQueryAlter(SelectQueryInterface $query) {
  }
}
