<?php

/**
 * @file
 * PMB browse location template.
 */

$header = array(
  '',
  t('Section'),
);

$rows = array();
foreach ($location->sections as $asection) {
  $rows[] = array(
    l('<img src="http://pmb_connector.olympe-network.com/' . $asection->section_image . '" />',
      _pmb_p('catalog/section/') . $asection->section_id, array('html' => TRUE)
    ),
    l($asection->section_caption, _pmb_p('catalog/section/') . $asection->section_id),
  );
}

$template .= theme('table', array('header' => $header, 'rows' => $rows));
