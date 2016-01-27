<?php

/**
 * @file
 * Provides access to World of Warcraft data provided by the Blizzard Community
 * Platform API.
 */

/**
 * Base Blizzard API class for World of Warcraft.
 */
abstract class BlizzardApiWow extends BlizzardApi {
  const ICON_SMALL  = 18;
  const ICON_MEDIUM = 36;
  const ICON_LARGE  = 56;
  
  /**
   * The relative path of static images used on the Battle.net site (this does
   * not include item icons).
   */
  const STATIC_IMAGE_PATH = '/wow/static/images';
  /**
   * The relative path of images rendered using game content on the Battle.net
   * site.
   */
  const STATIC_RENDER_PATH = '/static-render/{region}';
  
  private static $schema = array(
    'icons' => array(
      self::ICON_SMALL,
      self::ICON_MEDIUM,
      self::ICON_LARGE
    ),
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
        self::LANGUAGE_ITALIAN   => 'it_IT'
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
   * Constructs a BlizzardApiWow object.
   *
   * @param string $region
   *   (optional) The Battle.net region to request data from.
   *   Valid regions include:
   *   - us: Americas (default)
   *   - eu: Europe
   *   - kr: Korea
   *   - tw: Taiwan
   *   - cn: China
   * @param string $language
   *   (optional) The language to request data in.
   *   - If not provided, the site default language may be used.
   *   - If the language is not supported, the Battle.net region default will
   *     be used.
   */
  public function __construct($region = BlizzardApi::REGION_US, $language = BlizzardApi::LANGUAGE_DEFAULT) {
    parent::__construct($region, $language);
    $this->setApiPath($this->apiPath . '/wow');
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
   * - eu: en, es, fr, ru, de, pt, it
   * - kr: ko, en
   * - tw: zh, en
   * - cn: zh, en
   *
   * Note: English is not officially supported in the Korean, Taiwan, or Chinese
   * regions.
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
   * - cn: China
   */
  public static function isSupportedRegion($region) {
    return array_key_exists($region, self::$schema['locales']);
  }
  
  /**
   * Gets the path to an icon image. If there is no local copy of the image, it
   * will be downloaded from the specified region's Battle.net site.
   *
   * @param string $icon
   *   (optional) The name of the icon. If not given, the question mark icon
   *   will be retieved.
   * @param string $size
   *   (optional) The size of the icon.
   *   The following values are supported:
   *   - BlizzardApiWow::ICON_SMALL: 18 pixels
   *   - BlizzardApiWow::ICON_MEDIUM: 36 pixels
   *   - BlizzardApiWow::ICON_LARGE: 56 pixels (default)
   * @param string $region
   *   (optional) The Battle.net region to download the icon from.
   *   Valid regions include:
   *   - us: Americas (default)
   *   - eu: Europe
   *   - kr: Korea
   *   - tw: Taiwan
   *   - cn: China
   *
   * @return string
   *   The location that the icon was saved to, or FALSE if there was a problem
   *   retrieving the file.
   *
   * @throws InvalidArgumentException
   */
  public static function retrieveIcon($icon = '', $size = BlizzardApiWow::ICON_LARGE, $region = BlizzardApi::REGION_US) {
    if (empty($icon)) {
      $icon = 'inv_misc_questionmark';
    }
    
    if (!in_array($size, self::$schema['icons'])) {
      throw new InvalidArgumentException("Invalid icon size: $size");
    }
    
    $filename = $icon . '.jpg';
    $destination = file_default_scheme() . "://blizzardapi/icons/$size/" . drupal_basename($filename);
    $destination = file_stream_wrapper_uri_normalize($destination);
    
    $dirname = drupal_dirname($destination);
    if (!file_prepare_directory($dirname, FILE_CREATE_DIRECTORY)) {
      watchdog('blizzardapi', 'The directory %directory does not exist or is not writable.',
        array('%directory' => $dirname, WATCHDOG_ERROR));
      return FALSE;
    }
    
    if (file_exists($destination)) {
      return $destination;
    }
    
    $region = self::isSupportedRegion($region) ? $region : BlizzardApi::REGION_US;
    $host = ($region == BlizzardApi::REGION_CN) ? BlizzardApi::URL_MEDIA_HOST_CHINA : BlizzardApi::URL_MEDIA_HOST;
    $url = "http:$host/wow/icons/$size/$filename";
    $url = str_replace('{region}', $region, $url);
    
    return system_retrieve_file($url, $destination, FALSE, FILE_EXISTS_ERROR);
  }
  
  /**
   * Converts gold, silver, and copper into a single integer.
   *
   * @see BlizzardApiWow::unpackCurrency()
   */
  public static function packCurrency($copper, $silver, $gold) {
    return $copper + ($silver * 100) + ($gold * 10000);
  }
  
  /**
   * Converts a price into gold, silver, and copper components.
   *
   * @param int $amount
   *   The item value, such as an auction's bid or buyout price.
   * @param bool $assoc
   *   If TRUE, return the components in an associative array instead of an
   *   object. Defaults to FALSE.
   *
   * @see BlizzardApiWow::packCurrency()
   */
  public static function unpackCurrency($amount, $assoc = FALSE) {
    // There is no integer division in PHP.
    $unpacked = new stdClass();
    $unpacked->copper = (int) ($amount % 100);
    $unpacked->silver = (int) ($amount / 100 % 100);
    $unpacked->gold   = (int) ($amount / 10000);
    
    return $assoc ? (array) $unpacked : $unpacked;
  }
}

/**
 * Provides data about achievements.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiWowAchievement extends BlizzardApiWow {
  /**
   * Local copy of the achievement ID.
   *
   * @var int
   */
  protected $achievementId;
  
  /**
   * Constructs a BlizzardApiWowAchievement object.
   *
   * @param int $achievement_id
   *   The ID number of the achievement.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($achievement_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/achievement/{achievement_id}')
      ->setAchievement($achievement_id);
  }
  
  /**
   * Returns the achievement ID.
   */
  public function getAchievementId() {
    return $this->achievementId;
  }
  
  /**
   * Set the achievement to retrieve.
   *
   * @param int $achievement_id
   *   The ID number of the achievement.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setAchievement($achievement_id) {
    if (!is_numeric($achievement_id)) {
      throw new InvalidArgumentException('Achievement ID must be a number');
    }
    if ($achievement_id < 1) {
      throw new DomainException('Achievement ID must be greater than 0');
    }
    
    $this->achievementId = $achievement_id;
    $this->addPlaceholder('{achievement_id}', $achievement_id);
    return $this;
  }
}

/**
 * Provides information on the most recent batch of current auction data.
 *
 * Default cache lifetime: 1 hour.
 */
class BlizzardApiWowAuction extends BlizzardApiWow {
  /**
   * Less than 30 minutes.
   */
  const TIME_LEFT_SHORT = 'SHORT';
  /**
   * Less than 2 hours.
   */
  const TIME_LEFT_MEDIUM = 'MEDIUM';
  /**
   * Less than 12 hours.
   */
  const TIME_LEFT_LONG = 'LONG';
  /**
   * Greater than 12 hours.
   */
  const TIME_LEFT_VERY_LONG = 'VERY_LONG';
  
  /**
   * Local copy of the specified realm.
   *
   * @var string
   */
  protected $realm = '';
  
  /**
   * Constructs a BlizzardApiWowAuction object.
   *
   * @param string $realm
   *   The realm of the auction house to get data from. This should be a realm
   *   slug, but can also be the display name.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($realm, $options = array()) {
    $options += array(
      'region' => BlizzardApi::REGION_US,
      'language' => BlizzardApi::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/auction/data/{realm}')
      ->setCacheLifetime(3600)  // 1 hour
      ->setRealm($realm);
  }
  
  /**
   * Deletes all downloaded auction files.
   *
   * Warning: Do NOT use this method on managed files!
   *
   * @param string $path
   *   The base directory or URI to scan, without the trailing slash.
   *
   *   Defaults to 'public://blizzardapi/auctions/{region}-{realm}'.
   */
  public function clearAuctionFiles($path = '', $pattern = '') {
    $files = $this->getAuctionFiles($path, $pattern);
    foreach ($files as $timestamp => $info) {
      file_unmanaged_delete($info->uri);
    }
    
    return $this;
  }
  
  /**
   * Downloads the auction dump file returned by this API request to the
   * specified destination.
   *
   * @param string $destination
   *   Stream wrapper URI specifying where the file should be placed.
   * @param bool $managed
   *   If TRUE, the file API hooks will be invoked and the file will be
   *   registered in the database. Defaults to FALSE.
   * @param int $replace
   *   Replace behavior when the destination file already exists:
   *   - FILE_EXISTS_REPLACE: Replace the existing file (default).
   *   - FILE_EXISTS_RENAME: Append _{incrementing number} until the filename is
   *     unique.
   *   - FILE_EXISTS_ERROR: Do nothing and return FALSE.
   * 
   * @return mixed
   *   The location that the file was saved to or FALSE if there was an error.
   *
   * @throws BlizzardApiInvalidOperationException
   *
   * @see BlizzardApiWowAuction::prepareDefaultPath()
   */
  public function downloadAuctionFile($destination, $managed = FALSE, $replace = FILE_EXISTS_REPLACE, $options = array()) {
    if (!isset($this->data)) {
      throw new BlizzardApiInvalidOperationException('Cannot download auction dump file until realm index has been retrieved');
    }
    
    // Blizzard has stopped serving .gz files, but requesting compressed data
    // still saves bandwidth (usually going from ~3MB to ~500KB per file).
    return blizzardapi_retrieve_file($this->data['url'], $destination, $managed, $replace, $options);
  }
  
  /**
   * Finds all downloaded auction files in a given directory.
   *
   * @param string $path
   *   The base directory or URI to scan, without the trailing slash.
   *
   *   Defaults to 'public://blizzardapi/auctions/{region}-{realm}'.
   * @param string $key
   *   The key used for the returned associative array of files. Defaults to
   *   'name'.
   *
   *   Possible keys include:
   *   - filename: The file's name.
   *   - name: The file's name, without the extension.
   *   - uri: The file's URI.
   * @param string $pattern
   *   A regular expression of the files to find. Defaults to '/^[0-9]{13}\.json$/'.
   *
   * @return mixed
   *   A list of auction files, or FALSE if the default path could not be determined.
   */
  public function getAuctionFiles($path = '', $key = 'name', $pattern = '') {
    if (empty($path)) {
      $options = array('region' => $this->region, 'language' => $this->language);
      $slug = blizzardapi_realm_slug($this->realm, $options, TRUE);
      if (empty($slug)) {
        return FALSE;
      }
      
      $path = file_default_scheme() . '://blizzardapi/auctions/' . $slug;
    }
    if (empty($pattern)) {
      $pattern = '/^[0-9]{13}\.json$/';
    }
    // This method is also called when deleting files, so be picky about filenames.
    return file_scan_directory($path, $pattern, array('key' => $key));
  }
  
  /**
   * Returns the name of the realm that auction data was requested for.
   */
  public function getRealmName() {
    return $this->realm;
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    // Let's see if Blizzard ever includes more than 1 file...
    if (count($this->data['files']) > 1) {
      watchdog('blizzardapi', 'Multiple auction files found in response from %url.',
        array('%url' => $this->url), WATCHDOG_WARNING);
    }
    $this->data = $this->data['files'][0];
  }
  
  /**
   * Generates an auction dump filename and prepares the default directory to
   * save it in.
   *
   * Example: public://blizzardapi/auctions/{region}-{realm}/{timestamp}.json
   *
   * @return mixed
   *   A destination filename or FALSE if there was a problem setting up the
   *   directory.
   *
   * @see BlizzardApiWowAuction::downloadAuctionFile()
   */
  public function prepareDefaultPath() {
    if (!isset($this->data)) {
      throw new BlizzardApiInvalidOperationException('Cannot prepare auction dump path until realm index has been retrieved');
    }
    
    $options = array('region' => $this->region, 'language' => $this->language);
    $slug = blizzardapi_realm_slug($this->realm, $options, TRUE);
    if (empty($slug)) {
      return FALSE;
    }
    
    $path = $slug . '/' . $this->data['lastModified'];
    $destination = file_default_scheme() . '://blizzardapi/auctions/' . $path . '.json';
    $destination = file_stream_wrapper_uri_normalize($destination);
    
    $dirname = drupal_dirname($destination);
    if (!file_prepare_directory($dirname, FILE_CREATE_DIRECTORY)) {
      watchdog('blizzardapi', 'The directory %directory does not exist or is not writable.',
        array('%directory' => $dirname, WATCHDOG_ERROR));
      return FALSE;
    }
    
    return $destination;
  }
  
  /**
   * Set the realm to request auction house data from.
   *
   * @param string $realm
   *   The name of the realm. This should preferably be the realm slug.
   *
   * @throws InvalidArgumentException
   */
  public function setRealm($realm) {
    if (empty($realm)) {
      throw new InvalidArgumentException('Realm cannot be empty');
    }
    
    $this->realm = $realm;
    $this->addPlaceholder('{realm}', $realm);
    return $this;
  }
}

/**
 * Provides data about a battle pet ability, pet species, or pet stats.
 *
 * Default cache lifetime: 30 days.
 *
 * @see BlizzardApiWowData
 */
class BlizzardApiWowBattlePet extends BlizzardApiWow {
  const RESOURCE_ABILITY = 'ability';
  const RESOURCE_SPECIES = 'species';
  const RESOURCE_STATS = 'stats';
  
  private static $schema = array(
    'resource' => array(
      self::RESOURCE_ABILITY,
      self::RESOURCE_SPECIES,
      self::RESOURCE_STATS
    )
  );
  
  /**
   * A local copy of the selected resource type.
   *
   * @var string
   */
  protected $resource;
  
  /**
   * A local copy of the selected resource ID number.
   *
   * @var int
   */
  protected $resourceId;
  
  /**
   * Creates a BlizzardApiWowBattlePet object.
   *
   * @param string $resource
   *   One of the following battle pet resources:
   *   - 'abilities'
   *   - 'species'
   *   - 'stats'
   * @param int $resource_id
   *   This parameter varies by resource type. It is the ability ID if the
   *   selected resource is 'abilities' and the species ID if the selected
   *   resource is 'species' or 'stats'.
   * @param array $query
   *   (optional) If the selected resource is 'stats' then the following
   *   additional parameters may be supplied:
   *   - level: Pet level. Defaults to 1.
   *   - breedId: Pet breed. Defaults to 3.
   *   - qualityId: Pet quality. Defaults to 1.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($resource, $resource_id, $query = array(), $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/battlePet/{resource}/{resource_id}')
      ->setResource($resource, $resource_id);
    
    if ($this->resource == self::RESOURCE_STATS) {
      $this->setQuery($query);
    }
  }
  
  /**
   * The pet's breed. May only be used when requesting the 'stats' resource.
   *
   * @throws BlizzardApiInvalidOperationException
   *
   * @see BlizzardApiWowBattlePet::setQuery()
   */
  public function getBreedId() {
    if ($this->resource != self::RESOURCE_STATS) {
      throw new BlizzardApiInvalidOperationException("Resource type must be 'stats'");
    }
    return $this->fields['breedId'];
  }
  
  /**
   * The pet's level. May only be used when requesting the 'stats' resource.
   *
   * @throws BlizzardApiInvalidOperationException
   *
   * @see BlizzardApiWowBattlePet::setQuery()
   */
  public function getLevel() {
    if ($this->resource != self::RESOURCE_STATS) {
      throw new BlizzardApiInvalidOperationException("Resource type must be 'stats'");
    }
    return $this->fields['level'];
  }
  
  /**
   * The pet's quality. May only be used when requesting the 'stats' resource.
   *
   * @throws BlizzardApiInvalidOperationException
   *
   * @see BlizzardApiWowBattlePet::setQuery()
   */
  public function getQualityId() {
    if ($this->resource != self::RESOURCE_STATS) {
      throw new BlizzardApiInvalidOperationException("Resource type must be 'stats'");
    }
    return $this->fields['qualityId'];
  }
  
  /**
   * Returns the type of data to request.
   */
  public function getResourceType() {
    return $this->resource;
  }
  
  /**
   * Returns the ID of the requested resource.
   */
  public function getResourceId() {
    return $this->resourceId;
  }
  
  /**
   * Set additional parameters when retrieving battle pet stats.
   *
   * This method has no effect when requesting battle pet ability or species
   * information.
   *
   * @param array $query
   *   (optional) An array containing any of the following keys:
   *   - level: Pet level. Defaults to 1.
   *   - breedId: Pet breed. Defaults to 3.
   *   - qualityId: Pet quality. Defaults to 1.
   */
  public function setQuery($query = array()) {
    $query += array(
      'level' => 1,
      'breedId' => 3,
      'qualityId' => 1
    );
    
    $this->fields = array();
    if ($this->resource == self::RESOURCE_STATS) {
      $this->fields['level'] = $query['level'];
      $this->fields['breedId'] = $query['breedId'];
      $this->fields['qualityId'] = $query['qualityId'];
    }
    
    return $this;
  }
  
  /**
   * Set the type of battle pet information to retieve.
   *
   * @param string $resource
   *   One of the following battle pet resources:
   *   - 'abilities'
   *   - 'species'
   *   - 'stats'
   * @param int $resource_id
   *   This parameter varies by resource type. It is the ability ID if the
   *   selected resource is 'abilities' and the species ID if the selected
   *   resource is 'species' or 'stats'.
   *
   * @throws InvalidArgumentException
   */
  public function setResource($resource, $resource_id) {
    if (!in_array($resource, self::$schema['resource'])) {
      throw new InvalidArgumentException("Invalid resource: $resource");
    }
    if (!is_numeric($resource_id)) {
      throw new InvalidArgumentException('Resource ID must be a number');
    }
    
    $this->resource = $resource;
    $this->addPlaceholder('{resource}', $resource);
    $this->resourceId = $resource_id;
    $this->addPlaceholder('{resource_id}', $resource_id);
    
    // Reset any fields.
    return $this->setQuery();
  }
}

/**
 * Provides leaderboard information for the challenge mode maps on a given realm.
 *
 * Default cache lifetime: 1 day.
 */
class BlizzardApiWowChallenge extends BlizzardApiWow {
  /**
   * Local copy of the specified realm.
   *
   * @var string
   */
  protected $realm = '';
  
  /**
   * Constructs a BlizzardApiWowChallenge object.
   *
   * @param string $realm
   *   The name of the realm. This should preferably be the realm slug. To
   *   retrieve the regional leaderboard, you may specify 'region' instead.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($realm, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/challenge/{realm}')
      ->setCacheLifetime(86400)  // 1 day
      ->setRealm($realm);
  }
  
  /**
   * Returns the name of the challenge mode leaderboard realm.
   */
  public function getRealmName() {
    return $this->realm;
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    $this->data = $this->data['challenge'];
  }
  
  /**
   * Set the realm of the challenge mode leaderboard to request.
   *
   * @param string $realm
   *   The name of the realm. This should preferably be the realm slug.
   *
   * @throws InvalidArgumentException
   */
  public function setRealm($realm) {
    if (empty($realm)) {
      throw new InvalidArgumentException('Realm cannot be empty');
    }
    
    $this->realm = $realm;
    $this->addPlaceholder('{realm}', $realm);
    return $this;
  }
}

/**
 * Provides character profile information.
 *
 * Default cache lifetime: 1 day.
 *
 * @see BlizzardApiWowData
 */
class BlizzardApiWowCharacter extends BlizzardApiWow {
  const FEED_TYPE_ACHIEVEMENT = 'ACHIEVEMENT';
  const FEED_TYPE_BOSSKILL = 'BOSSKILL';
  const FEED_TYPE_CRITERIA = 'CRITERIA';
  const FEED_TYPE_LOOT = 'LOOT';
  
  const GENDER_MALE = 0;
  const GENDER_FEMALE = 1;
  
  const REPUTATION_HATED = 0;
  const REPUTATION_HOSTILE = 1;
  const REPUTATION_UNFRIENDLY = 2;
  const REPUTATION_NEUTRAL = 3;
  const REPUTATION_FRIENDLY = 4;
  const REPUTATION_HONORED = 5;
  const REPUTATION_REVERED = 6;
  const REPUTATION_EXALTED = 7;
  
  private static $schema = array(
    'fields' => array(
      'achievements',
      'appearance',
      'audit',
      'feed',
      'guild',
      'hunterPets',
      'items',
      'mounts',
      'pets',
      'petSlots',
      'professions',
      'progression',
      'pvp',
      'quests',
      'reputation',
      'stats',
      'statistics',
      'talents',
      'titles',
    )
  );
  
  /**
   * Local copy of the specified character.
   *
   * @var string
   */
  protected $character = '';
  
  /**
   * Local copy of the specified realm.
   *
   * @var string
   */
  protected $realm = '';
  
  /**
   * Constructs a BlizzardApiWowCharacter object.
   *
   * @param string $character
   *   The name of the character.
   * @param string $realm
   *   The realm that the character is on. This should be a realm slug, but can
   *   also be the realm's display name.
   * @param array $fields
   *   (optional) A list of additional fields to request.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   *
   * @see BlizzardApiWowCharacter::getSupportedFields()
   */
  public function __construct($character, $realm, $fields = array(), $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/character/{realm}/{character}')
      ->setCacheLifetime(86400)  // 1 day
      ->setRealm($realm)
      ->setCharacter($character)
      ->setFields($fields);
  }
  
  /**
   * Downloads a profile image of the character.
   *
   * @param bool $refresh
   *   (optional) If TRUE, download the image even if a local copy already
   *   exists. The previous file will be overwritten.
   *
   * @return mixed
   *   The location of the saved image or FALSE if there was an error saving it.
   */
  public function downloadProfileImage($refresh = FALSE) {
    $url = str_replace('-avatar.jpg', '-profilemain.jpg', $this->getThumbnailUrl());
    
    $profile = $this->retrieveImage($url, $refresh);
    if ($profile === FALSE) {
      watchdog('blizzardapi', 'Unable to save image from %remote.', array('%remote' => $url), WATCHDOG_ERROR);
    }
    
    return $profile;
  }
  
  /**
   * Downloads a thumbnail (avatar) image of the character.
   *
   * @param bool $refresh
   *   (optional) If TRUE, download the image even if a local copy already
   *   exists. The previous file will be overwritten.
   *
   * @return mixed
   *   The location of the saved image or FALSE if there was an error saving it.
   */
  public function downloadThumbnailImage($refresh = FALSE) {
    $url = $this->getThumbnailUrl();
    
    $avatar = $this->retrieveImage($url, $refresh);
    if ($avatar === FALSE) {
      watchdog('blizzardapi', 'Unable to save image from %remote.', array('%remote' => $url), WATCHDOG_ERROR);
    }
    
    return $avatar;
  }
  
  /**
   * Returns the name of the character.
   */
  public function getCharacterName() {
    return $this->character;
  }
  
  /**
   * Returns the name of the realm that the character is on.
   */
  public function getRealmName() {
    return $this->realm;
  }
  
  /**
   * Returns a list of supported character fields.
   */
  public static function getSupportedFields() {
    return self::$schema['fields'];
  }
  
  /**
   * Returns the full thumbnail URL.
   *
   * @throws BlizzardApiInvalidOperationException
   */
  public function getThumbnailUrl() {
    if (!isset($this->data['thumbnail'])) {
      throw new BlizzardApiInvalidOperationException('Cannot get URL until character data has been retrieved');
    }
    
    $host = ($this->region == self::REGION_CN) ? self::URL_HOST_CHINA : self::URL_HOST;
    $url = 'http:' . $host . self::STATIC_RENDER_PATH . '/' . $this->data['thumbnail'];
    return str_replace('{region}', $this->region, $url);
  }
  
  /**
   * Retrieves an image using drupal_http_request() and saves it to a local
   * directory.
   *
   * @param string $source
   *   The URL of the file to get.
   * @param bool $refresh
   *   (optional) If TRUE, download the image even if a local copy already
   *   exists. The previous file will be overwritten.
   *
   * @return mixed
   *   The location of the saved image or FALSE if there was an error saving it.
   */
  protected function retrieveImage($source, $refresh = FALSE) {
    if (!isset($this->data['thumbnail'])) {
      throw new BlizzardApiInvalidOperationException('Cannot determine destination path until character data has been retrieved');
    }
    
    // Notes for these variables:
    // - Realm will always contain an english slug. PHP shouldn't crash because
    //   of UTF-8 or special characters.
    // - Unknown is an integer that may or may not be required to save unique
    //   character images. Therefore it is included in the URI just in case.
    // - Filename is unused.
    list($realm, $unknown, $filename) = explode('/', $this->data['thumbnail'], 3);
    $destination = file_default_scheme() . "://blizzardapi/characters/$this->region-$realm/$unknown/" . drupal_basename($source);
    $destination = file_stream_wrapper_uri_normalize($destination);
    
    $dirname = drupal_dirname($destination);
    if (!file_prepare_directory($dirname, FILE_CREATE_DIRECTORY)) {
      watchdog('blizzardapi', 'The directory %directory does not exist or is not writable.',
        array('%directory' => $dirname, WATCHDOG_ERROR));
      return FALSE;
    }
    
    if (file_exists($destination) && !$refresh) {
      return $destination;
    }
    
    return system_retrieve_file($source, $destination, FALSE, FILE_EXISTS_REPLACE);
  }
  
  /**
   * Set the character to retrieve.
   *
   * @param string $character
   *   The name of the character.
   *
   * @throws InvalidArgumentException
   */
  public function setCharacter($character) {
    if (empty($character)) {
      throw new InvalidArgumentException('Character cannot be empty');
    }
    
    $this->character = $character;
    $this->addPlaceholder('{character}', $character);
    return $this;
  }
  
  /**
   * Set the additional character fields to retrieve.
   *
   * @param array $fields
   *   A list of datasets to get. By default, the character's realm, name, level,
   *   last modified timestamp, thumbnail, race ID, achievement points, gender
   *   ID, and class ID are returned. Any unsupported fields will not be included
   *   in the request.
   *
   * @see BlizzardApiWowCharacter::getSupportedFields()
   */
  public function setFields($fields) {
    $this->fields['fields'] = array_intersect($fields, self::$schema['fields']);
    return $this;
  }
  
  /**
   * Set the character's realm.
   *
   * @param string $realm
   *   The name of the realm. This should preferably be the realm slug.
   *
   * @throws InvalidArgumentException
   */
  public function setRealm($realm) {
    if (empty($realm)) {
      throw new InvalidArgumentException('Realm cannot be empty');
    }
    
    $this->realm = $realm;
    $this->addPlaceholder('{realm}', $realm);
    return $this;
  }
}

/**
 * Provides guild profile information.
 *
 * Default cache lifetime: 12 hours.
 * http://us.battle.net/en/forum/topic/14424152435#4
 *
 * @see BlizzardApiWowData
 */
class BlizzardApiWowGuild extends BlizzardApiWow {
  const NEWS_GUILD_ACHIEVEMENT = 'guildAchievement';
  const NEWS_GUILD_CREATED = 'guildCreated';
  const NEWS_GUILD_LEVEL = 'guildLevel';
  const NEWS_ITEM_LOOT = 'itemLoot';
  const NEWS_ITEM_PURCHASE = 'itemPurchase';
  const NEWS_PLAYER_ACHIEVEMENT = 'playerAchievement';
  
  private static $schema = array(
    'fields' => array(
      'achievements',
      'challenge',
      'members',
      'news'
    )
  );
  
  /**
   * Local copy of the specified guild.
   *
   * @var string
   */
  protected $guild = '';
  
  /**
   * Local copy of the specified realm.
   *
   * @var string
   */
  protected $realm = '';
  
  /**
   * Constructs a BlizzardApiWowGuild object.
   *
   * @param string $guild
   *   The name of the guild.
   * @param string $realm
   *   The realm the guild is on. This should be a realm slug, but can also be
   *   the realm's display name.
   * @param array $fields
   *   (optional) A list of additional fields to request.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   *
   * @see BlizzardApiWowGuild::getSupportedFields()
   */
  public function __construct($guild, $realm, $fields = array(), $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/guild/{realm}/{guild}')
      ->setCacheLifetime(43200)  // 12 hours
      ->setRealm($realm)
      ->setGuild($guild)
      ->setFields($fields);
  }
  
  /**
   * Returns the name of the guild.
   */
  public function getGuildName() {
    return $this->guild;
  }
  
  /**
   * Returns the name of the realm that the guild is on.
   */
  public function getRealmName() {
    return $this->realm;
  }
  
  /**
   * Returns a list of supported guild fields.
   */
  public static function getSupportedFields() {
    return self::$schema['fields'];
  }
  
  /**
   * Set the additional guild fields to retrieve.
   *
   * @param array $fields
   *   A list of datasets to get. By default, the guild's name, level,
   *   faction and achievement points are automatically returned. Any unsupported
   *   fields will not be included in the request.
   *
   * @see BlizzardApiWowGuild::getSupportedFields()
   */
  public function setFields($fields) {
    $this->fields['fields'] = array_intersect($fields, self::$schema['fields']);
    return $this;
  }
  
  /**
   * Set the guild's name.
   *
   * @param string $guild
   *   The name of the guild.
   *
   * @throws InvalidArgumentException
   */
  public function setGuild($guild) {
    if (empty($guild)) {
      throw new InvalidArgumentException('Guild cannot be empty');
    }
    
    $this->guild = $guild;
    $this->addPlaceholder('{guild}', $guild);
    return $this;
  }
  
  /**
   * Set the guild's realm.
   *
   * @param string $realm
   *   The name of the realm. This should preferably be the realm slug.
   *
   * @throws InvalidArgumentException
   */
  public function setRealm($realm) {
    if (empty($realm)) {
      throw new InvalidArgumentException('Realm cannot be empty');
    }
    
    $this->realm = $realm;
    $this->addPlaceholder('{realm}', $realm);
    return $this;
  }
}

/**
 * Provides item information.
 *
 * Default cache lifetime: 30 days.
 *
 * @see BlizzardApiWowItemSet, BlizzardApiWowData
 */
class BlizzardApiWowItem extends BlizzardApiWow {
  /**
   * Also includes items that bind to a Battle.net account.
   */
  const BINDS_ON_PICKUP = 1;
  const BINDS_ON_EQUIP  = 2;
  const BINDS_ON_USE    = 3;
  
  const CLASS_CONSUMABLE = 0;
  const CLASS_CONTAINER  = 1;
  const CLASS_WEAPON     = 2;
  const CLASS_GEM        = 3;
  const CLASS_ARMOR      = 4;
  const CLASS_REAGENT    = 5;  // Unused.
  const CLASS_PROJECTILE = 6;
  const CLASS_TRADE_GOOD = 7;
  const CLASS_RECIPE     = 9;
  const CLASS_CURRENCY   = 10;  // Unused.
  const CLASS_QUIVER     = 11;
  const CLASS_QUEST      = 12;
  const CLASS_KEY        = 13;
  const CLASS_MISC       = 15;
  const CLASS_GLYPH      = 16;
  const CLASS_BATTLE_PET = 17;
  
  const INVENTORY_TYPE_NONE     = 0;
  const INVENTORY_TYPE_HEAD     = 1;
  const INVENTORY_TYPE_NECK     = 2;
  const INVENTORY_TYPE_SHOULDER = 3;
  const INVENTORY_TYPE_SHIRT    = 4;
  const INVENTORY_TYPE_CHEST    = 5;
  const INVENTORY_TYPE_WAIST    = 6;
  const INVENTORY_TYPE_LEGS     = 7;
  const INVENTORY_TYPE_FEET     = 8;
  const INVENTORY_TYPE_WRIST    = 9;
  const INVENTORY_TYPE_HAND     = 10;
  const INVENTORY_TYPE_RING     = 11;
  const INVENTORY_TYPE_TRINKET  = 12;
  const INVENTORY_TYPE_ONEHAND  = 13;
  const INVENTORY_TYPE_SHIELD   = 14;
  const INVENTORY_TYPE_BOW      = 15;
  const INVENTORY_TYPE_CLOAK    = 16;
  /**
   * Includes fishing poles.
   */
  const INVENTORY_TYPE_TWOHAND  = 17;
  const INVENTORY_TYPE_BAG      = 18;
  const INVENTORY_TYPE_TABARD   = 19;
  const INVENTORY_TYPE_ROBE     = 20;  // Examples: 44692, 34085, 34685.
  const INVENTORY_TYPE_MAINHAND = 21;
  const INVENTORY_TYPE_OFFHAND  = 22;
  const INVENTORY_TYPE_HELD_IN_OFFHAND = 23;
  const INVENTORY_TYPE_AMMO     = 24;  // Unused.
  const INVENTORY_TYPE_THROWN   = 25;
  /**
   * Includes guns, crossbows, and wands.
   */
  const INVENTORY_TYPE_RANGED   = 26;
//const INVENTORY_TYPE_UNKNOWN  = 27;
  const INVENTORY_TYPE_RELIC    = 28;
  
  const QUALITY_POOR      = 0;
  const QUALITY_COMMON    = 1;
  const QUALITY_UNCOMMON  = 2;
  const QUALITY_SUPERIOR  = 3;
  const QUALITY_EPIC      = 4;
  const QUALITY_LEGENDARY = 5;
  const QUALITY_ARTIFACT  = 6;  // Unused.
  const QUALITY_HEIRLOOM  = 7;
  
  // @todo There may be more unknown item source types. Testing IDs follow each constant.
  const SOURCE_ACHIEVEMENT_REWARD  = 'ACHIEVEMENT_REWARD';  // 71033 Lil' Tarecgosa
  const SOURCE_CREATED_BY_SPELL    = 'CREATED_BY_SPELL';    // 69945 Don Tayo's Inferno Mittens
  const SOURCE_CREATURE_DROP       = 'CREATURE_DROP';       // 56431 Right Eye of Rajh
  const SOURCE_CREATURE_PICKPOCKET = 'CREATURE_PICKPOCKET'; // 38268 Spare Hand
  const SOURCE_FACTION_REWARD      = 'FACTION_REWARD';      // 62366 Arcanum of the Earthen Ring
  const SOURCE_GAME_OBJECT_DROP    = 'GAME_OBJECT_DROP';    // 56181 Alliance Survival Kit
  const SOURCE_NONE                = 'NONE';                // 65499 Foam Sword
  const SOURCE_PROMOTION           = 'PROMOTION';           // 49284 Reins of the Swift Spectral Tiger
  const SOURCE_REWARD_FOR_QUEST    = 'REWARD_FOR_QUEST';    // 49623 Shadowmorne
  const SOURCE_VENDOR              = 'VENDOR';              // 78694 Deep Earth Headpiece
  
  const STAT_HEALTH = 1;
  const STAT_AGILITY = 3;
  const STAT_STRENGTH = 4;
  const STAT_INTELLECT = 5;
  const STAT_SPIRIT = 6;
  const STAT_STAMINA = 7;
  const STAT_DEFENSE = 12;
  const STAT_DODGE = 13;
  const STAT_PARRY = 14;
  const STAT_BLOCK = 15;
  const STAT_HIT_MELEE = 16;
  const STAT_HIT_RANGED = 17;
  const STAT_HIT_SPELL = 18;
  const STAT_CRIT_MELEE = 19;
  const STAT_CRIT_RANGED = 20;
  const STAT_CRIT_SPELL = 21;
  const STAT_HIT_MELEE_TAKEN = 22;
  const STAT_HIT_RANGED_TAKEN = 23;
  const STAT_HIT_SPELL_TAKEN = 24;
  const STAT_CRIT_MELEE_TAKEN = 25;
  const STAT_CRIT_RANGED_TAKEN = 26;
  const STAT_CRIT_SPELL_TAKEN = 27;
  const STAT_HASTE_MELEE = 28;
  const STAT_HASTE_RANGED = 29;
  const STAT_HASTE_SPELL = 30;
  const STAT_HIT = 31;
  const STAT_CRIT = 32;
  const STAT_HIT_TAKEN = 33;
  const STAT_CRIT_TAKEN = 34;
  const STAT_PVP_RESILLIENCE = 35;
  const STAT_HASTE = 36;
  const STAT_EXPERTISE = 37;
  const STAT_ATTACK_POWER = 38;
  const STAT_RANGED_ATTACK_POWER = 39;
  const STAT_FERAL_ATTACK_POWER = 40;
  const STAT_SPELL_HEALING = 41;
  const STAT_SPELL_DAMAGE = 42;
  const STAT_MANA_REGENERATION = 43;
  const STAT_ARMOR_PENETRATION = 44;
  const STAT_SPELL_POWER = 45;
  const STAT_HEALTH_REGENERATION = 46;
  const STAT_SPELL_PENETRATION = 47;
  const STAT_BLOCK_VALUE = 48;
  const STAT_MASTERY = 49;
  const STAT_BONUS_ARMOR = 50;
  const STAT_FIRE_RESISTANCE = 51;
  const STAT_FROST_RESISTANCE = 52;
  const STAT_SHADOW_RESISTANCE = 54;
  const STAT_NATURE_RESISTANCE = 55;
  const STAT_ARCANE_RESISTANCE = 56;
  const STAT_PVP_POWER = 57;
  const STAT_AMPLIFY = 58;         // 107556 Rockshock Smasher
  const STAT_MULTISTRIKE = 59;
  const STAT_READINESS = 60;       // Removed during beta.
  const STAT_SPEED = 61;
  const STAT_LEECH = 62;
  const STAT_AVOIDANCE  = 63;
  const STAT_INDESTRUCTABLE = 64;
  const STAT_CLEAVE = 66;          // 107506 Charbelching Crescent
  const STAT_STR_AGI_INT = 71;     // 113082 Precious Bloodthorn Loop
  const STAT_STR_AGI = 72;         // ?
  const STAT_AGI_INT = 73;
  const STAT_STR_INT = 74;
  
  /**
   * Store a copy of the base path during initialization.
   *
   * @var string
   */
  private $baseApiPath;
  
  /**
   * Local copy of the specified item ID.
   *
   * @var int
   */
  protected $itemId;
  
  /**
   * Local copy of the specified item bonus list.
   *
   * @var array
   */
  protected $itemBonusList;
  
  /**
   * Local copy of the specified item context.
   *
   * @var string
   */
  protected $itemContext;
  
  /**
   * Constructs a BlizzardApiWowItem object.
   * 
   * @param int $item_id
   *   The ID of the item to get.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   *
   * @see BlizzardApiWowItem::setItemContext()
   */
  public function __construct($item_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->baseApiPath = $this->apiPath;
    $this->setItem($item_id);
  }
  
  /**
   * Returns a list of item bonuses.
   */
  public function getItemBonusList() {
    return $this->itemBonusList;
  }
  
  /**
   * Returns the item context.
   */
  public function getItemContext() {
    return $this->itemContext;
  }
  
  /**
   * Returns the item ID number.
   */
  public function getItemId() {
    return $this->itemId;
  }
  
  /**
   * Set the item to retrieve.
   *
   * @param int $item_id
   *   The ID of the item to get.
   * @param string $context
   *   (optional) The context of the item. If an item requires a context, but
   *   none is provided, a list of supported contexts is returned.
   * @param array $bonuses
   *   (optional) A list of bonuses for use with items that require a context.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setItem($item_id, $context = '', $bonuses = array()) {
    if (!is_numeric($item_id)) {
      throw new InvalidArgumentException('Item ID must be a number');
    }
    if ($item_id < 1) {
      throw new DomainException('Item ID must be greater than 0');
    }
    
    $this->itemId = $item_id;
    $this->addPlaceholder('{item_id}', $item_id);
    $this->setItemContext($context, $bonuses);
    return $this;
  }
  
  /**
   * Set the creation context for an item.
   *
   * @param string $context
   *   (optional) The context of the item. If an item requires a context, but
   *   none is provided, a list of supported contexts is returned.
   * @param array $bonuses
   *   (optional) A list of bonuses for use with items that require a context.
   */
  public function setItemContext($context = '', $bonuses = array()) {
    $this->itemContext = $context;
    $this->itemBonusList = empty($context) ? array() : array_values($bonuses);
    
    unset($this->fields['bl']);
    
    if (empty($context)) {
      $this->setApiPath($this->baseApiPath . '/item/{item_id}');
      $this->removePlaceholder('{item_context}');
    }
    else {
      $this->setApiPath($this->baseApiPath . '/item/{item_id}/{item_context}');
      $this->addPlaceholder('{item_context}', $this->itemContext);
      if (!empty($this->itemBonusList)) {
        $this->fields['bl'] = $this->itemBonusList;
      }
    }
    
    return $this;
  }
}

/**
 * Provides information on an item set.
 *
 * Default cache lifetime: 30 days.
 *
 * @see BlizzardApiWowItem
 */
class BlizzardApiWowItemSet extends BlizzardApiWow {
  /**
   * Local copy of the item set ID.
   */
  protected $setId;
  
  /**
   * Constructs a BlizzardApiWowItemSet object.
   *
   * @param int $set_id
   *   The ID number of the item set.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($set_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/item/set/{set_id}')
      ->setItemSet($set_id);
  }
  
  /**
   * Returns the item set ID.
   */
  public function getItemSetId() {
    return $this->setId;
  }
  
  /**
   * Set the item set to retrieve.
   *
   * @param int $setId
   *   The ID number of the item set.
   * 
   * @throws InvalidArgumentException
   */
  public function setItemSet($set_id) {
    if (!is_numeric($set_id)) {
      throw new InvalidArgumentException('Set ID must be a number');
    }
    
    $this->setId = $set_id;
    $this->addPlaceholder('{set_id}', $set_id);
    return $this;
  }
}

/**
 * Provides leaderboard information for the 2v2, 3v3, 5v5, and rated
 * battleground leaderboards.
 *
 * Default cache lifetime: 1 day.
 */
class BlizzardApiWowPvp extends BlizzardApiWow {
  const BRACKET_2v2 = '2v2';
  const BRACKET_3v3 = '3v3';
  const BRACKET_5v5 = '5v5';
  const BRACKET_RATEDBG = 'rbg';
  
  private static $schema = array(
    'bracket' => array(
      self::BRACKET_2v2,
      self::BRACKET_3v3,
      self::BRACKET_5v5,
      self::BRACKET_RATEDBG
    )
  );
  
  /**
   * Local copy of the specified bracket.
   *
   * @var string
   */
  protected $bracket;
  
  /**
   * Constructs a BlizzardApiWowPvp object.
   *
   * @param string $bracket
   *   A pvp leaderboard bracket.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($bracket, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/leaderboard/{bracket}')
      ->setCacheLifetime(86400)  // 1 day
      ->setBracket($bracket);
  }
  
  /**
   * Returns the pvp leaderboard bracket.
   */
  public function getBracket() {
    return $this->bracket;
  }
  
  /**
   * Returns a list of supported brackets.
   */
  public function getSupportedBrackets() {
    return self::$schema['bracket'];
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    $this->data = $this->data['rows'];
  }
  
  /**
   * Sets the pvp leaderboard bracket.
   *
   * @param string $bracket
   *   A pvp leaderboard bracket, supported values include: 2v2, 3v3, 5v5, and rbg.
   *
   * @throws InvalidArgumentException
   */
  public function setBracket($bracket) {
    if (!in_array($bracket, self::$schema['bracket'])) {
      throw new InvalidArgumentException("Invalid bracket: $bracket");
    }
    
    $this->bracket = $bracket;
    $this->addPlaceholder('{bracket}', $bracket);
    return $this;
  }
}


/**
 * Provides quest information.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiWowQuest extends BlizzardApiWow {
  /**
   * Local copy of the specified quest ID.
   *
   * @var int
   */
  protected $questId;
  
  /**
   * Constructs a BlizzardApiQuest object.
   *
   * @param int $quest_id
   *   The ID number of the quest.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($quest_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/quest/{quest_id}')
      ->setQuest($quest_id);
  }
  
  /**
   * Returns the quest ID.
   */
  public function getQuestId() {
    return $this->questId;
  }
  
  /**
   * Set the quest to retrieve.
   *
   * @param int $quest_id
   *   The ID number of the quest.
   *
   * @throws DomainException, InvalidArgumentException
   */
  public function setQuest($quest_id) {
    if (!is_numeric($quest_id)) {
      throw new InvalidArgumentException('Quest ID must be a number');
    }
    if ($quest_id < 1) {
      throw new DomainException('Quest ID must be greater than 0');
    }
    
    $this->questId = $quest_id;
    $this->addPlaceholder('{quest_id}', $quest_id);
    return $this;
  }
}

/**
 * Provides realm status information.
 *
 * Default cache lifetime: 10 minutes.
 */
class BlizzardApiWowRealm extends BlizzardApiWow {
  const POPULATION_LOW    = 'low';
  const POPULATION_MEDIUM = 'medium';
  const POPULATION_HIGH   = 'high';
  
