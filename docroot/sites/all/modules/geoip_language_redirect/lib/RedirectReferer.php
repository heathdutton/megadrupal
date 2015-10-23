<?php

namespace Drupal\geoip_language_redirect;

class RedirectReferer extends RedirectBase {
  public function checkAndRedirect() {
    $base_url = $this->api->baseUrl();
    return substr($this->api->referer(), 0, strlen($base_url)) != $base_url;
  }
}
