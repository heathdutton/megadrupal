<?php

/**
 * UI Controller for remote translation sources.
 */
class TMGMTRemoteSourceUIController extends TMGMTDefaultSourceUIController {

  /**
   * Overrides TMGMTDefaultSourceUIController::hook_menu().
   */
  public function hook_menu() {
    // The remote source overview is a View using Views Bulk Operations.
    // Therefore we don't need to provide any menu items.
    return array();
  }

  /**
   * Overrides TMGMTDefaultSourceUIController::hook_form().
   */
  public function hook_forms() {
    // The remote source overview is a View using Views Bulk Operations.
    // Therefore we don't need to provide any forms.
    return array();
  }

  /**
   * Overrides TMGMTDefaultSourceUIController::hook_views_default_views().
   */
  public function hook_views_default_views() {
    return _tmgmt_load_exports('tmgmt_server', 'views', 'view.inc', 'view');
  }

}

/**
 * Administrative interface for remote client entities.
 */
class TMGMTClientUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Turn the main menu callback into a local task and shorten the title.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items[$this->path]['title'] = 'Clients';
    $items[$this->path]['weight'] = 10;
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTable().
   *
   * @todo Create a Views / Views Bulk Operations based Entity API UI Controller
   * and make use of that instead of custom code.
   */
  public function overviewTable($conditions = array()) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

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
    ksort($entities);

    $rows = array();
    foreach ($entities as $entity) {
      $user = theme('username', array('account' => user_load($entity->uid)));
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity, array($user));
    }

    $render = array(
      '#theme' => 'table',
      '#header' => $this->overviewTableHeaders($conditions, $rows, array(t('Owner'))),
      '#rows' => $rows,
      '#empty' => t('None.'),
    );

    return $render;
  }
}