  const PVP_AREA_UNKNOWN    = -1;
  const PVP_AREA_IDLE       = 0;
  const PVP_AREA_POPULATING = 1;
  const PVP_AREA_ACTIVE     = 2;
  const PVP_AREA_CONCLUDED  = 3;
  
  const TYPE_PVE   = 'pve';
  const TYPE_PVP   = 'pvp';
  const TYPE_RP    = 'rp';
  const TYPE_RPPVP = 'rppvp';
  
  const STATUS_OFFLINE = 0;
  const STATUS_ONLINE  = 1;
  
  const QUEUE_NO  = 0;
  const QUEUE_YES = 1;
  
  private static $schema = array(
    'type' => array(
      self::TYPE_PVE,
      self::TYPE_PVP,
      self::TYPE_RP,
      self::TYPE_RPPVP
    ),
    'population' => array(
      self::POPULATION_LOW,
      self::POPULATION_MEDIUM,
      self::POPULATION_HIGH
    ),
    'status' => array(
      self::STATUS_OFFLINE,
      self::STATUS_ONLINE
    ),
    'queue' => array(
      self::QUEUE_NO,
      self::QUEUE_YES
    )
  );
  
  /**
   * Constructs a BlizzardApiWowRealm object.
   *
   * @param array $realms
   *   (optional) A list of realms to limit the results to. If not provided, the
   *   status of all realms will be returned.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($realms = array(), $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/realm/status')
      ->setCacheLifetime(600)  // 10 minutes
      ->setRealms($realms);
  }
  
  public static function filterByPopulation($realms, $population) {
    if (!in_array($population, self::$schema['population'])) {
      throw new InvalidArgumentException("Invalid realm population: $population");
    }
    return parent::filterBy($realms, 'population', $population);
  }
  
  public static function filterByQueue($realms, $queue) {
    if (!in_array($queue, self::$schema['queue'])) {
      throw new InvalidArgumentException("Invalid realm queue: $queue");
    }
    return parent::filterBy($realms, 'queue', $queue);
  }
  
  public static function filterByStatus($realms, $status) {
    if (!in_array($status, self::$schema['status'])) {
      throw new InvalidArgumentException("Invalid realm status: $status");
    }
    return parent::filterBy($realms, 'status', $status);
  }
  
  public static function filterByType($realms, $type) {
    if (!in_array($type, self::$schema['type'])) {
      throw new InvalidArgumentException("Invalid realm type: $type");
    }
    return parent::filterBy($realms, 'type', $type);
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   */
  protected function prepareData() {
    $this->data = $this->data['realms'];
  }
  
