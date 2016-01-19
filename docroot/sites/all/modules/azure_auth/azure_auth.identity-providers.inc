<?php

/**
 * @file
 * Provides default implementations for Google and Windows Live identity
 * providers.
 */

/**
 * Implements hook_azure_identity_providers().
 */
function azure_auth_azure_identity_providers() {
  return array(
    'Google' => array(
      'class' => 'AzureAuthenticationIdentityProviderGoogle',
    ),
    'uri:WindowsLiveID' => array(
      'class' => 'AzureAuthenticationIdentityProviderWindowsLive',
    ),
  );
}

class AzureAuthenticationIdentityProviderGoogle implements AzureAuthenticationIdentityProviderInterface {

  /**
   * Returns the formatted claim.
   *
   * Google's name identifier claim is a URL. This extracts only the profile
   * hash we care about.
   *
   * @param string $name
   *   The name of the claim.
   * @param string $raw_claim
   *   The unformatted claim value.
   *
   * @return string
   *   The formatted claim, or original unformatted claim.
   */
  public function formatClaim($name, $raw_claim) {
    switch ($name) {
      case AZURE_AUTH_NAME_IDENTIFIER_SCHEMA:
        $unprocessed_name_identifier = $raw_claim;
        $name_identifier_url = parse_url($unprocessed_name_identifier);
        parse_str($name_identifier_url['query'], $parsed_identitier_querystring);
        return $parsed_identitier_querystring['id'];
        break;

      default:
        return $raw_claim;

    }
  }

  /**
   * Returns the name of the identity provider.
   */
  public function getName() {
    return 'Google';
  }

}

class AzureAuthenticationIdentityProviderWindowsLive implements AzureAuthenticationIdentityProviderInterface {

  /**
   * Returns the claim.
   *
   * Windows Live doesn't do anything weird with their name identifier, so we
   * return the raw value here.
   *
   * @param string $name
   *   The name of the claim.
   * @param string $raw_claim
   *   The unformatted claim value.
   *
   * @return string
   *   The original claim value.
   */
  public function formatClaim($name, $raw_claim) {
    return $raw_claim;
  }

  /**
   * Returns the name of the identity provider.
   */
  public function getName() {
    return 'Windows Live';
  }

}
