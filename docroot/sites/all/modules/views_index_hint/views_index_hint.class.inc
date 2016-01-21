<?php

/**
 * @file
 *
 * Contains ViewsIndexHint.
 */

/**
 * The extender class for Select queries to add index hint.
 */
class ViewsIndexHint extends SelectQueryExtender {

  /**
   * Constant for USE INDEX.
   */
  const INDEX_HINT_TYPE_USE = 'USE';

  /**
   * Constant for IGNORE INDEX.
   */
  const INDEX_HINT_TYPE_IGNORE = 'IGNORE';

  /**
   * Constant for FORCE INDEX.
   */
  const INDEX_HINT_TYPE_FORCE = 'FORCE';

  /**
   * @var array
   */
  protected $index_hint;

  /**
   * @var string
   */
  protected $base_table;

  /**
   * Setter for base table.
   *
   * @param $base_table
   *
   * @return $this
   */
  public function setBaseTable($base_table) {
    $this->base_table = $base_table;
    return $this;
  }

  /**
   * Setter for index hint.
   *
   * @param string $index_hint
   * @param string $type
   *   Type of index operation. Allowed values are
   *   INDEX_HINT_TYPE_USE, INDEX_HINT_TYPE_IGNORE and INDEX_HINT_TYPE_FORCE
   *
   * @return $this
   */
  public function setIndexHint($index_hint, $type = self::INDEX_HINT_TYPE_USE) {
    $this->index_hint[$type] = $index_hint;
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function execute() {
    // If validation fails, simply return NULL.
    // Note that validation routines in preExecute() may throw exceptions instead.
    if (!$this->preExecute()) {
      return NULL;
    }
    $args = $this->getArguments();
    $query = (string) $this->query;

    if (!empty($this->index_hint)) {
      // There is no option to add the index hint except overriding
      // Execute as Drupal 7 uses SelectQuery object
      // (unlike Drupal 6 static query).
      $search = '{'. $this->base_table .'} '. $this->base_table;
      $index_string = '';
      foreach ($this->index_hint as $type => $hint) {
        $index_string .= $type . ' INDEX(' . check_plain($hint) . ') ';
      }

      $replace = $search . ' ' . trim($index_string);
      $query = str_replace($search, $replace, $query);
    }

    // @TODO: Skipped queryOptions as it is not in scope. See if there way to
    // add it back.
    // @see SelectQuery::execute.
    return $this->connection->query($query, $args);
  }

}
