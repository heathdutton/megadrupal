<?php

/**
 * @file
 * module configuration settings
 */

/**
 * Represents a configuration instance.
 */
interface TumblrConnectConfigInterface {

  /**
   * Gets the unique identifier for this configuration instance.
   *
   * @return integer
   *   The unique identifier for this configuration instance.
   */
  public function getId();

  /**
   * Sets the unique identifier for this configuration instance.
   *
   * @param integer $id
   *   The unique identifier for this configuration instance.
   */
  public function setId($id);

  /**
   * Gets the email credential for this configuration instance.
   *
   * @return string
   *   The email credential for this configuration instance.
   */
  public function getEmail();

  /**
   * Sets the email credential for this configuration instance.
   *
   * @param string $email
   *   The email credential for this configuration instance.
   */
  public function setEmail($email);

  /**
   * Gets the password credential for this configuration instance.
   *
   * @return string
   *   The password credential for this configuration instance.
   */
  public function getPassword();

  /**
   * Sets the password credential for this configuration instance.
   *
   * @param string $password
   *   The password credential for this configuration instance.
   */
  public function setPassword($password);

  /**
   * Gets the url of the tumblr remote page for this configuration instance.
   *
   * @return string
   *   The url of the tumblr remote page for this configuration instance.
   */
  public function getUrl();

  /**
   * Sets the url of the tumblr remote page for this configuration instance.
   *
   * @param $url
   *   The url of the tumblr remote page for this configuration instance.
   */
  public function setUrl($url);

  /**
   * Gets the view type property for this configuration instance.
   *
   * @return boolean
   *   The view type property for this configuration instance.
   *   TRUE use teaser view, FALSE use full view.
   */
  public function getTeaserOnly();

  /**
   * Sets the view type property for this configuration instance.
   *
   * @param boolean $teaser_only
   *   The view type property for this configuration instance.
   *   TRUE use teaser view, FALSE use full view.
   */
  public function setTeaserOnly($teaser_only);

  /**
   * Gets the list of content type names for this configuration instance.
   *
   * @return array
   *   The list of content type names for this configuration instance.
   */
  public function getContentTypes();

  /**
   * Sets the list of content type names for this configuration instance.
   *
   * @param array $content_types
   *   The list of content type names for this configuration instance.
   */
  public function setContentTypes($content_types);

  /**
   * Gets the properties of this configuration as an array..
   *
   * @return array
   *   The properties of this configuration.
   */
  public function toArray();
}

/**
 * Represents a configuration instance.
 *
 * Default implementation of TumblrConnectConfigInterface.
 */
class TumblrConnectConfig implements TumblrConnectConfigInterface {

  /**
   * Unique identfier for configuration instance.
   *
   * @var integer
   */
  protected $id;

  /**
   * Email credential for this configuration instance.
   *
   * @var string
   */
  protected $email;

  /**
   * Password credential for this configuration instance.
   *
   * @var string
   */
  protected $password;

  /**
   * Url of remote tumblr servive page.
   *
   * @var string
   */
  protected $url;

  /**
   * Defines view type (teaser or full).
   *
   * @var boolean
   *  TRUE for teaser view, FALSE for full view.
   */
  protected $teaserOnly;

  /**
   * Content types synchronized with remote tumblr service.
   *
   * @var array
   *
   */
  protected $contentTypes;

  /**
   * Default constructor.
   */
  function __construct() {

  }

  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }

  public function getEmail() {
    return $this->email;
  }

  public function setEmail($email) {
    $this->email = $email;
  }

  public function getPassword() {
    return $this->password;
  }

  public function setPassword($password) {
    $this->password = $password;
  }

  public function getUrl() {
    return $this->url;
  }

  public function setUrl($url) {
    $this->url = $url;
  }

  public function getTeaserOnly() {
    return $this->teaserOnly;
  }

  public function setTeaserOnly($teaser_only) {
    $this->teaserOnly = $teaser_only;
  }

  public function getContentTypes() {
    return unserialize($this->contentTypes);
  }

  /**
   * Implements TumblrConnectConfigInterface::setContentTypes().
   */
  public function setContentTypes($content_types) {
    if (is_array($content_types)) {
      $this->contentTypes = serialize($content_types);
    }
    else {
      $this->contentTypes = $content_types;
    }
  }

  /**
   * Implements TumblrConnectConfigInterface::toArray().
   */
  public function toArray() {

    return array(
        'id' => $this->id,
        'email' => $this->email,
        'password' => $this->password,
        'url' => $this->url,
        'teaser_only' => $this->teaserOnly,
        'content_types' => $this->contentTypes,
    );
  }

}

/**
 * Database access for configuration instances.
 */
class DbTumblrConnectConfig {

  /**
   * Singleton instance of the DbTumblrConnectConfig class.
   *
   * @var DbTumblrConnectConfig.
   */
  protected static $instance = NULL;

  /**
   * Default private constructor.
   */
  private function __construct() {

  }

  /**
   * Static factory constructor method.
   *
   * @return DbTumblrConnectConfig
   *
  */
  public static function factory() {
    if (self::$instance === NULL) {
      self::$instance = new DbTumblrConnectConfig();
    }
    return self::$instance;
  }

   /**
   * Adds or modifies content configuration instance in the database.
   *
   * @param: $config
   *   TumblrConnectConfigInterface.
   *
   * @throws TumblrConnectDatabaseException.
   */
  public function addOrModify(TumblrConnectConfigInterface $config) {

    $obj = db_query('SELECT id from {tumblr_connect_config}')->fetchObject();
    // if the database returns an id we have a record and thus do an update
    // if no id was returned then we add a new record
    if (isset($obj->id)) {
      try {
        $result = db_update('tumblr_connect_config')
          ->fields($config->toArray())
          ->execute();
      } catch (Exception $e) {
        throw new TumblrConnectDatabaseException(t('Unable to update tumblr config'));
      }
    }
    else {
      try {
        $result = db_insert('tumblr_connect_config')
          ->fields($config->toArray())
          ->execute();
      } catch (Exception $e) {
        throw new TumblrConnectDatabaseException(t('Unable to insert update tumblr config'));
      }
    }
  }

  /**
   * Gets configuration instance from database.
   *
   * @return tumblr_config
   *   this modules configuration settings.
   *
   * @throws TumblrConnectDatabaseException.
   */
  public function get() {
    $config = NULL;
    $result = db_query('SELECT id, email, password, url, teaser_only, content_types from {tumblr_connect_config}')->fetchObject();
    if ($result) {
      $config = new TumblrConnectConfig();
      $config->setId($result->id);
      $config->setContentTypes($result->content_types);
      $config->setEmail($result->email);
      $config->setPassword($result->password);
      $config->setUrl($result->url);
      $config->setTeaserOnly($result->teaser_only);
    }
    else {
      throw new TumblrConnectDatabaseNoRecordException(t('No Tumblr Config Record Found'));
    }
    return $config;
  }

  /**
   * Gats a static instance of configuration instance if available.
   *
   * @param $reset
   *    set to true to clear cached value
   * @return
   *   tumblr_config  -- this modules configuration settings
   */
  public function getCachedConfig($reset = FALSE) {
    static $config;
    if ($reset) {
      unset($config);
    }
    if (!isset($config)) {
      try {
        $config = $this->get();
      } catch (TumblrConnectDatabaseException $te) {
        throw $te;
      }
    }
    return $config;
  }

}
