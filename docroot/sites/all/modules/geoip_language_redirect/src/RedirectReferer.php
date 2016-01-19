<?php

namespace Drupal\geoip_language_redirect;

class RedirectReferer extends RedirectBase {
  /**
   * Redirect is not allowed for referers from the same $base_url.
   */
  public function checkAndRedirect() {
    $base_url = $this->api->baseUrl();
    if (substr($this->api->referer(), 0, strlen($base_url)) == $base_url) {
      return FALSE;
    }
    return NULL;
  }
}
