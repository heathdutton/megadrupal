<?php
/**
 * @file
 * Drush functions for the GovDelivery Integration module.
 */

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command()
 *   for a list of recognized keys.
 */
function govdelivery_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['govdelivery-send'] = array(
    // The name of the function implementing your command.
    'callback' => 'drush_govdelivery_send_queue',
    // A short description of your command.
    'description' => "Send the messages in the govdelivery queue",
    'arguments' => array(
      'type' => 'The type of the queue that should be sent.',
      'bid' => "The bin id range to send in form '1,10'",
      'num' => 'The number of messages to send at a time',
    ),
    'aliases' => array('govds'),
  );

  // The key in the $items array is the name of the command.
  $items['govdelivery-test'] = array(
    // The name of the function implementing your command.
    'callback' => 'drush_govdelivery_send_test_message',
    // A short description of your command.
    'description' => "Send a test message using the GovDelivery service test functionality.",
    'arguments' => array(
      'addresses' => 'Email to send a test message to.',
      'num' => 'Number of messages to queue.',
    ),
  );

  // The key in the $items array is the name of the command.
  $items['govdelivery-subscribe'] = array(
    // The name of the function implementing your command.
    'callback' => 'govdelivery_drush_subscribe',
    // A short description of your command.
    'description' => "Sends subscriptions from the subscription queue.",
    'arguments' => array(
      'code' => 'List code for subscription',
      'addresses' => 'Email to subscribe',
    ),
  );

  // The key in the $items array is the name of the command.
  $items['govdelivery-dump-subscriptions'] = array(
    // The name of the function implementing your command.
    'callback' => 'govdelivery_drush_dump_subscriptions',
    // A short description of your command.
    'description' => "Dump subscriptions for a specific topic from the subscriber api subscription queue.",
    'arguments' => array(
      'topic' => 'List code for subscription',
    ),
  );

  $items['govdelivery-test-topic'] = array(
    'callback' => 'govdelivery_drush_test_topic',
    'description' => 'Add a test topic to the queue for sending.',
    'arguments' => array(
      'title' => 'Title of the topic',
      'short_title' => 'Short title of the topic.',
      'topic_id' => 'Manually set topic ID',
      'num' => 'Number of topics to create, title will be used as a prefix.',
    ),
  );

  $items['govdelivery-test-subscribe'] = array(
    'callback' => 'govdelivery_drush_test_subscriber',
    'description' => 'Add a test subsciber to the queue for sending.',
    'arguments' => array(
      'email' => "The subscriber's email address.",
      'title' => 'The title of the topic. Optional',
      'topic_id' => 'The topic ID. Optional',
      'num' => 'Optional number of times to subscriber.',
    ),
  );

  $items['govdelivery-delivery-report'] = array(
    'callback' => 'govdelivery_drush_delivery_report',
    'description' => 'Returns up the number of results desired since the sequence provided or from the begining of time.',
    'arguments' => array(
      'results' => 'The number of results to return up to 10000',
      'sequence' => 'The sequence ID to pull results with.',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 */
function govdelivery_drush_help($section) {
  $help = NULL;
  switch ($section) {
    case 'drush:govdelivery send':
      $help = dt("Send messages or subscriptions from the GovDelivery queues.");
    case 'drush:govdelivery test':
      $help = dt("Sends a test message using govdelivery (this may be queued or sent immediately depending on the settings.)");
    case 'drush:govdelivery subscribe':
      $help = dt("Subscribes a user to a list managed by govdelivery (this may be queued or sent immediately depending on the settings.)");
  }
  return $help;
}

/**
 * Types of queue.
 *
 * @return array
 *   Queue types.
 */
function drush_govdelivery_queue_types() {
  $types = array(
    'messages' => array('govdelivery_process_message_queue'),
    'subscribers' => array('govdelivery_process_subscription_queue'),
    'topics' => array('govdelivery_process_subscriber_api_topic_queue'),
    'api_subscribers' => array('govdelivery_process_subscriber_api_subscriber_queue'),
  );
  return $types;
}

/**
 * Send messages from the govdelivery queue.
 *
 * @param string $type
 *   The number of messages to send from the queue.
 */
function drush_govdelivery_send_queue($type = NULL, $bid = NULL, $num = NULL) {
  $types = drush_govdelievery_queue_types();
  if ($type) {
    $choice = $type;
  }
  else {
    $choice = drush_choice($types, 'Which queue would you like to process', '!key');
  }

  $messages_sent = call_user_func(
    // Function to call.
    $types[$choice][0],
    // Max number of items.
    $num,
    // Ignore Execution Time Limit.
    TRUE,
    // CID for sending a single message.
    NULL,
    // The Range of Bin IDs in form '1,10'.
    $bid);

  if (($messages_sent) || ($messages_sent === 0)) {
    drush_log("Message queue has been processed, " . $messages_sent . " messages sent.", 'completed');
  }
  else {
    drush_log("Message queue could not be processed.  Check watchdog for errors.", 'failed', TRUE);
  }
}

/**
 * Send a test message.
 *
 * @param string $address
 *   Email address.
 * @param null $num
 *   Number of messages to send.
 *
 * @return bool
 *   If successful.
 */
function drush_govdelivery_send_test_message($address, $num = NULL) {
  module_load_include('inc', 'govdelivery', 'govdelivery.admin');
  if (!isset($address)) {
    drush_log("Cannot send a test message without an address to send to", 'failed', TRUE);
    return FALSE;
  }

  if (is_numeric($num)) {
    for ($i = 0; $i < $num; $i++) {
      $success = govdelivery_send_test_message($address);
    }
  }
  else {
    $success = govdelivery_send_test_message($address);
  }

  if ($success) {
    drush_log("Test message has been queued.  It will be sent the next time the queue is processed.", 'completed');
  }
  else {
    drush_log("Sending a test message failed", 'failed', TRUE);
  }

  return $success;
}

/**
 * Subscribe to list.
 *
 * @param int $code
 *   List code.
 * @param string $address
 *   Email address.
 *
 * @return bool
 *   If successful.
 */
function govdelivery_drush_subscribe($code, $address) {
  $success = govdelivery_subscribe($code, $address);
  if ($success) {
    drush_log("User has been subscribed to the list", 'completed');
  }
  else {
    drush_log("User could not be subscribed", 'failed', TRUE);
  }
  return $success;
}

/**
 * Dump subscriptions.
 *
 * @param string $topic_id
 *   Topic ID.
 *
 * @return bool
 *   If successful.
 */
function govdelivery_drush_dump_subscriptions($topic_id) {
  if (!is_string($topic_id)) {
    return FALSE;
  }
  $topic_id = 'petitions_' . $topic_id;
  $queue = 'govdelivery_subscriber_api_subscriber_queue';
  $query = db_select($queue, 'q');
  $query->fields('q', array('data', 'created', 'expire', 'serialized'));

  $result = $query->execute();
  // var_dump($result);
  // var_dump($result->fetchObject());
  $handle = fopen('/tmp/subscriber_export_' . time() . '.csv', 'x+');
  fwrite($handle, 'email' . PHP_EOL);

  while ($message = $result->fetchObject()) {
    if (isset($message->data) && !empty($message->data)) {
      $message->data = unserialize($message->data);
      if (is_string($message->data->email) && is_string($message->data->title)) {
        if ($message->data->title === $topic_id) {
          var_dump($message);
          fwrite($handle, $message->data->email . ',' . PHP_EOL);
        }
      }
    }
  }
  fclose($handle);
}

/**
 * Test a topic.
 *
 * @param string $title
 *   Title
 * @param string $short_title
 *   Short title
 * @param null $topic_id
 *   Topic ID
 * @param null $num
 *   Number of times to add.
 *
 * @return int
 *   List of topics.
 */
function govdelivery_drush_test_topic($title, $short_title = '', $topic_id = NULL, $num = NULL) {
  $topics = 0;
  if (is_numeric($num)) {
    for ($i = 0; $i < $num; $i++) {
      $topics += govdelivery_subscriber_api_add_topic($title . $i, $short_title, $topic_id);
    }
  }
  else {
    $topics += govdelivery_subscriber_api_add_topic($title, $short_title, $topic_id);
  }
  return $topics;
}

/**
 * Test subscribers.
 *
 * @param string $email
 *   Email address.
 * @param null $title
 *   Topic title.
 * @param null $topic_id
 *   Topic ID.
 * @param null $num
 *   Number of times to add a subscriber.
 *
 * @return bool
 *   If subscribed.
 */
function govdelivery_drush_test_subscriber($email, $title = NULL, $topic_id = NULL, $num = NULL) {
  $subscribed = 0;
  if (is_numeric($num)) {
    for ($i = 0; $i < $num; $i++) {
      $subscribed += govdelivery_subscriber_api_add_subscriber($email, $title, $topic_id);
    }
  }
  else {
    $subscribed = govdelivery_subscriber_api_add_subscriber($email, $title, $topic_id);
  }
  return $subscribed;
}

/**
 * Delivery report.
 *
 * @param int $results
 *   Number of results.
 * @param string $sequence
 *   Sequence.
 */
function govdelivery_drush_delivery_report($results = 100, $sequence = '') {
  $govdelivery_tms_settings = variable_get('govdelivery_tms_settings', array());
  $account_map = variable_get("govdelivery_account_map", NULL);
  $account = $account_map['default'];
  if (!isset($account) || empty($account)) {
    watchdog('govdelivery', 'No default account specified.  Check settings.php.', NULL, WATCHDOG_ERROR);
    return FALSE;
  }

  require_once 'tmsService.php';
  $credentials = new TMSCredentials($account, $govdelivery_tms_settings['accounts'][$account]['password']);
  $service = new tmsService($govdelivery_tms_settings['server'], $credentials);

  $activity = new TMSDeliveryActivitySince();
  $activity->maxResults = $results;
  $activity->sequence = $sequence;
  $result = $service->deliveryActivitySince($activity);

  $rows[] = array('Message ID', 'Email', 'Status', 'Time');
  foreach ($result->delivery as $item) {
    $rows[] = array(
      $item->messageID,
      $item->address,
      $item->delivered,
      $item->at,
    );
  }
  drush_print_table($rows, TRUE, array(), NULL);
  drush_print('Sequence: ' . $result->nextSequence);
}
