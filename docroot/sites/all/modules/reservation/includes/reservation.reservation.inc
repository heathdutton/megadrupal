<?php

/**
 * @file
 * Function and Class definitions for Reservations.
 */

/**
 * The class used for Reservation entities.
 */
class Reservation extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'reservation');
  }

  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array('path' => 'reservation/' . $this->reservation_id);
  }
}

/**
 * The Controller for RoomReserveRoom entities
 */
class ReservationController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    $values += array(
      'reservation_id' => '',
      'unit' => '',
      'title' => '',
      'uid' => '',
      'status' => RESERVATION_NOT_APPROVED,
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);

    return $content;
  }

  public function view($reservations, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    field_attach_prepare_view('reservation', $reservations, $view_mode);
    entity_prepare_view('reservation', $reservations);
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    $view = array();
    foreach ($reservations as $reservation) {
      $build = entity_build_content('reservation', $reservation, $view_mode, $langcode);
      $build += array(
        '#theme' => 'reservation',
        '#reservation' => $reservation,
        '#view_mode' => $view_mode,
        '#language' => $langcode,
      );
      // Allow modules to modify the structured entity.
      drupal_alter(array('reservation_view', 'entity_view'), $build, $this->entityType);
      $key = isset($reservation->reservation_id) ? $reservation->reservation_id : NULL;
      $view[$key] = $build;
    }

    return $view;
  }
}

/**
 * UI controller.
 */
class ReservationUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    $wildcard = $this->entityInfo['admin ui']['menu wildcard'];
    $include_path = drupal_get_path('module', $this->entityInfo['module']);

    $items[$this->path] = array(
      'title' => 'Reservations',
      'description' => 'Administer reservations.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('reservation_overview_form'),
      'access arguments' => array('access administration pages'),
      'weight' => -10,
      'file path' => $include_path,
      'file' => 'reservation.admin.inc',
    );

    $items['reservation/add'] = array(
      'title' => 'Add reservation',
      'description' => 'Add a new reservation',
      'page callback'  => 'reservation_add_page',
      'access callback'  => 'reservation_access',
      'access arguments' => array('create'),
      'weight' => 20,
      'file path' => $include_path,
      'file' => 'reservation.admin.inc',
    );

    foreach (reservation_get_units() as $unit) {
      $reservation = reservation_create(array('unit' => $unit->unit));
      $items['reservation/add/' . $unit->unit] = array(
        'title' => $unit->name,
        'description' => $unit->description,
        'page callback' => 'drupal_get_form',
        'page arguments' => array('reservation_form', $reservation),
        'access callback' => 'reservation_access',
        'access arguments' => array('create'),
        'file path' => $include_path,
        'file' => 'reservation.admin.inc',
      );
    }

    $items['reservation/%reservation'] = array(
      'title callback' => 'reservation_page_title',
      'title arguments' => array(1),
      'page callback' => 'reservation_page_view',
      'page arguments' => array(1),
      'access callback' => 'reservation_access',
      'access arguments' => array('view', 1),
    );

    $items['reservation/%reservation/view'] = array(
      'title' => 'View',
      'weight' => -10,
      'type' => MENU_DEFAULT_LOCAL_TASK,
    );

    $items['reservation/%reservation/edit'] = array(
      'title' => 'Edit',
      'description' => 'Edit a reservation',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('reservation_form', 1),
      'access callback' => 'reservation_access',
      'access arguments' => array('edit', 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file path' => $include_path,
      'file' => 'reservation.admin.inc',
    );

    $items['reservation/%reservation/reassign'] = array(
      'title' => 'Reassign',
      'description' => 'Reassign a reservation to another unit',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('reservation_reassign_form', 1),
      'access callback' => 'reservation_access',
      'access arguments' => array('edit', 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file path' => $include_path,
      'file' => 'reservation.admin.inc',
    );

    $items['reservation/%reservation/delete'] = array(
      'title' => 'Delete',
      'description' => 'Delete a reservation',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('reservation_delete_confirm', 1),
      'access callback' => 'reservation_access',
      'access arguments' => array('delete', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'file path' => $include_path,
      'file' => 'reservation.admin.inc',
    );

    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm().
   */
  public function overviewForm($form, &$form_state) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update options'),
      '#attributes' => array('class' => array('container-inline')),
      '#access' => user_access('administer reservations'),
    );
    $options = array();
    foreach (reservation_operations() as $operation => $array) {
      $options[$operation] = $array['label'];
    }
    $form['options']['operation'] = array(
      '#type' => 'select',
      '#title' => t('Operation'),
      '#title_display' => 'invisible',
      '#options' => $options,
      '#default_value' => 'approve',
    );
    $form['options']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#validate' => array('reservation_overview_form_validate'),
      '#submit' => array('reservation_overview_form_submit'),
    );

    $form['reservations'] = $this->overviewTable();
    $form['pager'] = array('#theme' => 'pager');

    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTable().
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().
   *
   * @return array
   *   A renderable array.
   */
  public function overviewTable($conditions = array()) {
    $header = array(
      'title' => array(
        'data' => t('Title'),
        'type' => 'property',
        'specifier' => 'title',
      ),
      'type' => array(
        'data' => t('Unit'),
        'type' => 'property',
        'specifier' => 'unit',
      ),
      'user' => array(
        'data' => t('User'),
        'type' => 'property',
        'specifier' => 'user',
      ),
      'status' => array(
        'data' => t('Status'),
        'type' => 'property',
        'specifier' => 'status',
      ),
      'updated' => array(
        'data' => t('Updated'),
        'type' => 'property',
        'specifier' => 'changed',
        'sort' => 'desc', # sort by this column by default
      ),
      'operations' => array(
        'data' => t('Operations'),
      ),
    );

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $query->tableSort($header);
    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $reservations = $ids ? entity_load($this->entityType, $ids) : array();

    $rows = array();
    foreach ($reservations as $reservation) {
      $rows[$reservation->reservation_id] = $this->overviewTableRow($conditions, $reservation->reservation_id, $reservation);
    }

    $render = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('None.'),
    );

    return $render;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableRow().
   *
   * @param $additional_cols
   *   Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row['title'] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType,
      ),
    );
    $row['type'] = reservation_unit_get_name($entity->unit);
    $row['user'] = theme('username', array('account' => user_load($entity->uid)));
    $row['status'] = $entity->status ? t('Approved') : t('Not approved');
    $row['updated'] = format_date($entity->changed, 'short');
    $row['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => array(
          'edit' => array(
            'title' => t('edit'),
            'href' => 'reservation/' . $id . '/edit',
          ),
          'delete' => array(
            'title' => t('delete'),
            'href' => 'reservation/' . $id . '/delete',
          ),
        ),
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );

    return $row;
  }

  /**
   * Create the add Reservations page.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('reservation_add_list', array('content' => $content));
  }
}
