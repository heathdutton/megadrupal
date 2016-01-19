<?php

namespace Drupal\soauth_facebook;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field\SimpleField;
use Drupal\soauth\Common\Field\CompoundField;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * Facebook SoAuth provider.
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
  public function getScope($glue=',') {
    return parent::getScope($glue);
  }
  
  /**
   * {@inheritdoc}
   */
  public function getName() {
    return 'facebook';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return 'Facebook';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('https://www.facebook.com/dialog/oauth');
  }  
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('https://graph.facebook.com/v2.3/oauth/access_token');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new SimpleField('id'),
      'mail' => new SimpleField('email'),
      'first_name' => new SimpleField('first_name'),
      'last_name' => new SimpleField('last_name'),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    // Build request
    $request = Request::create($this->buildApiEndpointUrl('/me')
      ->setQuery(array(
        'access_token' => $token,
        'fields' => 'id,email,first_name,last_name',
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
      'public_profile',
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri('https://graph.facebook.com'.$path);
  }
}
