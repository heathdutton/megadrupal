<?php

/**
 * @file
 * Defines the inline entity form controller for entities.
 */

/**
 * Coppied from RedHen.
 */
abstract class EntitySpiceInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::labels().
   */
  public function labels() {
    return array(
      'singular' => t($this::_entity_type),
      'plural' => t($this::_entity_type . 's'),
    );
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($f, &$fs) {
    $entity = $f['#entity'];
    $info = _entityspice_getinfo($this::_entity_type);

    // Do some prep work on the entities, similarly to node_form().
    if ($info['has title']) {
     $key = $info['title key'];
     $label = $info['title label'];
     if(!isset($entity->{$key})) {
       $entity->{$key} = '';
     }
     $f[$key] = array(
       '#type' => 'textfield',
       '#title' => t($label),
       '#default_value' => $entity->{$key},
       '#maxlength' => 255,
       '#required' => TRUE,
       '#weight' => -6,
     );
    }

    field_attach_form($this::_entity_type, $entity, $f, $fs);

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$f, &$fs) {
    parent::entityFormSubmit($f, $fs);

    $entity = & $f['#entity'];
    $info = _entityspice_getinfo($this::_entity_type);

    // $contact->first_name = $contact->name['first_name'];
    // $contact->last_name = $contact->name['last_name'];

    // Set the entity's author uid.
    if($info['has uid']) {
      $entity->uid = $GLOBALS['user']->uid;
    }

    // Save the entity.
    $entity = entity_save($this::_entity_type, $entity);
  }
}
