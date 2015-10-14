<?php

/**
 * @file
 * PMB browse thesauri template.
 */

$header = array(
  t('Thesaurus'),
);

$rows = array();
if (count($thesauri)) {
  foreach ($thesauri as $thesaurus) {
    $rows[] = array(
      l($thesaurus->thesaurus_caption, _pmb_p('catalog/category/') . $thesaurus->thesaurus_num_root_node),
    );
  }
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No thesaurus.')));
