<?php

/**
 * @file
 * Defines the inline entity form controller for Entityforms.
 */

class EntityformInlineEntityFormController extends EntityInlineEntityFormController {

   private $type;

   /**
    * Overiddes EntityInlineEntityFormController::__constructor().
    */
   public function __construct($entityType, array $settings) {
     parent::__construct($entityType, $settings);

     // Stash the entityform_type;
     $info = entity_get_info($this->entityType);
     $this->type = key($info['bundles']);
   }

  /**
   * Overrides EntityInlineEntityFormController::labels().
   */
  public function labels() {
    $labels = array(
      'singular' => t('entityform'),
      'plural' => t('entityforms'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {

    // Check if we're creating a new submission or editing an existing one.
    if (!empty($entity_form['#entity']->is_new)) {
      module_load_include('inc', 'entityform', 'entityform.admin');
      $entityform = entityform_empty_load($this->type);
    }
    else {
      $entityform = entityform_load($entity_form['#entity']->entityform_id);
      // If editing, the entityform['#entity]->type is overwritten by the
      // parent entity type. Override it here and in the submit handler, so
      // data is saved properly.
      $entityform->type = $this->type;
    }

    $form_state['entityform_form_mode'] = 'submit';
    $form_state['entityform_form_context'] = 'embed';
    $form_state['entityform'] = $entityform;

    $langcode = entity_language('entityform', $entityform);
    field_attach_form('entityform', $entityform, $entity_form, $form_state, $langcode);

    // A unique string identifying the inline entityform form.
    $form_id = $this->type . '_' . $entity_form['#bundle'];

    // Invoke hook_inline_entityform_form_alter(), hook_inline_entityform_form_BASE_FORM_ID_alter()
    // and hook_inline_entityform_form_FORM_ID_alter() implementations.
    $hooks = array('inline_entityform_form');
    if (isset($form_state['build_info']['base_form_id'])) {
      $hooks[] = 'inline_entityform_form_' . $form_state['build_info']['base_form_id'];
    }
    $hooks[] = 'inline_entityform_form_' . $form_id;
    drupal_alter($hooks, $entity_form, $form_state, $form_id, $form_state['build_info']['base_form_id']);

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    parent::entityFormSubmit($entity_form, $form_state);

    // Notify field widgets -> attach the $form_state['values'] to the $entityform object
    field_attach_submit('entityform', $form_state['entityform'], $entity_form, $form_state);

    // Call the submit handler, which saves a draft of the form.
    $entityform = entity_ui_controller('entityform')->entityFormSubmitBuildEntity($entity_form, $form_state);

    // Make sure the correct type is set. Again. *eyeroll*
    $entityform->type = $this->type;

    // Get back into the form.
    $entity_form['#entity'] = $entityform;
  }

  /**
   * Overrides EntityInlineEntityFormController::save().
   */
  public function save($entity, $context) {
    // Add in created and changed times.
    if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
      global $user;
      $entity->created = time();
      $entity->uid = !empty($user->uid) ? $user->uid : 0;
    }
    $entity->changed = time();

    // Save!
    $entity->save();
  }
}
