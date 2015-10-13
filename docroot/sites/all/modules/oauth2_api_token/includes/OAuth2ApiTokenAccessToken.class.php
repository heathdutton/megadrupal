<?php
/**
 * @file
 * Contains OAuth2ApiTokenAccessToken class.
 */

/**
 * Override \OAuth2\ResponseType\AccessToken to support API tokens.
 */
class OAuth2ApiTokenAccessToken extends \OAuth2\ResponseType\AccessToken {

  /**
   * Overrides \OAuth2\ResponseType\AccessToken::__construct().
   *
   * Removes all the arguments as they are irrelevant.
   */
  public function __construct() {
    parent::__construct(new \Drupal\oauth2_server\Storage(), NULL, array(
      'access_lifetime' => 0,
    ));
  }

  /**
   * Save a new API token.
   *
   * @param OAuth2ServerToken $token
   *   A token entity with pre-populated 'client_id', 'uid', and 'scopes'
   *   properties.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  public function saveApiToken(OAuth2ServerToken $token) {
    foreach (array('client_id', 'uid', 'scopes') as $property) {
      if (empty($token->$property)) {
        throw new \RuntimeException('Missing required API token property: ' . $property);
      }
    }

    if (!isset($token->token)) {
      $token->token = $this->generateAccessToken();
    }

    $token->type = 'api_token';

    // Force the expiry time to 19 January 2038: this is the easiest way to have
    // a never-expiring token, for now.
    // See https://github.com/bshaffer/oauth2-server-php/issues/166
    $token->expires = 2147483647;

    return $token->save();
  }

}
