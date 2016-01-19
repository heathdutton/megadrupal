<?php

/**
 * Extending the EntityAPIController for the distancedefault entity.
 */
class ImmoclientDistanceEntityController extends EntityAPIController {
  
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    
    $build['distance_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Distance to important places'),
      '#collapsible' => FALSE,
    );
      $build['distance_fieldset']['distanz_zu'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Distance to important places').'</b>'.': '.  immoclient_distance_option_callback($entity->distanz_zu),
      '#prefix' => '<div style="float:left; padding: 0 5px" class="distance">',
      '#suffix' => '</div>',
    ); 
      $build['distance_fieldset']['distance_measure'] = array(
      '#type' => 'markup',
      '#markup' => '<b>'.t('Distance').'</b>'.': '. $entity->distance_measure.' km',
      '#prefix' => '<div style="float:left; padding: 0 5px" class="distance">',
      '#suffix' => '</div>',
    );
    
    return $build;
  }
  
}

