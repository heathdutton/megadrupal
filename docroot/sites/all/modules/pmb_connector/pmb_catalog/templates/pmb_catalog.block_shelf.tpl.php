<?php

/**
 * @file
 * PMB shelf block template.
 */

$template .= l($shelf->comment, _pmb_p('catalog/shelf/') . $shelf->id);

if (count($notices)) {
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

  $link_maker_function = create_function('$page_number', 'return "' . addslashes(_pmb_p('catalog/ajax/block/shelf/') . $shelf->id . '/') . '" . $page_number;');

  // Start of div for ajax update.
  $template .= '<div id="block_shelf_' . $shelf->id . '-page">';

  // Display first page by default.
  $template .= theme('pmb_block_pager', array(
    'current_page' => 1,
    'page_count' => $parameters['page_count'],
    'tags' => array(),
    'id' => 'block_shelf_' . $shelf->id,
    'link_generator_callback' => $link_maker_function,
  ));

  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  // End of div for ajax update.
  $template .= '</div>';
}
else {
  $template .= '<br />' . t('This shelf has no records.');
}
