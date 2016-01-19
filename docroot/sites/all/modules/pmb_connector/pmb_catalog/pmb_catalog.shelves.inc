<?php

/**
 * @file
 * PMB browse shelves.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_catalog_shelves() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $shelves = $pmb_data->get_shelves();

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Shelves');
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_catalog_shelves', array(
    'shelves' => $shelves,
    'parameters' => array(),
  ));
}

function pmb_catalog_shelf($shelf_id = NULL, $page = 1) {
  if ($shelf_id === NULL) {
    return pmb_catalog_shelves();
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $shelves = $pmb_data->get_shelves();

  $shelf = array();
  foreach ($shelves as $ashelf) {
    if ($ashelf->id == $shelf_id) {
      $shelf = $ashelf;
      break;
    }
  }

  if (!$shelf) {
    drupal_set_title(t('Shelf not found!'));
    drupal_set_message(t('Shelf not found!'), 'error');
    return '';
  }

  drupal_set_title(t('Shelf: !item', array('!item' => $shelf->name)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Shelves'), _pmb_p('catalog/shelf'));
  $breadcrumb[] = check_plain($shelf->name);
  drupal_set_breadcrumb($breadcrumb);

  $notices_ids = $pmb_data->get_shelf_notices_ids($shelf_id);
  if (!$notices_ids)
    $notices_ids = array();

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_shelf');
  $notices_pages = array_chunk($notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_catalog_shelf', array(
    'shelf' => $shelf,
    'notices' => $notices,
    'parameters' => array(
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
      'section_notice_count' => count($notices_ids),
  )));
}