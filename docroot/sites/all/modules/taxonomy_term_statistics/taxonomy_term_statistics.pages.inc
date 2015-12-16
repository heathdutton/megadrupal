<?php

/**
 * @file
 * Page callbacks for the Taxonomy Term Statistics module.
 */

/**
 * Page callback: Displays statistics for a taxonomy term.
 *
 * @return
 *   A render array containing taxonomy term statistics. If information for the
 *   taxonomy term was not found, this will deliver a page not found error via
 *   drupal_not_found().
 */
function taxonomy_term_statistics_taxonomy_term_tracker() {
  if ($term = taxonomy_term_load(arg(2))) {

    $header = array(
        array('data' => t('Time'), 'field' => 'a.timestamp', 'sort' => 'desc'),
        array('data' => t('Referrer'), 'field' => 'a.url'),
        array('data' => t('User'), 'field' => 'u.name'),
        array('data' => t('Operations')));

    $query = db_select('accesslog', 'a', array('target' => 'slave'))->extend('PagerDefault')->extend('TableSort');
    $query->join('users', 'u', 'a.uid = u.uid');

    $query
      ->fields('a', array('aid', 'timestamp', 'url', 'uid'))
      ->fields('u', array('name'))
      ->condition(db_or()
        ->condition('a.path', 'taxonomy/term/' . $term->tid)
        ->condition('a.path', 'taxonomy/term/' . $term->tid . '/%', 'LIKE'))
      ->limit(30)
      ->orderByHeader($header);

    $result = $query->execute();
    $rows = array();
    foreach ($result as $log) {
      $rows[] = array(
        array('data' => format_date($log->timestamp, 'short'), 'class' => array('nowrap')),
        _statistics_link($log->url),
        theme('username', array('account' => $log)),
        l(t('details'), "admin/reports/access/$log->aid"),
      );
    }

    drupal_set_title($term->name);
    $build['statistics_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No statistics available.'),
    );
    $build['statistics_pager'] = array('#theme' => 'pager');
    return $build;
  }
  return MENU_NOT_FOUND;
}
