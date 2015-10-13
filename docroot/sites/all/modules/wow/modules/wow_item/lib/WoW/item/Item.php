<?php

/**
 * @file
 * Definition of WoW\item\Item.
 */

/**
 * Defines the wow_guild entity class.
 */

class WoWItem extends WoWRemoteEntity {

  /**
   * The item ID.
   *
   * @var integer
   */
  public $id;

  /**
   * The item language.
   *
   * @var string
   */
  public $language;

  /**
   * The item quality.
   *
   * @var integer
   */
  public $quality;

  /**
   * The icon of this item.
   *
   * @var string
   */
  public $icon;

  /**
   * The item API provides detailed item information.
   *
   * @param string $locale
   *   (Optional) The locale to fetch the resource with.
   *     It it the responsibility of the caller to pass a valid locale.
   *     @see wow_api_locale()
   * @param Boolean $catch
   *   Whether to catch exceptions or not.
   *
   * @throws WoWHttpException
   */
  public function fetch($locale = NULL, $catch = TRUE) {
    entity_get_controller('wow_item')->fetch($this, array(), $locale, $catch);
  }

  public function remotePath() {
    return "item/$this->id";
  }

}
