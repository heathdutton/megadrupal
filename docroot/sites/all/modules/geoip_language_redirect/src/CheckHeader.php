<?php

namespace Drupal\geoip_language_redirect;

class CheckHeader extends RedirectBase {
  /**
   * The $header['reset'] leads to an immediate decision.
   */
  public function checkAndRedirect() {
    $header = $this->api->redirectHeader();
    if (isset($header['redirect'])) {
      return $header['redirect'] != 'no';
    }
    return NULL;
  }
}