  /**
   * Set the specific realms to get information on.
   *
   * @param array $realms
   *   (optional) A list of realms to limit the results to. If not provided, the
   *   status of all realms will be returned.
   */
  public function setRealms($realms = array()) {
    $this->fields['realms'] = $realms;
    return $this;
  }
}

/**
 * Provides recipe information.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiWowRecipe extends BlizzardApiWow {
  /**
   * Local copy of the specified recipe ID.
   * 
   * @var int
   */
  protected $recipeId;
  
  /**
   * Constructs a BlizzardApiWowRecipe object.
   *
   * @param int $recipe_id
   *   The ID number of the recipe.
   * @param array $options
   *  (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($recipe_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/recipe/{recipe_id}')
      ->setRecipe($recipe_id);
  }
  
  /**
   * Returns the recipe ID.
   */
  public function getRecipeId() {
    return $this->recipeId;
  }
  
  /**
   * Set the recipe to retrieve.
   *
   * @param int $recipe_id
   *   The ID number of the recipe.
   * 
   * @throws DomainException, InvalidArgumentException
   */
  public function setRecipe($recipe_id) {
    if (!is_numeric($recipe_id)) {
      throw new InvalidArgumentException('Recipe ID must be a number');
    }
    if ($recipe_id < 1) {
      throw new DomainException('Recipe ID must be greater than 0');
    }
    
    $this->recipeId = $recipe_id;
    $this->addPlaceholder('{recipe_id}', $recipe_id);
    return $this;
  }
}

