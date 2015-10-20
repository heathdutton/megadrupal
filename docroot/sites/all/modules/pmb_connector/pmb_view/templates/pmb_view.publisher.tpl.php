<?php

/**
 * @file
 * PMB view publisher template.
 */

$publisher_id = $publisher->information->publisher_id;
$publisher_publisher = $publisher->information;

$template .= '<br />';
$template .= '<div id="publisher_' . $publisher_id . '">';

// Display main informations.
$template .= '<h2>' . t('Information') . '</h2>';
$template .= '<div style="float: left;" id="publisher_' . $publisher_id . '_table">';

$header = array();
$rows = array();
if ($publisher_publisher->publisher_name) {
  $rows[] = array(
    t('Name'),
    $publisher_publisher->publisher_name,
  );
}
if ($publisher_publisher->publisher_address1) {
  $rows[] = array(
    t('Address'),
    $publisher_publisher->publisher_address1,
  );
}
if ($publisher_publisher->publisher_address2) {
  $rows[] = array(
    t('Address'),
    $publisher_publisher->publisher_address2,
  );
}
if ($publisher_publisher->publisher_zipcode) {
  $rows[] = array(
    t('Zipcode'),
    $publisher_publisher->publisher_zipcode,
  );
}
if ($publisher_publisher->publisher_city) {
  $rows[] = array(
    t('City'),
    $publisher_publisher->publisher_city,
  );
}
if ($publisher_publisher->publisher_country) {
  $rows[] = array(
    t('Country'),
    $publisher_publisher->publisher_country,
  );
}
if ($publisher_publisher->publisher_web) {
  $rows[] = array(
    t('Web'),
    $publisher_publisher->publisher_web,
  );
}
if ($publisher_publisher->publisher_comment) {
  $rows[] = array(
    t('Comment'),
    $publisher_publisher->publisher_comment,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows));
$template .= '</div>';
$template .= '<br style="clear: both"/>';

// Display notices
$template .= '<h2>' . t('Records') . '</h2>';
$template .= '<div style="float: left;" id="publisher_' . $publisher_id . '_notices">';
if (isset($parameters['notices'])) {
  $header = array();
  $rows = array();
  foreach ($parameters['notices'] as $anotice) {
    $rows[] = array(
      theme('pmb_view_notice_display', array(
        'notice' => $anotice,
        'display_type' => 'medium_line',
        'parameters' => array(),
    )));
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));
}
else {
  foreach ($publisher->notices_ids as $anotice) {
    $anotice += 0;
    $template .= l($anotice, _pmb_p('catalog/record/') . $anotice . '/') . '<br />';
  }
}

// Display pager.
$link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/publisher/') . $publisher_id . '/" . $page_number;');
$template .= theme('pmb_pager', array(
  'current_page' => $parameters['page_number'],
  'page_count' => ceil(count($publisher->notices_ids) / $parameters['notices_per_pages']),
  'tags' => array(),
  'quantity' => 7,
  'link_generator_callback' => $link_maker_function
));
$template .= '</div>';
$template .= '<br style="clear: both"/>';

// Display linked entities.
$template .= '<div>';
$template .= '<h2>' . t('Linked entities') . '</h2>';
$header = array();
$rows = array();
foreach ($publisher->information->publisher_links as $alink) {
  $link = '';
  switch ($alink->autlink_to) {
    case 1:
      $link = _pmb_p('catalog/author/') . $alink->autlink_to_id;
      break;
    case 2:
      $link = _pmb_p('catalog/category/') . $alink->autlink_to_id;
      break;
    case 3:
      $link = _pmb_p('catalog/publisher/') . $alink->autlink_to_id;
      break;
    case 4:
      $link = _pmb_p('catalog/serie/') . $alink->autlink_to_id;
      break;
    case 5:
      $link = _pmb_p('catalog/subserie/') . $alink->autlink_to_id;
      break;
    default:
      break;
  }
  $rows[] = array(l($alink->autlink_to_libelle, $link, array('html' => TRUE)));
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No linked entity.')));
$template .= '</div>';

$template .= '</div>';
