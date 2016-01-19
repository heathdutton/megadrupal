<?php
/**
 * Copyright (c) 2013, Jorge Castro <jorgegc@gmail.com>
 * All rights reserved.
 */

/**
 * Show off methods implemented through magic methods.
 *
 * @method string getState()
 * @method void setState($state)
 *
 * @method string getCity()
 * @method void setCity($city)
 */
class WCAFeed {

  const WCA_HOST = 'http://rss.weather.com.au';
  const WCA_FORECAST_DAYS_LIMIT = 3;

  /**
   * @var array $data
   */
  protected $data;

  /**
   * Constructor.
   *
   * @param array $data
   */
  public function __construct(array $data = array()) {
    $defaultData = array(
      'state' => '',
      'city' => '',
      'feed' => '',
    );
    $this->data = array_merge($defaultData, $data);
  }

  /**
   * Dynamically implement getters and setters.
   *
   * @param string $name.
   *    The name of the property.
   * @param array $arguments
   *    The arguments to be passed to the methods.
   *
   * @return mixed
   */
  public function __call($name, $arguments) {
    $type = substr($name, 0, 3);
    $key = lcfirst(substr($name, 3, strlen($name) - 3));

    // Return all data.
    if ($name == 'getAll') {
      return $this->data;
    }

    // Default return value.
    $return = FALSE;

    // Manipulate data by key.
    switch ($type) {
      // Get data.
      case 'get':
        if (array_key_exists($key, $this->data)) {
          $return = $this->data[$key];
        }
        break;

      // Set data.
      case 'set':
        $this->data[$key] = reset($arguments);
        $return = TRUE;
        break;

      // Unset data
      case 'uns':
        if (array_key_exists($key, $this->data)) {
          unset($this->data[$key]);
          $return = TRUE;
        }
        break;

      // Has data?
      case 'has':
        $return = array_key_exists($key, $this->data);
        break;
    }

    return $return;
  }

  /**
   * Get the feed data either from the cache or freshly fetched.
   *
   * @param array $cache_options
   *    An array of cache options.
   *
   * @return SimpleXMLElement object
   *    An object of class SimpleXMLElement with properties containing the data.
   */
  public function getFeed($cache_options = array()) {
    // Provide cache defaults just in case.
    $cache_defaults = array(
      'cid' => NULL,
      'expire' => weathercomau_variable_get('cache_lifetime'),
      'refresh' => FALSE,
    );
    $cache_options += $cache_defaults;

    // Return data from the persistent cache.
    $cache = cache_get($cache_options['cid']);

    if (!$cache_options['refresh'] && isset($cache) && !empty($cache->data)) {
      $this->setFeed($cache->data);
    }
    else {
      $data = @file_get_contents($this->fetchURL(), FALSE, $this->fetchContext());
      if ($data) {
        $this->setFeed($data);
        cache_set($cache_options['cid'], $data, 'cache', $cache_options['expire']);
        watchdog('weathercomau', 'Weather data for @city @state has been fetched successfully.', array(
          '@city' => $this->getCity(),
          '@state' => $this->getState(),
        ), 6);
      }
      else {
        watchdog('weathercomau', 'Could not fetch weather data for @city @state.', array(
          '@city' => $this->getCity(),
          '@state' => $this->getState(),
        ), WATCHDOG_ERROR);
      }
    }

    return $this->data['feed'];
  }

  /**
   * Set the feed data.
   *
   * @param string $feed
   */
  public function setFeed($feed) {
    $this->data['feed'] = $feed;
  }

  /**
   * Return a stream context used when fetching data.
   *
   * @return resource
   *    A stream context resource.
   */
  protected function fetchContext() {
    return stream_context_create(array(
      'http' => array(
        'header' => 'Accept: application/xml',
      ),
    ));
  }

  /**
   * Return a URL which the data is fetched from.
   *
   * @return string
   *    A full URL containing protocol.
   */
  protected function fetchURL() {
    return self::WCA_HOST . '/' . $this->getState() . '/' . $this->getCity();
  }

  /**
   * Return a SimpleXMLElement object from the items in the feed.
   *
   * @param int $index
   *    The item index.
   *
   * @return SimpleXMLElement
   *    An item from the feed.
   */
  protected function getXMLItem($index) {
    $item = NULL;

    $xml = simplexml_load_string($this->data['feed']);
    if (isset($xml->channel->item[$index])) {
      // Define the namespaces that we are interested in.
      $ns = array('w' => 'http://rss.weather.com.au/w.dtd');

      // Retrieve the data held in namespaces.
      $item = $xml->channel->item[$index]->children($ns['w']);
    }

    return $item;
  }

  /**
   * Return an array containing current conditions.
   *
   * @return array
   *    An array of current conditions.
   */
  public function getCurrentConditions() {
    $current_conditions = array();

    $element = $this->getXMLItem(0);
    if ($element) {
      foreach ($element->current->attributes() as $key => $value) {
        $current_conditions[$key] = (string) $value;
      }
    }

    return $current_conditions;
  }

  /**
   * Return an array containing forecasts.
   *
   * @param int $days
   *    Maximum number of days (defaults to 3).
   *
   * @return array
   *    An array containing forecast data.
   */
  public function getForecasts($days = self::WCA_FORECAST_DAYS_LIMIT) {
    $forecasts = array();

    $element = $this->getXMLItem(1);
    if ($element) {
      for ($i = 0; $i < $days; $i++) {
        $attributes = array();
        foreach ($element->forecast[$i]->attributes() as $key => $value) {
          $attributes[$key] = (string) $value;
        }
        $forecasts[] = $attributes;
      }
    }

    return $forecasts;
  }

  /**
   * Return a URL back to weather.com.au website.
   *
   * @return string
   */
  public function getLink() {
    $xml = simplexml_load_string($this->data['feed']);
    if (isset($xml->channel->link)) {
      return $xml->channel->link;
    }
  }

}
