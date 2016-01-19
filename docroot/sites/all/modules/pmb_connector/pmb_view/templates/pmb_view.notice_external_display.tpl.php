<?php

/**
 * @file
 * PMB display external notice template.
 */

$notice_id = $notice['id'];
$notice_fields = &$notice['fields'];

// Set link according to record type.
$link = _pmb_p('catalog/external_record/') . $notice_id . '/';

// Get basic values from the notice.
if ($display_type != 'main_title_author') {
  $serial = '';
  if (isset($notice_fields['serial'])) {
    $item = array();
    foreach ($notice_fields['serial'] as $serial) {
      $item[] = $serial['full'];
    }
    $serial = l(implode(', ', $item), $link, array('html' => TRUE));
  }
  $title = '';
  if ($notice_fields['title']) {
    $item = array();
    foreach ($notice_fields['title'] as $title) {
      $item[] = $title['full'];
    }
    $title = l(implode(', ', $item), $link, array('html' => TRUE));
  }
  $author = '';
  if ($notice_fields['author']) {
    $item = array();
    foreach ($notice_fields['author'] as $author) {
      $item[] = $author['full_name'];
    }
    $author = implode(', ', $item);
  }
  $publisher = '';
  if ($notice_fields['publisher']) {
    $item = array();
    foreach ($notice_fields['publisher'] as $publisher) {
      $item[] = $publisher['full'];
    }
    $publisher = implode(', ', $item);
  }
}

// Prepare template according to display type.
switch ($display_type) {
  case 'medium_line':
    if (isset($notice_fields['cover_url'])) {
      $template .= '<div id="notice_' . $notice_id . '_cover" style="display:inline-block;">';
      $template .= '<img src="' . $notice_fields['cover_url'] . '" style="max-width: 80px;">';
      $template .= '</div>';
    }

    $template .= '<div id="notice_' . $notice_id . '_info" style="display:inline-block;margin-left:5px;vertical-align:top;">';

    if ($serial) {
      $template .= t('Serial: !item', array('!item' => $serial));
      $template .= '<br />';
    }
    if ($notice_fields['source']) {
      $template .= t('Source: !item', array('!item' => $notice_fields['source']));
      $template .= '<br />';
    }
    if ($title) {
      $template .= t('Title: !item', array('!item' => $title));
      $template .= '<br />';
    }
    if ($title) {
      $template .= t('Authors: !item', array('!item' => $author));
      $template .= '<br />';
    }
    if ($publisher) {
      $template .= t('Publishers: !item', array('!item' => $publisher));
      $template .= '<br />';
    }
    $template .= t('Date: !item', array('!item' => $notice_fields['main_date']));

    $template .= '</div>';
    break;
}
