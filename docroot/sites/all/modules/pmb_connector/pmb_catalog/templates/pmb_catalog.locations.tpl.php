<?php

/**
 * @file
 * PMB browse locations template.
 */

$header = array(
  t('Location'),
  t('Number of sections'),
);

$rows = array();

if (count($locations_and_sections)) {
  foreach ($locations_and_sections as $alocation) {
    $alocation->location->location_id += 0;
    $rows[] = array(
      l($alocation->location->location_caption, _pmb_p('catalog/location/') . $alocation->location->location_id),
      count($alocation->sections),
    );
  }
}

$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No location.')));
