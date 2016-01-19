<?php

namespace Drupal\geoip_language_redirect;

class RedirectCountry extends RedirectBase {
  public function checkAndRedirect() {
    $country = $this->api->getCountry();
    $mapping = $this->api->getMapping();
    if (variable_get('geoip_debug', FALSE)) {
      watchdog('geoip_language_redirect', 'Detected geoip_country=!country and language=!language for IP:!ip.', array('!country' => $country, '!language' => isset($mapping[$country]) ? $mapping[$country] : 'default', '!ip' => ip_address()), WATCHDOG_DEBUG);
    }  
    if (isset($mapping[$country])) {
      $this->redirect($mapping[$country]);
    } else {
      $this->redirect($this->api->defaultLanguage());
    }
  }
}
