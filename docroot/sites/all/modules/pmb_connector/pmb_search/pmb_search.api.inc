<?php

/**
 * @file
 * PMB search functions.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');
require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.forms.inc');
require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.templates.inc');

function pmb_search_base() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Local search');
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('pmb_search_base_form');
}

function pmb_search_base_result($search_fields, $search_terms, $page = 1) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.forms.inc');

  $allowed_search_fields = array_keys(pmb_search_get_fields());
  $search_fields = rawurldecode($search_fields);
  // $search_fields = explode(',', $search_fields);
  $search_fields = explode('/', _pmb_path_source(str_replace(',', '/', $search_fields)));
  $search_fields = array_intersect($search_fields, $allowed_search_fields);

  $search_terms = rawurldecode($search_terms);

  if (!$search_terms || !$allowed_search_fields) {
    drupal_set_title(t('Invalid search!'));
    drupal_set_message(t('Invalid search!'), 'error');
    return '';
  }

  drupal_set_title(t('Search: !item', array('!item' => $search_terms)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Local search'), _pmb_p('catalog/search/local'));
  $breadcrumb[] = t('Results');
  drupal_set_breadcrumb($breadcrumb);

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $notices_ids = $pmb_data->get_search_notices_ids($search_terms, $search_fields);
  if (!$notices_ids)
    $notices_ids = array();

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_search');
  $notices_pages = array_chunk($notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_search_base_result', array(
    'search_terms' => $search_terms,
    'search_fields' => $search_fields,
    'notices' => $notices,
    'parameters' => array(
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
      'section_notice_count' => count($notices_ids),
  )));
}

function pmb_search_external() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $search_sources = $pmb_data->get_search_external_sources();

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('External search');
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('pmb_search_external_form', $search_sources);
}

function pmb_search_external_result($search_fields, $search_sources, $search_terms, $page = 1) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_search') . '/pmb_search.forms.inc');

  $allowed_search_fields = array_keys(pmb_search_get_external_fields());
  $search_fields = rawurldecode($search_fields);
  // $search_fields = explode(',', $search_fields);
  $search_fields = explode('/', _pmb_path_source(str_replace(',', '/', $search_fields)));
  $search_fields = array_intersect($search_fields, $allowed_search_fields);

  $search_sources = explode(',', $search_sources);
  // Remove non integer items.
  array_walk($search_sources, create_function('&$a', '$a += 0;'));
  $search_sources = array_unique($search_sources);

  $search_terms = rawurldecode($search_terms);

  if (!$search_terms || !$allowed_search_fields || !$search_sources) {
    drupal_set_title(t('Invalid search!'));
    drupal_set_message(t('Invalid search!'), 'error');
    return '';
  }

  drupal_set_title(t('External search: !item', array('!item' => $search_terms)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('External search'), _pmb_p('catalog/search/external'));
  $breadcrumb[] = t('Results');
  drupal_set_breadcrumb($breadcrumb);

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $notices_ids = $pmb_data->get_search_notice_external_ids($search_terms, $search_fields, $search_sources);
//  // On n'a plus un tableau de d'ids, mais un tableau d'id de sources
//  // contenant un tableau d'ids
//  foreach ($notices_ids as $source_id => $notices_ids) {
//    if (!$notices_ids)
//      $notices_ids = array();
//
//    $page += 0;
//
//    $notice_count = pmb_variable_get('pmb_noticeperpage_search');
//    $notices_pages = array_chunk($notices_ids, $notice_count);
//    $page = min($page, count($notices_pages));
//    $page = max(1, $page);
//
//    $notices = (count($notices_pages)) ?
//      $pmb_data->get_notices_external($notices_pages[$page - 1]) :
//      array();
//    $output .= theme('pmb_search_external_result', array(
//      'search_terms' => $search_terms,
//      'search_sources' => array($source_id),
//      'search_fields' => $search_fields,
//      'notices' => $notices,
//      'parameters' => array(
//        'page_number' => $page,
//        'notices_per_pages' => $notice_count,
//        'section_notice_count' => count($notices_ids),
//    )));
//
//  }
//  return $output;
  if (!$notices_ids)
    $notices_ids = array();

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_search');
  $notices_pages = array_chunk($notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices_external($notices_pages[$page - 1]) :
    array();

  return theme('pmb_search_external_result', array(
    'search_terms' => $search_terms,
    'search_sources' => $search_sources,
    'search_fields' => $search_fields,
    'notices' => $notices,
    'parameters' => array(
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
      'section_notice_count' => count($notices_ids),
  )));
}

function pmb_search_aggregate($search_terms) {
  global $user;

  $search_fields = 'all_fields';
  $search_fields = explode(',', $search_fields);
  $allowed_search_fields = array_keys(pmb_search_get_fields());
  $search_fields = array_intersect($search_fields, $allowed_search_fields);
  $result = array();
  if ($search_terms && $allowed_search_fields) {
    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), NULL);
    $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
    $breadcrumb[] = l(t('Search the catalog'), _pmb_p('catalog/search'));
    $breadcrumb[] = t('Results');
    drupal_set_breadcrumb($breadcrumb);

    $pmb_data = new pmb_data();
    $pmb_data->set_reader($user);
    $notices_ids = $pmb_data->get_search_notices_ids($search_terms, $search_fields);

    if (!$notices_ids)
      $notices_ids = array();

    $page += 0;

    $notice_count = pmb_variable_get('pmb_noticeperpage_search');
    $notices_pages = array_chunk($notices_ids, $notice_count);
    $page = min($page, count($notices_pages));
    $page = max(1, $page);

    // @todo To be checked.
//   $notices = (count($notices_pages)) ?
//     $pmb_data->get_notices($notices_pages[$page - 1]) :
//     array();
    $notices = $pmb_data->get_notices($notices_ids);

    foreach ($notices as $notice) {
      $result[] = array(
        'link' => theme('pmb_view_notice_info', array(
          'notice' => $notice,
          'info' => 'link',
        )),
        'title' => theme('pmb_view_notice_info', array(
          'notice' => $notice,
          'info' => 'title',
        )),
        'snippet' => theme('pmb_view_notice_display', array(
          'notice' => $notice,
          'display_type' => 'medium_line',
          'parameters' => array(),
        )),
      );
    }
  }
  return $result;
}

function pmb_search_advanced() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $fields = $pmb_data->get_search_advanced_fields(TRUE);

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Advanced search'), _pmb_p('catalog/search/advanced'));
  $breadcrumb[] = t('Results');
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('pmb_search_advanced_form');
}

function pmb_search_advanced_result() {
}
