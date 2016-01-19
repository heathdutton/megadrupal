<?php
/**
 * @file
 * 
 */
class entityDialogRelation implements entityDialogContribInterface {
  /**
   * Enable the relation entity to use Entity Dialog.
   */
  public static function entitiesToEnable() {
    return array('relation');
  }

  /**
   * Form callback for relation entities.
   */
  public static function formCallback($entity, $entity_type) {
    if ($entity->is_new) {
      // Add.
      $form = 'entity_dialog_relation_add_form';
    }
    else {
      $form = 'relation_edit_form';
    }

    return drupal_get_form($form, $entity);
  }
  
  /**
   * Provide the endpoints for building relation forms.
   */
/*
  public static function getFormDefaults($entity_type, $bundle, &$defaults) {
    // Get info about this relation type.
    $relations_info = relation_get_types(array($bundle));
    $bundle_info = $relations_info[$bundle];
    
    $source_type = arg(4);
    $source_id = arg(5);
    $target_type = arg(6);
    $target_id = arg(7);
    
    // See if the source and target bundles match those set for this relation type.
    $vars = array(
      'type' => $source_type,
      'id' => $source_id,
      'source_bundles' => $bundle_info->source_bundles,
      'directional' => $bundle_info->directional,
    );
    
    $matching_source = _ed_relation_matching_source_target($vars);
    
    $vars['type'] = $target_type;
    $vars['id'] = $target_id;
    $vars['target_bundles'] = $bundle_info->target_bundles;
    $matching_target = _ed_relation_matching_source_target($vars);
    
    if ($matching_source && $matching_target) {
      // We have a source and a target that is allowed for this relation type.
      $defaults['endpoints'] = array(
        array('entity_type' => $source_type, 'entity_id' => $source_id),
        array('entity_type' => $target_type, 'entity_id' => $target_id),
      );
    }
    else {
      // How do we bail out of this? We really want to cancel the whole thing because we
      // were passed a source and/or target that isn't allowed for this relation type 
      // so we should never try and build a form for it.
    } 
  }
*/
}

function entity_dialog_relation_add_form($form, &$form_state, $relation) {
  $form_state['relation'] = $relation;    
  
  // @TODO: Note that pulling the types and ids out of the url and just
  // shoving them in as endpoints is very ignorant of directional realtions and unique
  // relations. See _ed_relation_matching_source_target() and getFormDefaults() 
  // for an attempt at handling directional. Perhaps for existing unique relations
  // we can return the existing relation's edit form instead of the add form. 
  $relation->endpoints[LANGUAGE_NONE] = array(
    array('entity_type' => arg(4), 'entity_id' => arg(5)),
    array('entity_type' => arg(6), 'entity_id' => arg(7)),
  );

  // Attach Field API fields to the form
  field_attach_form('relation', $relation, $form, $form_state);
  
  // Build a submit button.
  $form['actions']['#weight'] = 100;
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('entity_dialog_relation_form_submit'),
    '#value' => t('Save'),
  );
  
  //$form['#submit'][] = 'entity_dialog_relation_form_submit';

  return $form;
}

/**
 * Work out if the source/target matches.
 *
 * Build a string in the form of 'type:bundle' for the incoming source and target to match 
 * against the relation info stored for the relation type we are wanting to add.
 */
/*
function _ed_relation_matching_source_target($vars){
  $entity = reset(entity_load($vars['type'], array($vars['id'])));
  list( , , $bundle) = entity_extract_ids($vars['type'], $entity);
  $type_bundle = $vars['type'] . ':' . $bundle;
  
  if ($vars['directional'] && !empty($vars['target_bundles'])) {
    // The relation is directional and there are target bundles.
    $source_or_target = 'target_bundles';
  }
  else {
    $source_or_target = 'source_bundles';
  }
  
  // A relation can be enabled for a specific bundle or all bundles. In the case of all
  // bundle the string will look like 'entity_type:*'.
  if ( in_array($type_bundle, $vars[$source_or_target]) 
      || in_array($vars['type'] . ':*', $vars[$source_or_target]) ) {
    $matching_source = TRUE;
  }
  else {
    $matching_source = FALSE;
  }
  
  return $matching_source;
}
*/


/**
 * Create a relation between the two chosen entities.
 *
 * We know this include file will be loaded because this function cannot be called
 * except by the formCallback() method in the class above, which autoloads this include.
 */
/*
function entity_dialog_relation_form_submit(&$form, &$form_state) {
  $relation = $form_state['relation'];
  field_attach_submit('relation', $relation, $form, $form_state);
  relation_save($relation);
}
*/

/**
 * this is a hack.
 */
/*
function entityDialogRelation_creation_callback($values, $entity_type) {
  return relation_create($values['relation_type'], $values['endpoints']);
}
*/
