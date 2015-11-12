<?php
/**
 * @file
 * Custom migration sources.
 */

/**
 * Added functionality to skip rows, based on a function predicate.
 *
 * Count method now runs predicate function against each row, instead of just
 * returning the number of lines.
 */
class MigrateSourceCSVOnSteroids extends MigrateSourceCSV {

  /**
   * Simple initialization.
   *
   * @param string $path
   *   The path to the source file
   * @param array $csvcolumns
   *   Keys are integers. values are array(field name, description).
   * @param array $options
   *   Options applied to this source.
   * @param array $fields
   *   Optional - keys are field names, values are descriptions. Use to override
   *   the default descriptions, or to add additional source fields which the
   *   migration will add via other means (e.g., prepareRow()).
   */
  public function __construct($path, array $csvcolumns = array(), array $options = array(), array $fields = array()) {
    parent::__construct($path, $csvcolumns, $options, $fields);

    // Check whether there is a filter function, to skip certain rows.
    if (!empty($options['filter_predicate'])) {
      $this->filterPredicateSet = TRUE;
      $this->filterPredicate = $options['filter_predicate'];
    }
    else {
      $this->filterPredicateSet = FALSE;
    }
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * Return the next line of the source CSV file as an object.
   *
   * @return null|object
   *   Returns the next CSV row.
   */
  public function getNextRow() {
    $row = $this->getNextLine();

    if ($row) {
      // Only use rows specified in $this->csvcolumns().
      $row = array_intersect_key($row, $this->csvcolumns);
      // Set meaningful keys for the columns mentioned in $this->csvcolumns().
      foreach ($this->csvcolumns as $int => $values) {
        list($key, $description) = $values;
        // Copy value to more descriptive string based key and then unset
        // original.
        $row[$key] = isset($row[$int]) ? $row[$int] : NULL;
        unset($row[$int]);
      }

      $row['csvrownum'] = $this->rowNumber++;

      // Skip rows that were filtered out.
      $filter_predicate = $this->filterPredicate;
      if ($this->filterPredicateSet && $filter_predicate($row)) {
        return $this->getNextRow();
      }

      return (object) $row;
    }
    else {
      fclose($this->csvHandle);
      $this->csvHandle = NULL;
      return NULL;
    }
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    $this->csvHandle = fopen($this->file, 'r');
    // Skip all but the last header.
    for ($i = 0; $i < $this->headerRows; $i++) {
      fgets($this->csvHandle);
    }
    $count = 0;
    while ($this->getNextRow()) {
      $count++;
    }
    return $count;
  }

}

/**
 * MigrateSourceSQL with additional functionality.
 *
 * Extended with ability to check if the count of source rows has been cached.
 */
class MigrateSourceSQLOnSteroids extends MigrateSourceSQL {
  /**
   * Checks if the source rows count was cached.
   */
  public function countIsCached() {
    // If source has specified to skip count, we consider that the count was
    // cached.
    if ($this->skipCount) {
      return TRUE;
    }

    // Generate cache key if not present.
    if (!isset($this->cacheKey)) {
      $this->cacheKey = md5((string) $this);
    }

    // Get the count key from cache.
    $cache_object = cache_get($this->cacheKey, 'cache');

    if (is_object($cache_object)) {
      return TRUE;
    }

    return FALSE;
  }
}
