<?php

/**
 * @file
 * PMB reader reading list template.
 */

// Display reading list.
$header = array(
  t('Name'),
  t('Description'),
  t('Public'),
  t('Subscribers only'),
  t('Read only'),
);
$rows = array();
$rows[] = array(
  check_plain($reading_list->reading_list_name),
  check_plain($reading_list->reading_list_caption),
  $reading_list->reading_list_public ? t('Yes') : t('No'),
  $reading_list->reading_list_confidential ? t('Yes') : t('No'),
  $reading_list->reading_list_readonly ? t('Yes') : t('No'),
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

$template .= drupal_render(drupal_get_form('pmb_reader_add_cart_to_reading_list_form', $reading_list->reading_list_id));
