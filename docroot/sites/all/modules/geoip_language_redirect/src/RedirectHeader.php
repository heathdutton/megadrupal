<?php

namespace Drupal\geoip_language_redirect;

class RedirectHeader extends RedirectBase {
  public function checkAndRedirect() {
    $header = $this->api->redirectHeader();
    if (isset($header['country']) && ($country = $header['country'])) {
      $mapping = $this->api->getMapping();
      if (variable_get('geoip_debug', FALSE)) {
        watchdog('geoip_language_redirect', 'Detected geoip_country=!country and language=!language from HTTP header.', array('!country' => $country, '!language' => isset($mapping[$country]) ? $mapping[$country] : 'default'), WATCHDOG_DEBUG);
      }
      if (isset($mapping[$country])) {
        $this->redirect($mapping[$country]);
      } else {
        $this->redirect($this->api->defaultLanguage());
      }
    }
    return TRUE;
  }
}
