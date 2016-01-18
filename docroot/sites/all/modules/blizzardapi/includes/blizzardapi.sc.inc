<?php

/**
 * @file
 * Provides access to Starcraft 2 data provided by the Blizzard Community
 * Platform API.
 */

/**
 * Base Blizzard API class for Starcraft 2.
 */
abstract class BlizzardApiStarcraft extends BlizzardApi {
  private static $schema = array(
    'locales' => array(
      self::REGION_US => array(
        self::LANGUAGE_ENGLISH   => 'en_US',
        self::LANGUAGE_SPANISH   => 'es_MX',
        self::LANGUAGE_PORTUGUES => 'pt_BR'
      ),
      self::REGION_EU => array(
        self::LANGUAGE_ENGLISH   => 'en_GB',
        self::LANGUAGE_SPANISH   => 'es_ES',
        self::LANGUAGE_FRENCH    => 'fr_FR',
        self::LANGUAGE_RUSSIAN   => 'ru_RU',
        self::LANGUAGE_GERMAN    => 'de_DE',
        self::LANGUAGE_PORTUGUES => 'pt_PT',
        self::LANGUAGE_ITALIAN   => 'it_IT',
        self::LANGUAGE_POLISH    => 'pl_PL'
      ),
      self::REGION_KR => array(
        self::LANGUAGE_KOREAN  => 'ko_KR'
      ),
      self::REGION_TW => array(
        self::LANGUAGE_CHINESE => 'zh_TW'
      ),
      self::REGION_CN => array(
        self::LANGUAGE_CHINESE => 'zh_CN'
      ),
      self::REGION_SEA => array(
        self::LANGUAGE_ENGLISH => 'en_US'
      )
    )
  );
  
  /**
   * Constructs a BlizzardApiStarcraft object.
   *
   * @param string $region
   *   (optional) The Battle.net region to request data from.
   *   Valid regions include:
   *   - us: Americas (default)
   *   - eu: Europe
   *   - kr: Korea
   *   - tw: Taiwan
   * @param string $language
   *   (optional) The language to request data in.
   *   - If not provided, the site default language may be used.
   *   - If the language is not supported, the Battle.net region default will
   *     be used.
   */
  public function __construct($region = BlizzardApi::REGION_US, $language = BlizzardApi::LANGUAGE_DEFAULT) {
    parent::__construct($region, $language);
    $this->setApiPath($this->apiPath . '/sc2');
  }
  
  /**
   * Returns a list of valid API locales.
   */
  protected function getLocales() {
    return self::$schema['locales'];
  }
  
  /**
   * Returns an array containing all the supported regions and languages.
   */
  public static function getSupportedLocales() {
    return self::$schema['locales'];
  }
  
  /**
   * Checks if the supplied region and language are a valid Battle.net locale.
   * 
   * Valid languages by region:
   * - us: en, es, pt
   * - eu: en, es, fr, ru, de, pt, it, pl
   * - kr: en, ko
   * - tw: en, zh
   */
  public static function isSupportedLocale($region, $language) {
    if (!self::isSupportedRegion($region)) {
      return FALSE;
    }
    return array_key_exists($language, self::$schema['locales'][$region]);
  }
  
  /**
   * Checks if the supplied region is a valid Battle.net region.
   *
   * Valid regions:
   * - us: Americas
   * - eu: Europe
   * - kr: Korea
   * - tw: Taiwan
   */
  public static function isSupportedRegion($region) {
    return array_key_exists($region, self::$schema['locales']);
  }
}

/**
 * Provides data about a Starcraft 2 ladder.
 *
 * Default cache lifetime: 12 hours.
 */
class BlizzardApiStarcraftLadder extends BlizzardApiStarcraft {
  /**
   * Local copy of the ladder ID.
   *
   * @var int
   */
  protected $ladderId;
  
  /**
   * Constructs a BlizzardApiStarcraftLadder object.
   *
   * @param int $ladder_id
   *   The ID number of the ladder.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($ladder_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/ladder/{ladder_id}')
      ->setCacheLifetime(43200)  // 12 hours
      ->setLadder($ladder_id);
  }
  
  /**
   * Returns the ladder ID.
   */
  public function getLadder() {
    return $ladder_id;
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    $this->data = $this->data['ladderMembers'];
  }
  
  /**
   * Set the ladder to retrieve.
   *
   * @param int $ladder_id
   *   The ID number of the ladder.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setLadder($ladder_id) {
    if (!is_numeric($ladder_id)) {
      throw new InvalidArgumentException('Ladder ID must be a number');
    }
    if ($ladder_id <= 0) {
      throw new DomainException('Ladder ID must be greater than 0');
    }
    $this->ladderId = $ladder_id;
    $this->addPlaceholder('{ladder_id}', $ladder_id);
    return $this;
  }
}

/**
 * Provides Starcraft 2 profile information.
 *
 * Default cache lifetime: 1 day.
 */
class BlizzardApiStarcraftProfile extends BlizzardApiStarcraft {
  const RESOURCE_DEFAULT = '';
  const RESOURCE_LADDER = 'ladders';
  const RESOURCE_MATCHES = 'matches';
  
  private static $schema = array(
    'resource' => array(
      self::RESOURCE_DEFAULT,
      self::RESOURCE_LADDER,
      self::RESOURCE_MATCHES
    )
  );
  
  /**
   * Local copy of the specified profile name.
   *
   * @var string
   */
  protected $name;
  
  /**
   * Local copy of the profile ID.
   *
   * @var int
   */
  protected $profileId;
  
