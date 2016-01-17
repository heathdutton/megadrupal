<?php

/**
 * @file
 * Get Search Interests From Keywords.
 */

/**
 * Implements gets actual grassroot results.
 */
function grassroot_interests_search_interest_process($keyword, $disptext, $dispno) {
  $result = db_select('grassroot_interests_path_keyword', 'sipk');
  $result->fields('sipk', array('kw_title', 'root_url'));
  $result->distinct();
  $result->condition('sipk.keyword', $keyword);
  if ($dispno != 0) {
    $result->range(0, $dispno);
  }
  $output = $result->execute()->fetchAll();
  $attributes = array(
    'id' => 'my-interests-listing',
    'class' => 'list-messages',
  );
  // A string or indexed (string) array with the classes for the list tag.
  $count = 0;
  $data = array();
  foreach ($output as $key => $value) {
    $interests = t($disptext) . ' ' . l($value->kw_title, $value->root_url, array('attributes' => array('class' => array('specialty-channel-link')))) . '.';
    // Data ($data) here should be a array.
    $data[] = array(
      'data' => $interests,
      'id' => drupal_html_id($value->kw_title),
      'class' => array(drupal_html_class($value->kw_title), 'message-item'),
    );
    $count++;
  }
  $results = '';
  $results .= '<div class="message-specialty" id="search-results-visit-specialty">';
  $results .= theme('item_list', array(
    'items' => $data,
    'type' => 'ul',
    'attributes' => $attributes,
    )
  );
  $results .= '</div>';
  return $results;
}

/**
 * Implements gets actual grassroot list keywords.
 */
function grassroot_interests_search_interest_list() {
  $header = array(
    array(
      'data' => t('Title'),
      'field' => 'kw_title',
      'sort' => 'asc',
    ),
    t('Action'),
  );
  $query = db_select('grassroot_interests_path_keyword', 'sipk')
  ->fields('sipk', array('kw_title', 'url_id'))
  ->distinct()
  ->extend('PagerDefault')
  ->limit(9)
  ->execute();
  $i = 0;
  $rows = array();
  while ($record = $query->fetchAssoc()) {
    $rows[$i]['kw_title'] = $record['kw_title'];

    // Build a list of all the accessible operations/actions for the current keyword.
    $actions = array();
    if (user_access('edit search keywords')) {
      $actions['edit'] = array(
        'title' => t('Edit'),
        'href' => 'admin/grassroot-interests/add-keywords/' . $record['url_id'] . '/edit',
      );
    }

    if (user_access('delete search keywords')) {
      $actions['delete'] = array(
        'title' => t('Delete'),
        'href' => 'admin/grassroot-interests/keywords/' . $record['url_id'] . '/' . urlencode($record['kw_title']),
      );
    }

    $rows[$i]['actions'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $actions,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );

    $i++;
  }
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => array('grassroot')),
    )
  );
  $output .= theme('pager', $tags = array(), $element = 0, $parameters = array(), $quantity = 9);
  return $output;
}
