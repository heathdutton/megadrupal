<?php

namespace Drupal\soauth_linkedin;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * LinkedIn SoAuth provider.
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
    return 'linkedin';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return 'LinkedIn';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('https://www.linkedin.com/uas/oauth2/authorization');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('https://www.linkedin.com/uas/oauth2/accessToken');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new Field('id'),
      'mail' => new Field('emailAddress'),
      'first_name' => new Field('firstName'),
      'last_name' => new Field('lastName'),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    // Build request
    $request = Request::create($this->buildApiEndpointUrl('/v1/people/~:(id,email-address,first-name,last-name)')
      ->setQuery(array('format' => 'json',)))
      ->setHeader('Authorization', 'Bearer '.$token);
    
    // Send request
    $response = $request->send();
    
    // Throw exception if any error occured
    if (isset($response->error)) {
      throw new SoAuthError(t('@provider API returned an unexpected response. Text: @text', array(
        '@provider' => $this->getFullName(),
        '@text' => $response->error,
      )));
    }
    
    return drupal_json_decode($response->data);
  }
  
  /**
   * {@inheritdoc}
   */
  public function setDefaults() {
    $this->getStorage()->set('scope', array(
      'r_emailaddress',
      'r_basicprofile',
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri('https://api.linkedin.com'.$path);
  }
}
