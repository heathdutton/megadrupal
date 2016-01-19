<?php

namespace Drupal\soauth\Common;

use Drupal\soauth\Error\SoAuthError;


/**
 * Callback class
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Callback {
  
  /**
   * Try to invoke callback function.
   * @param string $callback
   * @return mixed
   * @throws SoAuthError
   */
  static public function tryInvoke($callback) {
    // Get arguments
    $args = func_get_args();
    
    if (!is_callable($callback)) {
      throw new SoAuthError(t('Invalid callback function "@func"', array(
        '@func' => print_r($callback, TRUE),
      )));
    }
    return call_user_func_array($callback, array_slice($args, -1));
  }
  
}
