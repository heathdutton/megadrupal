<?php
/**
 * @file
 * Commerce Credits Transaction rule actions, events, and conditions.
 */


/**
 * Implements hook_rules_action_info().
 */
function commerce_credits_transaction_rules_action_info() {
  return array(
    'commerce_credits_transaction_get_credits' => array(
      'label' => t('Fetch Credits by Entity'),
      'base' => 'commerce_credits_transaction_rules_get_credits',
      'group' => t('Commerce Credits'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'description' => t('Provide an entity that owns the credits'),
          'wrapped' => TRUE,
        ),
        'credit_type' => array(
          'type' => 'text',
          'label' => t('Credit type'),
          'description' => t('Credit type to be transfered.'),
          'options list' => 'commerce_credits_types_list',
        ),
      ),
      'provides' => array(
        'credits' => array(
          'type' => 'commerce_credits',
          'label' => t('Credits'),
        ),
      ),
    ),
    'commerce_credits_transaction_transfer' => array(
      'label' => t('Commerce Credits Transaction Transfer'),
      'base' => 'commerce_credits_transaction_rules_transfer',
      'group' => t('Commerce Credits'),
      'parameter' => array(
        'entity_from' => array(
          'type' => 'entity',
          'label' => t('Transfer from'),
          'description' => t('Entity to transfer from'),
          'wrapped' => TRUE,
        ),
        'entity_to' => array(
          'type' => 'entity',
          'label' => t('Transfer to'),
          'description' => t('Entity to transfer to'),
          'wrapped' => TRUE,
        ),
        'credit_type' => array(
          'type' => 'text',
          'label' => t('Credit type'),
          'description' => t('Credit type to be transfered.'),
          'options list' => 'commerce_credits_types_list',
        ),
        'number' => array(
          'type' => 'integer',
          'label' => t('Number of credits'),
          'description' => t('Number of credits that should be transfered.'),
        ),
        'status' => array(
          'type' => 'text',
          'label' => t('Transaction Status'),
          'description' => t('The transaction status to set.'),
          'options list' => 'commerce_credits_transaction_get_status_options',
        ),
      ),
    ),
    'commerce_credits_transaction_entity_reverse' => array(
      'label' => t('Commerce Credits Reverse by Entity'),
      'base' => 'commerce_credits_transaction_rules_entity_reverse',
      'group' => t('Commerce Credits'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'wrapped' => TRUE,
        ),
        'direction' => array(
          'type' => 'token',
          'label' => t('Gave or Has?'),
          'options list' => 'commerce_credits_transaction_get_directions',
        ),
        'type' => array(
          'type' => 'token',
          'label' => t('Credit type'),
          'options list' => 'commerce_credits_types_list',
        ),
      ),
    ),
    'commerce_credits_transaction_complete' => array(
      'label' => t('Commerce Credits Complete Transactions'),
      'base' => 'commerce_credits_transaction_rules_complete',
      'group' => t('Commerce Credits'),
      'parameter' => array(
        // This is necessary because Entity API can't handle generic entities.
        'entity_type' => array(
          'type' => 'text',
          'label' => t('Entity type'),
        ),
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'wrapped' => FALSE,
        ),
        'direction' => array(
          'type' => 'token',
          'label' => t('Gave or Has?'),
          'options list' => 'commerce_credits_transaction_get_directions',
        ),
      ),
    ),
  );
}

/**
 * Commerce Credits Transaction - Get Credits entity by entity
 */
function commerce_credits_transaction_rules_get_credits(EntityDrupalWrapper $entity, $type) {
  $credits = entity_get_controller('commerce_credits')->loadByEntity($entity->getIdentifier(), $entity->type(), $type);
  $credits = is_array($credits) ? array_shift($credits) : FALSE;

  if ($credits) {
    return array('credits' => $credits);
  }

  return FALSE;
}

/**
 * Commerce Credits Transaction Transfer callback.
 */
