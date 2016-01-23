<?php
/**
 * @file
 * Pure Import main class.
 */

/**
 * Interface IPureImportRunnable
 * Implementation scheme for an import runner class.
 */
interface IPureImportRunnable {

  /**
   * Check if the import can run.
   *
   * @return bool
   *  TRUE if all good. FALSE if import cannot run.
   */
  public function checkRequirements();

  /**
   * Returns the source query without defining range.
   *
   * @return SelectQuery
   */
  public function getSourceQuery();

  /**
   * Process of a source record - a row result of the source query.
   *
   * @param stdClass $record
   */
  public function processRecord(stdClass $record);

}
