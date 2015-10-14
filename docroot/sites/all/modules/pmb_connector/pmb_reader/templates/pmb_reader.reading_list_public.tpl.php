<?php

/**
 * @file
 * PMB reader public reading list template.
 */

// Display reading list.
$header = array(
  t('Name'),
  t('Description'),
  t('Author'),
);
$rows = array();
$rows[] = array(
  check_plain($reading_list->reading_list_name),
  check_plain($reading_list->reading_list_caption),
  check_plain($reading_list->reading_list_empr_caption),
);
$template .= theme('table', array('header' => $header, 'rows' => $rows));

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
