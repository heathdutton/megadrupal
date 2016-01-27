<?php

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_user_ckeditor_link_user_autocomplete($string) {
  $matches = array();

  $result = db_select('users')
    ->fields('users', array('uid', 'name'))
    ->condition('name', '%'. db_like($string) .'%', 'LIKE')
    ->range(0, 10)
    // ->addTag('user_access'); // Make a proper access hook
    ->execute();
    
  foreach ($result as $user) {
    $matches['user/'. $user->uid] = t('User: @name', array('@name' => $user->name));
  }

  return $matches;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_url().
 */
function ckeditor_link_user_ckeditor_link_user_url($path, $langcode) {
  if (!preg_match('`^user/(\d+)$`', $path, $matches)) {
    return;
  }
  
  $uid = $matches[1];

  return ckeditor_link_url("user/$uid", $langcode);
}