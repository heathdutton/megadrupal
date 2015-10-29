<?php

/**
 * @file
 * Zeitgeist list-type blocks.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * These blocks are lists and, as such, include a "count" parameter.
 */
abstract class ListBlock extends Block {
  /**
   * @const Default size of search links lists.
   */
  const DEFCOUNT = 5;

  /**
   * @const Search links list format.
   */
  const DEFDISPLAY = 'list';

  /**
   * @const Default separator for blocks in "inline" mode.
   */
  const DEFSEP = ' - ';

  /**
   * @const Variable name for the nofollow setting.
   */
  const VARNOFOLLOW = 'zeitgeist_nofollow';

  /**
   * @var int
   *   The number of items in the list displayed by the block.
   */
  public $count;

  /**
   * Constructor.
   *
   * @param string $delta
   *   The Drupal core block delta.
   */
  public function __construct($delta) {
    parent::__construct($delta);
    $this->count = $this->getCount();
  }

  /**
   * Return the number of items to show on instances of the concrete block.
   *
   * @param bool $return_default
   *   Return the default value instead of the actual value.
   *
   * @return int
   *   The number of items to return.
   */
  public function getCount($return_default = FALSE) {
    if ($return_default) {
      $ret = static::DEFCOUNT;
    }
    else {
      $variable = "zeitgeist_{$this->delta}_count";
      $ret = variable_get($variable, static::DEFCOUNT);
    }
    $ret = (int) $ret;
    return $ret;
  }

  /**
   * Helper for concrete hookTheme() implementations.
   *
   * @return array
   *   Common parts of a list block hookTheme() implementation.
   */
  protected function blockThemeDefaults() {
    $ret = array(
      'variables' => array(
        'count' => static::DEFCOUNT,
        'items' => array(),
        'nofollow' => static::isNoFollowEnabled(),
        'display' => static::DEFDISPLAY,
        'separator' => static::DEFSEP,
      ),
    );
    return $ret;
  }

  /**
   * Decide whether to emit a rel=nofollow attribute in feature blocks.
   *
   * @return bool
   *   TRUE by default.
   */
  public static function isNoFollowEnabled() {
    $ret = variable_get(static::VARNOFOLLOW, TRUE);
    return $ret;
  }

  /**
   * Delegated implementation of the old settings hook.
   *
   * Cannot be named settings() to avoid clashing with the non-static settings()
   * method in concrete classes.
   */
  public static function settingsAdvanced() {
    $arguments = array('!attr' => '<code>rel="nofollow"</code>');
    $ret = array(
      '#type'          => 'checkbox',
      '#title'         => t('Apply !attr to links on the "recent searches" and "top searches" blocks', $arguments),
      '#default_value' => ListBlock::isNoFollowEnabled(),
      '#return_value'  => 1,
      '#description'   => t('An attribute for the "a" (X)HTML alement, !attr is non-standard, but is recognized by several search engines, notably Google, MSN and Yahoo!', $arguments),
    );
    return $ret;
  }
}
