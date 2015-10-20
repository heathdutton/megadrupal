<?php

class OpenidSsoProviderRpsController extends EntityAPIControllerExportable {
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'realm' => '',
      'trusted' => '',
      'enabled' => '',
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('openid_sso_provider_rps', $entity);
    $content['realm'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Realm'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'openid_sso_provider_rps_realm',
      '#field_truested' => 'int',
      '#entity_type' => 'openid_sso_provider_rps',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->realm)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->realm))
    );
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

class OpenidSsoProviderRpsTypeController extends EntityAPIControllerExportable {
   public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'description' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }

  /**
   * Delete Type.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    parent::delete($ids, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * UI controller for Type.
 */
class OpenidSsoProviderRpsUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items['admin/structure/relying-parties']['title'] = t('OpenID relying parties');

    $items[$this->path . '/add'] = $items[$this->path . '/add'];
    $items[$this->path . '/add']['title'] = t('Add relying party');
    $items[$this->path . '/add']['weight'] = -2;

    return $items;
  }

  public function overviewForm($form, &$form_state) {
    $form = parent::overviewForm($form, $form_state);
    return $form;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {

    // Obtain information whether relying party is enabled.
    $enabled = ($entity->enabled) ? t('Yes') : t('No');

    // Get table rows.
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols = array($enabled));

    // Remove cloneing
    foreach ($row as $key => $value) {
      if (!is_array($value)) {
        if (strpos($value, '/clone')) {
          unset($row[$key]);
        }
      }
    }
    return $row;
  }

  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $additional_header[] = t('Enabled');
    return parent::overviewTableHeaders($conditions, $rows, $additional_header);
   }

}

/**
 * Entity class.
 */
class OpenidSsoProviderRps extends Entity {
  protected function defaultLabel() {
    return $this->label;
  }

  protected function defaultUri() {
    return array('path' => 'admin/structure/relying-parties/manage/' . $this->identifier());
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * UI controller for Type.
 */
class OpenidSsoProviderRpsTypeUIController extends EntityDefaultUIController {
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols = array());
    // Remove cloneing
    foreach ($row as $key => $value) {
      if (!is_array($value)) {
        if (strpos($value, '/clone')) {
          unset($row[$key]);
        }
      }
    }
    return $row;
  }
}

/**
 * Entity Type class.
 */
class OpenidSsoProviderRpsType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'openid_sso_provider_rps_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}
