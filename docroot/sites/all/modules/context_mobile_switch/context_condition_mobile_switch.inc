<?php

/**
 * Expose paths as a context condition.
 */
class context_condition_mobile_switch extends context_condition {
  function condition_values() {
    return array(
      'isnotmobile' => t('Is not mobile'),
      'ismobile' => t('Is mobile'),
    );
  }

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      $get['mode'] = mobile_switch_get_operating_mode();

      // Not supported operating mode.
      if ($get['mode'] === 'none') {
        return;
      }

      $bypass = FALSE;
      // Use the mobile switch functionality on tablet devices?
      $get['tablet_usage'] = (bool) variable_get('mobile_switch_tablet_usage', TRUE);
      // Prevent mobile devices?
      //$get['prevent_devices'] = (bool) variable_get('mobile_switch_prevent_devices', FALSE);
      // Desktop browser mode enabled?
      $get['deskbrowser'] = (bool) variable_get('mobile_switch_deskbrowser', FALSE);

      $detect = mobile_switch_mobile_detect();
      $ismobile = $detect['ismobiledevice'] ? 'ismobile' : 'isnotmobile';

      // Fake a mobile device if 'Desktop browser' usage configured.
      if ($get['deskbrowser'] === TRUE) {
        $ismobile = 'ismobile';
      }

      // Check if device prevented.
      if ($detect['prevent_device'] === TRUE) {
        $bypass = TRUE;
      }
      // Check if tablet usage enabled.
      if ($detect['istablet'] === TRUE && $get['tablet_usage'] === FALSE) {
        $bypass = TRUE;
      }

      foreach ($this->get_contexts() as $context) {
        $device_is = $this->fetch_from_context($context, 'values');
        if (array_key_exists($ismobile, $device_is) && $bypass === FALSE) {

          $this->condition_met($context);
        }
      }

    }
  }
}
