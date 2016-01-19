<?php

/**
 * @file
 * PMB view serie.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_view_serie($serie_id = NULL, $page = 1) {
  if ($serie_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $serie = $pmb_data->get_serie($serie_id);

  if (!$serie) {
    drupal_set_title(t('Serie not found!'));
    drupal_set_message(t('Serie not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($serie->information->serie_name))
    $title = $serie->information->serie_name;
  $title = trim($title);
  drupal_set_title(t('Serie: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Serie: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_serie');
  $notices_pages = array_chunk($serie->notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  $parent_publisher = $pmb_data->get_publisher($serie->information->serie_parent);

  return theme('pmb_view_serie', array(
    'serie' => $serie,
    'parent_publisher' => $parent_publisher,
    'parameters' => array(
      'notices' => $notices,
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
  )));
}