/**
 * Provides some information about spells.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiWowSpell extends BlizzardApiWow {
  /**
   * Local copy of the selected spell ID.
   *
   * @var int
   */
  protected $spellId;
  
  /**
   * Constructs a BlizzardApiWowSpell object.
   *
   * @param int $spell_id
   *   The ID number of the spell.
   * @param array $options
   *  (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   */
  public function __construct($spell_id, $options = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->setApiPath($this->apiPath . '/spell/{spell_id}')
      ->setSpell($spell_id);
  }
  
  /**
   * Returns the spell ID.
   */
  public function getSpellId() {
    return $this->spellId;
  }
  
  /**
   * Set the spell to retrieve.
   *
   * @param int $spell_id
   *   The ID number of the spell.
   * 
   * @throws DomainException, InvalidArgumentException
   */
  public function setSpell($spell_id) {
    if (!is_numeric($spell_id)) {
      throw new InvalidArgumentException('Spell ID must be a number');
    }
    if ($spell_id < 1) {
      throw new DomainException('Spell ID must be greater than 0');
    }
    
    $this->spellId = $spell_id;
    $this->addPlaceholder('{spell_id}', $spell_id);
    return $this;
  }
}

/**
 * Provides information that compliments other resources and provides structure,
 * definition, and/or context.
 *
 * Default cache lifetime: 30 days.
 */
