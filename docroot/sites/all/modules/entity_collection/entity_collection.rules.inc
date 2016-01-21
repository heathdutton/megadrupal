<?php
/**
 * @file
 * Rules integration for entity collection.
 */

class EntityCollectionRulesController extends EntityDefaultRulesController {

  /**
   * @see EntityDefaultRulesController::eventInfo().
   */
  public function eventInfo() {
    $items = parent::eventInfo();
    $info = $this->info;
    $type = $this->type;
    $label = $info['label'];
    $defaults = array(
      'module' => isset($info['module']) ? $info['module'] : 'entity',
      'group' => $label,
      'access callback' => 'entity_collection_access',
    );
    // Content has been inserted into the collection.
    $items[$type . '_content_insert_event'] = $defaults + array(
      'label' => t('After adding content to @entity', array('@entity' => drupal_strtolower($label))),
      'variables' => entity_rules_events_variables($type, t('updated @entity', array('@entity' => drupal_strtolower($label)))) + array(
        'item' => array(
          'type' => 'entity',
          'label' => t('Added entity'),
        ),
      ),
    );
    // Content has been removed from the collection.
    $items[$type . '_content_delete_event'] = $defaults + array(
      'label' => t('After content has been deleted from @entity', array('@entity' => drupal_strtolower($label))),
      'variables' => entity_rules_events_variables($type, t('updated @entity', array('@entity' => drupal_strtolower($label)))) + array(
        'items' => array(
          'type' => 'list<entity>',
          'label' => t('Removed entities'),
        ),
      ),
    );
    return $items;
  }
}

/**
 * Implements hook_rules_action_info().
 */
