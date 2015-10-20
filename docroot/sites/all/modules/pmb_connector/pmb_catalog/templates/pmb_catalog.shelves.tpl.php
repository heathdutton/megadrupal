<?php

/**
 * @file
 * PMB browse shelves template.
 */

$header = array(
  t('Shelf'),
  t('Comment'),
);

$rows = array();
if (count($shelves)) {
  foreach ($shelves as $ashelf) {
    $rows[] = array(
      l($ashelf->name, _pmb_p('catalog/shelf/') . $ashelf->id),
      $ashelf->comment,
    );
  }
}

$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No shelf.')));
