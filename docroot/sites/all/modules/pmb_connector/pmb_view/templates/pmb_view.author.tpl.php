<?php

/**
 * @file
 * PMB view author template.
 */

$author_id = $author->information->author_id;
$author_author = $author->information;

$template .= '<br />';
$template .= '<div id="author_' . $author_id . '">';

// Display main informations.
$template .= '<h2>' . t('Information') . '</h2>';
$template .= '<div style="float: left;" id="author_' . $author_id . '_table">';

$header = array();
$rows = array();
$author_author_name = check_plain($author_author->author_rejete . ($author_author->author_name ? ' ' . $author_author->author_name : '') . ($author_author->author_subdivision ? ', ' . $author_author->author_subdivision  : '') . ($author_author->author_numero ? ', ' . $author_author->author_numero : ''));
if ($author_author_name) {
  $rows[] = array(
    t('Name'),
    $author_author_name,
  );
}
if (check_plain($author_author->author_date)) {
  $rows[] = array(
    t('Date'),
    $author_author->author_date,
  );
}
if (check_plain($author_author->author_web)) {
  $rows[] = array(
    t('Web'),
    $author_author->author_web,
  );
}
$author_author_location = check_plain($author_author->author_lieu . ($author_author->author_ville ? ', ' . $author_author->author_ville : '') . ($author_author->author_pays ? ' (' . $author_author->author_pays . ')' : ''));
if ($author_author_location) {
  $rows[] = array(
    t('Location'),
    $author_author_location,
  );
}
if (check_plain($author_author->author_comment)) {
  $rows[] = array(
    t('Comment'),
    $author_author->author_comment,
  );
}
// TODO Check use of author see.
// if ($author_author->author_see != 0) {
//   $rows[] = array(
//     t('See also'),
//     $author_author->author_see,
//   );
// }
$template .= theme('table', array('header' => $header, 'rows' => $rows));
$template .= '</div>';
$template .= '<br style="clear: both"/>';

// Display notices.
$template .= '<h2>' . t('Records') .'</h2>';
$template .= '<div style="float: left;" id="author_' . $author_id . '_notices">';
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
  foreach ($author->notices_ids as $anotice) {
    $anotice += 0;
    $template .= l($anotice, _pmb_p('catalog/record/') . $anotice . '/') . '<br />';
  }
}

// Display pager.
$link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/author/') . $author_id . '/" . $page_number;');
$template .= theme('pmb_pager', array(
  'current_page' => $parameters['page_number'],
  'page_count' => ceil(count($author->notices_ids) / $parameters['notices_per_pages']),
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
foreach ($author->information->author_links as $alink) {
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
