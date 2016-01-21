<?php

namespace Drupal\soauth_twitter;

use Drupal\soauth\Account;
use Drupal\soauth\Request;
use Drupal\soauth\Plugin as BasePlugin;


/**
 * Twitter Plugin
 * @author Raman
 */
class Plugin extends BasePlugin {
  
  const NAME = 'twitter';
  
  public final function getName() {
    return self::NAME;
  }
  
  public final function getFullName() {
    return t('Twitter');
  }  
  
  protected final function getRequest($path='') {
    return new Request('https://api.twitter.com'.$path);
  }
  
  protected final function getApiRequest($path='') {
    return new ApiRequest($path);
  }
  
  protected final function getRedirect($token) {
    $url = $this->getRequest('/oauth/authenticate')
      ->param('oauth_token', $token);
    
    return $url;
  }
  
  public final function actionConnect($query) {
    // Twitter uses OAuth 1.1 authentication flow... we must adjust our it.
    // Try this: https://dev.twitter.com/web/sign-in/implementing
    // At first we must obtain a request token, but before it - calculate
    // @note See this: https://twittercommunity.com/t/invalid-oauth-verifier-parameter/38724/2
    // signature for this request.
    // @todo Error checking
    // @todo Check oauth_token match
    
    // Check if user confirm authorization
    if (isset($query['oauth_token']) && isset($query['oauth_verifier'])) {
      
      $params = array(
        'oauth_consumer_key' => $this->getAppId(),
        'oauth_nonce' => drupal_get_token(strval(time())),
        'oauth_signature_method' => 'HMAC-SHA1',
        'oauth_timestamp' => strval(time()),
        'oauth_token' => $query['oauth_token'],
        'oauth_verifier' => $query['oauth_verifier'],
        'oauth_version' => '1.0',
      );
      
      $request = $this->getApiRequest('/oauth/access_token');
      
      // Calculate request signature
      $signature = $request->makeSignature($this->getAppSecret(), $params);

      $params['oauth_signature'] = $signature;
      
      // Building the header string
      $header = 'OAuth ';

      foreach ($params as $key => $value) {
        $header .= rawurlencode($key);
        $header .= '="';
        $header .= rawurlencode($value);
        $header .= '", ';
      }
      
      //watchdog('Twitter', print_r($query['oauth_verifier'], TRUE));

      // Add authorization header to request
      $request->hdr('Authorization', substr($header, 0, -2));
      //$request->body('oauth_verifier='.$query['oauth_verifier']);

      // Obtaining a request token
      $token = $request->execute('POST', 'drupal_get_query_array');
      
      watchdog('Twitter', print_r($token, TRUE));
      
      return;
    }
    
    $key = $this->getAppId();
    $secret = $this->getAppSecret();
    
    $params = array(
      'oauth_consumer_key' => $key,
      'oauth_nonce' => drupal_get_token(strval(time())),
      'oauth_signature_method' => 'HMAC-SHA1',
      'oauth_timestamp' => strval(time()),
      'oauth_version' => '1.0',
    );
    
    $request = $this->getApiRequest('/oauth/request_token');
    
    // Calculate request signature
    $signature = $request->makeSignature($secret, $params);
    
    $params['oauth_signature'] = $signature;
    
    // Building the header string
    $header = 'OAuth ';
    
    foreach ($params as $key => $value) {
      $header .= rawurlencode($key);
      $header .= '="';
      $header .= rawurlencode($value);
      $header .= '", ';
    }
    
    // Add authorization header to request
    $request->hdr('Authorization', substr($header, 0, -2));
    
    // Obtaining a request token
    $token = $request->execute('POST', 'drupal_get_query_array');
    
    // Go to authorization page
    drupal_goto($this->getRedirect($token['oauth_token']));
  }
}
