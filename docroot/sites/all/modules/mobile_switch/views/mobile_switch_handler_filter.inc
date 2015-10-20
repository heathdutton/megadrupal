<?php
/**
 * @file
 * mobile_switch_handler_filter_ismobile.inc
 */

class mobile_switch_handler_filter extends views_handler_filter {
  function value_form(&$form, &$form_state) {
    if (variable_get('mobile_switch_mobile_theme', 'none') === 'none') {
      $form['message'] = array(
        '#markup' => t('Please configure the !basic-settings of Mobile Switch with a supported operating mode.', array('!basic-settings' => l(t('Basic settings'), 'admin/config/user-interface/mobile-switch')))
      );
    }
    $form['operator'] = array(
      '#type' => 'select',
      '#title' => t('Display only'),
      '#default_value' => isset($this->operator['value']) ? $this->operator['value'] : NULL,
      '#options' => mobile_switch_handler_filter_options()
    );
    return $form;
  }

  function can_expose() {
    return FALSE;
  }

  function show_operator_form(&$form, &$form_state) {
    $this->operator_form($form, $form_state);
    $form['operator']['#prefix'] = '<div class="views-group-box views-left-30">';
    $form['operator']['#suffix'] = '</div>';
  }

  function admin_summary() {
    $options = mobile_switch_handler_filter_options();
    $message = '';
    // Check if a supported operating mode enabled.
    if (variable_get('mobile_switch_mobile_theme', 'none') === 'none') {
      $message = ' - ' . t('The used operating mode is not supported.');
    }
    return check_plain((string) $options[$this->operator['value']] . $message);
  }

  function query() {
    $get['op_mode'] = variable_get('mobile_switch_mobile_theme', 'none');

    // Not supported operating mode.
    if ($get['op_mode'] === 'none') {
      return;
    };

    // Use the mobile switch functionality on tablet devices?
    $get['tablet_usage'] = (bool) variable_get('mobile_switch_tablet_usage', TRUE);
    // Prevent mobile devices?
    $get['prevent_devices'] = (bool) variable_get('mobile_switch_prevent_devices', FALSE);

    $detect = mobile_switch_mobile_detect();
    $ismobile = $detect['ismobiledevice'] ? $detect['ismobiledevice'] : 0;

    // Bypass if the mobile device prevented.
    // Bypass if mobile device tablet deactivated and a tablet detected.
    if (($get['prevent_devices'] === TRUE && $detect['prevent_device'] === TRUE) || ($get['tablet_usage'] === FALSE && $detect['istablet'] === TRUE)) {
      return;
    }

    // The operating modes 'detectonly' and 'redirect' works here
    // without additional code.

    // Theme switch mode.
    if ($get['op_mode'] != 'detectonly' && $get['op_mode'] != 'redirect') {
      // Bypass if theme cookie exists with value standard
      // or a theme by visitor switched to no the mobile version.
      // Theme changed means: Mobile site <-> standard site
      $theme_switch = mobile_switch_handler_filter_check_theme_switch();

      switch ($this->operator) {
        case '0':
          if ($theme_switch == 'standard' || $theme_switch == 'standard-rm') {
            return;
          }
          break;
        case '1':
          if ($ismobile == 1 && ($theme_switch == 'standard' || $theme_switch == 'standard-rm')) {
            $this->operator == 'dummy'; // todo: check this ==
            break;
          }
          break;
      }
    }

    $this->ensure_my_table();
    $join = new views_join();
    $this->query->add_where($this->options['group'], $ismobile, $this->operator, '=');
  }
}

/**
 * Check theme cookie exists or a theme switch action by visitor.
 *
 * This check is necessary to ensure a logical functionality when using the
 * Mobile Switch Block module.
 *
 * @return boolean|string
 *   Possible string values: standard, mobile
 */
function mobile_switch_handler_filter_check_theme_switch() {
  static $theme_switch;

  if (!$theme_switch) {
    $name = 'mobile_switch_mode';
    $theme_cookie = isset($_COOKIE[$name]) ? $_COOKIE[$name] : FALSE;

    // Theme switch from URL as visitor action.
    if (isset($_GET['mobile_switch'])) {
      switch ($_GET['mobile_switch']) {
        case 'standard':
          $theme_switch = 'standard';
          break;
        case 'standard-rm':
          $theme_switch = 'standard-rm';
          break;
        case 'mobile':
          $theme_switch = 'mobile';
          break;
        case 'mobile-rm':
          $theme_switch = 'mobile-rm';
          break;
      }
    }
    // Theme cookie exist.
    elseif ($theme_cookie) {
      switch ($theme_cookie) {
        case 'standard':
          $theme_switch = 'standard';
          break;
        case 'standard-rm':
          $theme_switch = 'standard-rm';
          break;
        case 'mobile':
          $theme_switch = 'mobile';
          break;
        case 'mobile-rm':
          $theme_switch = 'mobile-rm';
          break;
      }
    }
    else {
      $theme_switch = FALSE;
    }
  }

  return $theme_switch;
}

/**
 * Helper function that provides filter options.
 */
function mobile_switch_handler_filter_options() {
  return array(
    0 => t('Is not mobile'),
    1 => t('Is mobile')
  );
}
