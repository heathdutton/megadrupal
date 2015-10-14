<?php

/**
 * @file
 * PMB browse serials template.
 */

$categories = array();
foreach ($serials as $aserial) {
  if (!$aserial->serial_title) {
    $categories['?'][] = $aserial;
    continue;
  }
  $char = ord(drupal_strtoupper(drupal_substr($aserial->serial_title, 0, 1)));
  if ($char >= ord('0') && $char <= ord('9'))
    $categories['#'][] = $aserial;
  elseif ($char >= ord('A') && $char <= ord('Z'))
    $categories[chr($char)][] = $aserial;
  else
    $categories['?'][] = $aserial;
}

$links = '';
if (!empty($categories['?']))
  $links .= '<a href="#p' . ord('?') . '">?</a>&nbsp;';
else
  $links .= '?&nbsp;';
if (!empty($categories['#']))
  $links .= '<a href="#p' . ord('#') . '">#</a>&nbsp;';
else
  $links .= '#&nbsp;';
for($i = ord('A'); $i <= ord('Z'); $i++) {
  if (!empty($categories[chr($i)]))
    $links .= '<a href="#p' . ($i) . '">' . check_plain(chr($i)) . '</a>&nbsp;';
  else
    $links .= check_plain(chr($i)) . '&nbsp;';
}

$template .= $links;

$header = array(
  t('Title'),
  t('Number of issues'),
  t('Number of items'),
  t('Number of analysis'),
);
$rows = array();
foreach ($categories as $caption => $category) {
  $count = 0;
  foreach ($category as $aserial) {
    $count++;
    $anchor = $count ? '<a name="p' . ord($caption) . '"/>' : '';
    $rows[] = array($anchor . l($aserial->serial_title, _pmb_p('catalog/serial/') . $aserial->serial_id), $aserial->serial_issues_count, $aserial->serial_items_count, $aserial->serial_analysis_count);
  }
}
$template .= theme('table', array('header' => $header, 'rows' => $rows));
