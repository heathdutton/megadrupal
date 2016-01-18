<?php

/**
 * @file
 * Implements OAuth 2.0 requests to Battle.net endpoints.
 */

/**
 * Represents a request to an OAuth 2.0 endpoint.
 */
class BlizzardApiRequest {
  const BATTLENET_API_BASE = 'https://{region}.api.battle.net';
  const BATTLENET_API_BASE_CHINA = 'https://api.battlenet.com.cn';

  const BATTLENET_NAME = 'api.battle.net';
  const BATTLENET_NAME_CHINA = 'api.battlenet.com.cn';

  const REGION_US = 'us';
  const REGION_EU = 'eu';
  const REGION_KR = 'kr';
  const REGION_TW = 'tw';
  const REGION_CN = 'cn';

  private static $schema = array(
    'regions' => array(
      self::REGION_US,
      self::REGION_EU,
      self::REGION_KR,
      self::REGION_TW,
      self::REGION_CN
    )
  );

  /**
   * The URL path of the request.
   *
   * @var string
   */
  protected $path;

  /**
   * The Battle.net region to request data form.
   *
   * @var string
   */
  protected $region;

  /**
   * Creates a BlizzardApiRequest object.
   *
   * @param string $path
   *   The URL path of the request.
   * @param string $region
   *   (optional) The Battle.net region to request data from.
   */
  public function __construct($path, $region = self::REGION_US) {
    $this->setPath($path);
    $this->setRegion($region);
  }

  /**
   * Gets the base URL to use for a request.
   *
   * @return string
   *   The base URL for the request, based on the current region.
   */
  protected function getBaseUrl() {
    if ($this->region == self::REGION_CN) {
      return self::BATTLENET_API_BASE_CHINA;
    }
    return str_replace('{region}', $this->region, self::BATTLENET_API_BASE);
  }

  /**
   * Gets the HTTP method of this request.
   *
   * @return string
   *  An HTTP method (only GET requests are currently supported).
   */
  public function getMethod() {
    return 'GET';
  }

  /**
   * Gets the peer name to use during certificate validation.
   */
  public function getPeerName() {
    if ($this->region == self::REGION_CN) {
      return self::BATTLENET_NAME_CHINA;
    }
    return self::BATTLENET_NAME;
  }

  /**
   * Get the full URL to use for a request.
   */
  public function getUrl() {
    return $this->getBaseUrl() . $this->path;
  }

  /**
   * Sets the URL path for this requets.
   *
   * @param string $path
   *   A URL path.
   *
   * @return BlizzardApiRequest
   *   An instance of the current object.
   */
  public function setPath($path) {
    if (empty($path)) {
      throw new InvalidArgumentException('Path cannot be empty');
    }
    $this->path = $path;
    return $this;
  }

  /**
   * Set the Battle.net region to request data from.
   *
   * @param string $region
   *   (optional) A supported Battle.net region.
   */
  public function setRegion($region = self::REGION_US) {
    if (!in_array($region, self::$schema['regions'])) {
      throw new InvalidArgumentException('Unsupported region');
    }
    $this->region = $region;
    return $this;
  }
}

/**
 * Factory class to generate requests to OAuth 2.0 endpoints.
 */
class BlizzardApiRequestFactory {
  /**
   * Creates a request for Battle.net account information.
   */
  public static function getAccount($region = BlizzardApiRequest::REGION_US) {
    return new BlizzardApiRequest('/account/user', $region);
  }
  
  /**
   * Creates a request for Starcraft 2 profile information.
   */
  public static function getStarcraftProfile($region = BlizzardApiRequest::REGION_US) {
    return new BlizzardApiRequest('/sc2/profile/user', $region);
  }

  /**
   * Creates a request for World of Warcraft profile information.
   */
  public static function getWowProfile($region = BlizzardApiRequest::REGION_US) {
    return new BlizzardApiRequest('/wow/user/characters', $region);
  }
}
