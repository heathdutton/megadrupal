<?php

/**
 * @file
 * PMB view sub-serie.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_view_subserie($subserie_id = NULL, $page = 1) {
  if ($subserie_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $subserie = $pmb_data->get_subserie($subserie_id);

  if (!$subserie) {
    drupal_set_title(t('Sub-serie not found!'));
    drupal_set_message(t('Sub-serie not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($subserie->information->subserie_name))
    $title = $subserie->information->subserie_name;
  $title = trim($title);
  drupal_set_title(t('Sub-serie: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Sub-serie: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_subserie');
  $notices_pages = array_chunk($subserie->notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  $parent_serie = $pmb_data->get_serie($subserie->information->subserie_parent);

  return theme('pmb_view_subserie', array(
    'subserie' => $subserie,
    'parent_serie' => $parent_serie,
    'parameters' => array(
      'notices' => $notices,
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
  )));
}
