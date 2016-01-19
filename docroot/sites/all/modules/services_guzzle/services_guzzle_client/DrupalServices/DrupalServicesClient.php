<?php

namespace DrupalServices;

use Guzzle\Common\Collection;
use Guzzle\Service\Client;
use Guzzle\Service\Description\ServiceDescription;
use Guzzle\Plugin\Cookie\CookiePlugin;
use Guzzle\Plugin\Cookie\CookieJar\ArrayCookieJar;

/**
 * My example web service client
 */
class DrupalServicesClient extends Client
{
  /**
   * Factory method to create a new MyServiceClient
   *
   * The following array keys and values are available options:
   * - base_url: Base URL of web service
   * - scheme:   URI scheme: http or https
   * - username: API username
   * - password: API password
   *
   * @param array|Collection $config Configuration data
   *
   * @return self
   */
  public static function factory($config = array())
  {
    $default = array(
      'base_url' => '{scheme}://localhost/services/rest',
      'scheme'   => 'http'
    );
    $required = array('base_url');
    $config_collection = Collection::fromConfig($config, $default, $required);

    $client = new self($config_collection->get('base_url'), $config);
    // Attach a service description to the client
    $description = ServiceDescription::factory($config);
    $client->setDescription($description);

    $cookiePlugin = new CookiePlugin(new ArrayCookieJar());
    $client->addSubscriber($cookiePlugin);

    return $client;
  }
}