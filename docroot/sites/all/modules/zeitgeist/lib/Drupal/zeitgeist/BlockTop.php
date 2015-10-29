<?php

/**
 * @file
 * Zeitgeist "top searches" block.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * "Top Searches" block.
 */
class BlockTop extends ListBlock  {
  /**
   * @const Block minimum caching duration in minutes.
   */
  const VARCACHE = 'zeitgeist_top_cache';

  /**
   * @const How many top queries are displayed in the block ?
   */
  const VARCOUNT = 'zeitgeist_top_count';

  /**
   * @const 'list' or 'implode'.
   */
  const VARDISPLAY = 'zeitgeist_top_display';

  /**
   * @const Separator string for "implode" display.
   */
  const VARSEP = 'zeitgeist_top_sep';

  /**
   * Delegated implementation of the theme hook.
   *
   * @see \Drupal\zeitgeist\ThemableInterface::hookTheme()
   */
  public function hookTheme($existing, $type, $theme, $path) {
    $ret['zeitgeist_block_top'] = $this->blockThemeDefaults() + array(
      'template'  => 'theme/zeitgeist-block-top',
    );

    return $ret;
  }

  /**
   * Delegated implementation of the old settings hook.
   */
  public function settings() {
    $ret = array(
      '#type'          => 'fieldset',
      '#title'         => t('"Top searches" block'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );

    $ret[static::VARCOUNT] = array(
      '#default_value' => $this->getCount(),
      '#description'   => t('This is the number of top searches displayed in the "Top Searches" block provided by the zeitgeist module. Default: %def.',
        array('%def' => $this->getCount(TRUE))),
      '#element_validate' => array('element_validate_integer_positive'),
      '#maxlength'     => 2,
      '#size'          => 2,
      '#title'         => t('Top searches displayed'),
      '#type'          => 'textfield',
    );
    $ret[static::VARCACHE] = array(
      '#default_value' => (int) variable_get(static::VARCACHE, 0),
      '#description'   => t('Block minimum caching duration in minutes (1 day = 1440 minutes). Note that blocks are cached as themed.'),
      '#element_validate' => array('element_validate_integer_positive'),
      '#maxlength'     => 5,
      '#size'          => 5,
      '#title'         => t('Cache expiration'),
      '#type'          => 'textfield',
    );
    $ret[static::VARDISPLAY] = array(
      '#attributes'    => array('class' => array('container-inline')),
      '#default_value' => variable_get(static::VARDISPLAY, static::DEFDISPLAY),
      '#options'       => array('list' => t('HTML List'), 'implode' => t('Inline list with separators')),
      '#title'         => t('Display mode'),
      '#type'          => 'radios',
    );
    $ret[static::VARSEP] = array(
      '#default_value' => variable_get(static::VARSEP, static::DEFSEP),
      '#description'   => t('If you chose "Inline list with separators" above, results will be separated by this string. You may use HTML.'),
      '#title'         => t('Inline list separator'),
      '#type'          => 'textfield',
    );

    return $ret;
  }

  /**
   * Delegated implementation of block_info hook.
   */
  public function info() {
    $ret = array(
      'info' => t(variable_get('zeitgeist_top_info', t('ZG Top @count')),
        array('@count' => $this->getCount())),
      'cache'      => DRUPAL_CACHE_PER_ROLE,
      'properties' => array('administrative' => TRUE),
    );
    return $ret;
  }

  /**
   * Return a freshly built block.
   *
   * By default, stats are computed for the preceding month. See how to swap
   * the default interpretation by changing which block is commented out.
   *
   * @return array
   *   A subject/content hash.
   */
  public function viewUncached() {
    $count = $this->getCount();
    $display = variable_get(static::VARDISPLAY, static::DEFDISPLAY);
    $separator = variable_get(static::VARSEP, static::DEFSEP);

    // Return Zeitgeist for the previous month.
    $ar_date = getdate();
    // Mktime() wraps months cleanly, no worry.
    $start = mktime(0, 0, 0, $ar_date['mon'] - 1, 1, $ar_date['year']);
    unset($ar_date);

    // Return Zeitgeist for the current month.
    $start = NULL;

    $statistics = Statistics::getStatistics(Span::MONTH, $start, 'node', $count);

    $ret = array(
      'subject' => t('Top @count searches', array('@count' => $this->count)),
      'content' => array(
        '#theme' => 'zeitgeist_block_top',
        '#count' => $this->count,
        '#items' => $statistics->scores,
        '#nofollow' => static::isNoFollowEnabled(),
        '#display' => $display,
        '#separator' => $separator,
      ),
    );
    return $ret;
  }
}