class BlizzardApiWowData extends BlizzardApiWow {
  /**
   * A list of battlegroups in the given region.
   */
  const RESOURCE_BATTLEGROUPS = 1;
  /**
   * A list of all achievements that a character can earn as well as the
   * category structure and hierarchy.
   */
  const RESOURCE_CHARACTER_ACHIEVEMENTS = 2;
  /**
   * A list of character classes.
   */
  const RESOURCE_CLASSES = 3;
  /**
   * A list of all achievements that guilds can earn as well as the category
   * structure and hierarchy.
   */
  const RESOURCE_GUILD_ACHIEVEMENTS = 4;
  /**
   * A list of guild perks.
   */
  const RESOURCE_GUILD_PERKS = 5;
  /**
   * A list of guild rewards.
   */
  const RESOURCE_GUILD_REWARDS = 6;
  /**
   * A list of item classes.
   */
  const RESOURCE_ITEM_CLASSES = 7;
  /**
   * A list of character races.
   */
  const RESOURCE_RACES = 8;
  /**
   * A list of talents, specs and glyphs for each class.
   */
  const RESOURCE_TALENTS = 9;
  /**
   * A list of battle pet types (including what they are strong or weak against).
   */
  const RESOURCE_PET_TYPES = 10;
  
  /**
   * Each method in this class uses a unique API path, so we store a copy of the
   * base path during initialization.
   *
   * @var string
   */
  private $baseApiPath;
  
