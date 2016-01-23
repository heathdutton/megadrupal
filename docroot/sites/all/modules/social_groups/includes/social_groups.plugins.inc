<?php

/**
 * @file
 *  CTools plugins declarations.
 */

/**
 * Break out for social_groups_social_groups_plugins().
 */
function _social_groups_social_groups_plugins() {
  $path = drupal_get_path('module', 'social_groups') . '/includes/plugins';
  $info = array();
  
  $info['SocialGroupPluginFacebook'] = array(
    'handler' => array(
      'class' => 'SocialGroupPluginFacebook',
    ),
    'group' => array(
      'name' => 'facebook',
      'title' => t('Facebook Group'),
      'description' => t("Facebook group id or name. Leave blank if you don't want to use this group."),
      'url' => 'https://graph.facebook.com/',
      'group_url' => 'https://facebook.com/',
    ),
  );
  
  $info['SocialGroupPluginTwitter'] = array(
    'handler' => array(
      'class' => 'SocialGroupPluginTwitter',
    ),
    'group' => array(
      'name' => 'twitter',
      'title' => t('Twitter Group'),
      'description' => t("Twitter name. Leave blank if you don't want to use this group."),
      'group_url' => 'https://twitter.com/',
    ),
  );
  
  $info['SocialGroupPluginYoutube'] = array(
    'handler' => array(
      'class' => 'SocialGroupPluginYoutube',
    ),
    'group' => array(
      'name' => 'youtube',
      'title' => t('Youtube Group'),
      'description' => t("Youtube name. Leave blank if you don't want to use this group."),
      'url' => 'http://gdata.youtube.com/feeds/api/users/',
      'group_url' => 'http://youtube.com/',
    ),
  );
  
  return $info;
}