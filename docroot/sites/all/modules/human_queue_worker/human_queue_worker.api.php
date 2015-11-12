<?php

/**
 * @file human_queue_worker.api.php
 * Hooks provided by the Human queue worker module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare system queues to be usable by human workers.
 *
 * @return
 *  An array keyed by the queue name, where each item is an array of data about
 *  the queue containing the following properties:
 *  - 'queue': The queue name.
 *  - 'label': A human-readable label for the queue. This needn't include the
 *    word 'queue'; UI texts add this in their phrasing, as in 'The foo queue'.
 *  - 'lease time': The lease time to grant when claiming an item from the queue
 *    for human workers. This should be long enough for the user to read the
 *    form, go and make a cup of tea, and so on. Suggested time is 30 minutes.
 *  - 'entity type': The type of the entities being queued.
 *  - 'entity operations': An array of the names of operations to be made
 *    available in the queue item processing form. These should also be defined
 *    in hook_entity_operation_info(), where their handlers and other properties
 *    should be set.
 *  - 'view mode': (optional) The view mode in which to display the entity in
 *    the queue processing form. Defaults to 'full', the same as entity_view().
 *    Entities using this system may wish to provide a view mode for this which
 *    suppresses the output of the entity title as a link.
 */
function hook_human_queue_worker_info() {
  $info = array(
    'my_queue' => array(
      'queue' => 'my_queue',
      'label' => t("Dummy"),
      'lease time' => 30 * 60,
      'entity type' => 'entity_operations_test',
      'entity operations' => array(
        // These should be operations defined on the entity.
        'red',
        'blue',
        'lookup',
        'delete_confirm',
        'skip',
      ),
      'view mode' => 'queue_process',
    ),
  );

  return $info;
}
