<?php

namespace Drupal\soauth\Error;

/**
 * Class SoAuthError
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class SoAuthError extends \Exception {
  
  /**
   * Put error message into Drupal's log.
   */
  public function logMessage() {
    watchdog('SoAuth API', t('Exception with message @message take place in file @file line @line.'), array(
        '@message' => $this->message,
        '@file' => $this->file,
        '@line' => $this->line,
    ));
  }
  
}
