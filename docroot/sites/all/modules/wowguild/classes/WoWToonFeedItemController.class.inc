<?php



/**
 * The Controller for Model entities
 */
class WoWToonFeedItemController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }


  /**
   * Create a model - we first set up the values that are specific
   * to our model schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the model.
   *
   * @return
   *   A model object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
        'achid' => '',
        'is_new' => TRUE,
        'title' => '',
        'created' => '',
        'changed' => '',
        'data' => '',
    );

    $model = parent::create($values);
    return $model;
  }

  /**
   * Overriding the buldContent function to add entity specific fields
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    //$content['model_sample_data'] =  array(
    //  '#markup' => theme('model_sample_data', array('model_sample_data' => check_plain($entity->data['sample_data']), 'model' => $entity)),
    //);

    return $content;
  }

}