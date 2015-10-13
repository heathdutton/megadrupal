<?php

class treefield_views_handler_field_draggable extends views_handler_field {

  function post_execute(&$values) {
    // Compute the depth of each item.
    // @todo: we need a depth field in the database.
    $field = field_info_field($this->definition['field api']['field_name']);
    $behaviors = entityreference_get_behavior_handlers($field);

    $tree_behavior = NULL;
    foreach ($behaviors as $behavior) {
      if ($behavior instanceof TreeFieldEntityReferenceBehavior) {
        $tree_behavior = $behavior;
        break;
      }
    }
    if (!isset($tree_behavior)) {
      throw new TreeFieldMissingProviderException();
    }

    $storage = $behavior->storage($field);
    $provider = $behavior->provider($field);
    $column_map = $storage->getColumnMap();
    foreach ($values as $row_index => $row) {
      // Build an item.
      $item = new Tree_Storage_SQL_Item(array(
        $column_map['id'] => $row->{$this->aliases['id']},
        $column_map['parent'] => $row->{$this->aliases['parent']},
        $column_map['weight'] => $row->{$this->aliases['weight']},
      ), $column_map);
      $row->{$this->field_alias . '_depth'} = count($provider->ancestorsOf($item)->execute());
    }
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * The form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    foreach ($this->view->result as $row_index => $row) {
      if (!isset($form[$this->options['id']])) {
        $form[$this->options['id']] = array('#tree' => TRUE);
      }

      $form[$this->options['id']][$row_index]['id'] = array(
        '#type' => 'hidden',
        '#value' => (int) $row->{$this->aliases['id']},
        '#size' => 3,
        '#attributes' => array('class' => array('treefield-id')),
      );
      $form[$this->options['id']][$row_index]['parent'] = array(
        '#type' => 'textfield',
        '#default_value' => (int) $row->{$this->aliases['parent']},
        '#size' => 3,
        '#attributes' => array('class' => array('treefield-parent')),
      );
      $form[$this->options['id']][$row_index]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => (int) $row->{$this->aliases['weight']},
        '#size' => 3,
        '#attributes' => array('class' => array('treefield-weight')),
      );
    }
  }

  function views_form_submit($form, &$form_state) {
    // Save the entities in reverse tree order of the original tree,
    // so that every move is valid.
    $transaction = db_transaction();
    try {
      foreach (array_reverse($form_state['values'][$this->options['id']]) as $row_index => $values) {
        $entity_type = $this->definition['field api']['settings']['target_type'];
        $entity = entity_load_single($entity_type, $values['id']);

        if (empty($values['parent'])) {
          $values['parent'] = NULL;
        }

        $field_item = &$entity->{$this->definition['field api']['field_name']}[LANGUAGE_NONE][0];
        $save = FALSE;
        if (!isset($field_item['target_id']) || $field_item['target_id'] != $values['parent']) {
          $field_item['target_id'] = $values['parent'];
          $save = TRUE;
        }
        if (!isset($field_item['weight']) || $field_item['weight'] != $values['weight']) {
          $field_item['weight'] = $values['weight'];
          $save = TRUE;
        }
        if ($save) {
          entity_save($entity_type, $entity);
        }
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('tree', $e);
      throw $e;
    }
  }
}
