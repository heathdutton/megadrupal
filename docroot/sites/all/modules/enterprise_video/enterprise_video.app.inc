<?php
/**
 * Implements hook_apps_app_info().
 */
function enterprise_video_apps_app_info() {
  return array(
    //'configure form' => 'enterprise_video_apps_app_configure_form',
    'demo content module' => 'enterprise_videos_content',
    //'status callback' => 'enterprise_video_app_status',
    'post install callback' => 'enterprise_video_app_post_install',
  );
}

function enterprise_video_app_post_install() {
	enterprise_base_attach_field_inline_image('enterprise_video');
	enterprise_base_content_apps_install_cleanup();
}

function enterprise_video_apps_app_configure_form() {
	$form = array();

  return $form;
}

/**
 * Give the status of blog settings.
 */
function enterprise_video_app_status() {

}