<?php

/**
 * @file
 * The base class for manual payment activities.
 */
class pay_method_custom extends pay_method {
  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);

    // Eliminate the 'authorized' option from the settings.
    $group = $this->handler();
    unset($form[$group]['pay_form_action']['#options']['authorize']);
  }
}
