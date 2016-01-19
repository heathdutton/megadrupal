<?php
/**
 * @file
 */

/*
 * Implements hook_microservices_info().
 */
function microservices_watcher_microservices_info() {
  return array(
    MICROSERVICES_WATCHER => array(
      'actions' => array(
        MICROSERVICES_WATCHER_ACTION_CREATE,
      ),
      'bind events' => array(
        MICROSERVICES_CRON_EVENT_CRON,
        MICROSERVICES_CRON_EVENT_TICK,
      ),
    ),
  );
}

/**
 * Implements hook_microservices_event().
 */
function microservices_watcher_microservices_event($message) {
  if ($message->event == MICROSERVICES_CRON_EVENT_TICK) {
    microservices_watcher_event_check($message->params->time);
  }
  if ($message->event == MICROSERVICES_CRON_EVENT_CRON) {
    if (in_array('every1m', $message->params->expressions)) {
      microservices_watcher_event_clean($message->params->time);
    }
  }
}

/*
 * Implements hook_microservices_action().
 */
function microservices_watcher_microservices_action($message) {
  if ($message->action == 'create') {
    microservices_watcher_action_create($message);
  }
}

/*
 * Implements hook_microservices_result().
 */
function microservices_watcher_microservices_result($message) {
  if (!isset($message->context->watcher)) {
    throw new Exception('Watcher context should be specified');
  }

  $watcher_id = $message->context->watcher->id;

  $item = db_select('microservices_watcher', 't')
    ->fields('t')
    ->condition('id', $watcher_id)
    ->execute()
    ->fetchObject();

  if (!$item) {
    throw new Exception("Can not find watcher item #{$watcher_id}");
  }

  try {
    if (!is_null($message->params->error)) {
      db_delete('microservices_watcher')->condition('id', $watcher_id)->execute();
      microservices_send_result(unserialize($item->message), NULL, $message->params->error);
      watchdog('microservices_watcher', 'Watcher #%id Error', array('%id' => $watcher_id), WATCHDOG_ERROR);
    }
    elseif ($message->params->result) {
      db_delete('microservices_watcher')->condition('id', $watcher_id)->execute();
      microservices_send_result(unserialize($item->message), 'Ok');
      watchdog('microservices_watcher', 'Watcher #%id Ready', array('%id' => $watcher_id), WATCHDOG_INFO);
    }
    else {
      watchdog('microservices_watcher', 'Watcher #%id Not ready yet ', array('%id' => $watcher_id), WATCHDOG_INFO);
    }
  }
  catch (Exception $e) {
    db_delete('microservices_watcher')->condition('id', $watcher_id)->execute();
    watchdog('microservices_watcher', 'Failed to send result \'%message\'', array('%message' => $item->message), WATCHDOG_ERROR);
    watchdog_exception('microservices_watcher', $e, NULL, array(), WATCHDOG_ERROR);
  }
}

function microservices_watcher_action_create($message) {
  if (!isset($message->sender)) {
    throw new Exception('Message sender should be specified');
  }
  if (!isset($message->params->service)) {
    throw new Exception('Service should be specified');
  }
  if (!isset($message->params->action)) {
    throw new Exception('Action name should be specified');
  }

  $delay = isset($message->params->delay)
           ? $message->params->delay
           : variable_get('microservices_watcher_delay', 2); // Default delay is 2 seconds.

  $ttl = isset($message->params->ttl)
         ? $message->params->ttl
         : variable_get('microservices_watcher_ttl', 600); // Default TTL is 10 minutes.

  if ((time() + $delay) > (time() + $ttl)) {
    throw new Exception("Invalid Delay/TTL values: $delay/$ttl");
  }

  $watcher_id = db_insert('microservices_watcher', array('return' => Database::RETURN_INSERT_ID))
    ->fields(array(
      'message' => serialize($message),
      'delay' => $delay,
      'next' => time() + $delay,
      'expiration' => time() + $ttl,
    ))
    ->execute();
  watchdog('microservices_watcher', 'Watcher created #%id', array('%id' => $watcher_id), WATCHDOG_INFO);
}

function microservices_watcher_event_check($time) {
  $items = db_select('microservices_watcher', 't')
    ->fields('t')
    ->condition('next', $time, '<')
    ->condition('expiration', $time, '>')
    ->execute()
    ->fetchAll();

  if ($items) {
    foreach ($items as $item) {
      try {
        $message = unserialize($item->message);

        $context = $message->context;
        $context->watcher = new stdClass();
        $context->watcher->id = $item->id;

        watchdog('microservices_watcher', 'Checking #%id', array('%id' => $item->id), WATCHDOG_INFO);

        microservices_send_action(
          $message->params->service,
          $message->params->action,
          $message->params,
          MICROSERVICES_WATCHER,
          $context
        );

        db_update('microservices_watcher')
          ->fields(array('next' => $item->next + $item->delay))
          ->condition('id', $item->id)
          ->execute();

      } catch (Exception $e) {
        watchdog('microservices_watcher', 'Failed to check #%id', array('%id' => $item->id), WATCHDOG_WARNING);

        db_delete('microservices_watcher')
          ->condition('id', $item->id)
          ->execute();

        throw $e;
      }
    }
  }
  else {
    watchdog('microservices_watcher', 'Nothing to check', array(), WATCHDOG_NOTICE);
  }
}

function microservices_watcher_event_clean($time) {
  watchdog('microservices_watcher', 'Clean expired watches', NULL, WATCHDOG_INFO);

  $items = db_select('microservices_watcher', 't')
    ->fields('t', array('id', 'message'))
    ->condition('expiration', $time, '<')
    ->execute()
    ->fetchAll();

  if ($items) {
    foreach ($items as $item) {
      try {
        microservices_send_result(
          unserialize($item->message),
          NULL,
          new Exception("Watcher TTL is over #$item->id")
        );
      }
      catch (Exception $e) {
        watchdog('microservices_watcher', 'Watcher TTL is over #%id', array('%id' => $item->id), WATCHDOG_WARNING);
      }
      db_delete('microservices_watcher')->condition('id', $item->id)->execute();
    }
  }
  else {
    watchdog('microservices_watcher', 'Nothing to clean', array(), WATCHDOG_NOTICE);
  }
}
