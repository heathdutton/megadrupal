<?php
if (theme_get_setting('futurecss_enable')) {
  $pie_path = $abs_arctica_theme_path . '/features/arctica-cssfuture/PIE/';
  drupal_add_js(array('cssfuture' => array('piePath' => $pie_path)), 'setting');
}