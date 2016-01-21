<?php

namespace Drupal\geoip_language_redirect;

class RedirectUserAgent extends RedirectBase {
  /**
   * Only allow redirect for known non-bot user agents.
   */
  public function checkAndRedirect() {
    if (!($agent = $this->api->userAgent())) {
      return FALSE;
    }

    // Many bots use Mozilla too - so we have to do some extra checks.
    if (strpos($agent, 'Mozilla') !== FALSE) {
      $isBot = FALSE;
      foreach (array('bot', 'crawler', 'spider') as $search) {
        if (stripos($agent, $search) !== FALSE) {
          $isBot = TRUE;
        }
      }
      if (!$isBot) {
        return NULL;
      }
    }

    foreach (array('Webkit', 'Safari', 'Opera', 'Dillo', 'Lynx', 'Links', 'w3m', 'Midori', 'iCab') as $engine) {
      if (strpos($agent, $engine) !== FALSE) {
        return NULL;
      }
    }
    return FALSE;
  }
}
