<?php

/**
 * Filter handler for state.
 */
class cd_civicrm_handler_filter_state extends cd_sunlight_handler_filter_state {
  function get_value_options() {
    parent::get_value_options();
    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Core/PseudoConstant.php';
    $names = CRM_Core_PseudoConstant::stateProvince();
    $abbreviations = CRM_Core_PseudoConstant::stateProvinceAbbreviation();
    foreach ($this->value_options as $abbreviation => &$name) {
      if ($id = array_search($abbreviation, $abbreviations)) {
        $name = $names[$id];
      }
    }
  }
}
