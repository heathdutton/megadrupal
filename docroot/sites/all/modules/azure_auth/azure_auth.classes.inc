<?php

/**
 * @file
 * Defines interfaces and classes relating to Windows Azure authentication
 * tokens.
 */

interface AzureAuthenticationTokenInterface {
  /**
   * Ensures token is valid.
   */
  public function isValid();

  /**
   * Returns the provider name.
   */
  public function getProviderName();

  /**
   * Returns the name identifier.
   */
  public function getNameIdentifier();

  /**
   * Returns the specified claim.
   */
  public function getClaim($key);
}

interface AzureAuthenticationIdentityProviderInterface {
  /**
   * Formats a claim value or returns the original claim value.
   */
  public function formatClaim($name, $raw_claim);

  /**
   * Returns the name of the identity provider.
   */
  public function getName();
}

class AzureAuthenticationSWT implements AzureAuthenticationTokenInterface {

  const ISSUER_LABEL = "Issuer";
  const EXPIRES_LABEL = "ExpiresOn";
  const AUDIENCE_LABEL = "Audience";
  const HMACSHA256_LABEL = "HMACSHA256";

  protected $claims;

  /**
   * @var IAzureAuthenticationIdentityProvider
   */
  protected $provider;
  protected $providerMap;

  protected $rawToken;
  protected $audience;
  protected $issuer;
  protected $signingKey;
  protected $oldSigningKey;

  /**
   * Parses a raw token and populates a new instance.
   */
  public function __construct($raw_token, $audience, $issuer, $signing_key, $old_signing_key, $provider_map) {
    $this->rawToken = $raw_token;
    $this->audience = $audience;
    $this->issuer = $issuer;
    $this->signingKey = $signing_key;
    $this->old_SigningKey = $old_signing_key;
    $this->providerMap = $provider_map;

    $xml = new SimpleXMLElement(stripslashes($raw_token));
    $xml->registerXPathNamespace('t', 'http://schemas.xmlsoap.org/ws/2005/02/trust');
    $xml->registerXPathNamespace('wsse', 'http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd');

    $token_query = $xml->xpath('//t:RequestSecurityTokenResponse/t:RequestedSecurityToken/wsse:BinarySecurityToken');
    if (count($token_query)) {
      $token_query = $token_query[0];
    }
    else {
      return;
    }
    $base64_token = $token_query;
    $this->rawToken = base64_decode($base64_token);

    if (is_null($this->rawToken) || $this->rawToken == '') {
      return;
    }

    $this->claims = array();
    $token = explode("&", $this->rawToken);
    foreach ($token as $value) {
      $value = explode("=", $value);
      $this->claims[urldecode($value[0])] = urldecode($value[1]);
    }
    foreach ($this->providerMap as $provider_name => $provider) {
      if ($provider_name == $this->claims[AZURE_AUTH_IDENTITY_PROVIDER_SCHEMA]) {
        if (class_exists($provider['class'])) {
          $this->provider = new $provider['class']();
        }
      }
    }

  }

  /**
   * Ensures the token is valid.
   */
  public function isValid() {
    if (!$this->claims) {
      return FALSE;
    }
    if ($this->isExpired()) {
      return FALSE;
    }
    if (!$this->isIssuerTrusted()) {
      return FALSE;
    }
    if (!$this->isAudienceTrusted()) {
      return FALSE;
    }
    if (!$this->isHMACValid()) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Returns the provider name.
   */
  public function getProviderName() {
    if ($this->provider) {
      return $this->provider->getName();
    }
    else {
      return $this->claims[AZURE_AUTH_IDENTITY_PROVIDER_SCHEMA];
    }
  }

  /**
   * Returns the name identifier.
   */
  public function getNameIdentifier() {
    return $this->getClaim(AZURE_AUTH_NAME_IDENTIFIER_SCHEMA);
  }

  /**
   * Returns a claim from the token.
   *
   * Returns a claim from the token if present, NULL if not. Formats the claim
   * value if possible.
   *
   * @param string $key
   *   Claim name.
   *
   * @return null|string
   *   Claim if present, NULL if not.
   */
  public function getClaim($key) {
    if (isset($this->claims[$key])) {
      if ($this->provider) {
        return $this->provider->formatClaim($key, $this->claims[$key]);
      }
      else {
        return $this->claims[$key];
      }
    }
    else {
      return NULL;
    }
  }

  /**
   * Make sure the token is current.
   */
  protected function isExpired() {
    if (drupal_strlen($this->getClaim(self::EXPIRES_LABEL)) > 0) {
      if (intval(time()) > intval($this->getClaim(self::EXPIRES_LABEL))) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Make sure the issuer and the namespace match.
   */
  protected function isIssuerTrusted() {
    if (drupal_strlen($this->getClaim(self::ISSUER_LABEL)) > 0) {
      if ($this->getClaim(self::ISSUER_LABEL) == 'https://' . $this->issuer . '.accesscontrol.windows.net/') {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Ensure the audience and realm match.
   */
  protected function isAudienceTrusted() {
    if (drupal_strlen($this->getClaim(self::AUDIENCE_LABEL)) > 0) {
      if ($this->getClaim(self::AUDIENCE_LABEL) == $this->audience) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Match the HMAC hash against the token signing key.
   */
  protected function isHMACValid() {
    $token_hmac = explode('&' . self::HMACSHA256_LABEL . '=', $this->rawToken);
    if (count($token_hmac) != 2) {
      return FALSE;
    }
    $swt = $token_hmac[0];
    $token_hmac = explode('&', $token_hmac[1]);
    $hmac = urldecode($token_hmac[0]);

    $signature = base64_encode(hash_hmac('sha256', $swt, base64_decode($this->signingKey), TRUE));
    if ($hmac == $signature) {
      return TRUE;
    }

    $signature = base64_encode(hash_hmac('sha256', $swt, base64_decode($this->old_SigningKey), TRUE));
    if ($hmac == $signature) {
      return TRUE;
    }

    return FALSE;
  }

}
