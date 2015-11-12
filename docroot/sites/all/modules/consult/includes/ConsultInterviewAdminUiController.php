<?php
/**
 * @file
 * Interview admin controller class.
 */

/**
 * Class ConsultInterviewAdminUiController.
 *
 * Interview admin controller class.
 */
class ConsultInterviewAdminUiController extends EntityDefaultUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = array();
    // Set this on the object so classes that extend hook_menu() can use it.
    $this->id_count = count(explode('/', $this->path));

    $items['admin/structure/consult'] = array(
      'title' => t('Consult'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $this->entityType . '_overview_form',
        $this->entityType,
      ),
      'description' => 'Manage consult.',
      'access callback' => 'entity_access',
      'access arguments' => array('administer', $this->entityType),
      'file' => 'includes/entity.ui.inc',
    );
    $items['admin/structure/consult/list'] = array(
      'title' => 'List',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items['admin/structure/consult/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, NULL, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
    );
    $items['admin/structure/consult/manage/%' . $this->entityType] = array(
      'title' => 'Edit',
      'title callback' => 'entity_label',
      'title arguments' => array($this->entityType, 4),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, 4),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, 4),
    );
    $items['admin/structure/consult/manage/%' . $this->entityType . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
    );

    // Menu item for operations like revert and delete.
    $items['admin/structure/consult/manage/%' . $this->entityType . '/delete'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        $this->entityType . '_operation_form',
        $this->entityType,
        4,
        5,
      ),
      'access callback' => 'entity_access',
      'access arguments' => array('delete', $this->entityType, 4),
      'file' => 'includes/entity.ui.inc',
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/question/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', CONSULT_QUESTION_ENTITY_NAME),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_question_form', NULL, 4, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', CONSULT_QUESTION_ENTITY_NAME),
      'type' => MENU_LOCAL_ACTION,
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/question/edit/%' . CONSULT_QUESTION_ENTITY_NAME] = array(
      'title callback' => 'entity_label',
      'title arguments' => array(CONSULT_QUESTION_ENTITY_NAME, 7),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_question_form', 7, 4),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, 4),
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/question/delete/%' . CONSULT_QUESTION_ENTITY_NAME] = array(
      'title callback' => 'entity_label',
      'title arguments' => array(CONSULT_QUESTION_ENTITY_NAME, 7),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_question_delete_form', 7),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, 4),
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/result/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', CONSULT_RESULT_ENTITY_NAME),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_result_form', NULL, 4, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', CONSULT_RESULT_ENTITY_NAME),
      'type' => MENU_LOCAL_ACTION,
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/result/edit/%' . CONSULT_RESULT_ENTITY_NAME] = array(
      'title callback' => 'entity_label',
      'title arguments' => array(CONSULT_RESULT_ENTITY_NAME, 7),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_result_form', 7, 4),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, 4),
    );

    $items['admin/structure/consult/manage/%' . $this->entityType . '/result/delete/%' . CONSULT_RESULT_ENTITY_NAME] = array(
      'title callback' => 'entity_label',
      'title arguments' => array(CONSULT_RESULT_ENTITY_NAME, 7),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('consult_result_delete_form', 7),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, 4),
    );

    if (!empty($this->entityInfo['admin ui']['file'])) {
      // Add in the include file for the entity form.
      foreach ($items as $k => $item) {
        $items[$k]['file'] = $this->entityInfo['admin ui']['file'];
        $items[$k]['file path'] = isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']);
      }
    }

    return $items;
  }
}
