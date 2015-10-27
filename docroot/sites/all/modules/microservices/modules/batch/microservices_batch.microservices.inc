<?php
/**
 * @file
 *
 * @todo TTL support needed.
 */

/**
 * Implements hook_microservices_info().
 */
function microservices_batch_microservices_info() {
  return array(
    MICROSERVICES_BATCH => array(
      'actions' => array(
        MICROSERVICES_BATCH_ACTION_CREATE,
      ),
      'events' => array(
        MICROSERVICES_BATCH_EVENT_PROGRESS,
      ),
    ),
  );
}

/**
 * Implements hook_microservices_action().
 */
function microservices_batch_microservices_action($message) {
  if ($message->action == 'create') {
    microservices_batch_action_create($message);
  }
}

/**
 * Implements hook_microservices_result().
 */
function microservices_batch_microservices_result($message) {
  if (!empty($message->context->batch)) {
    microservices_batch_item_result($message->context->batch->item, $message);
    microservices_batch_process_begin($message->context->batch->id);
  }
  else {
    watchdog('microservices_batch', 'Batch result without required context', WATCHDOG_ERROR);
  }
}

/**
 * @param $message
 *
 * @throws \Exception
 */
function microservices_batch_action_create($message) {
  if (!isset($message->params)) {
    throw new Exception('Batch should not be empty');
  }

  $batch_id = db_insert('microservices_batch', array('return' => Database::RETURN_INSERT_ID))
    ->fields(array('message' => serialize($message), 'created' => time()))
    ->execute();

  $query = db_insert('microservices_batch_items')
    ->fields(array('batch_id', 'name', 'weight', 'wait', 'message', 'status'));
  $weight = 0;
  foreach ($message->params as $name => $item) {
    if ($item->type == MICROSERVICES_MESSAGE_ACTION) {
      microservices_assert_action($item->service, $item->action);
    }
    elseif ($item->type == MICROSERVICES_MESSAGE_EVENT) {
      microservices_assert_event($item->event);
    }
    else {
      throw new Exception("Unknown task type: $item->type");
    }

    $item->sender = MICROSERVICES_BATCH;
    $item->context = isset($item->context) ? $item->context : NULL;
    $item->params = isset($item->params) ? $item->params : NULL;

    $query->values(array(
      'batch_id' => $batch_id,
      'name' => $name,
      'weight' => ++$weight,
      'type' => $item->type,
      'wait' => (bool) (isset($item->nowait) ? $item->nowait : TRUE), // Wait is TRUE by default.
      'message' => serialize($item),
      'status' => MICROSERVICES_BATCH_STATUS_NEW,
    ));
  }
  $query->execute();

  watchdog('microservices_batch', 'Batch #%id created', array('%id' => $batch_id), WATCHDOG_INFO);

  microservices_batch_process_begin($batch_id);
}

function microservices_batch_item_result($item, $message) {
  db_update('microservices_batch_items')
    ->fields(array(
      'message' => serialize($message),
    ))
    ->condition('id', $item->id)
    ->execute();

  $status = !$message->params->error ? MICROSERVICES_BATCH_STATUS_DONE : MICROSERVICES_BATCH_STATUS_FAILED;
  $item->message = $message;
  microservices_batch_item_status($item, $status);
}

function microservices_batch_load($id) {
  $batch = db_select('microservices_batch', 'bt')
    ->fields('bt')
    ->condition('id', $id)
    ->execute()
    ->fetchObject();

  if (!$batch) {
    throw new Exception('Batch not found');
  }

  $batch->id = (int) $batch->id;
  $batch->created = (int) $batch->created;
  $batch->message = unserialize($batch->message);

  $batch->items = db_select('microservices_batch_items', 'bt')
    ->fields('bt')
    ->condition('batch_id', $id)
    ->orderBy('weight')
    ->execute()
    ->fetchAllAssoc('name');

  foreach ($batch->items as &$item) {
    $item->id = (int) $item->id;
    $item->batch_id = (int) $item->batch_id;
    $item->wait = (bool) $item->wait;
    $item->message = unserialize($item->message);
  }

  return $batch;
}

function microservices_batch_process_begin($id) {
  $batch = microservices_batch_load($id);
  $in_progress = FALSE;
  foreach ($batch->items as $item) {
    // If locked item failed, break current batch.
    if ($item->wait && ($item->status == MICROSERVICES_BATCH_STATUS_FAILED)) {
      $in_progress = FALSE;
      break;
    }

    // Skip not new tasks.
    if ($item->status != MICROSERVICES_BATCH_STATUS_NEW) {
      if ($item->status == MICROSERVICES_BATCH_STATUS_IN_PROGRESS) {
        $in_progress = TRUE;
      }
      continue;
    }

    // All tasks should be processed before locked task will be processed.
    if ($item->wait && $in_progress) {
      break;
    }

    if (microservices_batch_item_process($item) == MICROSERVICES_BATCH_STATUS_IN_PROGRESS) {
      $in_progress = TRUE;
    }
  }

  // If batch not in progress send a result and remove it.
  if (!$in_progress) {
    microservices_batch_process_end($id);
  }
}

