<?php
/**
 * Bookit Item translation handler.
 *
 * @see bookit_item_entity_info()
 */
class EntityTranslationBookitItemHandler extends EntityTranslationDefaultHandler {

  public function __construct($entity_type, $entity_info, $entity) {
    parent::__construct('bookit_item', $entity_info, $entity);
  }

  /**
   * Indicates whether this bookable item is a revision or not.
   */
  public function isRevision() {
    return !empty($this->entity->revision);
  }

  /**
   * Checks whether the current user has access to this bookable item.
   */
  public function getAccess($op) {
    return bookit_item_access($op, $this->entity);
  }

  /**
   * Tweaks the bookable item entity form to support multilingual elements.
   */
  public function entityForm(&$form, &$form_state) {
    parent::entityForm($form, $form_state);
    if (isset($form['change_history']['#weight'])) {
      $form['translation']['#weight'] = $form['change_history']['#weight'] - 0.01;
    }
    $form['actions']['delete_translation']['#suffix'] = $form['actions']['submit']['#suffix'];
    unset($form['actions']['submit']['#suffix']);
  }

  /**
   * @see EntityTranslationDefaultHandler::entityFormTitle()
   */
  protected function entityFormTitle() {
    return bookit_item_item_title($this->entity);
  }

  /**
   * Returns whether the bookable item is active (TRUE) or disabled (FALSE).
   */
  protected function getStatus() {
    return (boolean) $this->entity->status;
  }
}
