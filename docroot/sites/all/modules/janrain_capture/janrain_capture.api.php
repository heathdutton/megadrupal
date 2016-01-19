<?php
/**
 * @file
 * Hooks provided by Janrain Capture
 */

/**
 * Act on users when authenticated
 *
 * This hook is executed immediately after the user authentication method is
 * processed but before the page is rendered. Modules may use this to act on
 * user data received during authentication.
 *
 * @param array $capture_profile
 *   The profile data returned from the user's Capture record
 * @param array $account
 *   The local Drupal user account being authenticated
 * @param boolean $new_user
 *   TRUE if this is the users first time to this site
 */
function hook_janrain_capture_user_authenticated($capture_profile, $account, $new_user) {
  if ($new_user) {
    $params['account'] = $account;

    // EXAMPLE:
    // Execute the welcome_message_mail function with key being either 'male',
    // 'female', or NULL.
    drupal_mail('welcome_message',
      $capture_profile['gender'],
      $account->mail,
      user_preferred_language($account),
      $params);
  }
}

/**
 * Act on users when data is updated
 *
 * This hook is executed immediately after the user profile is updated in
 * Capture and synchronized with the local Drupal user. By default this the
 * completion of this process will redirect the user to $origin. If you wish
 * to prevent this behavior you can do so by returning FALSE from an
 * implementation of this hook.
 *
 * @param array $capture_profile
 *   The profile data returned from the user's Capture record
 * @param array $account
 *   The local Drupal user account being authenticated
 *
 * @return boolean
 *   If FALSE Drupal will not redirect the user to $origin
 */
function hook_janrain_capture_user_profile_updated($capture_profile, $account) {
  // Update status message and redirect user to home page.
  drupal_set_message(t('Profile Updated!'), 'status');
  drupal_goto();
  return FALSE;
}

/**
 * Add Capture attributes to sync to local user fields
 *
 * This hook allows modules to alter the user account object based on Janrain
 * Capture profile data. It is invoked whenever a user logs in or updates their
 * profile in Janrain Capture.
 *
 * The hook can be used in conjunction with Entity API module to set the values
 * of Field API fields on the user object, or without Entity API to alter any
 * properties (including fields, but the full array stucture of the field would
 * need to be provided) on the object.
 *
 * @param $account
 *   The user account to sync
 *
 * @param array $capture_profile
 *   The profile data returned from the user's Capture record
 */
function hook_janrain_capture_janrain_capture_profile_sync($account, $capture_profile) {
  // This example uses Entity API module to alter the 'field_gender' value based
  // on what's received from Janrain Capture. The $account parameter is an object
  // and so it is passed by reference.
  if (isset($capture_profile['gender'])) {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $account_wrapper->field_gender = $capture_profile['gender'];
  }
}

/**
 * User already mapped
 *
 * Triggered when 'Match users on email' is enabled and the matched account
 * is already mapped to another Capture record.
 *
 * @return boolean
 *   Display the default message or not
 */
function hook_janrain_capture_user_already_mapped() {
  drupal_set_message(t('There is another account with this email address.'), 'error');
  return FALSE;
}

/**
 * User exists
 *
 * Triggered when 'Match users on email' is disabled and a local user exists
 * with the email being used to authenticate.
 *
 * @return boolean
 *   Display the default message or not
 */
function hook_janrain_capture_user_exists() {
  drupal_set_message(t('There is another account with this email address.'), 'error');
  return FALSE;
}

/**
 * Failed to create user
 *
 * Triggered when the user_save() method doesn't return a valid user
 *
 * @return boolean
 *   Display the default message or not
 */
function hook_janrain_capture_failed_create() {
  drupal_set_message(t('We could not process your request.'), 'error');
  return FALSE;
}

/**
 * Email has not been verified
 *
 * Triggered when 'Enforce email verification' is enabled and user has not yet
 * verified the email in the Capture record.
 *
 * @param string $resend_link
 *   The URL to trigger a resend of the verification email
 *
 * @return boolean
 *   Display the default message or not
 */
function hook_janrain_capture_email_unverified($resend_link) {
  drupal_set_message(
    t("You haven't verified your email! <a href='@resend-link'>Click here to resend it</a>",
      array('@resend-link', $resend_link)), 'error');
  return FALSE;
}

/**
 * OAuth endpoint called without code
 *
 * Triggered when the janrain_capture/oauth URL is requested without a 'code'
 * parameter
 *
 * @return boolean
 *   Display the default message or not
 */
function hook_janrain_capture_no_oauth() {
  drupal_set_message(t('You cannot access this page directly.'), 'error');
  return FALSE;
}

/**
 * Verification email resent
 *
 * Triggered when the janrain_capture/resend_verification_email URL is
 * requested. This URL is passed in as the confirmation redirect to Capture's
 * email resend method.
 *
 * @return boolean
 *   Display the default message and redirect to the home page or neither
 */
function hook_janrain_capture_verification_resent() {
  drupal_set_message(t('Go check your email!'), 'status');
  return FALSE;
}
