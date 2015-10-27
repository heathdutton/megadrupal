<?php

/**
 * @file
 * Contains page callbacks.
 */

/**
 * Page callback showing the user an overview of the queues they may work on.
 */
function human_queue_worker_overview_page() {
  $human_queue_info = module_invoke_all('human_queue_worker_info');

  $build = array();
  $items = array();

  $access_all_queues = user_access('process all human queues');

  foreach ($human_queue_info as $queue_name => $info) {
    if ($access_all_queues || user_access("process queue $queue_name")) {
      $queue = DrupalQueue::get($queue_name);
      $item_count = $queue->numberOfItems();

      $items[] = t('<a href="!url">Process items in the %queue-label queue</a> (approximate item count: !count)', array(
        '!url' => url("queue_processing/$queue_name"),
        '%queue-label' => $info['label'],
        '!count' => $item_count,
      ));
    }
  }

  if ($items) {
    $build['queue_links'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }
  else {
    $build['message'] = array(
      '#markup' => t('There are no queues availble to process at this time.'),
    );
  }

  return $build;
}

/**
 * Form for processing an item from a queue.
 *
 * This claims an item for the user to work on, and presents the form with the
 * operations defined for the queue. The claimed item is stored in the form
 * state, so that operation forms that require a rebuild for multistep don't
 * cause a new item to be claimed. The claimed item is also stored in the
 * session, so that if the user reloads the page they get the same item,
 * provided its lease has not expired.
 *
 * @param $queue_name
 *  The name of a system queue. This must have additional information defined
 *  for it in hook_human_queue_worker_info().
 */
function human_queue_worker_form($form, &$form_state, $queue_name) {
  // Our access callback has already taken care of checking the queue exists.
  $human_queue_info = module_invoke_all('human_queue_worker_info');
  $human_queue_info = $human_queue_info[$queue_name];

  drupal_set_title(t("Processing @name queue", array(
    '@name' => $human_queue_info['label'],
  )));

  // Whether we should attempt to claim a new item.
  $claim_item = FALSE;

  // Check the form state for a queue item. It's possible that we are returning
  // here due to a multistep form in one of the operations, in which case we
  // don't want to claim a new item.
  if (!isset($form_state['human_queue_worker']['queue_item'])) {
    // It's also possible that the user has reloaded the page. If so, we should
    // preserve the item they had if it's still valid.
    if (isset($_SESSION['human_queue_worker'][$human_queue_info['queue']])) {
      $form_state['human_queue_worker'] = $_SESSION['human_queue_worker'][$human_queue_info['queue']];

      // Check expiry.
      // If the item has expired, it cannot be processed. Checking this now
      // saves the user from wasting their time filling in the form.
      if ($form_state['human_queue_worker']['expiry'] < REQUEST_TIME) {
        drupal_set_message(t("The item you were previously working on has expired. Please reload the page to get a fresh item to process"), 'warning');

        // Clear the expired item from the session.
        unset($_SESSION['human_queue_worker'][$human_queue_info['queue']]);

        $claim_item = TRUE;
      }
    }
    else {
      // Fresh form: get a new queue item.
      $claim_item = TRUE;
    }
  }

  if ($claim_item) {
    $queue = DrupalQueue::get($human_queue_info['queue']);
    $queue_item = $queue->claimItem($human_queue_info['lease time']);

    if (!$queue_item) {
      $form['empty'] = array(
        '#markup' => t("No items in the @queue queue are available for processing.", array(
          '@queue' => $human_queue_info['label'],
        )),
      );
      return $form;
    }

    // Store the queue item details in the form state and the session.
    $form_state['human_queue_worker']['queue_item'] = $queue_item;
    $form_state['human_queue_worker']['expiry'] = REQUEST_TIME + $human_queue_info['lease time'];
    $form_state['human_queue_worker']['queue'] = $human_queue_info['queue'];

    $_SESSION['human_queue_worker'][$human_queue_info['queue']] = $form_state['human_queue_worker'];
  }

  // Load the entity from the queue item.
  $entity = entity_load_single($human_queue_info['entity type'], $form_state['human_queue_worker']['queue_item']->data);

  // Check that the entity still exists.
  if (empty($entity)) {
    // Delete the item, and invite the user to reload the page.
    // (We probably could supply a new item for them, but it would require
    // more work than this corner case warrants. Ideally, if you're queueing
    // entities, you're not also deleting them!)
    $queue = DrupalQueue::get($human_queue_info['queue']);
    $queue->deleteItem($form_state['human_queue_worker']['queue_item']);

    // Clear the session.
    unset($_SESSION['human_queue_worker'][$human_queue_info['queue']]);

    $form['deleted'] = array(
      '#markup' => t("The entity for this item has already been deleted. Please reload the page to get a fresh item to process."),
    );
    return $form;
  }

  // Output the label of the entity. This helps with user error reporting, not
  // to mention testing.
  $label = entity_label($human_queue_info['entity type'], $entity);
  list($id,) = entity_extract_ids($human_queue_info['entity type'], $entity);
  $form['queue_item_info'] = array(
    '#markup' => t("Current item is '@label' (!id).", array(
      '@label' => $label,
      '!id' => $id,
    )),
  );


  // Show the entity, in either the requested view mode or the default.
  // Entities using this system may wish to provide a view mode which suppresses
  // the output of the entity title as a link.
  $view_mode = isset($human_queue_info['view mode']) ? $human_queue_info['view mode'] : 'full';

  $form['entity_wrapper'] = array(
    '#type' => 'fieldset',
  );

  // entity_view() expects an array of multiple entities, but the key is not
  // used.
  // We pass FALSE for the $page parameters.
  $form['entity_wrapper']['entity'] = entity_view($human_queue_info['entity type'], array($entity), $view_mode, NULL, FALSE);

  // Show the operations.
  $form = entity_operations_multiple_operation_form($form, $form_state, $human_queue_info['entity type'], $entity, $human_queue_info['entity operations']);

  return $form;
}

/**
 * Validate handler for the queue worker form.
 */
function human_queue_worker_form_validate($form, &$form_state) {
  // Check the expiry on the lease.
  if ($form_state['human_queue_worker']['expiry'] < REQUEST_TIME) {
    form_set_error('', t("The current queue item has expired."));

    // Clear the session data.
    $queue = $form_state['human_queue_worker']['queue'];
    unset($_SESSION['human_queue_worker'][$queue]);

    return;
  }

  // Run the entity operations form validation.
  entity_operations_multiple_operation_form_validate($form, $form_state);
}

/**
 * Submit handler for the queue worker form.
 */
function human_queue_worker_form_submit($form, &$form_state) {
  //dsm($form_state);

  // Run the entity operations form submission. This will execute the submit
  // handler for the right operation.
  entity_operations_multiple_operation_form_submit($form, $form_state);

  // If the entity operation's submit handler sets $form_state['rebuild'], then
  // this is a multistep form, the form is not yet complete, and therefore we
  // are not yet done with this queue item.
  // Conversely, this means that when an operation's submit handler has made use
  // of multistep and now wishes to indicate the submission is complete, it must
  // set this to FALSE -- which it should anyway for Form API so that's fine.
  if (!empty($form_state['rebuild'])) {
    return;
  }

  // Clear the session data.
  $queue = $form_state['human_queue_worker']['queue'];
  unset($_SESSION['human_queue_worker'][$queue]);

  // Delete the queue item.
  $queue = DrupalQueue::get($form_state['human_queue_worker']['queue']);
  $queue_item = $form_state['human_queue_worker']['queue_item'];
  $queue->deleteItem($queue_item);
}
