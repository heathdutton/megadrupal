<?php

/**
 * @file
 *
 * Entity class wrapping around a CollectiveAccess instance
 */

class CollectiveAccessInstance extends Entity {

  public $name;
  public $description = '';
  public $service_url = '';
  public $connector = '';
  public $connector_username = '';
  public $connector_password = '';
  private $connector_instance = NULL;

  public function __construct($values = array()) {
    parent::__construct($values, 'collectiveaccess_instance');
  }

  /**
   * Specifies the default label, which is picked up by label() by default.
   */
  protected function defaultLabel() {
    return $this->description . ' [' . $this->service_url . '] ';
  }

  /**
   * Execute a webservice API call
   * See CollectiveAccess Service API for more information
   * (http://wiki.collectiveaccess.org/index.php?title=Web_services)
   *
   * @param string $service
   *   Specifies the CollectiveAccess service you want to call. Typically one of:
   *   Cataloguing, ItemInfo, UserContent, Search, Browse
   * @param string $method
   *   Specifies the method to be called on the selected CollectiveAccess service
   * @param array $params
   *   Provide an array of parameter values to pass to the method.
   * @param boolean $cache
   *   Specify whether a cachehandler should be used to retrieve the data
   *   from a caching backend if available. If false or no cachehandler exists,
   *   The webservice is invoked directly
   * @return
   *   array
   *     An array containing the data as provided by the webservice
   */
  public function execute($service, $method, $params = NULL, $cachehandler = NULL) {
    // hand over the execution to a cache system if appropriate
    if ($cachehandler) {
      $cachedata = $cachehandler->get($service, $method, $params);
      if ($cachedata) {
        return $cachedata;
      }
      // else we just continue getting the data from the connector
    }

    if ($connector_instance = $this->get_connector()) {
      $result = $connector_instance->execute($service, $method, $params);
      if ($cachehandler) {
        $cachehandler->set($service, $method, $params, $result);
      }
      return $result;
    }
  }

  protected function get_connector($reset = FALSE) {
    if (!$reset && isset($this->connector_instance) && ($this->connector_instance instanceof CollectiveAccessConnector)) {
      return $this->connector_instance;
    }
    else {
      // Load the CollectiveAccessConnector class
      $connector_classname = $this->connector;
      if ($connector_classname) {
        $connector_class = new $connector_classname($this);
        if ($connector_class instanceof CollectiveAccessConnector) {
          // Authenticate to the webservice if authentication info is provided
          if (isset($this->connector_username) && !empty($this->connector_username)) {
            $params = array('username' => $this->connector_username, 'password'  => aes_decrypt($this->connector_password));
            $authenticated = $connector_class->execute('ItemInfo', 'auth', $params);
            if (!$authenticated) {
              watchdog('collectiveaccess', 'Authentication with the CollectiveAccess web service failed. Please check your configuration', array(), WATCHDOG_ERROR);
              return FALSE;
            }
          }
          $this->connector_instance = $connector_class;
          return $this->connector_instance;
        }
        else {
          // @TODO: error handling
        }
      }
      return FALSE;
    }
  }

  public function get_connector_type() {
    if ($connector = $this->get_connector()) {
      return $connector->get_type();
    }
    return FALSE;
  }

  /**
   * Get a list of available attributes for CollectiveAccess items.
   * Optionally filter the results by object type id
   */
  public function getAvailableAttributes($type = 'ca_objects', $type_id = NULL) {
    $attributes = &drupal_static(__FUNCTION__);

    if (!isset($attributes[$type][$type_id])) {
      $params = array('type' => $type, 'type_id' => $type_id, 'include_sub_element_codes' => 1);
      $codes = $this->execute('ItemInfo', 'getApplicableElementCodes', $params);
      $ret = array();
      if ($codes) {
        $params = array('element_codes' => $codes);
        $types = $this->execute('ItemInfo', 'getElementTypesForCodes', $params);
        foreach ($codes as $elementcode) {
          $ret[$elementcode] = $types[$elementcode];
        }
      }
      $attributes[$type][$type_id] = $ret;
    }
    return $attributes[$type][$type_id];
  }

  /**
   * Get info arrays for available cataloguing locales on the CollectiveAccess instance
   */
  public function getCataloguingLocales() {
    $params = array('pa_options' => array('available_for_cataloguing_only' => TRUE));
    $connector = $this->get_connector();
    return $connector->execute('ItemInfo', 'getLocaleList', $params);
  }

  /**
   * Get array list of available cataloguing locales on the CollectiveAccess instance
   */
  public function getCataloguingLocalesList($full = FALSE) {
    $locales = $this->getCataloguingLocales();
    $ret = array();
    if ($locales) {
      foreach ($locales as $locale) {
        $key = $locale['key'] = $locale['language'] . '_' . $locale['country'];
        if ($full) {
          $ret[$key] = $locale;
        }
        else {
          $ret[$key] = $locale['name'];
        }
      }
    }
    return $ret;
  }
}