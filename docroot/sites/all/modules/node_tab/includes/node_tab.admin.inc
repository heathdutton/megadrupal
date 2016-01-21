<?php
/**
 * @file
 * Admin UI for tabs.
 */

/**
 * UI controller.
 */
class NodeTabUIController extends EntityDefaultUIController {

  public function __construct($entity_type, $entity_info) {
    $this->statusKey = 'status';
    $this->entityType = $entity_type;
    $this->entityInfo = $entity_info;
    // Stop the UI from mentioning "Node Tab" everywhere.
    $this->entityInfo['label'] = 'Tab';
    // Replace the node type placeholder with the node type name, since the path
    // is used for links and redirects.
    $this->path = str_replace('%node_type', arg(4), $this->entityInfo['admin ui']['path']);
  }

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = array();
    $path = 'admin/structure/types/manage/%/tabs';
    $id_pos = count(explode('/', $path));

    $items[$path] = array(
      'title' => 'Tabs',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('node_tab_overview_form', 'node_tab'),
      'description' => 'Manage tabs.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', 'node_tab'),
      'type' => MENU_LOCAL_TASK,
      'file' => 'includes/entity.ui.inc',
      'weight' => 9,
    );
    $items[$path . '/add'] = array(
      'title' => 'Add tab',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array('node_tab', NULL, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', 'node_tab'),
      'type' => MENU_LOCAL_ACTION,
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => drupal_get_path('module', 'node_tab'),
    );
    $items[$path . '/%entity_object'] = array(
      'title' => 'Edit',
      'title callback' => 'entity_label',
      'title arguments' => array('node_tab', $id_pos),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array('node_tab', $id_pos),
      'load arguments' => array('node_tab'),
      'access callback' => 'entity_access',
      'access arguments' => array('update', 'node_tab', $id_pos),
      'file' => $this->entityInfo['admin ui']['file'],
      'file path' => drupal_get_path('module', 'node_tab'),
    );
    $items[$path . '/%entity_object/edit'] = array(
      'title' => 'Edit',
      'load arguments' => array('node_tab'),
      'type' => MENU_DEFAULT_LOCAL_TASK,
    );
    $items[$path . '/%entity_object/%'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array('node_tab_operation_form', 'node_tab', $id_pos, $id_pos + 1),
      'load arguments' => array('node_tab'),
      'access callback' => 'entity_access',
      'access arguments' => array('delete', 'node_tab', $id_pos),
      'file' => 'includes/entity.ui.inc',
    );
    $items[$path . '/import'] = array(
      'title' => 'Import tab',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('node_tab_operation_form', 'node_tab', NULL, 'import'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', 'node_tab'),
      'file' => 'includes/entity.ui.inc',
      'type' => MENU_LOCAL_ACTION,
    );

    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTable().
   *
   * Limits tabs to those belonging to the current node type, and makes sure
   * they are ordered by weight (by not resorting $entities like the parent
   * method does).
   */
  public function overviewTable($conditions = array()) {
    // Limit tabs to only the current node type, order them by weight and name.
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $this->entityType)
      ->propertyCondition('node_type', str_replace('-', '_', arg(4)))
      ->propertyOrderBy('weight', 'ASC')
      ->propertyOrderBy('name', 'ASC');

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();
    $rows = array();
    foreach ($entities as $entity) {
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity);
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
   * Overrides EntityDefaultUIController::overviewTableRow().
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row[] = array('data' => array(
      '#theme' => 'entity_ui_overview_item',
      '#label' => entity_label($this->entityType, $entity),
      '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
      '#url' => $entity_uri ? $entity_uri : FALSE,
      '#entity_type' => $this->entityType),
    );

    // Add in any passed additional cols.
    foreach ($additional_cols as $col) {
      $row[] = $col;
    }

    // Add a row for the exportable status.
    $row[] = array('data' => array(
      '#theme' => 'entity_status',
      '#status' => $entity->{$this->statusKey},
    ));
    // If i18n integration is enabled, add a link to the translate tab.
    $i18n = !empty($this->entityInfo['i18n controller class']) && module_exists('i18n_string');

    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $row[] = array('data' => '', 'colspan' => $this->operationCount());
    }
    else {
      $row[] = l(t('edit'), $this->path . '/' . $id);
      if ($i18n) {
        $row[] = l(t('translate'), $this->path . '/' . $id . '/translate');
      }

      if (!entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)) {
        $row[] = l(t('delete'), $this->path . '/' . $id . '/delete', array('query' => drupal_get_destination()));
      }
      elseif (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $row[] = l(t('revert'), $this->path . '/' . $id . '/revert', array('query' => drupal_get_destination()));
      }
      else {
        $row[] = '';
      }
    }

    $row[] = l(t('export'), $this->path . '/' . $id . '/export');
    return $row;
  }
}

/**
 * Generates the tab editing form.
 */
function node_tab_form($form, &$form_state, $tab, $op = 'edit') {
  // Make sure the parent node_type is present in form state.
  if (!isset($form_state['node_type'])) {
    $form_state['node_type'] = str_replace('-', '_', arg(4));
  }
  // Set the node_type on new tab entities.
  if (empty($tab->node_type)) {
    $tab->node_type = $form_state['node_type'];
  }

  $form['node_type'] = array(
    '#type' => 'value',
    '#value' => $form_state['node_type'],
  );
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $tab->label,
    '#description' => t('Shown on the tab.'),
    '#required' => TRUE,
    '#weight' => -1,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $tab->name,
    '#disabled' => entity_has_status('node_tab', $tab, ENTITY_IN_CODE),
    '#machine_name' => array(
      'exists' => 'node_tab_exists',
      'source' => array('label'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#description' => t('A unique machine-readable name for this tab. Used for the tab URL.'),
    '#weight' => -2,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => !empty($tab->weight) ? $tab->weight : 0,
    '#delta' => 15,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save tab'),
    '#weight' => 40,
  );

  if ($op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete tab'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('node_tab_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function node_tab_form_submit(&$form, &$form_state) {
  $tab = entity_ui_form_submit_build_entity($form, $form_state);
  $tab->save();

  $form_state['redirect'] = 'admin/structure/types/manage/' . $form_state['node_type'] . '/tabs';
}

/**
 * Form API submit callback for the delete button.
 */
function node_tab_form_submit_delete(&$form, &$form_state) {
  $node_type = arg(4);
  $form_state['redirect'] = 'admin/structure/types/manage/' . $node_type
                             . '/tabs/' . $form_state['node_tab']->tab_id . '/delete';
}

/**
 * Check whether a given tab exists.
 *
 * @param $name
 *   The name of the tab.
 * @param $element
 *   The name form element array.
 * @param $form_state
 *   Form state.
 *
 * @return
 *   TRUE if the given tab exists. FALSE otherwise.
 */
function node_tab_exists($name, $element, $form_state) {
  return node_tab_load($form_state['node_type'], $name);
}
