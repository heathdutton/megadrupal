<?php

/**
 * @file
 * PMB browse shelf template.
 */

$template .= $shelf->comment;

if (count($notices)) {
  $header = array();
  $rows = array();

  foreach ($notices as $anotice) {
    $rows[] = array(
      theme('pmb_view_notice_display', array(
        'notice' => $anotice,
        'display_type' => 'medium_line',
        'parameters' => array(),
    )));
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  // Display pager.
  $link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/shelf/') . $shelf->id . '/" . $page_number;');

  $template .= theme('pmb_pager', array(
    'current_page' => $parameters['page_number'],
    'page_count' => ceil($parameters['section_notice_count'] / $parameters['notices_per_pages']),
    'tags' => array(),
    'quantity' => 7,
    'link_generator_callback' => $link_maker_function,
  ));
}
else {
  $template .= t('This shelf has no records.');
}