  /**
   * The currently selected type of data to retrieve.
   *
   * @var int
   */
  protected $dataResource;
  
  /**
   * Constructs a BlizzardApiWowData object.
   *
   * @param int $resource
   *   The type of data to get. See the resource constants provided by this
   *   class for supported values.
   * @param array $options
   *   (optional) An array containing any combination of the following keys:
   *   - region: The Battle.net region to request data from.
   *   - language: The language to request data in.
   * @param array $args
   *   (optional) Reserved for future use.
   */
  public function __construct($resource, $options = array(), $args = array()) {
    $options += array(
      'region' => self::REGION_US,
      'language' => self::LANGUAGE_DEFAULT
    );
    
    parent::__construct($options['region'], $options['language']);
    $this->baseApiPath = $this->apiPath;
    $this->setResource($resource, $args);
  }
  
  /**
   * Returns the type of data to request.
   */
  public function getResourceType() {
    return $this->dataResource;
  }
  
  /**
   * Overrides BlizzardApi::prepareData().
   *
   * Removes the "wrapper" array from returned data, if present.
   */
  protected function prepareData() {
    // Technically, this could be the default implementation, but if an API has
    // an optional key at the root level, then the returned data would be
    // inconsistent. At least with this API, the data is supposedly constant.
    $keys = array_keys($this->data);
    if (count($keys) == 1 && is_string($keys[0])) {
      $this->data = $this->data[$keys[0]];
    }
  }
  
