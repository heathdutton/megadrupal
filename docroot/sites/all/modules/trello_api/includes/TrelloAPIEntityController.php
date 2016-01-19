<?php
/**
 * @file
 * Contains the TrelloAPIEntityController class.
 */

/**
 * Base class for Trello entity controllers.
 */
class TrelloAPIEntityController extends EntityAPIController {

  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Try to fetch the entity id key if the trello_id already exists.
    if (empty($entity->{$this->idKey})) {
      $id = db_select($this->entityInfo['base table'], 'base_table')
        ->fields('base_table', array($this->idKey))
        ->condition('trello_id', $entity->trello_id)
        ->execute()
        ->fetchField();
      // Mark the entity for update.
      if ($id) {
        $entity->{$this->idKey} = $id;
        $entity->is_new = FALSE;
      }
    }
    parent::save($entity, $transaction);
  }

}
