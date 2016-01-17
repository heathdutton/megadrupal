<?php
/**
 * @file
 * Test backend implementation.
 */

/**
 * Test backend.
 */
class PHRepositoryTestBackend extends PHRepositoryBackend {
  /**
   * Synchronizes the repository.
   */
  public function synchronize() {
    throw new Exception(__METHOD__ . ' test');
  }

  /**
   * Lists recognized versions in the repository, suitable for exporting.
   *
   * @return array
   *   An array of version arrays.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function listVersions() {
    return array(
      ph_version_make(array(
        'core' => 6,
        'major' => 1,
      )),
      ph_version_make(array(
        'core' => 7,
        'major' => 1,
      )),
    );
  }

  /**
   * Exports a version of a repository into the given directory.
   */
  public function export($path, $version, $versionSince = NULL) {
    $sampleDir = drupal_get_path('module', 'ph_repository_test') . '/sample_repository';
    file_unmanaged_copy($sampleDir . '/test.tpl.info', $path . '/test.info', FILE_EXISTS_REPLACE);
    file_unmanaged_copy($sampleDir . '/test.tpl.module', $path . '/test.module', FILE_EXISTS_REPLACE);
    return ph_version_make(array(
      'patch' => 0,
      // Mark a commit increment count.
      'increment' => 1,
    ) + $version);
  }
}
