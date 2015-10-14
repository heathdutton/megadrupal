<?php

/**
 * @file
 * Ajax functions for catalog.
 */

function pmb_catalog_block_shelf_get_page($shelf_id, $page = 1) {
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

  $output = '';

  if (isset($shelf)) {
    $notices_ids = $pmb_data->get_shelf_notices_ids($shelf_id);
    if (!$notices_ids)
      $notices_ids = array();

    $page += 0;

    $notice_count = pmb_variable_get('pmb_noticeperpage_block_shelf');
    $notices_pages = array_chunk($notices_ids, $notice_count);
    $page = min($page, count($notices_pages));
    $page = max(1, $page);

    $notices = (count($notices_pages)) ?
      $pmb_data->get_notices($notices_pages[$page - 1]) :
      array();

    if (isset($notices) && count($notices)) {
      $header = array();
      $rows = array();

      foreach ($notices as $anotice) {
        $rows[] = array(
          theme('pmb_view_notice_display', array(
            'notice' => $anotice,
            'display_type' => 'main_title_author',
            'parameters' => array(),
        )));
      }

      $link_maker_function = create_function('$page_number', 'return "' . addslashes(_pmb_p('catalog/ajax/block/shelf/') . $shelf_id . '/') . '" . $page_number;');

      $output .= theme('pmb_block_pager', array(
        'current_page' => $page,
        'page_count' => count($notices_pages),
        'tags' => array(),
        'id' => 'block_shelf_' . $shelf_id,
        'link_generator_callback' => $link_maker_function,
      ));

      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
    else {
      $output .= t('This shelf has no records.');
    }
  }
  else {
    $output = t('Shelf not found!');
  }

  print $output;
  exit();
}
