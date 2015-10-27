<?php

/**
 * @file
 *   Functions common for Simpletest DrupalWebTestCase.
 *
 *   Could be seperately included for a PhpUnit Drush test.
 */

/**
 * Create two queues, populate with items, return data array.
 *
 * @return array
 *   Data of items entered into queues.
 */
function advancedqueue_test_populate_two_queues() {
  // Create two queues.
  $queue1 = DrupalQueue::get('advancedqueue_test_1');
  $queue1->createQueue();
  $queue2 = DrupalQueue::get('advancedqueue_test_2');
  $queue2->createQueue();

  // Create four items in queue 1.
  $data = array();
  for ($i = 0; $i < 4; $i++) {
    $data[] = $item = array(DrupalTestCase::randomName() => DrupalTestCase::randomName());
    $queue1->createItem($item);
  }

  // Create four items in queue 2.
  for ($i = 0; $i < 4; $i++) {
    $data[] = $item = array(DrupalTestCase::randomName() => DrupalTestCase::randomName());
    $queue2->createItem($item);
  }

  return $data;
}

/**
 * Verify log entries exist.
 *
 * Called in the same way of the expected original watchdog() execution. Based
 * on ModuleTestCase::assertLogMessage().
 *
 * @param string $type
 *   The category to which this message belongs.
 * @param string $message
 *   The message to store in the log. Keep $message translatable
 *   by not concatenating dynamic values into it! Variables in the
 *   message should be added by using placeholder strings alongside
 *   the variables argument to declare the value of the placeholders.
 *   See t() for documentation on how $message and $variables interact.
 * @param array $variables
 *   Array of variables to replace in the message on display or
 *   NULL if message is already translated or not possible to
 *   translate.
 * @param int $severity
 *   The severity of the message, as per RFC 3164.
 * @param string $link
 *   A link to associate with the message.
 */
function advancedqueue_test_count_watchdog($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = '') {
  $count = db_select('watchdog', 'w')
    ->condition('type', $type)
    ->condition('message', $message)
    ->condition('variables', serialize($variables))
    ->condition('severity', $severity)
    ->condition('link', $link)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $count;
}
