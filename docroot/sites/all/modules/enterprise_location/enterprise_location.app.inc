<?php
/**
 * Implements hook_apps_app_info().
 */
function enterprise_location_apps_app_info() {
  return array(
    'configure form' => 'enterprise_location_apps_app_configure_form',
    'demo content module' => 'enterprise_locations_content',
    //'status callback' => 'enterprise_location_app_status',
    'post install callback' => 'enterprise_location_app_post_install',
  );
}

function enterprise_location_app_post_install() {
	enterprise_base_attach_field_inline_image('enterprise_location');
	enterprise_base_content_apps_install_cleanup();
}

function enterprise_location_apps_app_configure_form() {
	$form = array();
  // check if enterprise_base version has app_structures_load
  if (function_exists('enterprise_base_app_structures_load')) {
    $form['blocks'] = array(
      '#type' => 'fieldset',
      '#title' => t('Blocks'),
      '#description' => t("Place the blog blocks on your site. If they aren't on the site then they won't show up. You can also set these from the blocks admin page under Structure.")
    );
    $app_structures = enterprise_base_app_structures_load('enterprise_location');
    include_once (drupal_get_path('module', 'enterprise_base') . "/enterprise_base.admin.inc");
    $e = enterprise_base_apps_structures_blocks_form($form, $form_state, $app_structures);
    $form['blocks'] = array_merge($form['blocks'], $e);
    
    $form['#validate'][] = 'enterprise_base_apps_structures_blocks_form_validate';
    $form['#submit'][] = 'enterprise_base_apps_structures_blocks_form_submit';
  }
  return $form;
}

/**
 * Give the status of blog settings.
 */
function enterprise_location_app_status() {

}