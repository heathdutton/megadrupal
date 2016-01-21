<?php
/**
 * @file
 * Hostedpage retrieve page.
 */

/**
 * Page function.
 */
function chargebee_hostedpage_retrieve_page() {
  $params = drupal_get_query_parameters();
  if (!isset($params['id']) || !isset($params['state'])) {
    return FALSE;
  }
  $hostedpage = chargebee_hostedpage_retrieve($params['id']);
  if ($hostedpage) {
    switch ($hostedpage['state']) {
      case 'created':
      case 'requested':
      case 'cancelled':
        drupal_set_message('Hostedpage without paying', 'error');
        break;

      case 'succeeded':
        chargebee_hostedpage_retrieve_page_succeeded($hostedpage);
        break;

      case 'failed':
        drupal_set_message('Payment errors.');
    }
    // @TODO Redirect to subscription page.
    drupal_goto('<front>');
  }
  return FALSE;
}

/**
 * Create/update user entries for succeeded payment.
 *
 * @param array $hostedpage
 *   Hostedpage array.
 *
 * @return bool
 *   Subscription process state.
 */
function chargebee_hostedpage_retrieve_page_succeeded($hostedpage) {
  $user = user_load_by_mail($hostedpage['content']['customer']['email']);
  if (!$user) {
    return FALSE;
  }
  $entries = array('subscription', 'customer', 'card');
  foreach ($entries as $entry) {
    $load_function = 'chargebee_' . $entry . '_load';
    $entity = $load_function($user->uid);
    $status = NULL;
    if ($entity) {
      $update_function = 'chargebee_' . $entry . '_update';
      $status = $update_function($entity, $hostedpage['content'][$entry]);
    }
    else {
      $create_function = 'chargebee_' . $entry . '_create';
      $status = $create_function($hostedpage['content'][$entry]);
    }
    _chargebee_hostedpage_retrieve_page_message($entry, $status);
  }
  return TRUE;
}

/**
 * Display retrieve message.
 */
function _chargebee_hostedpage_retrieve_page_message($entry, $status) {
  $entry = ucfirst($entry);
  $message = $entry . ' created/updated';
  $message_status = 'status';
  if (!$status) {
    $message = $entry . ' not created/updated';
    $message_status = 'error';
  }
  drupal_set_message($message, $message_status);
}
