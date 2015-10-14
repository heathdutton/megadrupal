<?php

/**
 * @file
 * Contains \Drupal\protected_download_admin\Tests\ProtectedDownloadAdminTest.
 */

namespace Drupal\protected_download_admin\Tests;

/**
 * Tests administrative interface for stream wrapper.
 */
class AdminTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  /**
   * The admin user.
   *
   * @var object
   */
  protected $adminUser;

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Administrative interface',
      'description' => 'Tests administrative interface for stream wrapper.',
      'group' => 'Protected Download',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp(array('protected_download_admin'));

    $this->adminUser = $this->drupalCreateUser(array('administer site configuration'));
  }

  /**
   * Tests administrative interface for stream wrapper.
   */
  public function testFileSystemAdmin() {
    $this->drupalLogin($this->adminUser);

    $this->drupalGet('admin/config/media/file-system');
    $this->assertResponse(200);

    // If protected_download_file_path_protected is not set, there should be no
    // "protected"-radio for file_default_scheme.
    $this->assertNoFieldByName('file_default_scheme', 'protected');

    $protected_path = $this->private_files_directory . '/' . $this->randomName(8);
    $edit = array(
      'protected_download_file_path_protected' => $protected_path,
    );
    $this->drupalPost(NULL, $edit, 'Save configuration');
    $this->assertResponse(200);
    $this->assertFieldByName('file_default_scheme', 'protected');
  }

  /**
   * Tests administrative interface for protected download.
   */
  public function testProtectedDownloadAdmin() {
    $this->drupalGet('admin/config/media/protected-download');
    $this->assertResponse(403);

    $this->drupalLogin($this->adminUser);

    $this->drupalGet('admin/config/media/protected-download');
    $this->assertResponse(200);

    // Verify default settings for public and private file system.
    $this->assertFieldByName('protected_download_cache_control_public', 'public');
    $this->assertFieldByName('protected_download_ttl_mode_public', 'aligned');
    $this->assertFieldByName('protected_download_aligned_min_ttl_public', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL);
    $this->assertFieldByName('protected_download_aligned_max_ttl_public', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL);
    $this->assertFieldByName('protected_download_exact_ttl_public', PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL);
    $public_settings = protected_download_settings('public');
    $expected_settings = array(
      'cache_control' => 'public',
      'ttl_mode' => 'aligned',
      'aligned_min_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL,
      'aligned_max_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL,
      'exact_ttl' => PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL,
    );
    $this->assertEqual($public_settings, $expected_settings);

    $this->assertFieldByName('protected_download_cache_control_private', 'private');
    $this->assertFieldByName('protected_download_ttl_mode_private', 'exact');
    $this->assertFieldByName('protected_download_aligned_min_ttl_private', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL);
    $this->assertFieldByName('protected_download_aligned_max_ttl_private', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL);
    $this->assertFieldByName('protected_download_exact_ttl_private', PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL);
    $private_settings = protected_download_settings('private');
    $expected_settings = array(
      'cache_control' => 'private',
      'ttl_mode' => 'exact',
      'aligned_min_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL,
      'aligned_max_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL,
      'exact_ttl' => PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL,
    );
    $this->assertEqual($private_settings, $expected_settings);

    // No fields should be displayed for protected if file system path is not
    // configured.
    $this->assertNoFieldByName('protected_download_cache_control_protected');
    $this->assertNoFieldByName('protected_download_ttl_mode_protected');
    $this->assertNoFieldByName('protected_download_aligned_min_ttl_protected');
    $this->assertNoFieldByName('protected_download_aligned_max_ttl_protected');
    $this->assertNoFieldByName('protected_download_exact_ttl_protected');

    // Enable protected stream.
    $protected_path = $this->private_files_directory . '/' . $this->randomName(8);
    $edit = array(
      'protected_download_file_path_protected' => $protected_path,
    );
    $this->drupalPost('admin/config/media/file-system', $edit, 'Save configuration');
    $this->assertResponse(200);

    // Verify default settings for the protected file system.
    $this->drupalGet('admin/config/media/protected-download');
    $this->assertResponse(200);

    $this->assertFieldByName('protected_download_cache_control_protected', 'public');
    $this->assertFieldByName('protected_download_ttl_mode_protected', 'aligned');
    $this->assertFieldByName('protected_download_aligned_min_ttl_protected', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL);
    $this->assertFieldByName('protected_download_aligned_max_ttl_protected', PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL);
    $this->assertFieldByName('protected_download_exact_ttl_protected', PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL);
    $protected_settings = protected_download_settings('protected');
    $expected_settings = array(
      'cache_control' => 'public',
      'ttl_mode' => 'aligned',
      'aligned_min_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MIN_TTL,
      'aligned_max_ttl' => PROTECTED_DOWNLOAD_DEFAULT_ALIGNED_MAX_TTL,
      'exact_ttl' => PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL,
    );
    $this->assertEqual($protected_settings, $expected_settings);

    // Customize settings.
    $edit = array(
      'protected_download_cache_control_protected' => 'private',
      'protected_download_aligned_min_ttl_protected' => 3600,
      'protected_download_aligned_max_ttl_protected' => 90000,
    );
    $this->drupalPost(NULL, $edit, 'Save configuration');
    $this->assertResponse(200);

    $this->assertFieldByName('protected_download_cache_control_protected', 'private');
    $this->assertFieldByName('protected_download_ttl_mode_protected', 'aligned');
    $this->assertFieldByName('protected_download_aligned_min_ttl_protected', 3600);
    $this->assertFieldByName('protected_download_aligned_max_ttl_protected', 90000);
    $this->assertFieldByName('protected_download_exact_ttl_protected', PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL);
    $protected_settings = protected_download_settings('protected');
    $expected_settings = array(
      'cache_control' => 'private',
      'ttl_mode' => 'aligned',
      'aligned_min_ttl' => 3600,
      'aligned_max_ttl' => 90000,
      'exact_ttl' => PROTECTED_DOWNLOAD_DEFAULT_EXACT_TTL,
    );
    $this->assertEqual($protected_settings, $expected_settings);
  }

}
