<?php

/**
 * @file
 * PMB view author.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_view_author($author_id = NULL, $page = 1) {
  if ($author_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $author = $pmb_data->get_author($author_id);

  if (!$author) {
    drupal_set_title(t('Author not found!'));
    drupal_set_message(t('Author not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($author->information->author_name))
    $title = $author->information->author_name;
  if (isset($author->information->author_rejete))
    $title .= ' ' . $author->information->author_rejete;
  $title = trim($title);
  drupal_set_title(t('Author: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Author: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_author');
  $notices_pages = array_chunk($author->notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_view_author', array(
    'author' => $author,
    'parameters' => array(
      'notices' => $notices,
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
  )));
}
