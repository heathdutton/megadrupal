<?php

/**
 * @file
 * This class holds a collection of methods based on formulas.
 */
class Calculate {

  /**
   * Works out Strike Water Temp.
   *
   * @param array $form_values
   *   Values from form.
   * 
   * @return float
   *   The strike temp.
   */
  public function strikeTemp(array $form_values) {
    // Initial Infusion Equation:
    // strike temp = desired mash temp x (water l + (0.4 x kg malt))
    // – (0.4 x kg malt x malt temp)/water l.
    return (!empty($form_values['input']['mash_temp'])) ? round((($form_values['input']['mash_temp'] * ($form_values['input']['water_volume'] + (0.4 * $form_values['input']['malt_weight'])) - (0.4 * $form_values['input']['malt_weight'] * $form_values['input']['malt_temp'])) / $form_values['input']['water_volume']), 1) : 0;
  }

  /**
   * Works out the hop ultilisation factor.
   * 
   * @param array $form_values
   *   Values from form.
   * 
   * @return float
   *   Utilistaion factor.
   */
  public function utilFactor(array $form_values) {
    return round($this->bignessValue($form_values['input']['wort_gravity']) * $this->boilTimeFactor($form_values['input']['time']) * 100, 1);
  }

  /**
   * Calculates bigness value.
   * 
   * @param float $wort_gravity
   *   The gravity of the wort.
   * 
   * @return float 
   *   The bigness factor.
   */
  protected function bignessValue($wort_gravity) {
    return (1.65 * pow(0.000125, $wort_gravity - 1));
  }

  /**
   * Calculates boil time factor.
   * 
   * @param int $time 
   *   Minutes.
   * 
   * @return float 
   *   The boil time factor.
   */
  protected function boilTimeFactor($time) {
    return ((1 - exp(-variable_get('brew_tools_max_utilization', 4.15) * $time)) / variable_get('brew_tools_utilization_time_curve', 4.15));
  }

  /**
   * Calulates ABV.
   * 
   * @param array $form_values
   *   Values from form.
   * 
   * @return float
   *   The ABV.
   */
  public function getABV(array $form_values) {
    return round(($form_values['input']['og'] - $form_values['input']['fg']) * 131, 2);
  }

  /**
   * Converts temperature.
   * 
   * @param array $form_values
   *   Values from form.
   * 
   * @return string
   *   Temperature string.
   */
  public function getTemp(array $form_values) {
    return temperature_format($form_values['input']['temp'], ($form_values['input']['units'] == "c") ? "f" : "c", 1, $form_values['input']['units']);
  }

  /**
   * Converts between Plato and Gravity
   * 
   * @param type $form_state
   * 
   * @return int The result
   */
  public function getPlatoGrav(array $form_values) { //plato_grav_value
    //{Plato/(258.6-([Plato/258.2]*227.1)}+1 = Gravity
    //°Plato = -616.868+1111.14*SG-630.272*SG^2+135.997*SG^3
    switch ($form_values['input']['plato_grav_radio']) {
      case "p":
        return round(-616.868 + 1111.14 * $form_values['input']['plato_grav_value'] - 630.272 * ($form_values['input']['plato_grav_value'] * $form_values['input']['plato_grav_value']) + 135.997 * ($form_values['input']['plato_grav_value'] * $form_values['input']['plato_grav_value'] * $form_values['input']['plato_grav_value']), 0);

      case "g":
        return round($form_values['input']['plato_grav_value'] / (258.6 - (($form_values['input']['plato_grav_value'] / 258.2) * 227.1)) + 1, 3);
    }
  }

}