function microservices_batch_process_end($id) {
  $batch = microservices_batch_load($id);
  $error = NULL;
  foreach ($batch->items as &$item) {
    if (!$error && $item->wait && ($item->status == MICROSERVICES_BATCH_STATUS_FAILED)) {
      $error = $item->message->params->error;
    }

    // Unset batch useless batch attributes.
    unset($item->weight);
    unset($item->status);
    if (isset($item->id)) {
      unset($item->id);
    }
    unset($item->batch_id);
    if (isset($item->batch_id)) {
      unset($item->batch_id);
    }
    if (isset($item->context->batch)) {
      unset($item->context->batch);
    }
  }

  // Add batch items to context.
  if (!isset($batch->message->context)) {
    $batch->message->context = new stdClass();
  }
  $batch->message->context->batch = new stdClass();
  $batch->message->context->batch->items = $batch->items;

  if (!$error) {
    $status = MICROSERVICES_BATCH_STATUS_DONE;
    microservices_send_result($batch->message, 'Ok');
  }
  else {
    $status = MICROSERVICES_BATCH_STATUS_FAILED;
    microservices_send_result($batch->message, NULL, new Exception('Batch failed ' . serialize($error)));
  }

  $severity = ($status == MICROSERVICES_BATCH_STATUS_DONE) ? WATCHDOG_INFO : WATCHDOG_ERROR;

  watchdog('microservices_batch', 'Batch #%id %status', array('%id' => $batch->id, '%status' => $status), $severity);

  // Clear batch-related data.
  db_delete('microservices_batch')
    ->condition('id', $batch->id)
    ->execute();
  db_delete('microservices_batch_items')
    ->condition('batch_id', $batch->id)
    ->execute();
}

function microservices_batch_item_status($item, $status) {
  // Update item status.
  db_update('microservices_batch_items')
    ->fields(array('status' => $status))
    ->condition('id', $item->id)
    ->execute();

  $info_severity_statuses = array(
    MICROSERVICES_BATCH_STATUS_NEW,
    MICROSERVICES_BATCH_STATUS_IN_PROGRESS,
    MICROSERVICES_BATCH_STATUS_DONE,
  );

  // Send batch progress event.
  $batch_message = db_select('microservices_batch', 'bt')
    ->fields('bt', array('message'))
    ->condition('id', $item->batch_id)
    ->execute()
    ->fetchField();
  $batch_message = unserialize($batch_message);

  microservices_send_event(
    MICROSERVICES_BATCH_EVENT_PROGRESS,
    array(
      'name' => $item->name,
      'status' => $status,
      'result' => isset($item->message->params->result) ? $item->message->params->result : NULL,
      'error' => isset($item->message->params->error) ? $item->message->params->error : NULL,
    ),
    $batch_message->context
  );

  $severity = in_array($status, $info_severity_statuses) ? WATCHDOG_INFO : WATCHDOG_ERROR;

  watchdog('microservices_batch', 'Batch #%id step \'%step\' %status', array('%id' => $item->batch_id, '%step' => $item->name, '%status' => $status), $severity);
}

function microservices_batch_item_process($item) {
  if (!isset($item->message->context)) {
    $item->message->context = new stdClass();
  }

  $item->message->context->batch = new stdClass();
  $item->message->context->batch->id = $item->batch_id;
  $item->message->context->batch->item = new stdClass();
  $item->message->context->batch->item->id = $item->id;
  $item->message->context->batch->item->name = $item->name;
  $item->message->context->batch->item->batch_id = $item->batch_id;

  microservices_batch_item_params($item);

  if ($item->message->type == MICROSERVICES_MESSAGE_ACTION) {
    microservices_send_action(
      $item->message->service,
      $item->message->action,
      $item->message->params,
      $item->message->sender,
      $item->message->context
    );
    microservices_batch_item_status($item, MICROSERVICES_BATCH_STATUS_IN_PROGRESS);
    return MICROSERVICES_BATCH_STATUS_IN_PROGRESS;
  }

  elseif ($item->message->type == MICROSERVICES_MESSAGE_EVENT) {
    microservices_send_event(
      $item->message->event,
      $item->message->params,
      $item->message->context
    );
    microservices_batch_item_status($item, MICROSERVICES_BATCH_STATUS_DONE);

    return MICROSERVICES_BATCH_STATUS_DONE;
  }

  else {
    throw new Exception("Unknown task type: {$item->message->type}");
  }
}

function microservices_batch_item_params($item) {
  foreach ((array) $item->message->params as $name => $value) {
    $new_value = NULL;
    $matches = [];
    if (is_string($value) && preg_match_all('/^batch:(\w+)$/', $value, $matches)) {
      $batch_item = db_select('microservices_batch_items', 'items')
        ->fields('items', array('id', 'message'))
        ->condition('batch_id', $item->batch_id)
        ->condition('name', $matches[1][0])
        ->execute()
        ->fetchObject();

      $message = unserialize($batch_item->message);
      if (isset($message->params->result)) {
        $item->message->params->$name = $message->params->result;
      }
    }
  }
}
