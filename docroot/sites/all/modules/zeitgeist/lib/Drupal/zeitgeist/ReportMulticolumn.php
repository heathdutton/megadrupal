<?php

/**
 * @file
 * Report page for the Zeitgeist module, multicolumn version.
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
 * The multi-column report class.
 */
class ReportMulticolumn extends Report {
  /**
   * Page callback showing ZG results.
   *
   * @return string
   *   Rendered page content.
   */
  public function page() {
    $limit = REQUEST_TIME - Statistics::ONEDAY * $this->days;
    drupal_set_title(t('Zeitgeist over the last @days days', array('@days' => $this->days)), PASS_THROUGH);

    $height = $this->getHeight();

    $ar_searches = array();
    $sql = <<<SQL
SELECT DISTINCT zg.search, count(zg.ts) AS nts
FROM {zeitgeist} zg
WHERE zg.ts >= :ts
GROUP BY zg.search
ORDER BY nts DESC, zg.search ASC
SQL;

    // No access control on this table.
    $results = db_query_range($sql, 0, $height * 3, array(':ts' => $limit));
    foreach ($results as $result) {
      if ($result->search == '') {
        $result->search = '&lt;vide&gt;';
      }
      $ar_searches[$result->search] = $result->nts;
    }

    $results_count = count($ar_searches);
    $break = $results_count / 3;
    $rows = array();
    $offset_in_column = 0;
    foreach ($ar_searches as $search => $count) {
      if ($offset_in_column < $break) {
        $rows[$offset_in_column] = array(
          $search, $count,
          NULL, NULL,
          NULL, NULL,
        );
      }
      elseif ($offset_in_column < 2 * $break) {
        $rows[$offset_in_column - $break][2] = $search;
        $rows[$offset_in_column - $break][3] = $count;
      }
      else {
        $rows[$offset_in_column - 2 * $break][4] = $search;
        $rows[$offset_in_column - 2 * $break][5] = $count;
      }
      $offset_in_column++;
    }

    $header = array(
      t('Search'), t('#'),
      t('Search'), t('#'),
      t('Search'), t('#'),
    );
    $attributes = array();

    $ret = array();
    $ret['items'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => $attributes,
    );
    $ret['note'] = array(
      '#markup' => '<p>' . t('Searches below the pager <a href="!limit">limit</a> are not included in this list.',
        array('!limit' => url(ZGPATHSETTINGS))) . "</p>\n",
    );
    return $ret;
  }
}
