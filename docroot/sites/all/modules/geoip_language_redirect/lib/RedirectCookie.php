<?php

namespace Drupal\geoip_language_redirect;

class RedirectCookie extends RedirectBase {
  public function checkAndRedirect() {
    drupal_load('module', 'session_cache');
    $cookie = $this->api->readCookie();
    if (!$cookie)
      return TRUE;
    if ($cookie == $this->api->currentLanguage()) {
      return FALSE;
    } else {
      return $this->redirect($cookie);
    }
  }
}

