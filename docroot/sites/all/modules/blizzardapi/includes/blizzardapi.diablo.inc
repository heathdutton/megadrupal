<?php

/**
 * @file
 * Provides access to Diablo 3 data provided by the Blizzard Community Platform API.
 */

/**
 * Base Blizzard API class for Diablo 3.
 */
abstract class BlizzardApiDiablo extends BlizzardApi {
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
        self::LANGUAGE_KOREAN  => 'ko_KR',
        self::LANGUAGE_ENGLISH => 'en_US'
      ),
      self::REGION_TW => array(
        self::LANGUAGE_CHINESE => 'zh_TW',
        self::LANGUAGE_ENGLISH => 'en_US'
      ),
      self::REGION_CN => array(
        self::LANGUAGE_CHINESE => 'zh_CN',
        self::LANGUAGE_ENGLISH => 'en_US'
      )
    )
  );
  
  /**
   * Constructs a BlizzardApiDiablo object.
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
    $this->setApiPath($this->apiPath . '/d3');
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
  
  /**
   * Separates a BattleTag into its name and code components.
   *
   * @param string $battletag
   *   A BattleTag identifier.
   *
   * @return array
   *   An array containing both BattleTag segments (which may be an empty string
   *   if the tag is invalid).
   * 
   * @see http://us.battle.net/support/en/article/battletag-naming-policy
   */
  public static function splitBattletag($battletag) {
    // Always return an array with 2 elements.
    return explode('#', $battletag, 2) + array('', '');
  }
}

/**
 * Access detailed artisan information.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiDiabloArtisan extends BlizzardApiDiablo {
  const ARTISAN_BLACKSMITH = 'blacksmith';
  const ARTISAN_JEWELER = 'jeweler';
  const ARTISAN_MYSTIC = 'mystic';
  
  private static $schema = array(
    'types' => array(
      self::ARTISAN_BLACKSMITH,
      self::ARTISAN_JEWELER,
      self::ARTISAN_MYSTIC
    )
  );
  
  /**
   * Local copy of the requested artisan type.
   *
   * @var string
   */
  protected $artisanType;
  
  /**
   * Constructs a BlizzardApiDiabloArtisan object.
   *
   * @param string $artisan
   *   A supported artisan type, either 'blacksmith' or 'jeweler'.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($artisan, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/data/artisan/{artisantype}')
      ->setArtisanType($artisan);
  }
  
  /**
   * Returns the artisan type.
   */
  public function getArtisanType() {
    return $this->artisanType;
  }
  
  /**
   * Set the artisan to retrieve.
   *
   * @param string $artisan
   *   A supported artisan type, either 'blacksmith' or 'jeweler'.
   *
   * @throws InvalidArgumentException
   */
  public function setArtisanType($artisan) {
    if (!in_array($artisan, self::$schema['types'])) {
      throw new InvalidArgumentException("Invalid artisan type: $artisan");
    }
    
    $this->artisanType = $artisan;
    $this->addPlaceholder('{artisantype}', $artisan);
    return $this;
  }
}

/**
 * Provides account-level career profile information.
 *
 * Default cache lifetime: 1 day.
 */
class BlizzardApiDiabloCareer extends BlizzardApiDiablo {
  /**
   * Local copy of the specified BattleTag.
   *
   * @var string
   */
  protected $battletag;
  
  /**
   * Constructs a BlizzardApiDiabloCareer object.
   *
   * @param string $battletag
   *   A BattleTag identifier.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($battletag, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    // Appending 'index' is no longer required, but a trailing slash is.
    // Previous issue: http://us.battle.net/d3/en/forum/topic/6308662062
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/profile/{name}-{code}/')
      ->setCacheLifetime(86400)  // 1 day
      ->setBattletag($battletag);
  }
  
  /**
   * Returns the BattleTag identifier.
   */
  public function getBattletag() {
    return $this->battletag;
  }
  
  /**
   * Set the BattleTag of the account to retrieve.
   *
   * @param string $battletag
   *   A BattleTag identifier.
   *
   * @throws InvalidArgumentException, UnexpectedValueException
   */
  public function setBattletag($battletag) {
    if (empty($battletag)) {
      throw new InvalidArgumentException('BattleTag cannot be empty');
    }
    
    list($name, $code) = BlizzardApiDiablo::splitBattletag($battletag);
    if (empty($name)) {
      throw new UnexpectedValueException('Missing BattleTag name');
    }
    if (empty($code)) {
      throw new UnexpectedValueException('Missing BattleTag code');
    }
    
    $this->battletag = $battletag;
    $this->addPlaceholder('{name}', $name)->addPlaceholder('{code}', $code);
    return $this;
  }
}

