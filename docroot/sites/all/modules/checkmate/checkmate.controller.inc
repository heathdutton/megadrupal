<?php
/**
 * @file
 * The checkmate entity API implementation.
 */

/**
 * Checkmate Item class.
 */
class CheckmateItem extends Entity {

  /**
   * The label for a checkmate item.
   */
  protected function defaultLabel() {
    return $this->title;
  }
}

/**
 * Checkmate Item Controller class.
 */
class CheckmateItemController extends EntityAPIController {

  /**
   * Create a new checmate item.
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'description' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
      'check_group' => '',
      'check_subgroup' => '',
      'callback' => 'checkmate_manual_check',
      'parameters' => '',
    );
    return parent::create($values);
  }

  /**
   * Build the entity view.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('checkmate_item', $entity);
    // Make Description like a default fields.
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t('Description'),
      '#access' => TRUE,
      '#label_display' => 'hidden',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'checkmate_item',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => nl2br(check_plain($entity->description))),
    );
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

/**
 * Checkmate Item Type class.
 */
class CheckmateItemType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * Checkmate item type constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'checkmate_item_type');
  }

  /**
   * isLocked().
   */
  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Checkmate Item Type Controller class.
 */
class CheckmateItemTypeController extends EntityAPIControllerExportable {

  /**
   * Create a new checmate item type.
   */
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save checkmate Item Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * UI controller for checkmate item Type.
 */
class CheckmateItemTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage checkmate item types.';

    return $items;
  }
}

/**
 * Checkmate Item Status class.
 */
class CheckmateItemStatus extends Entity {
}

/**
 * checkmate Item Status Controller class.
 */
class CheckmateItemStatusController extends EntityAPIController {

  /**
   * Create a new checkmate item status.
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );

    return parent::create($values);
  }

  /**
   * Build the output of a checkmate item status.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('checkmate_item_status', $entity);
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
