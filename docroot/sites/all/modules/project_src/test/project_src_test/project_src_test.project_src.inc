<?php

/**
 * @file
 * Hooks invoked by the Project Source API.
 */


/**
 * Implements hook_project_src_info().
 */
function project_src_test_project_src_info() {
  return array(
    '6.x' => array(
      'drewpaul_inc_module' => array(
        'short_name' => 'drewpaul_inc_module',
        'title' => 'Drew Paul Inc. Module',
        'creator' => 'drewpaul',
        'link' => 'https://example.com/drewpaul/drewpaul_inc_module',
        'api_version' => '6.x',
      ),
    ),
    '7.x' => array(
      'drewpaul_inc_module' => array(
        'short_name' => 'drewpaul_inc_module',
        'title' => 'Drew Paul Inc. Module',
        'creator' => 'drewpaul',
        'link' => 'https://example.com/drewpaul/drewpaul_inc_module',
        'api_version' => '7.x',
      ),
      'drewpaul_inc_theme' => array(
        'short_name' => 'drewpaul_inc_theme',
        'title' => 'Drew Paul Inc. Theme',
        'creator' => 'drewpaul',
        'link' => 'https://example.com/drewpaul/drewpaul_inc_theme',
        'api_version' => '7.x',
      ),
    ),
  );
}


/**
 * Implements hook_project_src_releases().
 */
function project_src_test_project_src_releases($short_name, $api_version, $info) {
  // The 6.x-1.0 release of drewpaul_inc_module.
  $releases['6.x']['drewpaul_inc_module']['6.x-1.0'] = array(
    'name' => 'drewpaul_inc_module 6.x-1.0',
    'version' => '6.x-1.0',
    'tag' => '6.x-1.0',
    'date' => 1345423902,
    'version_major' => 1,
    'version_patch' => 0,
    'version_extra' => '',
    'status' => 'published',
    'release_link' => 'http://example.com/project/drewpaul_inc_module/releases/6.x-1.0',
    'download_link' => 'http://ftp.example.com/files/projects/drewpaul_inc_module-7.x-1.0.tar.gz',
    'filesize' => '10132',
    'mdhash' => '675288f8194d9eb34c28f2f7cffab8ad',
  );

  // A dev release of drewpaul_inc_module 7.x-1.x.
  $releases['7.x']['drewpaul_inc_module']['7.x-1.x'] = array(
    'name' => 'drewpaul_inc_module 7.x-1.x-dev',
    'version' => '7.x-1.x-dev',
    'tag' => '7.x-1.x',
    'date' => 1345423902,
    'version_major' => 1,
    'version_patch' => '',
    'version_extra' => 'dev',
    'status' => 'published',
    'release_link' => 'http://example.com/project/drewpaul_inc_module/releases/7.x-1.x',
    'download_link' => 'http://ftp.example.com/files/projects/drewpaul_inc_module-7.x-1.x-dev.tar.gz',
    'filesize' => '10132',
    'mdhash' => '675288f8194d9eb34c28f2f7cffab8ad',
  );

  // A tagged release of drewpaul_inc_module 7.x-2.x.
  $releases['7.x']['drewpaul_inc_module']['7.x-2.0-beta1'] = array(
    'name' => 'drewpaul_inc_module 7.x-2.0-beta1',
    'version' => '7.x-2.0-beta1',
    'tag' => '7.x-2.0-beta1',
    'date' => 1345423902,
    'version_major' => 2,
    'version_patch' => 0,
    'version_extra' => 'beta1',
    'status' => 'published',
    'release_link' => 'http://example.com/project/drewpaul_inc_module/releases/7.x-2.0-beta1',
    'download_link' => 'http://ftp.example.com/files/projects/drewpaul_inc_module-7.x-2.0-beta1.tar.gz',
    'filesize' => '10132',
    'mdhash' => '675288f8194d9eb34c28f2f7cffab8ad',
  );

  // A tagged release of drewpaul_inc_theme 7.x-1.x.
  $releases['7.x']['drewpaul_inc_theme']['7.x-1.1'] = array(
    'name' => 'drewpaul_inc_theme 7.x-1.1',
    'version' => '7.x-1.1',
    'tag' => '7.x-1.1',
    'date' => 1345423902,
    'version_major' => 1,
    'version_patch' => 1,
    'version_extra' => '',
    'status' => 'published',
    'release_link' => 'http://example.com/project/drewpaul_inc_theme/releases/7.x-1.1',
    'download_link' => 'http://ftp.example.com/files/projects/drewpaul_inc_theme-7.x-1.1.tar.gz',
    'filesize' => '10132',
    'mdhash' => '675288f8194d9eb34c28f2f7cffab8ad',
  );

  $available = isset($releases[$api_version][$short_name]);
  return $available ? $releases[$api_version][$short_name] : array();
}


/**
 * Implements hook_project_src_terms().
 */
function project_src_test_project_src_terms($type, $short_name, $api_version, $info) {
  // Always return this.
  return array(
    array('Arbitrary term name', 'Arbitrary term value'),
  );
}
