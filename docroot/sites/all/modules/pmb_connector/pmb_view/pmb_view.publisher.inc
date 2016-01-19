<?php

/**
 * @file
 * PMB view publisher.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_view_publisher($publisher_id = NULL, $page = 1) {
  if ($publisher_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $publisher = $pmb_data->get_publisher($publisher_id);

  if (!$publisher) {
    drupal_set_title(t('Publisher not found!'));
    drupal_set_message(t('Publisher not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($publisher->information->publisher_name))
    $title = $publisher->information->publisher_name;
  $title = trim($title);
  drupal_set_title(t('Publisher: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Publisher: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_publisher');
  $notices_pages = array_chunk($publisher->notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_view_publisher', array(
    'publisher' => $publisher,
    'parameters' => array(
      'notices' => $notices,
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
  )));
}
