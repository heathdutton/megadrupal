<?php

function mongo_entity_drush_command() {

  // @todo move this to a mongo module
  $items['mongo-entity-drop'] = array(
    'description' => 'Drop all Mongo Entities in a given database',
  );
  return $items;


}

function drush_mongo_entity_drop() {

  $count = 0;
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (is_a($entity_info['controller class'], 'MongoEntityController', TRUE)) {
      $collection = mongodb_collection($entity_info['collection']);
      $collection->drop();
      $count++;
    }
  }
  drush_print(format_plural($count, 'Dropped 1 collection.', 'Dropped @count collections.'));

}
