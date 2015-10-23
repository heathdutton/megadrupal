<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup eloqua_subscription_center Eloqua Subscription Center module
 * integrations.
 *
 * Module integrations with the Eloqua Subscription Center module.
 */

/**
 * @defgroup eloqua_subscription_center_hooks Eloqua Subscription Center's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend the Eloqua
 * Subscription Center module.
 */


/**
 * Alter the e-mail address used to search for contact details on the "my
 * subscriptions" redirect menu item. This allows you to substitute an alternate
 * e-mail address that might be, for example, stored on another field on the
 * user.
 *
 * @param string $email
 *   The e-mail address used to lookup the contact record corresponding to the
 *   currently authenticated user.
 *
 * @see eloqua_subscription_center_my_page()
 */
function hook_eloqua_subscription_center_my_email_alter(&$email) {
  global $user;
  $email = $user->my_custom_email_field[LANGUAGE_NONE][0]['value'];
}


/**
 * @}
 */
