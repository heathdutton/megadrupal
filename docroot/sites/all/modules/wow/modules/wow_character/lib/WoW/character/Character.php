<?php

/**
 * @file
 * Definition of WoW\character\Character.
 */

/**
 * Defines the wow_character entity class.
 */
class WoWCharacter extends WoWRemoteEntity {

  /**
   * The character ID.
   *
   * @var integer
   */
  public $cid;

  /**
   * The character owner's user ID.
   *
   * @var integer
   */
  public $uid;

  /**
   * Timestamp for character's last update.
   *
   * @var integer
   */
  public $lastModified = 0;

  /**
   * The character realm (slug).
   *
   * @var string
   */
  public $realm;

  /**
   * The character name.
   *
   * @var string
   */
  public $name;

  /**
   * The character level.
   *
   * @var integer
   */
  public $level;

  /**
   * Whether the character is active(1) or blocked(0).
   *
   * @var integer
   */
  public $status;

  /**
   * Whether the character is main(1) or alt(0).
   *
   * @var integer
   */
  public $isMain;

  /**
   * The character thumbnail.
   *
   * @var string
   */
  public $thumbnail;

  /**
   * The character race.
   *
   * @var integer
   */
  public $race;

  /**
   * The character achievement points.
   *
   * @var integer
   */
  public $achievementPoints;

  /**
   * The character gender.
   *
   * @var integer
   */
  public $gender;

  /**
   * The character class.
   *
   * @var integer
   */
  public $class;

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
   *   - guild: A summary of the guild that the character belongs to. If the
   *     character does not belong to a guild and this field is requested, this
   *     field will not be exposed.
   *   - stats: A map of character attributes and stats.
   *   - talents: A list of talent structures.
   *   - items: A list of items equipted by the character. Use of this field will
   *     also include the average item level and average item level equipped for
   *     the character.
   *   - reputation: A list of the factions that the character has an associated
   *     reputation with.
   *   - titles: A list of the titles obtained by the character including the
   *     currently selected title.
   *   - professions: A list of the character's professions. It is important to
   *     note that when this information is retrieved, it will also include the
   *     known recipes of each of the listed professions.
   *   - appearance: A map of values that describes the face, features and
   *     helm/cloak display preferences and attributes.
   *   - companions: A list of all of the non-combat pets obtained by the character.
   *   - mounts: A list of all of the mounts obtained by the character.
   *   - pets: A list of all of the combat pets obtained by the character.
   *   - achievements: A map of achievement data including completion timestamps
   *     and criteria information.
   *   - progression: A list of raids and bosses indicating raid progression and
   *     completedness.
   *   - pvp: A map of pvp information including arena team membership and rated
   *     battlegrounds information.
   *   - quests: A list of quests completed by the character.
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
    entity_get_controller('wow_character')->fetch($this, $fields, $locale, $catch);
  }

  public function remotePath() {
    return drupal_encode_path("character/$this->realm/$this->name");
  }
}
