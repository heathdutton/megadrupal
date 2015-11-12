<?php
/**
 * @file
 * Interface for exporting Job content.
 */

/**
 * Interface to export Job content to a format for the GCM translation service.
 */
interface TmgmtXConnectExportInterface {
  /**
   * Export a complete Job to 1 string.
   *
   * @param TMGMTJob $job
   *   The job to export.
   *
   * @return string
   *   The string to use in the service.
   */
  public function exportJob(TMGMTJob $job);

  /**
   * Export a single Job Item.
   *
   * @param TMGMTJobItem $item
   *   The job item.
   *
   * @return string
   *   The string to use in the service.
   */
  public function exportJobItem(TMGMTJobItem $item);

}
