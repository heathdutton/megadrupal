<?php

/**
 * An abstract class providing fallbacks for all Tree_Provider methods.
 */
abstract class Tree_Provider_Abstract implements Tree_Provider {

  /**
   * The storage object used by this provider.
   *
   * @var Tree_Storage
   */
  public $storage;

  public function __construct(Tree_Storage $storage) {
    $this->storage = $storage;
  }

  public function postDelete($item_id) {
    // Nothing to do here.
  }

  public function postLoad(Tree_Storage_Item $item) {
    // Nothing to do here.
  }

  public function postInsert(Tree_Storage_Item $item) {
    // Nothing to do here.
  }

  public function postUpdate(Tree_Storage_Item $item) {
    // Nothing to do here.
  }

  public function preSave(Tree_Storage_Item $item) {
    // Nothing to do here.
  }
}
