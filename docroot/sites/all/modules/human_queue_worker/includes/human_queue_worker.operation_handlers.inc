<?php

/**
 * @file
 * Contains handler classes for entity operations.
 *
 */

/**
 * Operation which removes the item from the queue and does nothing to it.
 *
 * Entities that use this for their queues may wish to subclass this and set
 * conditions for when this can be used by overriding operationAccess().
 */
class HumanQueueWorkerOperationUnQueue extends EntityOperationsOperationForm {

  public $access_verb = 'unqueue';

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Remove from queue',
      'page title' => 'Remove %label from queue',
      'button label' => t('Remove from the queue'),
      'confirm question' => t('Are you sure you want to remove this %entity-type from the queue?'),
      'submit message' => t('%entity-type %label has been removed from the queue.'),
    );
  }

  /**
   * Form submit handler for this operation.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Do nothing. human_queue_worker_form_submit() takes care of removing the
    // item from the queue.
  }

}

/**
 * Operation which skips the current entity to be processed later.
 *
 * This works by re-queueing the entity, which puts it at the end of the queue.
 * We rely on the consumer of the operation, such as human_queue_worker_form(),
 * to delete the current queue item for the entity.
 */
class HumanQueueWorkerOperationSkip extends EntityOperationsOperationForm {

  public $access_verb = 'skip';

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Skip',
      'page title' => 'Skip %label',
      'button label' => t('Skip this item'),
      'confirm question' => t('Are you sure you want to skip this %entity-type?'),
      'submit message' => t('%entity-type %label has been skipped and placed at the end of the queue.'),
    );
  }

  /**
   * Form submit handler for this operation.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Re-queue the entity.
    // We expect the consumer of this operation, e.g. human_queue_worker_form(),
    // to take care of deleting the current item for the entity.
    $queue = DrupalQueue::get($form_state['human_queue_worker']['queue']);
    $queue_item = $form_state['human_queue_worker']['queue_item'];
    $queue->createItem($queue_item->data);
  }

}

/**
 * Operation which moves the item to a different queue.
 *
 * This may be useful if you have two or more levels of work, where problematic
 * items should be moved to the second queue for more detailed examination.
 *
 * This works by re-queueing the entity, which puts it at the end of the queue.
 * We rely on the consumer of the operation, such as human_queue_worker_form(),
 * to delete the current queue item for the entity.
 *
 * This requires the operation definition in hook_entity_operation_info() to
 * specify a system queue name in the 'queue name' property within the
 * 'handler configuration' array, thus:
 * @code
 * $operation_info['my_entity']['move_queue_beta'] = array(
 *   'handler' => 'HumanQueueWorkerOperationMoveQueue',
 *   'handler configuration' => array(
 *     'queue name' => 'test_queue_beta',
 *   ),
 * @endcode
 */
class HumanQueueWorkerOperationMoveQueue extends EntityOperationsOperationForm {

  /**
   * Get the access verb for this operation.
   *
   * @return
   *  The access verb for the operation, e.g. 'view', 'edit', and so on.
   */
  public function getAccessVerb() {
    $operation_info = entity_operations_get_operation_info($this->entityType);
    $queue_name = $operation_info[$this->path]['handler configuration']['queue name'];

    $verb = "move to $queue_name queue";
    return $verb;
  }

  /**
   * Returns strings for the operations.
   *
   * @return
   *  An array containing the following keys:
   *  - 'form': An array of strings for the operation form, containing:
   *    - 'button label'
   *    - 'confirm question'
   *    - 'submit message'
   */
  function operationStrings() {
    return array(
      'tab title' => 'Move item',
      'page title' => 'Skip %label',
      'button label' => t("Move item to '@queue-label' queue"),
      'confirm question' => t('Are you sure you want to move this %entity-type to the %queue-label queue?'),
      'submit message' => t('%entity-type %label has been moved to the %queue-label queue.'),
    );
  }

  /**
   * Return an array of string substitutions for t().
   *
   * We add the themed username for the submit message.
   */
  function getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters = array()) {
    $substitutions = parent::getOperationStringSubstitutions($entity_type, $entity, $operation_path, $parameters);

    $operation_info = entity_operations_get_operation_info($this->entityType);
    $queue_name = $operation_info[$this->path]['handler configuration']['queue name'];
    $human_queue_info = module_invoke_all('human_queue_worker_info');
    $human_queue_info = $human_queue_info[$queue_name];

    $substitutions['%queue-label'] = $human_queue_info['label'];
    $substitutions['@queue-label'] = $human_queue_info['label'];

    return $substitutions;
  }

  /**
   * Form submit handler for this operation.
   */
  function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    // Get the operation definition, which states which queue we move items to.
    $operation_info = entity_operations_get_operation_info($this->entityType);
    $queue_name = $operation_info[$this->path]['handler configuration']['queue name'];

    // Re-queue the entity.
    // We expect the consumer of this operation, e.g. human_queue_worker_form(),
    // to take care of deleting the current item for the entity.
    $queue = DrupalQueue::get($queue_name);
    list($id) = entity_extract_ids($entity_type, $entity);
    $queue->createItem($id);
  }

}
