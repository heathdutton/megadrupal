<?php

/**
 * @file
 * Mediabox display entity class.
 */
class MediaboxDisplayEntity extends Entity {

  public function __construct($values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'modified' => REQUEST_TIME,
    );
    parent::__construct($values, 'mediabox_display');
  }

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

class MediaboxDisplayViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    return $data;
  }
}

class MediaboxDisplayMetaDataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {

    $info = parent::entityPropertyInfo();

//    $properties = &$info[$this->type]['properties'];
//
//    $properties['created']['type'] = 'date';
//    $properties['modified']['type'] = 'date';
//
//    $properties['user_id'] = array(
//      'label' => t('Author'),
//      'type' => 'user',
//      'description' => t('Author of mediabox core item'),
//      'required' => TRUE,
//      'schema field' => 'user_id',
//    );

    return $info;
  }
}

class MediaboxDisplayType extends Entity {
  public $type;
  public $title;

  public function __construct($values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'modified' => REQUEST_TIME,
    );
    parent::__construct($values, 'mediabox_display_type');
  }

  function identifier() {
    return $this->type;
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

class MediaboxDisplayTypeController extends EntityAPIControllerExportable {

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

class MediaboxDisplayTypeUIController extends EntityDefaultUIController {

}


/**
 *
 * @param $form
 * @param $form_state
 * @param MediaboxType $bundle
 * @return array
 */
function mediabox_display_type_form($form, &$form_state, MediaboxDisplayType $bundle = NULL) {
  $form = array();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => isset($bundle->id) ? $bundle->id : '',
  );

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t(
      'The human-readable name of this content type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'
    ),
    '#default_value' => isset($bundle->title) ? $bundle->title : '',
  );

  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t(
      'The machine-readable name of this content type. It is recommended that this code contain only letters, numbers, and underscores. This name must be unique.'
    ),
    '#default_value' => isset($bundle->type) ? $bundle->type : '',
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#description' => t('Describe this media type. The text will be displayed on the Add new media.'),
    '#default_value' => isset($bundle->description) ? $bundle->description : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save media type'),
    '#submit' => array('mediabox_display_type_form_submit'),
  );

  return $form;
}

function mediabox_display_type_form_submit($form, &$form_state) {
  $model_type = entity_ui_form_submit_build_entity($form, $form_state);
  $model_type->save();

  // Add mediabox_image image type field.
  _mediabox_ui_add_default_fields($model_type->type);

  $form_state['redirect'] = 'admin/structure/mediabox-ui';
}

/**
 * Add default fields to mediabox_display bundle.
 *
 * @param $bundle string
 *   Mediabox_display bundle type.
 */
function _mediabox_ui_add_default_fields($bundle) {
  // If field is not created create it.
  if (!field_info_field('mediabox_image')) {
    field_create_field(array(
      'field_name' => 'mediabox_image',
      'type' => 'image',
      'entity_types' => array('mediabox', 'mediabox_display'),
    ));
  }

  // This function used in installation also.
  $t = get_t();

  // Create field instance in given bundle.
  if (!field_info_instance('mediabox_display', 'mediabox_image', $bundle)) {
    field_create_instance(array(
      'field_name' => 'mediabox_image',
      'entity_type' => 'mediabox_display',
      'bundle' => $bundle,
      'label' => $t('Mediabox image'),
      'widget' => array(
        'weight' => 0,
        'type' => 'image_image'
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'mediabox_image',
          'module' => 'image',
          'weight' => 1,
        ),
        'display' => array(
          'label' => 'hidden',
          'type' => 'mediabox_image',
          'module' => 'image',
          'weight' => 1,
        ),
      ),
    ));
  }
}
