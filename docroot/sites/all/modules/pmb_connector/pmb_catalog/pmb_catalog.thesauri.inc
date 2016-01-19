<?php

/**
 * @file
 * PMB browse thesauri.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_catalog_thesauri() {
  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $thesauri = $pmb_data->get_thesauri();

  if (!$thesauri) {
    drupal_set_title(t('No thesaurus found!'));
    drupal_set_message(t('No thesaurus found!'), 'error');
    return '';
  }
  if (count($thesauri) == 1) {
    drupal_goto(_pmb_p('catalog/category/') . $thesauri[0]->thesaurus_num_root_node);
  }

  // Presents all thesaurus.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Thesaurus');
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_catalog_thesauri', array(
    'thesauri' => $thesauri,
    'parameters' => array(),
  ));
}

function pmb_catalog_category($category_id = NULL, $page = 1) {
  if ($category_id === NULL) {
    return pmb_catalog_thesauri();
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $category = $pmb_data->get_category($category_id);

  if (!$category || !$category->node->node_id) {
    drupal_set_title(t('Category not found!'));
    drupal_set_message(t('Category not found!'), 'error');
    return '';
  }

  if ($category->node->node_target_id) {
    drupal_goto(_pmb_p('catalog/category/') . $category->node->node_target_id);
  }

  $is_thesaurus = FALSE;
  $thesauri = $pmb_data->get_thesauri();
  $the_thesaurus = NULL;
  foreach ($thesauri as $athesaurus) {
    if ($athesaurus->thesaurus_id == $category->node->node_thesaurus) {
      $the_thesaurus = $athesaurus;
    }
  }
  $is_thesaurus = $the_thesaurus->thesaurus_num_root_node == $category->node->node_id;
  if ($is_thesaurus) {
    $caption = $the_thesaurus->thesaurus_caption;
    drupal_set_title(t('Thesaurus: !item', array('!item' => $caption)));
  }
  else {
    $chosen_language = 'fr_FR';
    $caption = '';
    foreach ($category->node->node_categories as $acategory) {
      if ($acategory->category_lang == $chosen_language) {
        $caption = $acategory->category_caption;
        break;
      }
    }
    if (!$caption) {
      $caption = count($category->node->node_categories) ? $category->node->node_categories[0]->category_caption : t('Unknown caption');
      if (!$caption) {
        $caption = t('Unknown caption');
      }
    }
    drupal_set_title(t('Category: !item', array('!item' => $caption)));
  }
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Thesaurus'), _pmb_p('catalog/thesaurus'));
  if ($the_thesaurus) {
    $breadcrumb[] = l($the_thesaurus->thesaurus_caption, _pmb_p('catalog/category/') . $the_thesaurus->thesaurus_num_root_node);
  }

  if (!$is_thesaurus) {
    foreach ($category->node->node_path as $key => $apath) {
      $caption = '';
      foreach ($apath->categories as $acategory) {
        if ($acategory->category_lang == $chosen_language) {
          $caption = $acategory->category_caption;
          break;
        }
      }
      if (!$caption) {
        $caption = count($apath->categories) ? $apath->categories[0]->category_caption : t('Unknown caption');
        if (!$caption) {
          $caption = t('Unknown caption');
        }
      }
      // Current category is added after, without link in order to get a more
      // compliant: last item shouldn't be a link to the current page.
      if ($apath->node_id == $category_id) {
        $current_category = $caption;
        continue;
      }
      $breadcrumb[] = l($caption, _pmb_p('catalog/category/') . $apath->node_id);
    }
    $breadcrumb[] = $current_category;
  }
  drupal_set_breadcrumb($breadcrumb);

  $notices_ids = $category->notices_ids;
  if (!$notices_ids)
    $notices_ids = array();

  $page += 0;

  $notice_count = pmb_variable_get('pmb_noticeperpage_categories');
  $notices_pages = array_chunk($notices_ids, $notice_count);
  $page = min($page, count($notices_pages));
  $page = max(1, $page);

  $notices = (count($notices_pages)) ?
    $pmb_data->get_notices($notices_pages[$page - 1]) :
    array();

  return theme('pmb_catalog_category', array(
    'category' => $category,
    'notices' => $notices,
    'parameters' => array(
      'page_number' => $page,
      'notices_per_pages' => $notice_count,
      'notice_count' => count($notices_ids),
  )));
}
