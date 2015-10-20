<?php
/**
 * @file
 * Contains Drupal\GitClone\AdminUIController.
 */

namespace Drupal\GitClone;

/**
 * Class EntityUI.
 *
 * @package Drupal\GitClone
 */
class AdminUIController extends \EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Add back in the admin overview form.
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';
    $items[$this->path] = array(
      'title' => $plural_label,
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
    );

    // Override the "add" path.
    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_page_title',
      'title arguments' => array('add', $this->entityType),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, NULL, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
    );

    // Advanced.
    $items[$this->path . '/advanced'] = array(
      'title' => t('Advanced'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('git_clone_advanced_form'),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType),
      'type' => MENU_LOCAL_TASK,
    );

    if (!empty($this->entityInfo['admin ui']['file'])) {
      // Add in the include file for the entity form.
      foreach (array('/add', '/advanced') as $path_end) {
        $items[$this->path . $path_end]['file'] = $this->entityInfo['admin ui']['file'];
        $items[$this->path . $path_end]['file path'] = isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']);
      }
    }

    return $items;
  }

}
