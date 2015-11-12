<?php
/**
 * @file
 */

/**
 * Entity controller class.
 */
class ReviewEntityController extends EntityAPIController {
  public $entity;

  /**
   * If user is no administer or moderator and the review needs to be moderated.
   *
   * @param object $entity
   *
   * @return bool
   */
  private function _needs_approval($entity) {
    return variable_get("entity_review_{$entity->bundle}_needs_approval", FALSE)
      && !user_access('administer entity_review')
      && !user_access('moderate entity_review');
  }

  /**
   * Set the review entity to previous revision.
   *
   * @param object $entity
   *
   * @return bool
   */
  private function setPreviousRevision($entity) {
    $vids = db_select('entity_review_revision', 'r')
      ->fields('r', array('vid'))
      ->condition('r.id', $entity->id)
      ->condition('r.vid', $entity->vid, '<>')
      ->execute()->fetchCol();

    if (!empty($vids)) {
      $entity_revision = entity_revision_load('review', max($vids));
      entity_revision_set_default('review', $entity_revision);
      $entity_revision->save();

      return TRUE;
    }

    return FALSE;
  }

  public function deleteRevision($revision_id) {
    if ($entity_revision = entity_revision_load($this->entityType, $revision_id)) {
      if (entity_revision_is_default($this->entityType, $entity_revision)) {
        // Try to set previous version or delete.
        $this->setPreviousRevision($entity_revision)
          ? $this->deleteRevision($revision_id)
          : $entity_revision->delete();
      }
      else {
        parent::deleteRevision($revision_id);
      }
    }
  }

  public function save($entity) {
    $entity = (object) $entity;

    $entity->timestamp = REQUEST_TIME;

    // Set the timestamp fields.
    if (empty($entity->created)) {
      $entity->created = REQUEST_TIME;
    }

    $needs_approval = $this->_needs_approval($entity);

    if ($needs_approval) {
      // Review is updated.
      if (isset($entity->vid)) {
        $entity->old_vid = $entity->vid;
        $entity->is_new_revision = TRUE;
      }
    }

    parent::save($entity);

    // Roll revision back.
    if ($needs_approval && isset($entity->old_vid)) {
      $previous_revisions = $this->load(FALSE, array('vid' => $entity->old_vid));
      $previous_revision = reset($previous_revisions);

      entity_revision_set_default('review', $previous_revision);

      parent::save($previous_revision);
    }
  }
}