  /**
   * Local copy of the profile region.
   *
   * @var int
   */
  protected $profileRegion;
  
  /**
   * Local copy of the specified profile resource.
   *
   * @var string
   */
  protected $resource;
  
  /**
   * Constructs a BlizzardApiStarcraftProfile object.
   *
   * @param int $profile_id
   *   The profile ID number.
   * @param int $profile_region
   *   The profile region. Do not confuse with the API region option.
   * @param string $name
   *   The name of the profile.
   * @param string $resource
   *   (optional) One of the following profile resources:
   *   - RESOURCE_DEFAULT: Basic data about an individual profile (default).
   *   - RESOURCE_LADDER: An individual profile's ladders.
   *   - RESOURCE_MATCHES: An individual profile's match history.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($profile_id, $profile_region, $name, $resource = self::RESOURCE_DEFAULT, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/profile/{profile_id}/{profile_region}/{name}/{resource}')
      ->setCacheLifetime(86400)  // 1 day
      ->setProfile($profile_id, $profile_region, $name)
      ->setResource($resource);
  }
  
  /**
   * Returns the profile name.
   */
  public function getName() {
    return $this->name;
  }
  
  /**
   * Returns the profile ID.
   */
  public function getProfileId() {
    return $this->profileId;
  }
  
  /**
   * Returns the profile region.
   */
  public function getProfileRegion() {
    return $this->profileRegion;
  }
  
  /**
   * Returns the selected profile resource.
   */
  public function getResourceType() {
    return $this->resource;
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    if ($this->resource == self::RESOURCE_MATCHES) {
      $this->data = $this->data['matches'];
    }
  }
  
  /**
   * Set the profile to retrieve.
   *
   * @param int $profile_id
   *   The profile ID number.
   * @param int $profile_region
   *   The profile region.
   * @param string $name
   *   The name of the profile.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setProfile($profile_id, $profile_region, $name) {
    if (!is_numeric($profile_id)) {
      throw new InvalidArgumentException('Profile ID must be a number');
    }
    if ($profile_id <= 0) {
      throw new DomainException('Profile ID must be greater than 0');
    }
    if (!is_numeric($profile_region)) {
      throw new InvalidArgumentException('Profile region must be a number');
    }
    if ($profile_region <= 0) {
      throw new DomainException('Profile region must be greater than 0');
    }
    if (empty($name)) {
      throw new InvalidArgumentException('Name cannot be empty');
    }
    
    $this->profileId = $profile_id;
    $this->profileRegion = $profile_region;
    $this->name = $name;
    
    $this->addPlaceholder('{profile_id}', $profile_id);
    $this->addPlaceholder('{profile_region}', $profile_region);
    $this->addPlaceholder('{name}', $name);
    
    return $this;
  }
  
  /**
   * Set the type of profile information to retrieve.
   *
   * @param string $resource
   *   One of the following profile resources:
   *   - RESOURCE_DEFAULT: Basic data about an individual profile (default).
   *   - RESOURCE_LADDER: An individual profile's ladders.
   *   - RESOURCE_MATCHES: An individual profile's match history.
   *
   * @throws InvalidArgumentException
   */
  public function setResource($resource) {
    if (!in_array($resource, self::$schema['resource'])) {
      throw new InvalidArgumentException("Invalid resource: $resource");
    }
    $this->resource = $resource;
    $this->addPlaceholder('{resource}', $resource);
    return $this;
  }
}

/**
 * Provides information that compliments other Starcraft 2 resources.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiStarcraftData extends BlizzardApiStarcraft {
  const RESOURCE_ACHIEVEMENTS = 'achievements';
  const RESOURCE_REWARDS = 'rewards';
  
  private static $schema = array(
    'resource' => array(
      self::RESOURCE_ACHIEVEMENTS,
      self::RESOURCE_REWARDS
    )
  );
  
  /**
   * Local copy of the selected resource type.
   *
   * @var string
   */
  protected $resource;
  
  /**
   * Constructs a BlizzardApiStarcraftData object.
   *
   * @param string $resource
   *   One of the following resource types:
   *   - 'achievements'
   *   - 'rewards'
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($resource, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/data/{resource}')
      ->setResource($resource);
  }
  
  /**
   * Returns the type of data that was requested.
   */
  public function getResourceType() {
    return $this->resource;
  }
  
  /**
   * Set the type of data to retrieve.
   *
   * @param string $resource
   *   One of the following resource types:
   *   - 'achievements'
   *   - 'rewards'
   *
   * @throws InvalidArgumentException
   */
  public function setResource($resource) {
    if (!in_array($resource, self::$schema['resource'])) {
      throw new InvalidArgumentException("Invalid resource: $resource");
    }
    $this->resource = $resource;
    $this->addPlaceholder('{resource}', $resource);
    return $this;
  }
}

/**
 * Factory class to generate Blizzard API objects for Starcraft 2.
 */
class BlizzardApiStarcraftResource {
  public static function getLadder($ladder_id, $options = array()) {
    return new BlizzardApiStarcraftLadder($ladder_id, $options);
  }
  
  public static function getProfile($profile_id, $profile_region, $name, $resource = BlizzardApiStarcraftProfile::RESOURCE_DEFAULT, $options = array()) {
    return new BlizzardApiStarcraftProfile($profile_id, $profile_region, $name, $resource, $options);
  }
  
  public static function getData($resource, $options = array()) {
    return new BlizzardApiStarcraftData($resource, $options);
  }
}
