<?php

namespace Drupal\geoip_language_redirect;

class RedirectUserAgent extends RedirectBase {
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
        return TRUE;
      }
    }

    foreach (array('Webkit', 'Safari', 'Opera', 'Dillo', 'Lynx', 'Links', 'w3m', 'Midori', 'iCab') as $engine) {
      if (strpos($agent, $engine) !== FALSE) {
        return TRUE;
      }
    }
    return FALSE;
  }
}
