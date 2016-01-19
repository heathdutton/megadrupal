<?php

/**
 * @file
 * StudyroomReservation type editing UI.
 */

/**
 * UI controller.
 */
class StudyroomReservationTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage reservation types, including adding
    and removing fields and the display of fields.';
    return $items;
  }

  // All the following code can be removed when http://drupal.org/node/1437258
  // is applied to entity_api.
  // Allows access to variables.
  public $entityType;
  public $entityInfo;
  public $path;
  /**
   * Defines the delta for the weight of entities.
   */
  public $overviewDelta = 30;

  /**
   * Add the weight.
   */
  public function __construct($entity_type, $entity_info) {
    parent::__construct($entity_type, $entity_info);
    $this->weightKey = empty($this->entityInfo['entity keys']['weight']) ? 'weight' : $this->entityInfo['entity keys']['weight'];
  }

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    if (!empty($this->entityInfo['sortable'])) {
      // If the entity type supports weights we need a tabledrag form.
      $form = $this->overviewWeightForm($form, $form_state);
    }
    else {
      // By default just show a simple overview for all entities.
      $form['table'] = $this->overviewTable();
      $form['pager'] = array('#theme' => 'pager');
    }
    return $form;
  }

  /**
   * Builds the entity overview form.
   */
  public function overviewWeightForm($form, &$form_state, $conditions = array()) {
    $entities = $this->overviewQuery($conditions, FALSE);
    $rows = array();
    foreach ($entities as $entity) {
      $id = entity_id($this->entityType, $entity);
      $weight = isset($form_state['values']['table'][$id]['weight']) ? $form_state['values']['table'][$id]['weight'] : $entity->{$this->weightKey};
      $rows[$id]['#row'] = $this->overviewTableRow($conditions, $id, $entity);
      $rows[$id]['#weight'] = $weight;
      $rows[$id]['weight'] = array(
        '#type' => 'weight',
        '#delta' => $this->overviewDelta,
        '#default_value' => $weight,
        '#title' => t('Weight'),
        '#title-display' => 'invisible',
      );
    }

    $form['table'] = $rows + array(
      '#theme' => 'studyroom_reservation_type_overview_tabledrag_form',
      '#tree' => TRUE,
      '#header' => $this->overviewTableHeaders($conditions, $rows),
      '#entity_type' => $this->entityType,
    );

    // Only show the save button when there are entities in the list.
    if (!empty($rows)) {
      $form['actions']['#type'] = 'actions';
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
    }

    return $form;
  }

  /**
   * Submit handler for entity overview form.
   */
  public function overviewFormSubmit($form, &$form_state) {
    // Update entity weights.
    if ($this->entityInfo['sortable'] && !empty($form_state['values']['table'])) {
      $entities = entity_load($this->entityType, array_keys($form_state['values']['table']));
      foreach ($entities as $key => $entity) {
        $id = entity_id($this->entityType, $entity);
        $entity->{$this->weightKey} = $form_state['values']['table'][$id]['weight'];
        entity_save($this->entityType, $entities[$key]);
      }
    }
  }

  /**
   * Overview table.
   *
   * Generates the render array for a overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param array $conditions
   *   An array of conditions as needed by entity_load().
   *
   * @return array
   *   A renderable array.
   */
  public function overviewTable($conditions = array()) {
    $entities = $this->overviewQuery($conditions);

    $rows = array();
    foreach ($entities as $entity) {
      $id = entity_id($this->entityType, $entity);
      $rows[$id] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity);
    }

    $render = array(
      '#theme' => 'table',
      '#header' => $this->overviewTableHeaders($conditions, $rows),
      '#rows' => $rows,
      '#empty' => t('None.'),
    );
    return $render;
  }

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = $additional_header;
    array_unshift($header, t('Label'));
    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }
    if (!empty($this->entityInfo['sortable'])) {
      $header[] = t('Weight');
    }
    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount());
    return $header;
  }

  /**
   * Helper method for retrieving a list of entities for the overview table.
   */
  protected function overviewQuery($conditions = array(), $paged = TRUE) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($paged && $this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();
    ksort($entities);

    return $entities;
  }

  /**
   * Overview table row.
   *
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType,
      ),
    );

    // Add in any passed additional cols.
    foreach ($additional_cols as $col) {
      $row[] = $col;
    }

    // Add a row for the exportable status.
    if (!empty($this->entityInfo['exportable'])) {
      $row[] = array(
        'data' => array(
          '#theme' => 'entity_status',
          '#status' => $entity->{$this->statusKey},
        ),
      );
    }

    // Add a row for the weight.
    if (!empty($this->entityInfo['sortable'])) {
      // We need to explicitly define an array key here so we can replace it
      // with the actual weight dropdown in case we are displaying a tabledrag
      // form.
      $row['weight'] = $entity->{$this->weightKey};
    }

    // In case this is a bundle, we add links to the field ui tabs.
    $field_ui = !empty($this->entityInfo['bundle of']) && entity_type_is_fieldable($this->entityInfo['bundle of']) && module_exists('field_ui');
    // For exportable entities we add an export link.
    $exportable = !empty($this->entityInfo['exportable']);
    // If i18n integration is enabled, add a link to the translate tab.
    $i18n = !empty($this->entityInfo['i18n controller class']);

    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $row[] = array(
        'data' => l(t('clone'), $this->path . '/manage/' . $id . '/clone'), 'colspan' => $this->operationCount());
    }
    else {
      $row[] = l(t('edit'), $this->path . '/manage/' . $id);

      if ($field_ui) {
        $row[] = l(t('manage fields'), $this->path . '/manage/' . $id . '/fields');
        $row[] = l(t('manage display'), $this->path . '/manage/' . $id . '/display');
      }
      if ($i18n) {
        $row[] = l(t('translate'), $this->path . '/manage/' . $id . '/translate');
      }
      if ($exportable) {
        $row[] = l(t('clone'), $this->path . '/manage/' . $id . '/clone');
      }
      // Changed the lock check to isLocked function.
      if (empty($this->entityInfo['exportable']) || !$entity->isLocked()) {
        $row[] = l(t('delete'), $this->path . '/manage/' . $id . '/delete', array('query' => drupal_get_destination()));
      }
      elseif (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $row[] = l(t('revert'), $this->path . '/manage/' . $id . '/revert', array('query' => drupal_get_destination()));
      }
      else {
        $row[] = '';
      }
    }
    if ($exportable) {
      $row[] = l(t('export'), $this->path . '/manage/' . $id . '/export');
    }
    return $row;
  }
}

/**
 * Generates the studyroom_reservation type editing form.
 */