function entity_collection_rules_action_info() {
  $items = array();
  $items['entity_collection_load_action'] = array(
    'label' => t('Load entity collection'),
    'parameter' => array(
      'name' => array(
        'type' => 'text',
        'label' => t('Entity collection'),
        'save' => FALSE,
        'options list' => 'entity_collection_get_names',
        'restriction' => 'input',
      ),
    ),
    'provides' => array(
      'loaded_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Loaded collection'),
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_append_action'] = array(
    'label' => t('Append an item to the collection'),
    'parameter' => array(
      'entity_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Entity collection'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
      'item' => array(
        'type' => 'entity',
        'label' => t('Item'),
        'save' => FALSE,
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_prepend_action'] = array(
    'label' => t('Push an item to the top of the collection'),
    'parameter' => array(
      'entity_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Entity collection'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
      'item' => array(
        'type' => 'entity',
        'label' => t('Item'),
        'save' => FALSE,
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_pop_action'] = array(
    'label' => t('Remove the first item in the collection'),
    'parameter' => array(
      'entity_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Entity collection'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
    ),
    'provides' => array(
      'first_item' => array(
        'type' => 'entity_collection',
        'label' => t('Loaded collection'),
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_remove_last_action'] = array(
    'label' => t('Remove the last item in a collection'),
    'parameter' => array(
      'entity_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Entity collection'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
    ),
    'provides' => array(
      'last_item' => array(
        'type' => 'entity_collection',
        'label' => t('Loaded collection'),
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_remove_item_action'] = array(
    'label' => t('Remove an item in a collection'),
    'parameter' => array(
      'entity_collection' => array(
        'type' => 'entity_collection',
        'label' => t('Entity collection'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
      'item' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'restriction' => 'selector',
        'save' => FALSE,
      ),
    ),
    'provides' => array(
      'last_item' => array(
        'type' => 'entity_collection',
        'label' => t('Loaded collection'),
      ),
    ),
    'group' => t('Entity collection'),
  );
  $items['entity_collection_get_collections_action'] = array(
    'label' => t('Get all collections an entity is in'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity to look for'),
        'save' => FALSE,
        'restriction' => 'selector',
      ),
      'bundles' => array(
        'type' => 'list<text>',
        'label' => t('Bundles'),
        'restriction' => 'input',
        'options list' => 'entity_collection_get_bundle_options',
      ),
      'contexts' => array(
        'label' => t('Contexts'),
        'type' => 'list<entity>',
        'optional' => TRUE,
        'label' => t('Contexts'),
        'save' => FALSE,
      ),
      'exclude' => array(
        'label' => t('List of entity collections to exclude'),
        'type' => 'list<entity_collection>',
        'restriction' => 'selector',
        // Entity collections needs to be wrapped since they are loaded
        // by name and not ID.
        'wrapped' => TRUE,
        'optional' => TRUE,
        'save' => FALSE,
      ),
    ),
    'provides' => array(
      'collections' => array(
        'type' => 'list<entity_collection>',
        'label' => t('Loaded collection'),
      ),
    ),
    'group' => t('Entity collection'),
  );
  return $items;
}

/**
 * Get the names of all entity collections,
 * formatted for options.
 */
function entity_collection_get_names() {
  $items = array();
  foreach (entity_collection_load_all() as $name => $collection) {
    $items[$name] = $collection->title;
  }
  return $items;
}

function entity_collection_get_bundle_options() {
  $items = array();
  foreach (entity_collection_bundles() as $name => $bundle) {
    $items[$name] = $bundle->label;
  }
  return $items;
}

/**
 * Callback for loading an entity collection.
 * @param string $name
 */
function entity_collection_load_action($name) {
  return array('loaded_collection' => entity_collection_load($name));
}

function entity_collection_pop_action($entity_collection) {
  $storage = EntityCollectionStorage::getBundleStorage($entity_collection->bundle);
  $items = $storage->retrieve($entity_collection);
  $first_item = current($items->getFlat());
  $storage->delete($entity_collection, $first_item->type, array($first_item->entity_id));
}

function entity_collection_append_action($entity_collection, $item) {
  $entity = $item->value();
  $entity_type = $item->type();
  $entity_id = entity_id($entity_type, $entity);
  $item = new EntityCollectionTreeNode($entity_type, $entity_id, $entity);
  entity_collection_append_item($entity_collection, $item);
}

function entity_collection_prepend_action($entity_collection, $item) {
  $entity = $item->value();
  $entity_type = $item->type();
  $entity_id = entity_id($entity_type, $entity);
  $item = new EntityCollectionTreeNode($entity_type, $entity_id, $entity);
  entity_collection_prepend_item($entity_collection, $item);
}

function entity_collection_remove_last_action($entity_collection) {
  $storage = EntityCollectionStorage::getBundleStorage($entity_collection->bundle);
  $items = $storage->retrieve($entity_collection);
  $last_item = end($items->getFlat());
  $storage->delete($entity_collection, $last_item->type, array($last_item->entity_id));
}

function entity_collection_remove_item_action($entity_collection, $item) {
  $storage = EntityCollectionStorage::getBundleStorage($entity_collection->bundle);
  $entity_type = $item->type();
  $entity_id = entity_id($entity_type, $item->value());
  $storage->delete($entity_collection, $entity_type, array($entity_id));
}

function entity_collection_get_collections_action($item, $bundles = array(), $contexts = array(), $exclude_wrapper = array()) {
  $entity_type = $item->type();
  $entity_id = entity_id($entity_type, $item->value());
  ctools_include('context');
  foreach ($contexts as &$context) {
    $context = ctools_context_create('entity:' . $context->type(), $context->value());
  }
  $exclude = array();
  foreach ($exclude_wrapper as $entity_wrapper) {
    $exclude[] = $entity_wrapper->value()->name;
  }
  $query = new EntityCollectionQuery();
  $result = $query->bundleCondition($bundles)
    ->entityCondition($entity_type, array($entity_id))
    ->collectionCondition($exclude, 'NOT IN')
    ->useContexts($contexts)
    ->execute();
  return array('collections' => entity_load('entity_collection', $result));
}