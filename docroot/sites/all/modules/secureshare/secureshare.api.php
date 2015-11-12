<?php

/**
 * @file
 * Hooks provided by the Secureshare module
 */

/**
 * @addtogroup hooks
 * @{
 */


/**
 * Alter the profile configs before used.
 *
 * This hook is useful for overriding the final config settings from profile settings after it
 * has been loaded.
 *
 * @param (array) $config
 *   An associative config array.
 * @param (string) $info
 *   An string with the name of the profile
 */
function hook_secureshare_config_alter(&$config, $profile) {
  $config = array(
      'css_path' => '',
      'info_link' => '',
      'txt_help' => '',
      'settings_perma' => '',
      'cookie_expires' => '365',
      'services' => array(
        'facebook' => array(
          'status' => 0,
          'dummy_img' => '',
          'txt_info' => '',
          'txt_fb_off' => '',
          'txt_fb_on' => '',
          'perma_option' => 0,
          'display_name' => '',
          'language' => 'system',
          'action' => 'like',
        ),
        'twitter' => array(
          'status' => 0,
          'dummy_img' => '',
          'txt_info' => '',
          'txt_twitter_off' => '',
          'txt_twitter_on' => '',
          'perma_option' => 0,
          'display_name' => '',
          'tweet_text' => 'default',
          'language' => 'system',
        ),
        'gplus' => array(
          'status' => 0,
          'dummy_img' => '',
          'txt_info' => '',
          'txt_gplus_off' => '',
          'txt_gplus_on' => '',
          'perma_option' => 0,
          'display_name' => '',
          'language' => 'user',
        ),
      ),
    );
}

/**
 * @} End of "addtogroup hooks".
 */
