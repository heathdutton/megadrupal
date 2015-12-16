<?php

namespace Drupal\geoip_language_redirect;

class LanguageRedirect  {
  public static $instance = NULL;
  protected $api;
  protected $redirectPossible = TRUE;
  protected $classes;
  
  public static function fromDefaults() {
    return new static(
      new Drupal(),
      array('RedirectReferer', 'RedirectUserAgent'),
      array('RedirectCookie', 'RedirectCountry')
    );
  }
  
  public function __construct($api, $boot_classes, $language_init_classes) {
    $this->api = $api;
    $this->classes = array(
      'boot' => $boot_classes,
      'language_init' => $language_init_classes,
    );
  }
  
  public function hook_boot() {
    $classes = $this->classes['boot'];
    while ($this->redirectPossible && ($class = array_shift($classes))) {
      $class = __NAMESPACE__ . '\\' . $class;
      $redirector = new $class($this->api);
      $this->redirectPossible = $redirector->checkAndRedirect();
      if (variable_get('geoip_debug', FALSE)) {
      	watchdog('geoip_language_redirect', 'After !class redirectPossible=!bool', array('!class' => $class, '!bool' => $this->redirectPossible), WATCHDOG_DEBUG);
      }
    }
    if ($this->redirectPossible) {
      $this->api->disableCache();
    }
  }

  public function hook_language_init() {
    $classes = $this->classes['language_init'];
    while ($this->redirectPossible && ($class = array_shift($classes))) {
      $class = __NAMESPACE__ . '\\' . $class;
      $redirector = new $class($this->api);
      $this->redirectPossible = $redirector->checkAndRedirect();
    }
    return $this->api->serveFromCache();
  }
}
