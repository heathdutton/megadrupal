<?php

/**
 * An abstract storage for trees.
 */
interface Tree_Storage {
  /**
   * Build a query for this tree.
   *
   * @return Tree_Storage_Query
   */
  public function query();
}

/**
 * A storage that is testable.
 *
 * Necessary for building fixtures for tests.
 */
interface Tree_Storage_Testable {
  /**
   * Set the provider for this storage.
   */
  public function setProvider(Tree_Provider $provider);

  /**
   * Create a memory representation of a tree item.
   *
   * @return Tree_Storage_Item
   */
  public function create();

  /**
   * Save a tree item to the underlying storage.
   */
  public function save(Tree_Storage_Item $item);

  /**
   * Delete a tree item from the underlying storage.
   */
  public function delete($item_id);
}

/**
 * An abstract query for trees.
 */
interface Tree_Storage_Query {
  /**
   * Apply a condition to the query.
   *
   * @param $column
   *   The name of the tree column to check ('id', 'parent' or 'weight').
   * @param $value
   *   The value to test the column against. In most cases, this is a scalar.
   *   For more complex options, it is an array. The meaning of each element in
   *   the array is dependent on the $operator.
   * @param $operator
   *   The comparison operator, such as =, <, or >=. It also accepts more
   *   complex options such as IN, LIKE, or BETWEEN. Defaults to IN if $value is
   *   an array, and = otherwise.
   *
   * @return Tree_Storage_Query
   *   The called object.
   */
  public function condition($column, $value, $operator);

  /**
   * Sets a condition that the specified column is NULL.
   *
   * @param $column
   *   The name of the tree column to check ('id', 'parent' or 'weight').
   *
   * @return Tree_Storage_Query
   *   The called object.
   */
  public function isNull($column);

  /**
   * Sets a condition that the specified column is not NULL.
   *
   * @param $column
   *   The name of the tree column to check ('id', 'parent' or 'weight').
   *
   * @return Tree_Storage_Query
   *   The called object.
   */
  public function isNotNull($column);

  /**
   * Sets a condition that is always false.
   *
   * @return Tree_Storage_Query
   *   The called object.
   */
  public function alwaysFalse();

  /**
   * Orders the result set by a given column.
   *
   * If called multiple times, the query will order by each specified field in the
   * order this method is called.
   *
   * @param $column
   *   The tree column on which to order ('id', 'parent' or 'weight').
   * @param $direction
   *   The direction to sort ('ASC' or 'DESC').
   *
   * @return Tree_Storage_Query
   *   The called object.
   */
  public function orderBy($column, $direction = 'ASC');

  /**
   * Runs the query.
   *
   * @return
   *   An array of Tree_Storage_Item
   */
  public function execute();
}

/**
 * An abstract item stored in a tree.
 *
 * Every tree item contains tree public properties:
 *  - id: the ID of this item
 *  - parent: the ID of the parent of this item in the tree
 *  - weight: the weight of this item among its siblings
 */
interface Tree_Storage_Item {

}
