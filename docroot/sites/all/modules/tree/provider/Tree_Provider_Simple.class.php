<?php

/**
 * This is the base implementation of the trees.
 *
 * It works regardless of the underlying storage engine.
 */
class Tree_Provider_Simple extends Tree_Provider_Abstract implements Tree_Provider {

  public function parentOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    if ($item->parent !== NULL) {
      $query->condition('id', $item->parent);
    }
    else {
      // We are a root node.
      $query->alwaysFalse();
    }
    return $query;
  }

  public function ancestorsOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    if ($item->parent === NULL) {
      // This is a root item, it does not have any parent.
      $query->alwaysFalse();
      return $query;
    }

    // In this simple implementation, we have to load all the ancestors
    // and manually add them to the query.

    // Keep a list of seen IDs to avoid possible recursion.
    $parents = array($item->parent => $item);

    $current = $item;
    do {
      $items = $this->parentOf($current)->execute();
      $current = reset($items);
      if ($current) {
        if (!isset($parents[$current->parent])) {
          $parents[$current->parent] = $current;
          continue;
        }
      }
      break;
    }
    while (TRUE);

    if ($parents) {
      $query->condition('id', array_keys($parents));
    }
    else {
      $query->alwaysFalse();
    }

    return $query;
  }

  public function childrenOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    $query->condition('parent', $item->id);
    $query->orderBy('weight');
    return $query;
  }

  public function siblingsOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    if ($item->parent !== NULL) {
      // Return the children of the parent of the current item.
      $items = $this->parentOf($item)->execute();
      $parent = reset($items);
      $this->childrenOf($parent, $query);
    }
    else {
      // The siblings of a root item are all the root items.
      $this->isRoot($query);
    }
    return $query;
  }

  public function rootOf(Tree_Storage_Item $item, Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    if ($item->parent !== NULL) {
      // The root item of a normal item is its last ancestor.
      $ancestors = $this->ancestorsOf($item)->execute();
      foreach ($ancestors as $ancestor) {
        if ($ancestor->parent === NULL) {
          $root = $ancestor;
        }
      }
      $query->condition('id', $root->id);
    }
    else {
      // The root of a root item is itself.
      $query->condition('id', $item->id);
    }
    return $query;
  }

  public function isRoot(Tree_Storage_Query $query = NULL) {
    if (!isset($query)) {
      $query = $this->storage->query();
    }

    $query->isNull('parent', NULL);
    return $query;
  }
}
