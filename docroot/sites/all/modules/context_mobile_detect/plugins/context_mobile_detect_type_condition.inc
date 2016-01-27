<?php

/**
 * @file
 * The file, which controls Mobile Device Type plugin.
 */

/**
 * Main context class to detect user's device type.
 */
class cmd_type_condition extends context_condition {
  function condition_values() {
    return _context_mobile_detect_devices_types();
  }

  function execute() {
    $data = _context_mobile_detect_detect(FALSE);

    foreach ($this->get_contexts($data['device_type']) as $context) {
      $this->condition_met($context, $data['device_type']);
    }
  }
}
