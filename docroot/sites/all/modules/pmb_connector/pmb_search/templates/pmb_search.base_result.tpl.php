<?php

/**
 * @file
 * PMB search results template.
 */

$template .= t('Search: @item', array('@item' => $search_terms));
$template .= '<br />';

if (count($notices)) {
  // Display notices.
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
  $search_terms_u = $search_terms;
  $search_fields_u = implode(',', $search_fields);
  $page_path = _pmb_p('catalog/search/local/') . $search_fields_u . '/' . $search_terms_u . '/';
  $link_maker_function = create_function('$page_number', 'return "' . addslashes($page_path) . '" . $page_number;');
  $template .= theme('pmb_pager', array(
    'current_page' => $parameters['page_number'],
    'page_count' => ceil($parameters['section_notice_count'] / $parameters['notices_per_pages']),
    'tags' => array(),
    'quantity' => 7,
    'link_generator_callback' => $link_maker_function,
  ));
}
else {
  $template .= t('This search has no results.');
}
