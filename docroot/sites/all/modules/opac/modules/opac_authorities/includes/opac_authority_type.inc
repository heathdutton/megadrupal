<?php
/**
 * @file
 * Opac authority types class and controller.
 */

/**
 * Authority Type class.
 */
class OpacAuthorityType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * Override parent::__construct.
   *
   * @param array $values
   *   An array of values to set.
   *
   * @return array
   *   A instance of the entity.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'opac_authority_type');
  }

  /**
   * We need this to know if the entity is locked.
   *
   * If locked, it is impossible to edit/delete it.
   *
   * @return bool
   *   Boolean telling if the entity is locked.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Authority type controller.
 */
class OpacAuthorityTypeController extends EntityAPIControllerExportable {
  /**
   * Override parent::create.
   *
   * @param array $values
   *   An array of values to set.
   *
   * @return array
   *   A instance of the entity.
   */
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Task Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see _http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * UI controller for authority type.
 */
class OpacAuthorityTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage authority types.';
    return $items;
  }
}
