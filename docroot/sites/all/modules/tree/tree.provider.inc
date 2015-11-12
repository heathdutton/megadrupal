<?php

/**
 * The interface defining a provider for a tree.
 */
interface Tree_Provider {
  /**
   * Get the parent for an item.
   *
   * @return Tree_Storage_Query
   */
  public function parentOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL);

  /**
   * Get the whole ancestry of an item.
   *
   * @return Tree_Storage_Query
   */
  public function ancestorsOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL);

  /**
   * Get the children of an item.
   *
   * @return Tree_Storage_Query
   */
  public function childrenOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL);

  /**
   * Get the siblings of an item.
   *
   * @return Tree_Storage_Query
   */
  public function siblingsOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL);

  /**
   * Get the root item of an item.
   *
   * @return Tree_Storage_Query
   */
  public function rootOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL);

  /**
   * Get all the root items of the tree.
   *
   * @return Tree_Storage_Query
   */
  public function isRoot(Tree_Storage_Query $query = NULL);

  /**
   * Act after deletion has happend.
   *
   * @param int $item_id
   */
  public function postDelete($item_id);

  /**
   * Act after insertion has happened.
   *
   * @param Tree_Storage_Item $item
   */
  public function postInsert(Tree_Storage_Item $item);

  /**
   * Act after load has happened.
   */
  public function postLoad(Tree_Storage_Item $item);

  /**
   * Act after update has happened.
   *
   * @param Tree_Storage_Item $item
   */
  public function postUpdate(Tree_Storage_Item $item);

  /**
   * Act on pre-save.
   */
  public function preSave(Tree_Storage_Item $item);
}

/**
 * A tree provider that supports querying in tree order.
 */
interface Tree_Provider_Orderable extends Tree_Provider {
  public function orderByTree(Tree_Storage_Query $query);
}

/**
 * A tree provider that requires a SQL-joinable storage.
 */
interface Tree_Provider_SQL extends Tree_Provider {
  /**
   * Returns the database schema used by this tree provider.
   */
  public function schema();

  /**
   * Alter views data definition.
   */
  public function views_data_alter(&$data);
}
