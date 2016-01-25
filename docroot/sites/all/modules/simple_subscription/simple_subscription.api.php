<?php

/**
 * @file
 * Hooks provided by Simple subscription.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to do something with the simple subscription form results.
 *
 * @param array $form_data
 *   An array with the form results. Currently only contains the submitted
 *   email address.
 */
function hook_simple_subscription($form_data) {
  $email = $form_data['simple-subscription-submit-values']['email'];

  // do something with this ...
}

/**
 * Act on a subscription object after creation.
 *
 * @param stdClass $subscription
 *   A simple subscription object.
 */
function hook_subscription_insert($subscription) {
  // do something with this ...
}

/**
 * Act on a subscription object after modification.
 *
 * @param stdClass $subscription
 *   A simple subscription object.
 */
function hook_subscription_update($subscription) {
  // do something with this ...
}

/**
 * Act on a subscription object after delete.
 *
 * @param stdClass $subscription
 *   A simple subscription object.
 */
function hook_subscription_delete($subscription) {
  // do something with this ...
}

/**
 * @} End of "addtogroup hooks".
 */
