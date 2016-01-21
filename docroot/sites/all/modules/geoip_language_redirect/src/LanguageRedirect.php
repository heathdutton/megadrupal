<?php

namespace Drupal\geoip_language_redirect;

class LanguageRedirect  {
  public static $instance = NULL;
  protected $api;
  protected $redirectPossible = NULL;
  protected $classes;
  
  public static function fromDefaults() {
    return new static(
      new Drupal(),
      array('CheckHeader', 'RedirectReferer', 'RedirectUserAgent'),
      array('RedirectHeader', 'RedirectCountry')
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
    while (is_null($this->redirectPossible) && ($class = array_shift($classes))) {
      $class = __NAMESPACE__ . '\\' . $class;
      $redirector = new $class($this->api);
      $this->redirectPossible = $redirector->checkAndRedirect();
      if (variable_get('geoip_debug', FALSE)) {
        watchdog('geoip_language_redirect', 'After !class redirectPossible=!bool', array('!class' => $class, '!bool' => ($this->redirectPossible ? "Yes" : "No")) , WATCHDOG_DEBUG);
      }
    }
    if (is_null($this->redirectPossible)) {
      $this->redirectPossible = TRUE;
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
