<?php
/**
 * Implements hook_apps_app_info().
 */
function enterprise_forum_apps_app_info() {
  return array(
    //'configure form' => 'enterprise_forum_apps_app_configure_form',
    //'demo content module' => 'enterprise_forum_content',
    //'status callback' => 'enterprise_forum_app_status',
    'post install callback' => 'enterprise_forum_app_post_install',
  );
}

function enterprise_forum_app_post_install() {
	enterprise_base_attach_field_inline_image('forum');
	enterprise_base_content_apps_install_cleanup();
}

function enterprise_forum_apps_app_configure_form() {
	$form = array();

  return $form;
}

/**
 * Give the status of blog settings.
 */
function enterprise_forum_app_status() {

}