function studyroom_reservation_type_form($form, &$form_state, $type, $op = 'edit') {
  if ($op == 'clone') {
    $type->label .= ' (cloned)';
    $type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $type->label,
    '#description' => t('The human-readable name of this reservation type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($type->type) ? $type->type : '',
    '#maxlength' => 32,
    '#disabled' => $type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'studyroom_reservation_type_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this reservation type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($type->description) ? $type->description : '',
    '#description' => t('Describe this reservation type. The text will be displayed on the <em>Add new reservation</em> page.'),
  );

  $form['increment_limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Increment limits'),
    '#description' => t('Optionally limit the duration choices presented on the create reservation form.  Profiles
    will override these settings with min and max values allowed.'),
    '#tree' => TRUE,
  );

  $increment = variable_get('studyroom_time_increment', '15');
  $form['increment_limits']['minutes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit options after 4 hours to only hours (no minutes). This only applies if your minute increment is less than 60 minutes.'),
    '#default_value' => isset($type->increment_limits['minutes']) ? $type->increment_limits['minutes'] : 0,
    '#access' => ($increment == 60) ? FALSE : TRUE,
  );
  $form['increment_limits']['hours_large'] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit options after 8 hours to only 12, 18, and 24 hours.'),
    '#default_value' => isset($type->increment_limits['hours_large']) ? $type->increment_limits['hours_large'] : 0,
  );

  $form['min_duration'] = array(
    '#type' => 'select',
    '#title' => t('Minimum duration'),
    '#options' => $type->optionsMinDuration(),
    '#default_value' => isset($type->min_duration) ? $type->min_duration : $type->defaultMinDuration(),
    '#description' => t('The minimum length of time of a reservation.'),
    '#access' => $type->public,
  );
  $form['max_duration'] = array(
    '#type' => 'select',
    '#title' => t('Maximum duration'),
    '#options' => $type->optionsMaxDuration(),
    '#default_value' => isset($type->max_duration) ? $type->max_duration : $type->defaultMaxDuration(),
    '#description' => t('The maximum length of time of a reservation.'),
    '#access' => $type->public,
  );

  // Allow additional elements to be added to form.
  drupal_alter('studyroom_reservation_type_form', $form, $type);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save reservation type'),
    '#weight' => 40,
  );

  if (!$type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete reservation type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('studyroom_reservation_type_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Validate studyroom_reservation_type_form form.
 */
function studyroom_reservation_type_form_validate($form, &$form_state) {
  $invalid_types = array('add', 'edit', 'delete');
  if (in_array($form_state['values']['type'], $invalid_types)) {
    form_set_error('type', t('You can not use %types as machine names.', array('%types' => implode(', ', $invalid_types))));
  }
}

/**
 * Form API submit callback for the studyroom_reservation_type_form form.
 */
function studyroom_reservation_type_form_submit(&$form, &$form_state) {
  $type = entity_ui_form_submit_build_entity($form, $form_state);
  $type->save();
  $form_state['redirect'] = STUDYROOM_RESERVATION_TYPE_ADMIN_PATH;
}

/**
 * Form API submit callback for the delete button.
 */
function studyroom_reservation_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = STUDYROOM_RESERVATION_TYPE_ADMIN_PATH . '/manage/' . $form_state['studyroom_reservation_type']->type . '/delete';
}