  /**
   * Set the data resource type to request.
   *
   * @param int $resource
   *   The type of data to get. See the resource constants provided by this
   *   class for supported values.
   * @param array $args
   *   (optional) Reserved for future use.
   *
   * @throws InvalidArgumentException
   */
  public function setResource($resource, $args = array()) {
    switch ($resource) {
      case self::RESOURCE_BATTLEGROUPS:
        // Appending 'index' is no longer required, but a trailing slash is.
        // Previous issue: http://us.battle.net/wow/en/forum/topic/6522902777
        $this->setApiPath($this->baseApiPath . '/data/battlegroups/');
        break;
      case self::RESOURCE_CHARACTER_ACHIEVEMENTS:
        $this->setApiPath($this->baseApiPath . '/data/character/achievements');
        break;
      case self::RESOURCE_CLASSES:
        $this->setApiPath($this->baseApiPath . '/data/character/classes');
        break;
      case self::RESOURCE_GUILD_ACHIEVEMENTS:
        $this->setApiPath($this->baseApiPath . '/data/guild/achievements');
        break;
      case self::RESOURCE_GUILD_PERKS:
        $this->setApiPath($this->baseApiPath . '/data/guild/perks');
        break;
      case self::RESOURCE_GUILD_REWARDS:
        $this->setApiPath($this->baseApiPath . '/data/guild/rewards');
        break;
      case self::RESOURCE_ITEM_CLASSES:
        $this->setApiPath($this->baseApiPath . '/data/item/classes');
        break;
      case self::RESOURCE_PET_TYPES:
        $this->setApiPath($this->baseApiPath . '/data/pet/types');
        break;
      case self::RESOURCE_RACES:
        $this->setApiPath($this->baseApiPath . '/data/character/races');
        break;
      case self::RESOURCE_TALENTS:
        $this->setApiPath($this->baseApiPath . '/data/talents');
        break;
      default:
        throw new InvalidArgumentException("Invalid data resource type: $resource");
    }
    
    $this->dataResource = $resource;
    return $this;
  }
}

