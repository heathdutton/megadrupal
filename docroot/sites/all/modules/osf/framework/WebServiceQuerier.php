<?php

/**
 * Provides a wrapper layer over the OSF-WS-PHP-API WebServiceQuerier
 * to allow for ease of migration to using that API directly and removing
 * this class completely.
 *
 * This class does two things, it adapts the interface for
 * \StructuredDynamics\osf\framework\WebServiceQuerier to match
 * the previous interface for this class and performs some adjustments
 * on this class's previous operation to allow for designed interaction
 * points built into the WebServiceQuerier to operate, particularly
 * with respect to the WebServiceQuery class. This lets the
 * DrupalQuerierExtension class react to display errors and such in ways
 * that are germane to Drupal and to provide hooks for altering requests
 *
 */
class WebServiceQuerier {
  private $query;
  public $error;
  private $mime;

  function __construct($url, $method, $mime, $parameters, $timeout = 0) {       
    $this->query = new DrupalWebServiceQuery;

    $parsed_url = parse_url($url);
    
    $network = $parsed_url['scheme'] . '://' . $parsed_url['host'] . (isset($parsed_url['port']) ? ':'. $parsed_url['port'] : '');

    // set credentials
    $defaultEndpoint = osf_configure_get_endpoint_by_uri($network.'/ws/');
    
    $this->query->setAppID($defaultEndpoint->app_id);
    $this->query->setApiKey($defaultEndpoint->api_key);
    $this->query->setUserID(osf_configure_get_current_user_uri());
    
    // technically this may not accurately represent the network and the endpoint
    // as parts of the network will show up in the endpoint path but as they are
    // assembled to make the request this should be an acceptable error
    $this->query->setEndpoint(ltrim($parsed_url['path'], '/'));
    $this->query->setNetwork($network);
    if (strtoupper($method) == "GET") {
      $this->query->setMethodGet();
    }
    if (strtoupper($method) == "POST") {
      $this->query->setMethodPost();
    }
    
    if (is_array($parameters)) {
      $this->query->setParameters($parameters);
    }
    else {
      // Extract the parameters without impacting the parameters' encoding
      // we cannot use parse_str() here since it URL decode all the parameters.
      // we play safer just by exploding the URI in parts for creating the
      // same array, without impacting the encoding.
      $parameters = explode('&', $parameters);
      
      $params = array();
      
      foreach($parameters as $parameter)
      {
        $param = explode('=', $parameter);
        $params[$param[0]] = $param[1];
      }
      
      $this->query->setParameters($params);
    }

    // hold onto the requested mime type(s) so that
    // the desired response can be returned
    $this->mime = $mime;

    if (is_array($mime)) {
      $this->query->setSupportedMimes($mime);
    }
    else {
      $this->query->setSupportedMimes(array($mime));
    }

    $this->query->mime($mime);
    
    
    $this->query->setTimeout($timeout);

    $this->query->send(new DrupalQuerierExtension());
    $this->error = $this->query->error;
    if ($this->error) {
      return FALSE;
    }
  }

  /* Wrapper for getting status from query */
  public function getStatus() {
    return $this->query->getStatus();
  }

  /* Wrapper for getting status message from query */
  public function getStatusMessage() {
    return $this->query->getStatusMessage();
  }

  /* Wrapper for getting status message description from query */
  public function getStatusMessageDescription() {
    return $this->query->getStatusMessageDescription();
  }

  /* Adapt the interface of the query's getResultset method to maintain
   * what was previously returned by this class's getResultset call
   * If multiple mime types were specified as acceptable, return the
   * first one we can
   */
  public function getResultset() {   
    return $this->query->getResultset();
  }

  /* Wrapper for query's display error handler (which passes through to the
   * DrupalQuerierExtension displayError handler
   */
  public function displayError() {
    $this->query->displayError();
  }
}
