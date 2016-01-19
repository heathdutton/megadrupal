<?php
/**
 * @file
 * Contains GitCloneStreamWrapper.
 */

/**
 * Drupal system stream wrapper abstract class.
 */
class GitCloneStreamWrapper extends DrupalLocalStreamWrapper {

  /**
   * {@inheritdoc}
   */
  public function getDirectoryPath() {
    return _git_clone_path();
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl() {
    return FALSE;
  }

}
