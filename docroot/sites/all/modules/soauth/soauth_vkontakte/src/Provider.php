<?php

namespace Drupal\soauth_vkontakte;

use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Common\Url;
use Drupal\soauth\Common\Field;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\OAuth2\AbstractProvider;


/**
 * Class Provider
 * VKontakte SoAuth provider.
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
    return 'vkontakte';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getFullName() {
    return 'VKontakte';
  }
  
  /**
   * {@inheritdoc}
   */
  public function getAuthorizeUrl() {
    return Url::fromUri('https://oauth.vk.com/authorize');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRequestTokenUrl() {
    return Url::fromUri('https://oauth.vk.com/access_token');
  }
  
  /**
   * {@inheritdoc}
   */
  public function getUniformFieldMap() {
    return array(
      'id' => new Field('response/0/uid'),
      'mail' => new Field(''),
      'first_name' => new Field('response/0/first_name'),
      'last_name' => new Field('response/0/last_name'),
    );
  }
  
  /**
   * {@inheritdoc}
   */
  public function getRawUserData($token) {
    // Build request
    $request = Request::create($this->buildApiEndpointUrl('/users.get')
      ->setQuery(array(
        'fields' => 'uid,first_name,last_name',
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
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function buildApiEndpointUrl($path) {
    return Url::fromUri('https://api.vk.com/method'.$path);
  }
}
