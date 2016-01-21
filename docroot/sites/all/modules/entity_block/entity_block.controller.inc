<?php

/**
 * @file
 * The controller for the EntityBlock entity.
 */

/**
 * Interface for Entity Block.
 */
interface EntityBlockControllerInterface
  extends DrupalEntityControllerInterface {

  /**
   * Creates an entity.
   */
  public function create();

  /**
   * Saves an entity.
   *
   * @param object $entity
   *   The entity to save.
   */
  public function save($entity);

  /**
   * Deletes an entity.
   *
   * @param object $entity
   *   The entity to delete.
   */
  public function delete($entity);

}

/**
 * The controller class for Entity Block.
 */
class EntityBlockController
  extends DrupalDefaultEntityController
  implements EntityBlockControllerInterface {

  /**
   * Create and return a new entity_block entity.
   */
  public function create() {
    $entity = new stdClass();
    $entity->type = 'entity_block';
    $entity->bundle = 'entity_block';
    $entity->title = '';
    $entity->entity_block_id = 0;
    $entity->target_entity_type = 'node';
    $entity->target_bundle = '';
    $entity->target_view_mode = '';
    $entity->target_entity_id = '';
    return $entity;
  }

  /**
   * Saves the entity.
   */
  public function save($entity) {
    // Invoke hook_entity_presave().
    module_invoke_all('entity_presave', $entity, 'entity_block');

    // Get the primary key.
    $primary_keys = $entity->entity_block_id ? 'entity_block_id' : array();

    // Save the entity.
    drupal_write_record('entity_block', $entity, $primary_keys);

    // Callback hooks.
    $invocation = 'entity_insert';
    if (empty($primary_keys)) {
      field_attach_insert('entity_block', $entity);
    }
    else {
      field_attach_update('entity_block', $entity);
      $invocation = 'entity_update';
    }
    module_invoke_all($invocation, $entity, 'entity_block');

    return $entity;
  }

  /**
   * Deletes an entity_block.
   */
  public function delete($entity) {
    $this->deleteMultiple(array($entity));
  }

  /**
   * Deletes multiple entity_blocks.
   *
   * @param array $entities
   *   An array of entity IDs or a single numeric ID.
   * @throws \Exception
   */
  public function deleteMultiple($entities) {
    $entity_block_ids = array();
    if (!empty($entities)) {
      $transaction = db_transaction();
      try {
        foreach ($entities as $entity) {
          // Invoke hook_entity_delete().
          module_invoke_all('entity_delete', $entity, 'entity_block');
          field_attach_delete('entity_block', $entity);
          $entity_block_ids[] = $entity->entity_block_id;
        }
        db_delete('entity_block')
          ->condition('entity_block_id', $entity_block_ids, 'IN')
          ->execute();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('entity_block', $e);
        throw $e;
      }
    }
  }
}