/**
 * Access detailed follower information
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiDiabloFollower extends BlizzardApiDiablo {
  const FOLLOWER_ENCHANTRESS = 'enchantress';
  const FOLLOWER_SCOUNDREL = 'scoundrel';
  const FOLLOWER_TEMPLAR = 'templar';
  
  private static $schema = array(
    'types' => array(
      self::FOLLOWER_ENCHANTRESS,
      self::FOLLOWER_SCOUNDREL,
      self::FOLLOWER_TEMPLAR
    )
  );
  
  /**
   * Local copy of the requested follower type.
   *
   * @var string
   */
  protected $followerType;
  
  /**
   * Constructs a BlizzardApiDiabloFollower object.
   *
   * @param string $follower
   *  A supported follower type.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($follower, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/data/follower/{followertype}')
      ->setFollowerType($follower);
  }
  
  /**
   * Returns the follower type.
   */
  public function getFollowerType() {
    return $this->followerType;
  }
  
  /**
   * Set the follower to retrieve.
   *
   * @param string $artisan
   *   A supported follower type.
   *
   * @throws InvalidArgumentException
   */
  public function setFollowerType($follower) {
    if (!in_array($follower, self::$schema['types'])) {
      throw new InvalidArgumentException("Invalid follower type: $follower");
    }
    
    $this->followerType = $follower;
    $this->addPlaceholder('{followertype}', $follower);
    return $this;
  }
}

/**
 * Access detailed hero information.
 *
 * Default cache lifetime: 1 day.
 */
class BlizzardApiDiabloHero extends BlizzardApiDiablo {
  /**
   * Local copy of the specified BattleTag.
   *
   * @var string
   */
  protected $battletag;
  
  /**
   * Local copy of specified hero identifier.
   *
   * @var int
   */
  protected $heroId;
  
  /**
   * Constructs a BlizzardApiDiabloHero object.
   *
   * @param string $battletag
   *   A BattleTag identifier.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($battletag, $hero_id, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/profile/{name}-{code}/hero/{id}')
      ->setCacheLifetime(86400)  // 1 day
      ->setBattletag($battletag)
      ->setHeroId($hero_id);
  }
  
  /**
   * Returns the BattleTag identifier.
   */
  public function getBattletag() {
    return $this->battletag;
  }
  
  /**
   * Returns the hero identifier.
   */
  public function getHeroId() {
    return $this->heroId;
  }
  
  /**
   * Set the BattleTag of the account to retrieve.
   *
   * @param string $battletag
   *   A BattleTag identifier.
   *
   * @throws InvalidArgumentException, UnexpectedValueException
   */
  public function setBattletag($battletag) {
    if (empty($battletag)) {
      throw new InvalidArgumentException('BattleTag cannot be empty');
    }
    
    list($name, $code) = BlizzardApiDiablo::splitBattletag($battletag);
    if (empty($name)) {
      throw new UnexpectedValueException('Missing BattleTag name');
    }
    if (empty($code)) {
      throw new UnexpectedValueException('Missing BattleTag code');
    }
    
    $this->battletag = $battletag;
    $this->addPlaceholder('{name}', $name)->addPlaceholder('{code}', $code);
    return $this;
  }
  
  /**
   * Set the hero identifier of a career profile.
   *
   * @param int $hero_id
   *   The hero ID.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setHeroId($hero_id) {
    if (!is_numeric($hero_id)) {
      throw new InvalidArgumentException('Hero ID must be a number');
    }
    if ($hero_id < 1) {
      throw new DomainException('Hero ID must be greater than 0');
    }
    
    $this->heroId = $hero_id;
    $this->addPlaceholder('{id}', $hero_id);
    return $this;
  }
}

/**
 * Access detailed item information.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiDiabloItem extends BlizzardApiDiablo {
  /**
   * Local copy of the item data string.
   *
   * @var string
   */
  protected $itemData;
  
  /**
   * Constructs a BlizzardApiDiabloItem object.
   *
   * @param string $item_data
   *   A string containing data identifying a particular item.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($item_data, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/data/item/{itemdata}')
      ->setItemData($item_data);
  }
  
  /**
   * Returns the item data string.
   */
  public function getItemData() {
    return $this->itemData;
  }
  
  /**
   * Set the item to retrieve.
   *
   * @param string $item_data
   *   A string containing data identifying a particular item.
   *
   * @throws InvalidArgumentException
   */
  public function setItemData($item_data) {
    if (empty($item_data)) {
      throw new InvalidArgumentException('Item data cannot be empty');
    }
    
    // Automatically remove the 'item/' prefix if it is present.
    if (!strncmp($item_data, 'item/', 5)) {
      $item_data = substr($item_data, 5);
    }
    
    $this->itemData = $item_data;
    $this->addPlaceholder('{itemdata}', $item_data);
    return $this;
  }
}

/**
 * Factory class to generate Blizzard API objects for Diablo 3.
 */
class BlizzardApiDiabloResource {
  public static function getArtisan($artisan, $options = array()) {
    return new BlizzardApiDiabloArtisan($artisan, $options);
  }
  
  public static function getCareer($battletag, $options = array()) {
    return new BlizzardApiDiabloCareer($battletag, $options);
  }
  
  public static function getFollower($follower, $options = array()) {
    return new BlizzardApiDiabloFollower($follower, $options);
  }
  
  public static function getHero($battletag, $hero_id, $options = array()) {
    return new BlizzardApiDiabloHero($battletag, $hero_id, $options);
  }
  
  public static function getItem($item_data, $options = array()) {
    return new BlizzardApiDiabloItem($item_data, $options);
  }
}
