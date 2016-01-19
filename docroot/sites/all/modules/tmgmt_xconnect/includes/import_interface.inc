<?php
/**
 * @file
 * Interface for importing translation results.
 */

/**
 * Interface for importing translation results.
 */
interface TmgmtXConnectImportInterface {
  /**
   * Validate if the content has a Job to import to.
   *
   * @param string $content
   *   The string to import.
   *
   * @return TMGMTJob
   *   The Job belonging to the provided content.
   */
  public function import($content);

}