/**
 * Factory class to generate Blizzard API objects for World of Warcraft.
 */
class BlizzardApiWowResource {
  public static function getAchievement($achievement_id, $options = array()) {
    return new BlizzardApiWowAchievement($achievement_id, $options);
  }
  
  public static function getAuction($realm, $options = array()) {
    return new BlizzardApiWowAuction($realm, $options);
  }
  
  public static function getBattlePet($resource, $resource_id, $query = array(), $options = array()) {
    return new BlizzardApiWowBattlePet($resource, $resource_id, $query, $options);
  }
  
  public static function getChallenge($realm, $options = array()) {
    return new BlizzardApiWowChallenge($realm, $options = array());
  }
  
  public static function getCharacter($character, $realm, $fields = array(), $options = array()) {
    return new BlizzardApiWowCharacter($character, $realm, $fields, $options);
  }
  
  public static function getData($resource, $options = array(), $args = array()) {
    return new BlizzardApiWowData($resource, $options, $args);
  }
  
  public static function getGuild($guild, $realm, $fields = array(), $options = array()) {
    return new BlizzardApiWowGuild($guild, $realm, $fields, $options);
  }
  
  public static function getItem($item_id, $options = array()) {
    return new BlizzardApiWowItem($item_id, $options);
  }
  
  public static function getItemSet($set_id, $options = array()) {
    return new BlizzardApiWowItemSet($set_id, $options);
  }
  
  public static function getPvp($bracket, $options = array()) {
    return new BlizzardApiWowPvp($bracket, $options);
  }
  
  public static function getQuest($quest_id, $options = array()) {
    return new BlizzardApiWowQuest($quest_id, $options);
  }
  
  public static function getRealm($realms = array(), $options = array()) {
    return new BlizzardApiWowRealm($realms, $options);
  }
  
  public static function getRecipe($recipe_id, $options = array()) {
    return new BlizzardApiWowRecipe($recipe_id, $options);
  }
  
  public static function getSpell($spell_id, $options = array()) {
    return new BlizzardApiWowSpell($spell_id, $options);
  }
}
