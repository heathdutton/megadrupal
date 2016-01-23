<?php

/**
 * @file
 * Mediabox entity class.
 */
class MediaboxEntity extends Entity {

  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * @return array|void
   */
  protected function defaultUri() {
    // nothing to display here
    // return array('path' => 'task/' . $this->identifier());
  }
}

class MediaboxViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    return $data;
  }
}

class MediaboxMetaDataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {

    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['created']['type'] = 'date';
    $properties['modified']['type'] = 'date';

    $properties['user_id'] = array(
      'label' => t('Author'),
      'type' => 'user',
      'description' => t('Author of mediabox core item'),
      'required' => TRUE,
      'schema field' => 'user_id',
    );

    return $info;
  }
}

class MediaboxType extends Entity {
  public $type;
  public $title;

  public function __construct($values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'modified' => REQUEST_TIME,
    );
    parent::__construct($values, 'mediabox_type');
  }

  function identifier() {

    return $this->type;
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

class MediaboxTypeController extends EntityAPIControllerExportable {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

   public function create(array $values = array()) {

    $values += array(
      'id' => '',
      'title' => '',
      'description' => '',
    );

    return parent::create($values);
  }

  /**
   * Save mediabox Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see _http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }
}
