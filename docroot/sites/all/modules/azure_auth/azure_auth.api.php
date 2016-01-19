<?php

/**
 * @file
 * Hooks provided by Windows Azure Authentication module.
 */

/**
 * Informs Windows Azure Authentication module of available identity providers.
 *
 * @return array
 *   An array of identity provider names along with the corresponding
 *   IAzureAuthenticationIdentityProvider implementation.
 */
function hook_azure_identity_providers() {
  return array(
    'MyProvider' => array(
      'class' => 'AzureAuthenticationIdentityProviderMyProvider',
    ),
  );
}


/**
 * Allows the modification of the identity provider map.
 */
function hook_azure_identity_providers_alter(&$identity_providers) {
  $identity_providers['MyProvider']['class'] = 'AzureAuthenticationIdentityProviderMyProvider';
}
