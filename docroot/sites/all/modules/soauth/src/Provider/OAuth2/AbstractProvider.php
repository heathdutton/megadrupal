<?php

namespace Drupal\soauth\Provider\OAuth2;

use Drupal\soauth\Service;
use Drupal\soauth\Error\SoAuthError;
use Drupal\soauth\Error\OAuth\OAuthError;
use Drupal\soauth\Manager\AccountManager;
use Drupal\soauth\Common\Entity\Account;
use Drupal\soauth\Common\Entity\User;
use Drupal\soauth\Common\Http\Request;
use Drupal\soauth\Provider\AbstractBaseProvider;


/**
 * Class AbstractProvider
 * @author Raman Liubimau <raman@cmstuning.net>
 */
abstract class AbstractProvider extends AbstractBaseProvider {
  
  /**
   * Construct AbstractProvider
   */
  public function __construct() {
    parent::__construct();
  }
  
  /**
   * Get App Id
   * @return string
   */
  public function getAppId() {
    return $this->getStorage()->get('app_id', '');    
  }
  
  /**
   * Get App Secret
   * @return string
   */
  public function getAppSecret() {
    return $this->getStorage()->get('app_secret', '');
  }
  
  /**
   * Exchange code for an access token
   * @param string $code
   * @return string
   */
  protected function getAccessToken($code) {
    // Build request url
    $url = $this->getRequestTokenUrl()->setQuery(array(
      'client_id' => $this->getAppId(),
      'client_secret' => $this->getAppSecret(),
      'code' => $code,
      'grant_type' => 'authorization_code',
      'redirect_uri' => $this->getRedirectUrl(),
    ));
    
          watchdog('SoAuth', print_r($this, TRUE));
    
    // Send HTTP request
    $response = Request::create($url, 'POST')->send();
    
    // Throw exception if any error occured
    if (isset($response->error)) {
      throw new OAuthError(t('@provider API returned an unexpected response. Text: @text', array(
        '@provider' => $this->getFullName(),
        '@text' => $response->data,
      )));
    }
    
    // Parse response. Format: JSON
    $body = drupal_json_decode($response->data);
    
    return $body['access_token'];
  }

  /**
   * Make authorization url
   * @return Url
   */
  private function makeAuthUrl() {
    return $this->getAuthorizeUrl()->setQuery(array(
      'scope' => $this->getScope(),
      'state' => drupal_get_token(), /* TODO!! */
      'client_id' => $this->getAppId(),
      'redirect_uri' => $this->getRedirectUrl(),
      'response_type' => 'code',
    ));
  }
  
  /**
   * {@inheritdoc}
   */
  public function makeSettingsForm() {
    return array(
      '#tree' => TRUE,
      'name' => array(
        '#markup' => $this->getFullName(),),
      'weight' => array(
        '#type' => 'weight',
        '#title' => '',
        '#default_value' => $this->getWeight(),
        '#attributes' => array(
          'class' => array(
            'provider-weight',),),),
      'app_id' => array(
        '#type' => 'textfield',
        '#size' => 48,
        '#title' => '',
        '#default_value' => $this->getAppId(),),
      'app_secret' => array(
        '#type' => 'textfield',
        '#size' => 48,
        '#title' => '',
        '#default_value' => $this->getAppSecret(),),
      'scope' => array(
        '#type' => 'textfield',
        '#size' => 30,
        '#title' => '',
        '#default_value' => $this->getScope(' '),),
    );   
  }
  
  /**
   * {@inheritdoc}
   */
  public function submitSettings($values) {
    // Store values
    $this->getStorage()->map(array(
      'scope' => explode(' ', $values['scope']),
      'weight' => $values['weight'],
      'app_id' => $values['app_id'],
      'app_secret' => $values['app_secret'],
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function actionConnect($query) { 
    // Check errors in sever response
    if (isset($query['error'])) {
      throw new OAuthError(t('OAuth error: "@error". Reason: "@reason". Description: "@description"', array(
        '@error' => $query['error'],
        '@reason' => (isset($query['error_reason']) ? $query['error_reason'] : ''),
        '@description' => (isset($query['error_description']) ? $query['error_description'] : ''),
      )));
    }
    
    // If server response is code
    if (isset($query['code'])) {
      // Get access token
      $token = $this->getAccessToken($query['code']);
      
      // Get uniform user data
      $data = $this->getUniformUserData($token);
      
      if (!(isset($data['id']) && isset($data['mail']))) {
        throw new SoAuthError(t('Missing reuired data. '
          .'At least, provider must provide [id] and [mail] for user, but it seems look like not.'
          .'Dump: @dump', array(
          '@dump' => print_r($data, TRUE),
        )));
      }
      
      $result = $this->findOrCreateUser($data);
      
      // Get SoAuth service settings
      $settings = Service::getInstance()->getSettings();
      
      // Submit new user account
      $am = new AccountManager($result['user']);
      $am->addAccount(Account::fromData($this, $data));
      
      $result['user']->login();
      
      if ($settings->get('after_login_message_enabled', FALSE) && 
          ($result['action'] === 'login' || $result['action'] === 'connect')) {
        drupal_set_message(t($settings->get('after_login_message_text'), array(
          '@provider' => $this->getFullName(),
        )));
      }
      
      if ($settings->get('after_registration_message_enabled', FALSE) && $result['action'] === 'registration') {
        drupal_set_message($settings->get(t('after_registration_message_text'), array(
          '@provider' => $this->getFullName(),
        )));
      }
      
      return;
    }
    
    // Save destination url for redirect after authentication.
    $this->setDestinationUrl($query);
    
    // Take redirect
    drupal_goto($this->makeAuthUrl());
  }
  
  /**
   * {@inheritdoc}
   */
  public function actionDeauthorize($query) {
    // Insert code here
  }
}
