<?php
/**
 * @file
 * Control: Geolocate.
 */

namespace Drupal\openlayers_geolocate_button\Plugin\Control\Geolocate;
use Drupal\openlayers\Component\Annotation\OpenlayersPlugin;
use Drupal\openlayers\Types\Control;

/**
 * Class Geolocate.
 *
 * @OpenlayersPlugin(
 *  id = "Geolocate",
 *  description = "Geolocate button"
 * )
 */
class Geolocate extends Control {

  /**
   * @inheritDoc
   */
  public function optionsForm(array &$form, array &$form_state) {
    parent::optionsForm($form, $form_state);

    // Zoom level.
    $form['options']['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom level'),
      '#description' => t('An integer zoom level for when a location is found.  0 is the most zoomed out and higher numbers mean more zoomed in (the number of zoom levels depends on your map).'),
      '#default_value' => $this->getOption('zoom', 12),
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    );

    return $form;
  }

}
