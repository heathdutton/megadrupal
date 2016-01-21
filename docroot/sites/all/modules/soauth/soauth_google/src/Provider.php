<?php

namespace Drupal\soauth_google;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field\SimpleField;
use Drupal\soauth\Common\Field\CompoundField;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * Google Plus SoAuth provider.
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
  public function getScope($glue=' ') {
    return parent::getScope($glue);
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return 'google';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return 'Google Plus';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('https://accounts.google.com/o/oauth2/auth');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('https://www.googleapis.com/oauth2/v3/token');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new SimpleField('id'),
      'mail' => new SimpleField('emails/0/value'),
      'first_name' => new SimpleField('name/givenName'),
      'last_name' => new SimpleField('name/familyName'),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    // Build request
    $request = Request::create($this->buildApiEndpointUrl('/plus/v1/people/me')
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
      'email',
      'profile',
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri('https://www.googleapis.com'.$path);
  }
}
