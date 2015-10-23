<?php
/**
 * Booking option translation handler.
 *
 * @see bookit_option_entity_info()
 */
class EntityTranslationBookitOptionHandler extends EntityTranslationDefaultHandler {

  public function __construct($entity_type, $entity_info, $entity) {
    parent::__construct('bookit_option', $entity_info, $entity);
  }

  /**
   * Indicates whether this bookit_option is a revision or not.
   */
  public function isRevision() {
    return !empty($this->entity->revision);
  }

  /**
   * Checks whether the current user has access to this bookit_option.
   */
  public function getAccess($op) {
    return bookit_option_access($op, $this->entity);
  }

  /**
   * Tweaks the booking option form to support multilingual elements.
   */
  public function entityForm(&$form, &$form_state) {
    parent::entityForm($form, $form_state);
    $form['actions']['delete_translation']['#suffix'] = $form['actions']['submit']['#suffix'];
    unset($form['actions']['submit']['#suffix']);
  }

  /**
   * @see EntityTranslationDefaultHandler::entityFormTitle()
   */
  protected function entityFormTitle() {
    return bookit_option_option_title($this->entity);
  }

  /**
   * Returns whether the bookit_option is active (TRUE) or disabled (FALSE).
   */
  protected function getStatus() {
    return (boolean) $this->entity->status;
  }
}
