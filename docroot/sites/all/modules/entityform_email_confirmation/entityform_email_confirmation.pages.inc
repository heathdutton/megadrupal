<?php

/**
 * Confirmation link callback.
 */
function entityform_email_confirmation_page() {
  $parameters = drupal_get_query_parameters();

  if (!isset($parameters['id']) || !isset($parameters['email']) || !isset($parameters['hash'])) {
    drupal_goto();
  }

  $entityform_id = $parameters['id'];
  $email = $parameters['email'];
  $hash = $parameters['hash'];

  $real_hash = entityform_email_confirmation_get_hash($entityform_id, $email);

  if ($hash === $real_hash && $entityform = entityform_load($entityform_id)) {
    rules_invoke_event('entityform_email_confirmation_confirmed', $entityform, $email);
    return t('Your e-mail: @email has been confirmed. Thanks!', array('@email' => $email));
  }
  return t('Wrong confirmation URL.');
}
