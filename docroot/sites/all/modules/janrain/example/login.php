<?php
/**
 * @file
 * Login example configuration.
 */

/**
 * Implements hook_janrain_profile_received().
 */
function janrain_example_janrain_profile_received($profile) {
  $identifiers = $profile->getIdentifiers();
  $account = user_external_load($identifiers[0]);
  if (!$account) {
    return;
  }
  $pic_url = $profile->getFirst('$.profile.photo');
  if (!$pic_url) {
    return;
  }
  $pic_dir = sprintf('%s://%s',
    file_default_scheme(),
    variable_get('user_picture_path', 'pictures'));
  if (!file_prepare_directory($pic_dir, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Unable to update avatar, upload folder preparation failed.'));
    return;
  }
  $pic_data = file_get_contents($pic_url);
  $pic_path = sprintf('%s/picture-%d-%d.jpg', $pic_dir, $account->uid, REQUEST_TIME);
  $pic_path = file_stream_wrapper_uri_normalize($pic_path);
  $pic_file = file_save_data($pic_data, $pic_path, FILE_EXISTS_REPLACE);
  file_validate_image_resolution($pic_file, variable_get('user_picture_dimensions', '1024x1024'));
  $pic_file->uid = $account->uid;
  $pic_file = file_save($pic_file);
  file_usage_add($pic_file, 'user', 'user', $account->uid);
  db_update('users')
    ->fields(array('picture' => $pic_file->fid))
    ->condition('uid', $account->uid)
    ->execute();
  $account->picture = $pic_file->fid;
}

/**
 * Implements hook_block_info_alter().
 */
function janrain_example_block_info_alter(&$blocks, $theme, $code_blocks) {
  $block = &$blocks['janrain_widgets']['social_login'];
  $block['region'] = 'content';
  $block['status'] = 1;
  $block['visibility'] = BLOCK_VISIBILITY_LISTED;
  $block['pages'] = "user/login\nuser";
  $block['weight'] = -1;
}

/**
 * Implements hook_block_view_alter().
 *
 * Hides the sign-in block for logged in users.
 */
function janrain_example_block_view_alter(&$data, $block) {
  if ('janrain_widgets' != $block->module) {
    // Skip non-janrain widgets.
    return;
  }
  if (user_is_logged_in()) {
    $data['content'] = FALSE;
  }
}

/**
 * Implements hook_block_view_MODULE_DELTA_alter() for user_login().
 */
function janrain_example_block_view_user_login_alter(&$data, $block) {
  if (!isset($data['content'])) {
    return;
  }
  $data['content'] = array('#markup' => "<a href='user/login'>Sign In / Sign Up</a>");
}
