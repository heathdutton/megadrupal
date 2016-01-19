<?php

namespace Drupal\soauth_twitter;

use Drupal\soauth\ApiRequest as AbstractApiRequest;


/**
 * Twitter API helper class.
 * @author Raman Liubimau
 */
class ApiRequest extends AbstractApiRequest {
  
  public function __construct($path) {
    parent::__construct($path);
  }
  
  /**
   * Generate an OAuth 1.0a HMAC-SHA1 signature for a HTTP request
   * @see https://dev.twitter.com/oauth/overview/creating-signatures
   * @todo NOTE! you must call this method after set query params!
   * @param string $secret Consumer secret
   * @param array $data
   * @return string
   */
  public function makeSignature($secret, $data=array()) {
    // Collect parameters
    $map = array_merge($data, $this->query());
    
    // Sort the list of parameters alphabetically
    ksort($map);
    
    $params = '';
    
    foreach ($map as $key => $value) {
      $params .= rawurlencode($key);
      $params .= '=';
      $params .= rawurlencode($value);
      $params .= '&';
    }
    
    // Signature base string
    $base = 'POST&';
    $base .= rawurlencode($this->getPath());
    $base .= '&';    
    $base .= rawurlencode(substr($params, 0, -1));
    
    $binary = hash_hmac('sha1', $base, $secret.'&', TRUE);
    
    return base64_encode($binary);    
  }
  
  protected final function url($path) {
    return 'https://api.twitter.com'.$path;
  }
}
