<?php
//@codingStandardsIgnoreFile

/**
 * The bulk of the methods in this class are identical to the logic of the
 * parent class except for the hunks between @start-no-op and @end-no-op. We
 * have marked the whole file as @codingStandardsIgnoreFile in order to prevent
 * errors being raised from those hunks from the parent class that don't meet
 * coding standards. Instead of fixing them, we leave them as is so that if the
 * parent class changes, we can easily get a diff and apply it to this class
 * without trivial coding-standards interfering in the diff. For the same reason
 * we've left some multi-line hunks from the parent class commented out - which
 * would also fail coding-standards.
 */

/**
 * @file
 * Contains null storage controller for entity forms.
 */


/**
 * Class EntityFormNullStorage overrides default storage.
 */
class EntityFormNullStorage extends EntityFormController {

  /**
   * Overwrites base implementation to perform a null op.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $entityform_type = entityform_type_load($entity->type);
    if (empty($entityform_type->data['entityform_null_storage'])) {
      return parent::save($entity, $transaction);
    }
    $transaction = isset($transaction) ? $transaction : db_transaction();
    try {
      // Load the stored entity, if any.
      if (!empty($entity->{$this->idKey}) && !isset($entity->original)) {
        // In order to properly work in case of name changes, load the original
        // entity using the id key if it is available.
        $entity->original = entity_load_unchanged($this->entityType, $entity->{$this->idKey});
      }
      $entity->is_new = !empty($entity->is_new) || empty($entity->{$this->idKey});
      $this->invoke('presave', $entity);

      if ($entity->is_new) {
        // @start-no-op.
        $return = SAVED_NEW;
        // @end-no-op.
        if ($this->revisionKey) {
          $this->saveRevision($entity);
        }
        $this->invoke('insert', $entity);
      }
      else {
        // Update the base table if the entity doesn't have revisions or
        // we are updating the default revision.
        if (!$this->revisionKey || !empty($entity->{$this->defaultRevisionKey})) {
          // @start-no-op.
          $return = SAVED_UPDATED;
          // @end-no-op.
        }
        if ($this->revisionKey) {
          $return = $this->saveRevision($entity);
        }
        $this->resetCache(array($entity->{$this->idKey}));
        $this->invoke('update', $entity);

        // Field API always saves as default revision, so if the revision saved
        // is not default we have to restore the field values of the default
        // revision now by invoking field_attach_update() once again.
        if ($this->revisionKey && !$entity->{$this->defaultRevisionKey} && !empty($this->entityInfo['fieldable'])) {
          // @start-no-op.
          // field_attach_update($this->entityType, $entity->original);
          // @end-no-op.
        }
      }

      // Ignore slave server temporarily.
      db_ignore_slave();
      unset($entity->is_new);
      unset($entity->is_new_revision);
      unset($entity->original);

      return $return;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }

  /**
   * Saves an entity revision.
   *
   * @param Entity $entity
   *   Entity revision to save.
   */
  protected function saveRevision($entity) {
    $entityform_type = entityform_type_load($entity->type);
    if (empty($entityform_type->data['entityform_null_storage'])) {
      return parent::saveRevision($entity);
    }
    // Convert the entity into an array as it might not have the same properties
    // as the entity, it is just a raw structure.
    $record = (array) $entity;
    // File fields assumes we are using $entity->revision instead of
    // $entity->is_new_revision, so we also support it and make sure it's set to
    // the same value.
    $entity->is_new_revision = !empty($entity->is_new_revision) || !empty($entity->revision) || $entity->is_new;
    $entity->revision = &$entity->is_new_revision;
    $entity->{$this->defaultRevisionKey} = !empty($entity->{$this->defaultRevisionKey}) || $entity->is_new;



    // When saving a new revision, set any existing revision ID to NULL so as to
    // ensure that a new revision will actually be created.
    if ($entity->is_new_revision && isset($record[$this->revisionKey])) {
      $record[$this->revisionKey] = NULL;
    }

    if ($entity->is_new_revision) {
      // @start-no-op.
      // drupal_write_record($this->revisionTable, $record);
      // @end-no-op.
      $update_default_revision = $entity->{$this->defaultRevisionKey};
    }
    else {
      // @start-no-op.
      // drupal_write_record($this->revisionTable, $record, $this->revisionKey);
      // @end-no-op.
      // @todo: Fix original entity to be of the same revision and check whether
      // the default revision key has been set.
      $update_default_revision = $entity->{$this->defaultRevisionKey} && $entity->{$this->revisionKey} != $entity->original->{$this->revisionKey};
    }
    // Make sure to update the new revision key for the entity.
    $entity->{$this->revisionKey} = $record[$this->revisionKey];

    // Mark this revision as the default one.
    if ($update_default_revision) {
      // @start-no-op.
      /* db_update($this->entityInfo['base table'])
        ->fields(array($this->revisionKey => $record[$this->revisionKey]))
        ->condition($this->idKey, $entity->{$this->idKey})
        ->execute(); */
      // @end-no-op.
    }
    return $entity->is_new_revision ? SAVED_NEW : SAVED_UPDATED;
  }

  /**
   * Override to prevent storage hooks.
   */
  public function invoke($hook, $entity) {
    $entityform_type = entityform_type_load($entity->type);
    if (empty($entityform_type->data['entityform_null_storage'])) {
      return parent::invoke($hook, $entity);
    }
    // entity_revision_delete() invokes hook_entity_revision_delete() and
    // hook_field_attach_delete_revision() just as node module does. So we need
    // to adjust the name of our revision deletion field attach hook in order to
    // stick to this pattern.

    // @start-no-op
    /* $field_attach_hook = ($hook == 'revision_delete' ? 'delete_revision' : $hook);
    if (!empty($this->entityInfo['fieldable']) && function_exists($function = 'field_attach_' . $field_attach_hook)) {
      $function($this->entityType, $entity);
    }*/
    // @end-no-op

    if (!empty($this->entityInfo['bundle of']) && entity_type_is_fieldable($this->entityInfo['bundle of'])) {
      $type = $this->entityInfo['bundle of'];
      // Call field API bundle attachers for the entity we are a bundle of.
      if ($hook == 'insert') {
        field_attach_create_bundle($type, $entity->{$this->bundleKey});
      }
      elseif ($hook == 'delete') {
        field_attach_delete_bundle($type, $entity->{$this->bundleKey});
      }
      elseif ($hook == 'update' && $entity->original->{$this->bundleKey} != $entity->{$this->bundleKey}) {
        field_attach_rename_bundle($type, $entity->original->{$this->bundleKey}, $entity->{$this->bundleKey});
      }
    }
    // Invoke the hook.
    module_invoke_all($this->entityType . '_' . $hook, $entity);
    // Invoke the respective entity level hook.
    if ($hook == 'presave' || $hook == 'insert' || $hook == 'update' || $hook == 'delete') {
      module_invoke_all('entity_' . $hook, $entity, $this->entityType);
    }
    // Invoke rules.
    if (module_exists('rules')) {
      rules_invoke_event($this->entityType . '_' . $hook, $entity);
    }
  }

}
