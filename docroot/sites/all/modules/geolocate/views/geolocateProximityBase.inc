  <?php

/**
 * @file
 *   Contains geolocateProximityPluginInterface and geolocateProximityBase.
 */

interface geolocateProximityPluginInterface {
  /**
   * All methods in geolocateProximityPluginInterface maps directly to a 
   * method in a views_handler class, except for 'getSourceValue,' which
   * is probably not needed. 
   */
  public function option_definition(&$options, $views_plugin);
  public function options_form(&$form, &$form_state, $views_plugin);
  public function options_validate(&$form, &$form_state, $views_plugin);
  public function value_form(&$form, &$form_state, $views_plugin);
  public function value_validate(&$form, &$form_state, $views_plugin);
  public function getSourceValue($views_plugin);
}

class geolocateProximityBase implements geolocateProximityPluginInterface {
  public function option_definition(&$options, $views_plugin) {
    
  }

  public function options_form(&$form, &$form_state, $views_plugin) {
    
  }

  public function options_validate(&$form, &$form_state, $views_plugin) {

  }

  public function value_form(&$form, &$form_state, $views_plugin) {
  }

  public function value_validate(&$form, &$form_state, $views_plugin) {
    
  }

  public function getSourceValue($views_plugin) {
    $geolocate = array(
      'latitude' => 0,
      'longitude' => 0,
    );
    
    return $geolocate;
  }
}
