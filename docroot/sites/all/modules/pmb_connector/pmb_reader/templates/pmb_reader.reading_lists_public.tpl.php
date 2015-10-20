<?php

/**
 * @file
 * PMB reader public reading lists template.
 */

// Display public reading list.
$header = array(
  t('Name'),
  t('Description'),
  t('Author'),
  t('Access'),
);
$rows = array();
foreach ($reading_lists as $a_reading_list) {
  $link = l($a_reading_list->reading_list_name, _pmb_p('reader/') . $reader->uid . _pmb_p('/reading_list_public/') . $a_reading_list->reading_list_id);
  $access = t('Open');
  if ($a_reading_list->reading_list_confidential && $a_reading_list->reading_list_public) {
    $access = t('Confidential');
  }
  $rows[] = array(
    $link,
    check_plain($a_reading_list->reading_list_caption),
    check_plain($a_reading_list->reading_list_empr_caption),
    $access,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No public reading list.')));
