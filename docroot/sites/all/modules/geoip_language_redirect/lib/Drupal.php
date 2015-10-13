<?php

namespace Drupal\geoip_language_redirect;

/**
 * Wrap Drupal methods to make this module
 * unit-testable.
 */
class Drupal {
  protected $originalCache = NULL;

  public function readCookie() {
    return \session_cache_get('geoip_redirect');
  }
  /**
   * Set the language cookie.
   */
  public function setCookie($value) {
    \session_cache_set('geoip_redirect', $value);
  }
  public function currentPath() {
    return $_GET['q'];
  }
  public function currentParameters() {
    return drupal_get_query_parameters();
  }
  public function currentLanguage() {
    return $GLOBALS['language']->language;
  }
  public function defaultLanguage() {
    return \variable_get(
      'geoip_language_redirect_default_language',
      \language_default()->language
    );
  }
  public function redirect($path, $language, $options = array()) {
    $options = array('language' => $language) + $options;
    if (!isset($options['query'])) {
      $options['query'] = $this->currentParameters();
    }
    \drupal_goto($path, $options);
  }
  public function switchLinks($path) {
    $links = \language_negotiation_get_switch_links('language', $path);
    if (variable_get('site_frontpage', 'node') == $path) {
      $front_links = \language_negotiation_get_switch_links('language', '');
      foreach ($links->links as $lang => $link) {
        if (empty($link['href'])) {
          $links->links[$lang] = $front_links->links[$lang];
        }
      }
    }
    return $links ? $links->links : NULL;
  }
  /**
   * Check if the current logged-in user has access to a path.
   */
  public function checkAccess($path, $langCode) {
    // Extra handling for front-page.
    if (empty($path)) {
      if (module_exists('i18n_variable')) {
      	$path = \i18n_variable_get('site_frontpage', $langCode, $path);
      } else {
      	$path = \variable_get('site_frontpage', $path);
      }
    }
    return ($router_item = \menu_get_item($path)) && $router_item['access'];
  }

  /**
   * Disable Drupal's page-cache during hook_boot().
   */
  public function disableCache() {
    if ($this->originalCache = \variable_get('cache')) {
      $GLOBALS['conf']['skip_cache'] = TRUE;
    }
  }
  
  /**
   * Serve the page from cache and end execution.
   */
  public function serveFromCache() {
    if ($this->originalCache) {
      $cache = \drupal_page_get_cache();
      if (\is_object($cache)) {
        \header('X-Drupal-Cache: HIT');
        \drupal_serve_page_from_cache($cache);
        if (\variable_get('page_cache_invoke_hooks', TRUE)) {
          \bootstrap_invoke_all('exit');
        }
        // We are done.
        exit;
      }
    }
  }
  
  /**
   * Get current users country from GeoIP.
   */
  public function getCountry() {
    if (variable_get('geoip_debug', FALSE)) {
      if (isset($_GET['geoip_country'])) {
        return $_GET['geoip_country'];
      }
      if (isset($_GET['geoip'])) {
        return $_GET['geoip'];
      }
    }
    // use @: see https://bugs.php.net/bug.php?id=59753
    if (function_exists('geoip_country_code_by_name')) {
      return @geoip_country_code_by_name(ip_address());
    } else {
      watchdog('geoip_language_redirect', 'geoip_country_code_by_name() does not exist. Check your installation.', array(), WATCHDOG_WARNING);
    }
  }
  
  /**
   * Get mapping from ISO country-codes to language-codes.
   */
  public function getMapping() {
    return variable_get('geoip_redirect_mapping', array());
  }
  
  public function baseUrl() {
    return $GLOBALS['base_url'];
  }
  
  public function referer() {
    return $_SERVER['HTTP_REFERER'];
  }

  public function userAgent() {
    return isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : FALSE;
  }
}
