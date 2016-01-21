<?php

/**
 * @file
 * Entity types added with eck module.
 */

class entityDialogECK implements entityDialogContribInterface {
  /**
   * Entity types added with eck module don't have a form callback so 
   * we need to add it manually.
   */
  public static function entitiesToEnable() {
    // eck_entity_info() should do this for me but I'm stuffed if I could get it to work.
    $eck_entity_types = array();
    foreach (entity_get_info() as $entity_type => $info) {
      if (isset($info['module']) &&  $info['module'] == 'eck') {
        $eck_entity_types[] = $entity_type;
      }
    }
    
    return $eck_entity_types;
  }
  
/**
 * Form callback for eck entities.
 *
 * entity_form() doesn't call the entity's form callback with the same args as what 
 * is expected by eck__entity__add() so we need to call it ourselves.
 */
  public static function formCallback($entity, $entity_type) {
    if (module_exists('eck')) {
      if (isset($entity->id)) {
        $form = eck__entity__edit($entity_type, $entity->type, $entity->id);
      }
      else {
        $form = eck__entity__add($entity_type, $entity->type);
      }
    }
    
    return isset($form) ? $form : NULL;
  }
  
}
