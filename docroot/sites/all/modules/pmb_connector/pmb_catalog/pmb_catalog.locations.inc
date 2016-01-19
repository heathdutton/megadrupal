<?php

/**
 * @file
 * PMB browse locations and sections.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_catalog_locations() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $locations_and_sections = $pmb_data->get_locations_and_sections();

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Locations');
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_catalog_locations', array(
    'locations_and_sections' => $locations_and_sections,
    'parameters' => array(),
  ));
}

function pmb_catalog_location($location_id = NULL) {
  if ($location_id === NULL) {
    return pmb_catalog_locations();
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $locations_and_sections = $pmb_data->get_locations_and_sections();

  if (!isset($locations_and_sections[$location_id])) {
    drupal_set_title(t('Location not found!'));
    drupal_set_message(t('Location not found!'), 'error');
    return '';
  }
  $location = $locations_and_sections[$location_id];

  drupal_set_title(t('Location: !item', array('!item' => $location->location->location_caption)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Locations'), _pmb_p('catalog/location'));
  $breadcrumb[] = check_plain($location->location->location_caption);
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_catalog_location', array(
    'location' => $location,
    'parameters' => array(),
  ));
}

function pmb_catalog_section($section_id = NULL, $page = 1) {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $locations_and_sections = $pmb_data->get_locations_and_sections();

  $section = array();
  $location = array();
  foreach ($locations_and_sections as $alocation) {
    foreach ($alocation->sections as $asection) {
      if ($asection->section_id == $section_id) {
        $section = $asection;
        break;
      }
    }
    if ($section) {
      $location = $alocation;
      break;
    }
  }

  if (!$section) {
    drupal_set_title(t('Section not found!'));
    drupal_set_message(t('Section not found!'), 'error');
    return '';
  }
  if (!$location) {
    drupal_set_title(t('Location not found!'));
    drupal_set_message(t('Location not found!'), 'error');
    return '';
  }

  drupal_set_title(t('Section: !item', array('!item' => $section->section_caption)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Locations'), _pmb_p('catalog/location'));
// $breadcrumb[] = l($location->location->location_caption, _pmb_p('catalog/location/') . $location->location->location_id);
  $breadcrumb[] = t('Section: @item', array('@item' => $section->section_caption));
  drupal_set_breadcrumb($breadcrumb);

  $notices_ids = $pmb_data->get_section_notices_ids($section_id);
  if (!$notices_ids)
    $notices_ids = array();

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_section');
  $notices_pages = array_chunk($notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_catalog_section', array(
    'location' => $location,
    'section' => $section,
    'notices' => $notices,
    'parameters' => array(
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
      'section_notice_count' => count($notices_ids),
  )));
}
