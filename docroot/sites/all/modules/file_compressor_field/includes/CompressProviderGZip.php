<?php

/**
 * @file
 * Contains CompressProviderGZip.
 */

/**
 * Implements GZip compressor for File Compressor field.
 */
class CompressProviderGZip implements CompressProviderInterface {

  /**
   * The extension for files using this plugin.
   *
   * @var string
   */
  protected $extension = 'tar.gz';

  /**
   * @{@inheritdoc}
   */
  public function getExtension() {
    return $this->extension;
  }

  /**
   * @{@inheritdoc}
   */
  public function generateCompressedFileUri($base_uri) {
    return "$base_uri.$this->extension";
  }

  /**
   * @{@inheritdoc}
   */
  public function generateCompressedFile($file_uri, $files) {
    $full_file_uri = drupal_realpath($file_uri);
    $tar = new Archive_Tar($full_file_uri, 'gz');
    $tmp = 'temporary://file_compressor_field' . time() . user_password();
    drupal_mkdir($tmp);
    foreach ($files as $file) {
      file_unmanaged_copy($file, $tmp, FILE_EXISTS_REPLACE);
    }
    $tar->createModify($tmp, '', $tmp);
    file_unmanaged_delete_recursive($tmp);

    return TRUE;
  }
}
