<?php

/**
 * @file
 * The file, which controls Mobile Device plugin.
 */

/**
 * Main context class to detect user's device.
 */
class cmd_condition extends context_condition {
  function condition_values() {
    return array(
      1 => t('Mobile Device'),
      2 => t('Tablet Device'),
      3 => t('Desktop Device')
    );
  }

  function execute() {
    $data = _context_mobile_detect_detect(FALSE);

    foreach ($this->get_contexts($data['device']) as $context) {
      $this->condition_met($context, $data['device']);
    }
  }
}
