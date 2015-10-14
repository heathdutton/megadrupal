<?php

/**
 * @file
 * PMB reader reading lists template.
 */

// Display reading list.
$template .= '<h2>' . t('Your lists') . '</h2>';
$header = array(
  t('Name'),
  t('Description'),
);
$rows = array();
foreach ($reading_lists as $a_reading_list) {
  $link = l($a_reading_list->reading_list_name, _pmb_p('reader/') . $reader->uid . _pmb_p('/reading_list/') . $a_reading_list->reading_list_id);
  $rows[] = array(
    $link,
    check_plain($a_reading_list->reading_list_caption),
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No reading list.')));

// Display link to public reading list.
$template .= '<h2>' . t('Public lists') . '</h2>';
$template .= l(t('View public reading lists'), _pmb_p('reader/') . $reader->uid . _pmb_p('/reading_list_public/'));
