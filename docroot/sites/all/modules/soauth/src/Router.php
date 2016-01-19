<?php

namespace Drupal\soauth;

use Drupal\soauth\Common\Url;


/**
 * Class Router
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Router {
  
  /**
   * Build provider action URL
   * @param string $provider
   * @param string $action
   * @param array $query
   * @return Url
   */
  static public function buildActionUrl($provider, $action, $query=array()) {
    // Make path from 
    $path = implode('/', array('soauth', $provider, $action));
    
    // Build url from path
    $url = Url::fromUri($path, array(
      'query' => $query
    ));
    
    return $url->setAbsolute();
  }
  
}
