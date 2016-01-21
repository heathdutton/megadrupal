<?php

/**
* Description
*/
class EntityCollectionStorageDBContext extends EntityCollectionStorageDB {

  public function save(EntityCollection $collection, EntityCollectionTreeNode $tree, $contexts = array(), $position = 0, $depth = 0) {
    $this->saveContexts($contexts);
    parent::save($collection, $tree, $contexts, $position, $depth);
  }

  public function appendItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    $this->saveContexts($contexts);
    parent::appendItem($collection, $item, $contexts);
  }

  public function prependItem(EntityCollection $collection, EntityCollectionTreeNode $item, $contexts = array()) {
    $this->saveContexts($contexts);
    parent::prependItem($collection, $item, $contexts);
  }

  protected function serializeContexts($contexts) {
    if (empty($contexts)) {
      return '';
    }
    $serialized = array();
    foreach ($contexts as $context) {
      if (isset($context->argument)) {
        $type = is_array($context->type) ? reset($context->type) : $context->type ;
        $serialized[] = "{$type}:{$context->argument}";
      }
    }

    return md5(implode("|",$serialized));
  }
  
  protected function saveContexts($contexts) {
    $hash = $this->serializeContexts($contexts);
    if ( !$this->contextHashExists($hash) && is_array($contexts) ) {
      foreach ($contexts as $ctxt) {
        $record = $this->buildContextRecord($hash, $ctxt);
        drupal_write_record('entity_collection_storage_contexts', $record);
      }
    }
  }
  
  protected function contextHashExists($hash) {
    $count = db_select('entity_collection_storage_contexts', 'ecc')
      ->fields('ecc', array('hash'))
      ->condition('hash', $hash)
        ->countQuery()
          ->execute()
          ->fetchField();

    return $count > 0;
  }
  
  protected function buildContextRecord($hash, $context){
    $record = new stdClass();
    $record->hash = $hash;
    $record->context = is_array($context->type) ? reset($context->type) : $context->type;
    $record->value = $context->argument;
    $record->raw = serialize($context);
    return $record;
  }
}