<?php

/**
 * @file
 * PMB reader reading list form template.
 */

$reading_list = $form['reading_list']['#value'];

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
foreach ($form['notices_ids']['#value'] as $anotice_id => $anotice) {
  $rows[] = array(
    array(
      'data' => drupal_render($form['checked_notices'][$anotice_id]),
      'class' => array('checkbox'),
    ),
    theme('pmb_view_notice_display', array(
      'notice' => $anotice,
      'display_type' => 'medium_line',
      'parameters' => array(),
  )));
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('This list is empty.')));

$template .= drupal_render_children($form);

$template .= drupal_render(drupal_get_form('pmb_reader_add_cart_to_reading_list_form', $reading_list->reading_list_id));
