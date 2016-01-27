<?php
namespace devtools\components\entity;

class Hooks extends \devtools\Hooks {
  /**
   * Extends hook_entity_load().
   */
  function hook_entity_load($entities, $type) {
    // add the entity type & id to all entity objects for easy data access
    // from other hooks & modules
    foreach ($entities as $entity) {
      // we don't want to add these entity fields when the entity is being exported into
      // a feature, otherwise the feature will always show as overridden and will not 
      // revert
      if (  preg_match('#admin/structure/features#', $_GET['q'])  // features page
              || 
            (                                                     // drush fu command
              isset($_SERVER['SCRIPT_FILENAME']) && 
              preg_match('/drush\.php$/', $_SERVER['SCRIPT_FILENAME']) &&
              isset($_SERVER['argv'][2]) &&
              $_SERVER['argv'][2] == 'fu'
            )
         ) {
        continue;
      }

      // set entity_type
        $entity_type_field = (!isset($entity->entity_type)) ? 'entity_type' : 'devtools_entity_type';
        $entity->{$entity_type_field} = $type;

      // set entity_id
        $entity_id_field = (!isset($entity->entity_id)) ? 'entity_id' : 'devtools_entity_id';
        list($entity->{$entity_id_field}, $vid, $bundle) = entity_extract_ids($type, $entity);
    }
  }
}