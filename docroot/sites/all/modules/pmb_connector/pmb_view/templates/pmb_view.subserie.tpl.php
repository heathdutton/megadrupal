<?php

/**
 * @file
 * PMB view sub-serie template.
 */

$subserie_id = $subserie->information->subserie_id;
$subserie_subserie = $subserie->information;

$template .= '<br />';
$template .= '<div id="subserie_' . $subserie_id . '">';

// Display main informations.
$template .= '<h2>' . t('Information') . '</h2>';
$template .= '<div style="float: left;" id="subserie_' . $subserie_id . '_table">';

$header = array();
$rows = array();
$rows[] = array(
  t('Name'),
  $subserie_subserie->subserie_name,
);
if ($parent_serie) {
  $rows[] = array(
    t('Parent serie'),
    l($parent_serie->information->serie_name, 'catalog/serie/' . $parent_serie->information->serie_id),
  );
}
if ($subserie_subserie->subserie_issn) {
  $rows[] = array(
    t('ISSN'),
    $subserie_subserie->subserie_issn,
  );
}
if ($subserie_subserie->subserie_web) {
  $rows[] = array(
    t('Web'),
    $subserie_subserie->subserie_web,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows));
$template .= '</div>';
$template .= '<br style="clear: both"/>';

// Display notices.
$template .= '<h2>' . t('Records') . '</h2>';
$template .= '<div style="float: left;" id="subserie_' . $subserie_id . '_notices">';
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
  foreach ($subserie->notices_ids as $anotice) {
    $anotice += 0;
    $template .= l($anotice, _pmb_p('catalog/record/') . $anotice . '/') .'<br />';
  }
}

// Display pager.
$link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/subserie/') . $subserie_id . '/" . $page_number;');
$template .= theme('pmb_pager', array(
  'current_page' => $parameters['page_number'],
  'page_count' => ceil(count($subserie->notices_ids) / $parameters['notices_per_pages']),
  'tags' => array(),
  'quantity' => 7,
  'link_generator_callback' => $link_maker_function,
));
$template .= '</div>';
$template .= '<br style="clear: both"/>';

// Display linked entities.
$template .= '<div>';
$template .= '<h2>' . t('Linked entities') . '</h2>';
$header = array();
$rows = array();
foreach ($subserie->information->subserie_links as $alink) {
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
