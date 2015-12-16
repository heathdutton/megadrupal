<?php
/**
* Implements hook_apps_app_info().
*/
function seo_apps_app_info() {
  return array(
    'configure form' => 'seo_apps_app_configure_form',
    //'demo content module' => 'enterprise_blog_content',
  );
}

function seo_apps_app_configure_form() {
  require_once(drupal_get_path('module', 'seo') . '/seo.setup.inc');
  $form = drupal_get_form('seo_setup_0_form');

  return $form;
}