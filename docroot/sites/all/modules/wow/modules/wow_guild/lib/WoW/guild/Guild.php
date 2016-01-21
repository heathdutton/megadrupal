<?php

/**
 * @file
 * Definition of WoW\guild\Guild.
 */

/**
 * Defines the wow_guild entity class.
 */

class WoWGuild extends WoWRemoteEntity {

  /**
   * The guild ID.
   *
   * @var integer
   */
  public $gid;

  /**
   * Timestamp for guild's last update.
   *
   * @var integer
   */
  public $lastModified = 0;

  /**
   * The guild realm (slug).
   *
   * @var string
   */
  public $realm;

  /**
   * The guild name.
   *
   * @var string
   */
  public $name;

  /**
   * The guild level.
   *
   * @var integer
   */
  public $level;

  /**
   * Whether the guild is alliance(0) or horde(1).
   *
   * @var integer
   */
  public $side;

  /**
   * The guild achievement points.
   *
   * @var integer
   */
  public $achievementPoints;

  /**
   * The Character Profile API is the primary way to access character information.
   *
   * This Character Profile API can be used to fetch a single character at a time
   * through an HTTP GET request to a URL describing the character profile resource.
   *
   * By default, a basic dataset will be returned and with each request and zero
   * or more additional fields can be retrieved. To access this API, craft a
   * resource URL pointing to the character whos information is to be retrieved.
   *
   * @param array $fields
   *   An array of fields to fetch:
   *   - members: A list of characters that are a member of the guild.
   *   - achievements: A set of data structures that describe the achievements
   *     earned by the guild.
   *   - news: A set of data structures that describe the news feed of the guild.
   * @param string $locale
   *   (Optional) The locale to fetch the resource with.
   *     It it the responsibility of the caller to pass a valid locale.
   *     @see wow_api_locale()
   * @param Boolean $catch
   *   Whether to catch exceptions or not.
   *
   * @throws WoWHttpException
   */
  public function fetch(array $fields = array(), $locale = NULL, $catch = TRUE) {
    entity_get_controller('wow_guild')->fetch($this, $fields, $locale, $catch);
  }

  public function remotePath() {
    return drupal_encode_path("guild/$this->realm/$this->name");
  }
}
