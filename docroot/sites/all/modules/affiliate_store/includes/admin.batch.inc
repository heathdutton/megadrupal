<?php

/**
 * @file
 * Batch processing functions for admin pages.
 */

/**
 * Batch callback; Import affiliate store subscription.
 *
 * @param string $message
 *   Status message to show when running the batch. Available placeholders for
 *   used in message are @current and @total, which are currently processing
 *   item and total item count respectively.
 * @param array $options
 *   Options specified when submitting batch from form.
 */
function affiliate_store_batch_import_subscription($message, $options,
&$context) {
  affiliate_store_include('batch');
  $type = 'product:%';
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = affiliate_store_batch_count($type);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  // Run as user 1 so node creation has user 1 as author.
  affiliate_store_include('common');
  affiliate_store_impersonate_user(1);
  $context['max_operation'] = $options['update_throttle'];
  $result = affiliate_store_batch_dequeue($type, $context);
  affiliate_store_revert_user();

  if ($result === TRUE) {
    // Override original batch message with number of batches finished.
    $context['message'] = strtr(
      $message,
      array(
        '@current' => $context['sandbox']['progress'] + 1,
        '@total' => $context['sandbox']['total'],
      )
    );
    $context['sandbox']['progress'] += (int) $context['finished'];
  }
  elseif ($result === NULL) {
    return;
  }

  $context['finished'] = (
    $context['sandbox']['progress'] / $context['sandbox']['total']
  );
  sleep($options['sleep']);
}

/**
 * Batch callback; Fetch affiliate store subscriptions and perform update.
 *
 * @param array $options
 *   Options specified when submitting batch from form.
 */
function affiliate_store_batch_fetch_subscription($options, &$context) {
  affiliate_store_include('batch');
  $type = 'subscription:%';
  if (!isset($context['sandbox']['progress'])) {
    if (!affiliate_store_batch_count($type, TRUE)) {
      // Queue new update if no existing batch.
      affiliate_store_include('subscription');
      affiliate_store_subscription_sync($context['results']['errors']);
    }
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = affiliate_store_batch_count($type);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  $context['max_operation'] = $options['update_throttle'];
  $result = affiliate_store_batch_dequeue($type, $context);
  if ($result === TRUE) {
    // Override original batch message with number of batches finished.
    $context['message'] = t(
      'Fetching subscriptions: @current of @total.',
      array(
        '@current' => $context['sandbox']['progress'] + 1,
        '@total' => $context['sandbox']['total'],
      )
    );
    $context['sandbox']['progress'] += (int) $context['finished'];
  }
  elseif ($result === NULL) {
    return;
  }

  $context['finished'] = (
    $context['sandbox']['progress'] / $context['sandbox']['total']
  );
  sleep($options['sleep']);
}

/**
 * Batch finish callback for affiliate store update process.
 */
function affiliate_store_batch_update_finish($success, $results, $operations) {
  if ($success) {
    affiliate_store_include('batch');
    $errors = affiliate_store_batch_results($results, 'errors');
    if ($errors) {
      $message = t('Error occurred during update.');
      $message .= theme('item_list', array('items' => $errors));
      drupal_set_message($message, 'error');
    }
    else {
      // Update affiliate store last update time so that cron knows that update
      // is run already.
      variable_set('affiliate_store_last_update', REQUEST_TIME);
    }

    $added = affiliate_store_batch_results($results, 'product:added', TRUE);
    if ($added) {
      $message = format_plural(
        count($added),
        '1 product has been added.',
        '@count products have been added.'
      );
      drupal_set_message($message);
    }
    $updated = affiliate_store_batch_results($results, 'product:updated', TRUE);
    if ($updated) {
      $message = format_plural(
        count($updated),
        '1 product has been updated.',
        '@count products have been updated.'
      );
      drupal_set_message($message);
    }
    $deleted = affiliate_store_batch_results($results, 'product:deleted', TRUE);
    if ($deleted) {
      $message = format_plural(
        count($deleted),
        '1 product has been deleted.',
        '@count products have been deleted.'
      );
      drupal_set_message($message);
    }

    $messages = affiliate_store_batch_results($results);
    if ($messages) {
      watchdog(
        'affiliate store',
        'Affiliate store manually updated. !result',
        array('!result' => theme('item_list', array('items' => $messages)))
      );
    }
    elseif (!$errors) {
      drupal_set_message(t('Affiliate store is up to date.'));
    }
  }
  else {
    $operation = reset($operations);
    $message = t(
      'An error occurred while processing %op with arguments: @args',
      array('%op' => $operation[0], '@args' => print_r($operation[1], TRUE))
    );
    drupal_set_message($message, 'error');
  }
}

/**
 * Batch callback; Rename existing term names and synonyms using conversion
 * case configured at settings page.
 */
function affiliate_store_batch_rename_terms(&$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['data'] = array();
    $categories = array('category', 'subcategory', 'subsubcategory');
    foreach (affiliate_store_vocabularies($categories) as $vocab => $info) {
      $context['sandbox']['data'] += db_query(
        "SELECT {$info['as']}tid AS tid, title FROM {affiliate_store_$vocab}"
      )->fetchAllAssoc('tid');
    }
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($context['sandbox']['data']);
    if (!$context['sandbox']['total']) {
      return;
    }
  }

  $context['message'] = t(
    'Renaming term: @current of @total.',
    array(
      '@current' => $context['sandbox']['progress'] + 1,
      '@total' => $context['sandbox']['total'],
    )
  );

  affiliate_store_include('common');
  $data = current($context['sandbox']['data']);
  $term = taxonomy_term_load($data->tid);
  $name = affiliate_store_convert_term_case($data->title);
  $term->name = $name;
  $synonym = affiliate_store_get_term_synonym($term);
  if ($synonym) {
    $synonym = affiliate_store_convert_term_case($synonym);
    affiliate_store_save_data('taxonomy_term', $term, '_synonym', $synonym);
  }
  taxonomy_term_save($term);
  array_shift($context['sandbox']['data']);

  $context['results'][] = t('Updated term %name', array('%name' => $name));
  ++$context['sandbox']['progress'];

  $context['finished'] = (
    $context['sandbox']['data'] ?
    ($context['sandbox']['progress'] / $context['sandbox']['total']) : 1
  );
}

/**
 * Batch finish callback for term renaming.
 */
function affiliate_store_batch_rename_terms_finish($success, $results,
$operations) {
  if ($success) {
    $count = count($results);
    if ($count) {
      $message = format_plural(
        $count,
        '1 term has been renamed.',
        '@count terms have been renamed.'
      );
      watchdog(
        'affiliate store',
        'Existing terms renamed. !result',
        array('!result' => theme('item_list', array('items' => $results)))
      );
    }
    else {
      $message = t('No term has been renamed.');
    }
  }
  else {
    $operation = reset($operations);
    $message = t(
      'An error occurred while processing %op with arguments: @args',
      array('%op' => $operation[0], '@args' => print_r($operation[1], TRUE))
    );
  }
  drupal_set_message($message, $success ? 'status' : 'error');
}
