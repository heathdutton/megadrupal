<?php

/**
 * @file
 * Drush integration for WYSIWYG editors.
 */

interface DrushEditorInterface {
  /**
   * Get library name.
   */
  public function getLibraryName();

  /**
   * Discover all versions of editor.
   *
   * @return array
   *   List of versions.
   */
  public function discover();

  /**
   * Switch to version.
   *
   * @param string $version
   *   Version.
   *
   * @return string
   *   Path to downloaded package.
   */
  public function select($version);

  /**
   * Download the editor.
   *
   * @return string
   *   Path to downloaded package.
   */
  public function download();

  /**
   * Build your own editor.
   *
   * @return string
   *   Path to built package.
   */
  public function build();

  /**
   * Checkout source code from git.
   *
   * @return string
   *   Path to source folder.
   */
  public function checkoutGit();

  /**
   * Checkout source code from svn.
   *
   * @return string
   *   Path to source folder.
   */
  public function checkoutSvn();

  /**
   * Checkout source code from vcs.
   *
   * @return string
   *   Path to source folder.
   */
  public function checkout($vcs = 'git');
}
