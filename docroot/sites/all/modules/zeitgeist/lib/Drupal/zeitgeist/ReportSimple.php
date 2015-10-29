<?php

/**
 * @file
 * Report page for the Zeitgeist module, simple paged version.
 *
 * While usable, these three pages should mostly be considered as examples.
 * For smarter results, build your own page with Statistics::getStatistics()
 * and the Span::* human-oriented duration constants, or use the Views 3
 * integration as show in zeitgeist_page_results_views(). Either way, this will
 * free you from having to build the database query yourself.
 *
 * @see Drupal\zeitgeist\Statistics::getStatistics()
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * The simple, single-column-with-pager report type.
 */
class ReportSimple extends Report {

  /**
   * Sample basic stats page.
   *
   * @return string
   *   HTML
   */
  public function page() {
    $limit = REQUEST_TIME - Statistics::ONEDAY * $this->days;
    drupal_set_title(t('Zeitgeist over the last @days days', array('@days' => $this->days)), PASS_THROUGH);
    $height = $this->getHeight();

    // Do not use positional groupBy(), as this breaks PagerDefault.
    $query = db_select(Statistics::TABLE, 'zg')
      ->extend('PagerDefault')
      ->fields('zg', array('search'));
    $query->addExpression('COUNT(zg.ts)', 'cnt');
    $query->condition('zg.category', 'node')
      ->condition('zg.ts', $limit, '>')
      ->limit($height)
      ->groupBy('search')
      ->orderBy(2, 'DESC')
      ->orderBy(1, 'ASC');
    $results = $query->execute();

    $items = array();
    $cnt = 0;
    foreach ($results as $result) {
      $items[] = "$result->cnt - " . l($result->search, "search/node/$result->search");
      $cnt += $result->cnt;
    }

    $ret = array();
    $ret['items'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
    $ret['summary'] = array(
      '#markup' => '<p>' . t('For a total of %cnt hits for these %height searches.', array(
        '%cnt' => $cnt,
        '%height' => $height,
      )) . "</p>\n",
    );

    // Attach the pager theme.
    $ret['pager_pager'] = array('#theme' => 'pager');
    return $ret;
  }
}
