<?php

/**
 * @file
 * Contains the services authentication callback and helper functions.
 */

/**
 * Authentication callback for the remote client authentication.
 */
function tmgmt_server_authentication_callback($settings, $method, $args) {
  global $user;

  $error = NULL;
  $headers = getallheaders();

  // No authenticate header provided - nothing to do.
  if (empty($headers['Authenticate'])) {
    return NULL;
  }

  try {
    tmgmt_auth_issuer_controller()->verifyAuthToken($headers['Authenticate']);
    $authenticated_entity = tmgmt_auth_issuer_controller()->getAuthenticatedEntity();

    if (empty($authenticated_entity)) {
      $error = t('Unknown remote entity.');
    }
    else {
      $client = entity_load_single($authenticated_entity->entityType(), $authenticated_entity->identifier());
      $user = user_load($client->uid);
      watchdog('tmgmt_server',
        'Remote client id %id authenticated as user %name using auth_token.',
        array('%id' => $authenticated_entity->identifier(), '%name' => $user->name),
        WATCHDOG_INFO
      );
    }
  }
  catch (TMGMTAuthIssuerException $e) {
    $error = t('Invalid authentication data.');
  }
  catch (TMGMTAuthMalformedAuthStringException $e) {
    $error = t('Malformed authentication data.');
  }
  catch (Exception $e) {
    $error = t('Error occurred during the authentication process.');
  }

  if ($error) {
    watchdog('tmgmt_server',
      'Remote client failed authentication using auth_token %token with error %error.',
      array('%token' => $headers['Authenticate'], '%error' => $error),
      WATCHDOG_WARNING
    );
  }

  return $error;
}