function commerce_credits_transaction_rules_transfer(EntityDrupalWrapper $from, EntityDrupalWrapper $to, $type, $number, $status) {
  if ($status == 'canceled') {
    return FALSE;
  }

  $controller = entity_get_controller('commerce_credits');

  $from_credits = $controller->loadByEntity($from->getIdentifier(), $from->type(), $type);
  $from_credits = is_array($from_credits) ? array_shift($from_credits) : FALSE;

  if (!$from_credits || $number > $from_credits) {
    // there were not enough credits to fulfill the transfer.
    $args = array(
      '%from-type' => $from->type(),
      '%from' => $from->getIdentifier(),
      '%to-type' => $to->type(),
      '%to' => $to->getIdentifier(),
      '%credits' => $number,
    );
    watchdog('commerce_credits', 'Unable to fulfill commerce credits transfer from %from-type:%from to %to-type:%to of %credits', $args, WATCHDOG_CRITICAL);
    return FALSE;
  }

  $to_credits = $controller->loadByEntity($to->getIdentifier(), $to->type(), $type);
  $to_credits = is_array($to_credits) ? array_shift($to_credits) : FALSE;

  if (!$to_credits) {
    // Create a credits entity if it does not exist yet.
    $to_credits = $controller->createbyEntity($to->getIdentifier(), $to->type(), $type);
  }

  $controller->transferCreditWithTransaction($from_credits, $to_credits, $type, $number, $status);
}

/**
 * Commerce Credits Transaction Entity Reverse callback
 */
function commerce_credits_transaction_rules_entity_reverse(EntityDrupalWrapper $entity, $direction, $type) {
  try {
    $credits = entity_get_controller('commerce_credits')->loadByEntity($entity->getIdentifier(), $entity->type(), $type);
    $credits = is_array($credits) ? array_shift($credits) : FALSE;

    if (!$credits) {
      throw new Exception('Entity does not have any credits to reverse.');
    }

    $normalized_direction = $direction == 'gave' ? 'from' : 'to';

    $transaction_ids = commerce_credits_transaction_load_by_credit_entity($credits, $normalized_direction, 'pending');
    $transactions = commerce_credits_transaction_load_multiple(array_keys($transaction_ids));

    if (!$transaction_ids) {
      throw new Exception('Entity does not have credit transactions to reverse.');
    }

    // Get the total number of credits in the transactions.
    $total = array_reduce($transaction_ids, function(&$result, $item) {
      if (!isset($result)) {
        $result = 0;
      }

      $result += $item;

      return $result;
    });

    if ($credits->credits < $total) {
      $missing_plural = format_plural($total - $credits->credits, 'Entity does not have enough credits by 1 credit to reverse.', 'Entity does not have enough credits by @count credits to reverse.');
      throw new Exception($missing_plural);
    }

    $queues = 0;
    $queue = DrupalQueue::get('commerce_credits_transaction_reverse');

    $total = count($transactions);
    while (!empty($transactions)) {
      // Create queue items with 50 transactions at a time.
      $to_queue = array_splice($transactions, 0, 50);

      $data = array(
        'transactions' => $to_queue,
      );

      $queue->createItem($data);
      $queues++;
    }

    $plural = format_plural(count($transaction_ids), '1 transaction', '@count transactions');
    $queue_plural = format_plural($queues, '1 queue item', '@count queue items');
    watchdog('commerce_credits_transaction', 'Queued !plural to reverse in !queue_plural', array('!plural' => $plural, '!queue_plural' => $queue_plural));
    return TRUE;
  }
  catch (Exception $e) {
    debug($e->getMessage());
    watchdog_exception('commerce_credits', $e, $e->getMessage(), NULL, WATCHDOG_CRITICAL);
    return FALSE;
  }
}

/**
 * Complete transactions
 */
function commerce_credits_transaction_rules_complete($entity_type, $entity, $direction) {
  $normalized_direction = $direction == 'gave' ? 'from' : 'to';

  $num_updated = entity_get_controller('commerce_credits_transaction')->completeTransaction($entity_type, $entity, $normalized_direction);
}
