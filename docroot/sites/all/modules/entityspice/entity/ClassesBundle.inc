<?php

/**
 * The controller class for bundle entities contains methods for CRUD
 * operations.
 */
abstract class EntitySpiceEntityBundleController extends EntityAPIControllerExportable {
  /**
   * Delete one or more entity bundles.
   *
   * Overrides EntityAPIControllerExportable::delte().
   *
   * @param array $ids
   *   Array of entity bundle IDs.
   * @param DatabaseTransaction $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $transaction = isset($transaction) ? $transaction : db_transaction();
    try {
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', $this::_entity_type)
        ->entityCondition('bundle', $ids, 'IN')
        ->execute();

      if ($result) {
        $ids = array_keys($result[$this::_entity_type]);
        _entityspice_entity_delete_multiple($this::_entity_type, $ids);
      }

      parent::delete($ids, $transaction);
      menu_rebuild();
    }
    catch(Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }
}

/*
 * The class used for bundle entities.
 */
abstract class EntitySpiceEntityBundle extends Entity {
  /**
   * Override parent constructor to set entity type.
   *
   * @param array $values
   *   Default entity values.
   */
  public function __construct(array $values = array()) {
    $name = _entityspice_getinfo($this::_entity_type, 'bundle machine name');
    parent::__construct($values, $name);
  }
}

/**
 * Entity type UI controller.
 */
abstract class EntitySpiceEntityBundleUIController extends EntityDefaultUIController {
  /**
   * Override parent::operationForm to set a more meaningful message on delete.
   *
   * Delegates to parent on other opetations.
   *
   * @FIXME when bundle does not have label.
   */
  public function operationForm($form, &$form_state, $entity, $op) {
    switch ($op) {
      case 'delete':
        $label = entity_label($this->entityType, $entity);
        $targ = array('%entity' => $this->entityInfo['label'], '%label' => $label);
        $confirm_question = t('Are you sure you want to delete the %entity %label? ', $targ);
        $desc = t('This action will also delete <strong>all</strong> entities of type %label and cannot be undone.', $targ);
        return confirm_form($form, $confirm_question, $this->path, $desc);

      default:
        return parent::operationForm($form, $form_state, $entity, $op);
    }
  }
}
