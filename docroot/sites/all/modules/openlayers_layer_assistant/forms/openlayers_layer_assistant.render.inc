<?php
/**
 * Renders form for certain layer type. Used to provide simpler forms for layer creation by subclassing.
 */
class openlayer_layer_assistant_render {
  /**
   * @var openlayers_layer_type
   */
  protected $layerType;
  
  /**
   * @var string Title of layer type
   */
  public $title;
  
  /**
   * @var string description of layer type (HTML source)
   */
  public $description;
  
  /**
   * @param openlayers_layer_type $layerType Layer type for which to show form
   */
  public function __construct(openlayers_layer_type $layerType){
    $this->layerType = $layerType;
    $chosenLayerType = get_class($this->layerType);
    $layerTypes = openlayers_layer_types();
    $layerTypeRecord = $layerTypes[$chosenLayerType];
    $this->title = $layerTypeRecord['title'];
    $this->description = $layerTypeRecord['description'];
  }
  
  /**
   * Add fields that are common for all layers to form (see openlayers_ui_layers_form)
   * @param array $form Drupal form
   */
  protected function mergeBasicFields(&$form){
    $form['basics'] = array(
      '#tree' => FALSE,
      '#type' => 'fieldset',
      '#title' => t('Layer Basics'),
      '#description' => t('The following settings are for all layers.'),
    );
    $form['basics']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Layer Title'),
      '#default_value' => '',
      '#description' => t('The friendly name of your layer, which will appear in the administration interface as well on the map interface if it is exposed.'),
    );
    $form['basics']['name'] = array(
      '#type' => 'machine_name',
      '#default_value' => '',
      '#maxlength' => 32,
      '#machine_name' => array(
        'exists' => 'openlayers_layer_type_load',
        'source' => array('basics', 'title'),
      ),
      '#description' => t('A unique machine-readable name for this block type. It must only contain lowercase letters, numbers, and underscores.'),
      '#disabled' => FALSE,
    );
    $form['basics']['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Layer Description'),
      '#default_value' => '',
      '#description' => t('The description of your layer, which will appear in the administration interface.'),
    );
    // Include fields that are present for all layers
    $parent = new openlayers_layer_type();
    foreach($parent->options_form() as $fieldName => $field){
      $form['basics'][$fieldName] = $field;
    }
  }
  
  /**
   * Part of form that default add-layer-GUI shows after selecting the layer type form the checkbox
   */
  protected function mergeLayerTypeFields(&$form){
    $layers_option = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Layer specific options for @layer_title', array('@layer_title' => $this->title)),
    );
    $layers_option += $this->layerType->options_form();
    $form[get_class($this->layerType)] = $layers_option;
  }
  
  /**
   * Merges layer type specific form fields to given form.
   * @return array Drupal form array (same reference as parameter)
   */
  public function render(&$form){
    $this->mergeBasicFields($form);
    
    $this->mergeLayerTypeFields($form);
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create Layer')
    );
    
    return $form;
  }
  
  /**
   * Gets invoked on AJAX requests.
   * @param array $input Form entries
   * @param array $callbackArguments URL suffix parameters
   */
  public function callback(array $input, $callbackArguments){
    return array();
  }
  
  /**
   * Called just before save to adjust users entries
   * @param array $form_state
   */
  public function beforeSave(&$form_state){}
  
  /**
   * Get form input
   * @param array $input Form entries
   * @param array $path Path in form
   * @return mixed
   */
  protected function getFieldValue(array $input, array $path){
    $value = $input;
    foreach($path as $level) {
      $value = $value[$level];
    }
    return $value;
  }
  
  /**
   * Sets form field
   * @param array $path Path in form
   * @param mixed $value
   * @return array Drupal AJAX command (invocation of jQuery method)
   */
  protected function getSetFieldValueCommand(array $path, $value){
    $inputFieldId = NULL;
    foreach($path as $level) {
      if($inputFieldId===NULL){
        $inputFieldId = '[name="'.$level;
      } else {
        $inputFieldId = $inputFieldId.'\['.mb_ereg_replace('([!"#$%&\'()*+,./:;?\\\\@\\[\\]^`{|}~])', '\\\\1', $level).'\]';
      }
    }
    $inputFieldId = $inputFieldId.'"]';
    return ajax_command_invoke($inputFieldId, 'val', array($value));
  }
}