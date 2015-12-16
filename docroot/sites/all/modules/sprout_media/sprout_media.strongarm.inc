<?php
/**
 * @file
 * sprout_media.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function sprout_media_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imce_custom_content';
  $strongarm->value = array(
    'imce_file_path_content' => 1,
    'imce_mkdir_content' => 1,
    'imce_search_content' => 1,
  );
  $export['imce_custom_content'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imce_custom_process';
  $strongarm->value = array(
    'imce_mkdir_process_profile' => 1,
  );
  $export['imce_custom_process'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imce_profiles';
  $strongarm->value = array(
    1 => array(
      'name' => 'User-1',
      'usertab' => 1,
      'filesize' => '0',
      'quota' => '0',
      'tuquota' => '0',
      'extensions' => '*',
      'dimensions' => '0',
      'filenum' => '0',
      'directories' => array(
        0 => array(
          'name' => '.',
          'subnav' => 1,
          'browse' => 1,
          'upload' => 1,
          'thumb' => 1,
          'delete' => 1,
          'resize' => 1,
          'mkdir' => 1,
          'rmdir' => 1,
        ),
      ),
      'thumbnails' => array(
        0 => array(
          'name' => 'Small',
          'dimensions' => '90x90',
          'prefix' => 'small_',
          'suffix' => '',
        ),
        1 => array(
          'name' => 'Medium',
          'dimensions' => '120x120',
          'prefix' => 'medium_',
          'suffix' => '',
        ),
        2 => array(
          'name' => 'Large',
          'dimensions' => '180x180',
          'prefix' => 'large_',
          'suffix' => '',
        ),
      ),
      'mkdirnum' => '0',
    ),
    2 => array(
      'name' => 'WYSIWYG',
      'usertab' => 1,
      'filesize' => 0,
      'quota' => 0,
      'tuquota' => 0,
      'extensions' => 'gif png jpg jpeg pdf doc txt',
      'dimensions' => '1200x1200',
      'filenum' => 1,
      'mkdirnum' => 0,
      'directories' => array(
        0 => array(
          'name' => 'wyswiyg_uploads',
          'subnav' => 1,
          'browse' => 1,
          'upload' => 1,
          'thumb' => 0,
          'delete' => 1,
          'resize' => 0,
          'mkdir' => 1,
          'rmdir' => 1,
        ),
      ),
      'thumbnails' => array(),
    ),
  );
  $export['imce_profiles'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imce_roles_profiles';
  $strongarm->value = array(
    5 => array(
      'weight' => 0,
      'youtube_pid' => 1,
      'http_pid' => 1,
      'https_pid' => 1,
      'feed_pid' => 1,
      'public_pid' => 1,
    ),
    3 => array(
      'weight' => 0,
      'youtube_pid' => 0,
      'http_pid' => 0,
      'https_pid' => 0,
      'feed_pid' => 0,
      'public_pid' => 2,
    ),
    4 => array(
      'weight' => 0,
      'youtube_pid' => 0,
      'http_pid' => 0,
      'https_pid' => 0,
      'feed_pid' => 0,
      'public_pid' => 2,
    ),
    2 => array(
      'weight' => 11,
      'youtube_pid' => 0,
      'http_pid' => 0,
      'https_pid' => 0,
      'feed_pid' => 0,
      'public_pid' => 2,
    ),
    1 => array(
      'weight' => 12,
      'youtube_pid' => 0,
      'http_pid' => 0,
      'https_pid' => 0,
      'feed_pid' => 0,
      'public_pid' => 0,
    ),
  );
  $export['imce_roles_profiles'] = $strongarm;

  return $export;
}
