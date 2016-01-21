<?php




/**
 * The Controller for Model entities
 */
class WoWToonFeedItemTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a model type - we first set up the values that are specific
   * to our model type schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the model.
   *
   * @return
   *   A model type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
        'id' => '',
        'is_new' => TRUE,
        'data' => '',
    );
    $model_type = parent::create($values);
    return $model_type;
  }

}