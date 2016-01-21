<?php

class EventbriteApiAttendeesController extends EntityAPIController {

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $query->innerJoin('eventbrite_api_attendees_answers', 'a', 'base.aid = a.aid');
    $query->addField('a', 'qid', 'qid');
    $query->addField('a', 'answer_text', 'answer_text');
    $query->addField('a', 'question', 'question');
    $query->addField('a', 'question_type', 'question_type');
    return $query;
  }

  /**
   * Overridden.
   * @see EntityAPIController#load($ids, $conditions)
   *
   * In contrast to the parent implementation, multiple rows are
   * grouped by field api_key to handle multiple methods for each key.
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = array();

    // Revisions are not statically cached, and require a different query to
    // other conditions, so separate the revision id into its own variable.
    if ($this->revisionKey && isset($conditions[$this->revisionKey])) {
      $revision_id = $conditions[$this->revisionKey];
      unset($conditions[$this->revisionKey]);
    }
    else {
      $revision_id = FALSE;
    }

    // Create a new variable which is either a prepared version of the $ids
    // array for later comparison with the entity cache, or FALSE if no $ids
    // were passed. The $ids array is reduced as items are loaded from cache,
    // and we need to know if it's empty for this reason to avoid querying the
    // database when all requested entities are loaded from cache.
    $passed_ids = !empty($ids) ? array_flip($ids) : FALSE;

    // Try to load entities from the static cache.
    if ($this->cache && !$revision_id) {
      $entities = $this->cacheGet($ids, $conditions);
      // If any entities were loaded, remove them from the ids still to load.
      if ($passed_ids) {
        $ids = array_keys(array_diff_key($passed_ids, $entities));
      }
    }

    // Support the entitycache module if activated.
    if (!empty($this->entityInfo['entity cache']) && !$revision_id && $ids && !$conditions) {
      $cached_entities = EntityCacheControllerHelper::entityCacheGet($this, $ids, $conditions);
      // If any entities were loaded, remove them from the ids still to load.
      $ids = array_diff($ids, array_keys($cached_entities));
      $entities += $cached_entities;

      // Add loaded entities to the static cache if we are not loading a
      // revision.
      if ($this->cache && !empty($cached_entities) && !$revision_id) {
        $this->cacheSet($cached_entities);
      }
    }

    // Load any remaining entities from the database. This is the case if $ids
    // is set to FALSE (so we load all entities), if there are any ids left to
    // load or if loading a revision.
    if (!($this->cacheComplete && $ids === FALSE && !$conditions) && ($ids === FALSE || $ids || $revision_id)) {
      $queried_entities = array();
      foreach ($this->query($ids, $conditions, $revision_id) as $record) {
        // Skip entities already retrieved from cache.
        if (isset($entities[$record->{$this->idKey}])) {
          continue;
        }

        // For DB-based entities take care of serialized columns.
        if (!empty($this->entityInfo['base table'])) {
          $schema = drupal_get_schema($this->entityInfo['base table']);

          foreach ($schema['fields'] as $field => $info) {
            if (!empty($info['serialize']) && isset($record->$field)) {
              $record->$field = unserialize($record->$field);
              // Support automatic merging of 'data' fields into the entity.
              if (!empty($info['merge']) && is_array($record->$field)) {
                foreach ($record->$field as $key => $value) {
                  $record->$key = $value;
                }
                unset($record->$field);
              }
            }
          }
        }

        $queried_entities[$record->{$this->idKey}] = $record; // d p m  this

        /*if (isset($queried_entities[$record->{$this->idKey}])) {
          $queried_entities[$record->{$this->idKey}]->methods[] = $record->method;
        }
        else {
          $record->methods = array();
          $record->methods[] = $record->method;
          unset($record->method);
          $queried_entities[$record->{$this->idKey}] = $record;
        }*/

      }
    }

    // Pass all entities loaded from the database through $this->attachLoad(),
    // which attaches fields (if supported by the entity type) and calls the
    // entity type specific load callback, for example hook_node_load().
    if (!empty($queried_entities)) {
      $this->attachLoad($queried_entities, $revision_id);
      $entities += $queried_entities;
    }

    // Entitycache module support: Add entities to the entity cache if we are
    // not loading a revision.
    if (!empty($this->entityInfo['entity cache']) && !empty($queried_entities) && !$revision_id) {
      EntityCacheControllerHelper::entityCacheSet($this, $queried_entities);
    }

    if ($this->cache) {
      // Add entities to the cache if we are not loading a revision.
      if (!empty($queried_entities) && !$revision_id) {
        $this->cacheSet($queried_entities);

        // Remember if we have cached all entities now.
        if (!$conditions && $ids === FALSE) {
          $this->cacheComplete = TRUE;
        }
      }
    }
    // Ensure that the returned array is ordered the same as the original
    // $ids array if this was passed in and remove any invalid ids.
    if ($passed_ids && $passed_ids = array_intersect_key($passed_ids, $entities)) {
      foreach ($passed_ids as $id => $value) {
        $passed_ids[$id] = $entities[$id];
      }
      $entities = $passed_ids;
    }
    return $entities;

  }

}
