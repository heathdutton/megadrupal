<?php
/**
 * @file
 * Contains implementation of StingrayCacheControl
 */

/**
 * Class StingrayCacheControl
 */
class StingrayCacheControl extends StingrayServiceBase {

  /**
   * Clears the provided URLs from the Stingray Content Cache.
   *
   * @param $paths
   *    A path (or array of paths) to clear from Stingray
   *
   * @return boolean
   *   TRUE or FALSE indicating cache clearing success
   * @throws Exception
   *   Exception on connecting to server or clear cache.
   */
  function clearByPath($paths) {
    // make paths an array.
    if (!is_array($paths)) {
      $paths = array($paths);
    }

    $base_url = $this->getBasePath();
    if (empty($base_url)) {
      throw new Exception('Base path is missing');
    }
    // Allowed schema values are http, https, both.
    $url_schema = isset($base_url['scheme']) ? $base_url['scheme'] : 'both';
    $url_host = isset($base_url['host']) ? $base_url['host'] : '';

    // prepend base path to paths to make URIs
    $uris = array();
    foreach ($paths as $path) {
      $path= rtrim(preg_match("/^\//", $path) ? $path : "/$path");
      array_push($uris, $path);
    }
    try {
      $wsdl = $this->getWsdl();
      $options = $this->getRequestOptions();
      $cache = new SoapClient($wsdl, $options);
      foreach ($uris as $uri) {
        $cache->clearMatchingCacheContent($url_schema, $url_host, $uri);
        watchdog('Stingray', t("Cleared cache of %uri"),array('%uri' => $uri));
      }
      return TRUE;
    }
    catch (Exception $e) {
      watchdog('Stingray', t("Error Clearing Stingray Cache: %msg"), array('%msg' => $e->getMessage()));
      throw $e;
    }
  }

  /**
   * @return string
   */
  protected function getWsdlName() {
    return 'System.Cache.wsdl';
  }

  /**
   * @return string
   */
  protected function getWsdl() {
    return $this->getWsdlPath() . '/' . $this->getWsdlName();
  }
}