<?php

namespace Drupal\soauth_instagram;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field\SimpleField;
use Drupal\soauth\Common\Field\CompoundField;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * Instagram SoAuth provider.
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Provider extends AbstractProvider {
  
  /**
   * Construct Provider
   */
  public function __construct() {
    parent::__construct();
  }
  
  /**
   * {@inheritdoc}
   */
  public function getScope($glue='+') {
    return parent::getScope($glue);
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return 'instagram';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return 'Instagram';
  }
  
  /**
   * {@inheritdoc}
   */
  protected function getAccessToken($code) {
    // Build POST body
    $query = drupal_http_build_query(array(
      'client_id' => $this->getAppId(),
      'client_secret' => $this->getAppSecret(),
      'code' => $code,
      'grant_type' => 'authorization_code',
      'redirect_uri' => $this->getRedirectUrl(),
    ));
    
    $response = Request::create($this->getRequestTokenUrl(), 'POST')
      ->setBody($query)
      ->send();
    
    // Throw exception if any error occured
    if (isset($response->error)) {
      throw new OAuthError(t('@provider API returned an unexpected response. Text: @text', array(
        '@provider' => $this->getFullName(),
        '@text' => $response->error,
      )));
    }
    
    // Parse response. Format: JSON
    $body = drupal_json_decode($response->data);
    
    return $body['access_token'];
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('https://api.instagram.com/oauth/authorize');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('https://api.instagram.com/oauth/access_token/');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new SimpleField('data/id'),
      'mail' => new SimpleField('email', 'raman@cmstuning.net'),
      'first_name' => new SimpleField('data/full_name'),
      'last_name' => new SimpleField('data/full_name'),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    $request = Request::create($this->buildApiEndpointUrl('/users/self')
      ->setQuery(array(
        'access_token' => $token,
      ))
    );
    
    // Send request
    $response = $request->send();
    
    // Throw exception if any error occured
    if (isset($response->error)) {
      throw new SoAuthError(t('@provider API returned an unexpected response.', array(
        '@provider' => $this->getFullName(),
      )));
    }
        
    return drupal_json_decode($response->data);
  }
  
  /**
   * {@inheritdoc}
   */
  public function setDefaults() {
    $this->getStorage()->set('scope', array(
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri('https://api.instagram.com/v1'.$path);
  }
}
