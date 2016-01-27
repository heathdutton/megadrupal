<?php
/**
 * @file
 * Imgly Statistics admin pages.
 */

/**
 * Form callback.
 *
 * Returns Imgly Statistics settings form.
 *
 * @see imgly_stats_menu().
 */
function imgly_stats_settings_form() {
  $form = array();

  $form['imgly_stats_download'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track image downloads'),
    '#description' => t('Check this to track the number of times the full image has been downloaded and display the counter in the image template.'),
    '#default_value' => variable_get('imgly_stats_download', 1),
  );

  $form['imgly_stats_anonymous_token'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start session for anonymous users'),
    '#description' => t("We use tokens to validate our AJAX requests. However, tokens don't work for anonymous users because there is no session in place. By default, we will skip token validation for anonymous users to overcome this. However, for security or other reasons, you can opt to force a session to be created and validate tokens for anonymous users."),
    '#default_value' => variable_get('imgly_stats_anonymous_token', 0),
  );

  return system_settings_form($form);
}

/**
 * Page callback.
 *
 * Generates Imgly Statistics report.
 *
 * @see imgly_stats_menu().
 */
function imgly_stats_report() {
  $header = array(
    array('data' => t('Title'), 'field' => 'n.title'),
    array('data' => t('Created'), 'field' => 'n.created'),
    array('data' => t('Author'), 'field' => 'u.name'),
    array('data' => t('Views'), 'field' => 'views', 'sort' => 'desc'),
    array('data' => t('Downloads'), 'field' => 'downloads'),
  );

  $query = db_select('imgly_stats', 'is1')->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('is1', array('nid'))
    ->fields('n', array('title', 'uid', 'created'))
    ->fields('u', array('name'))
    ->condition('is1.action', 0, '=')
    ->groupBy('is1.nid')
    ->limit(30)
    ->orderByHeader($header);
  $query->join('node', 'n', 'n.nid = is1.nid');
  $query->join('users', 'u', 'u.uid = n.uid');
  $query->addExpression('COUNT(is1.action)', 'views');
  // This is done like this instead of using a "proper" subquery because of the second WHERE condition.
  $query->addExpression('(SELECT COUNT(is2.action) FROM imgly_stats is2 WHERE is2.action = 1 AND is2.nid = is1.nid)', 'downloads');
  $result = $query->execute();

  $rows = array();
  foreach ($result as $node) {
    $rows[] = array(
      l($node->title, 'node/' . $node->nid),
      array('data' => format_date($node->created, 'short'), 'class' => array('nowrap')),
      l($node->name, 'user/' . $node->uid),
      $node->views,
      $node->downloads,
    );
  }

  $build['imgly_stats_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No statistics available.'),
  );
  $build['imgly_stats_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback.
 *
 * Generates Imgly Statistics accesslog report.
 *
 * @see imgly_stats_menu().
 */
function imgly_stats_accesslog() {
  $header = array(
    array('data' => t('Title'), 'field' => 'n.title'),
    array('data' => t('Created'), 'field' => 'n.created'),
    array('data' => t('Author'), 'field' => 'u.name'),
    array('data' => t('Action'), 'field' => 'action'),
    array('data' => t('Timestamp'), 'field' => 'timestamp', 'sort' => 'desc'),
    array('data' => t('User'), 'field' => 'uid'),
    array('data' => t('IP Address'), 'field' => 'ip'),
  );

  $query = db_select('imgly_stats', 'is1')->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('is1', array('nid', 'action', 'timestamp', 'ip', 'uid'))
    ->fields('n', array('title', 'uid', 'created'))
    ->fields('u1', array('name'))
    ->fields('u2', array('name'))
    ->limit(30)
    ->orderByHeader($header);
  $query->join('node', 'n', 'n.nid = is1.nid');
  $query->join('users', 'u1', 'u1.uid = n.uid');
  $query->join('users', 'u2', 'u2.uid = is1.uid');
  $result = $query->execute();

  $rows = array();
  foreach ($result as $row) {
    $rows[] = array(
      l($row->title, 'node/' . $row->nid),
      array('data' => format_date($row->created, 'short'), 'class' => array('nowrap')),
      l($row->name, 'user/' . $row->n_uid),
      $row->action == IMGLY_VIEW ? t('View') : t('Download'),
      array('data' => format_date($row->timestamp, 'short'), 'class' => array('nowrap')),
      $row->uid > 0 ? l($row->u2_name, 'user/' . $row->uid) : t('Anonymous User'),
      $row->ip,
    );
  }

  $build['imgly_stats_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No information available.'),
  );
  $build['imgly_stats_pager'] = array('#theme' => 'pager');
  return $build;
}