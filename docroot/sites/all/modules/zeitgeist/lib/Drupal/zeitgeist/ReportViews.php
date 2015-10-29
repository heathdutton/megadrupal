<?php

/**
 * @file
 * Report page for the Zeitgeist module, Views 3 version.
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
 * The Views-based report class.
 */
class ReportViews extends Report {
  /**
   * Page callback: sample Views-based report.
   *
   * @see zeitgeist_preprocess_views_view()
   *
   * @return string
   *   HTML
   */
  public function page() {
    $ret = views_embed_view('zeitgeist');
    return $ret;
  }
}
