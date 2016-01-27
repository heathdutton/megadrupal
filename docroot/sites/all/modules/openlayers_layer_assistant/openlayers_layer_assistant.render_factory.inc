<?php
module_load_include('inc', 'openlayers_layer_assistant', 'forms/openlayers_layer_assistant.render');

abstract class openlayers_layer_assistant_render_factory {
  /**
   * @param string $chosenLayerType
   * @return openlayer_layer_assistant_render
   */
  public static function getRender($chosenLayerType){
    $layerType = openlayers_layer_type_load($chosenLayerType);
    if($layerType instanceof openlayers_layer_type){
      // Get specialized render for layer type if available
      $typeName = mb_ereg_replace('openlayers_layer_type_', '', $chosenLayerType);
      $includedFile = module_load_include('inc', 'openlayers_layer_assistant', 'forms/openlayers_layer_assistant.render_'.$typeName);
      if($includedFile===FALSE){
        module_load_include('inc', 'openlayers_layer_assistant', 'forms/openlayers_layer_assistant.render');
        return new openlayer_layer_assistant_render($layerType);
      } else {
        $className = "openlayer_layer_assistant_render_$typeName";
        return new $className($layerType);
      }
    } else {
      throw new Exception(t('Layer type !name does not exist', array(
        '!name' => $chosenLayerType
      )));
    }
  }
}