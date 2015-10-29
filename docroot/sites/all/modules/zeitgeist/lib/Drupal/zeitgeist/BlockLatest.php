<?php

/**
 * @file
 * Zeitgeist "latest searches" block.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * "Latest searches" block.
 */
class BlockLatest extends ListBlock {
  /**
   * @const Never display search history.
   */
  const DISPLAY_NEVER = 0;
  /**
   * @const Display search history only to administrators.
   */
  const DISPLAY_ADMIN = 1;
  /**
   * @const Display search history only to authenticated users.
   */
  const DISPLAY_AUTH = 2;
  /**
   * @const Always display search history.
   */
  const DISPLAY_ALWAYS = 3;

  /**
   * @const Block minimum caching duration in minutes.
   */
  const VARCACHE = 'zeitgeist_latest_cache';

  /**
   * @const How many recent queries are displayed in the block ?
   */
  const VARCOUNT = 'zeitgeist_latest_count';

  /**
   * @const 'list' or 'implode'.
   */
  const VARDISPLAY = 'zeitgeist_latest_display';

  /**
   * @const Separator string for "implode" display.
   */
  const VARSEP = 'zeitgeist_latest_sep';

  /**
   * @const Name of the variable for the search type display policy.
   */
  const VARPOLICY = 'zeitgeist_type';

  /**
   * Return the rule used to include the search type in results.
   *
   * @return int
   *   The display rule: one of the BlockLatest::DISPLAY_* constants.
   */
  public function getDisplayPolicy() {
    $ret = variable_get(static::VARPOLICY, static::DISPLAY_ADMIN);
    return $ret;
  }

  /**
   * Decide whether block displays may include category information.
   *
   * @return int
   *   One of the BlockLatest::DISPLAY_* constants.
   */
  public function hasCategory() {
    switch ($this->getDisplayPolicy()) {
      // Display only for admin.
      case static::DISPLAY_ADMIN:
        $ret = ($GLOBALS['user']->uid == 1);
        break;

      // Display for all authenticated users.
      case static::DISPLAY_AUTH:
        $ret = user_is_logged_in();
        break;

      // Always display.
      case static::DISPLAY_ALWAYS:
        $ret = TRUE;
        break;

      // BlockLatest::DISPLAY_NEVER and any other value.
      default:
        $ret = FALSE;
        break;
    }

    return $ret;
  }

  /**
   * Delegated implementation of the theme hook.
   *
   * @see \Drupal\zeitgeist\ThemableInterface::hookTheme()
   */
  public function hookTheme($existing, $type, $theme, $path) {
    $ret['zeitgeist_block_latest'] = $this->blockThemeDefaults() + array(
      'template'  => 'theme/zeitgeist-block-latest',
    );

    return $ret;
  }

  /**
   * Delegated implementation of the old settings hook.
   */
  public function settings() {
    $ret = array(
      '#type'          => 'fieldset',
      '#title'         => t('"Latest searches" block'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );

    $ret[static::VARCOUNT] = array(
      '#type'          => 'textfield',
      '#title'         => t('Latest searches displayed'),
      '#default_value' => $this->getCount(),
      '#description'   => t('This is the number of recent searches displayed in the "Latest Searches" block provided by the zeitgeist module. Default: %def.',
        array('%def' => $this->getCount(TRUE))),
      '#element_validate' => array('element_validate_integer_positive'),
      '#maxlength'     => 2,
      '#size'          => 2,
    );
    $ret[static::VARPOLICY] = array(
      '#type'          => 'radios',
      '#title'         => t('When should the search type be displayed in the "Latest searches" block ?'),
      '#default_value' => $this->getDisplayPolicy(),
      '#options'       => array(
        static::DISPLAY_NEVER  => t('Never'),
        static::DISPLAY_ADMIN  => t('Admin only'),
        static::DISPLAY_AUTH   => t('Authenticated users'),
        static::DISPLAY_ALWAYS => t('Always'),
      ),
      '#description'   => t('<p>Recommended value is "Admin only"; "Always" is typically disturbing for visitors not already well-versed in Drupal.</p>'),
    );
    $ret[static::VARCACHE] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache expiration'),
      '#default_value' => (int) variable_get(static::VARCACHE, 0),
      '#description'   => t('Block minimum caching duration in minutes (1 day = 1440 minutes). Note that blocks are cached as themed.'),
      '#element_validate' => array('element_validate_integer_positive'),
      '#size'          => 5,
      '#maxlength'     => 5,
    );
    $ret[static::VARDISPLAY] = array(
      '#type'          => 'radios',
      '#title'         => t('Display mode'),
      '#options'       => array('list' => t('HTML List'), 'implode' => t('Inline list with separators')),
      '#attributes'    => array('class' => array('container-inline')),
      '#default_value' => variable_get(static::VARDISPLAY, static::DEFDISPLAY),
    );
    $ret[static::VARSEP] = array(
      '#type'          => 'textfield',
      '#title'         => t('Inline list separator'),
      '#description'   => t('If you chose "Inline list with separators" above, results will be separated by this string. You may use HTML.'),
      '#default_value' => variable_get(static::VARSEP, static::DEFSEP),
    );

    return $ret;
  }

  /**
   * Delegated implementation of the block_info hook.
   */
  public function info() {
    $ret = array(
      'info' => t(variable_get('zeitgeist_latest_info', t('ZG Latest @count')),
        array('@count' => $this->getCount())),
      'cache' => DRUPAL_NO_CACHE,
      'properties' => array('administrative' => TRUE),
    );
    return $ret;
  }

  /**
   * Return a freshly built block.
   *
   * @return array
   *   A subject/content hash.
   */
  public function viewUncached() {
    $count = $this->getCount();
    $display = variable_get(static::VARDISPLAY, static::DEFDISPLAY);
    $separator = variable_get(static::VARSEP, static::DEFSEP);

    $items = Statistics::getLatestSearches($count, $this->hasCategory());

    $ret = array(
      'subject' => t('Latest @count searches', array('@count' => $count)),
      'content' => array(
        '#theme' => 'zeitgeist_block_latest',
        '#count' => $count,
        '#items' => $items,
        '#nofollow' => static::isNoFollowEnabled(),
        '#display' => $display,
        '#separator' => $separator,
      ),
    );
    return $ret;
  }
}
