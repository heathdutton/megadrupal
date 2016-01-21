<?php

$plugin = array(
  'title' => t('Site template'),
  'theme' => 'ns_core_site_template',
  'icon' => 'ns-core-site-template.png',
  'category' => 'NodeStream',
  'regions' => array(
    'branding' => t('Branding'),
    'branding_left' => t('Branding left'),
    'branding_right' => t('Branding right'),
    'nav' => t('Navigation'),
    'main' => t('Main'),
    'footer' => t('Footer'),
  ),
);

/**
 * Preprocess the site template layout.
 */
function ns_core_preprocess_ns_core_site_template(&$vars) {
  ns_core_check_layout_variables($vars);
}
