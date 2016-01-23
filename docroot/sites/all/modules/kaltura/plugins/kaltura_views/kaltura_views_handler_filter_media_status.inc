<?php

/**
 * @file
 * Class kaltura_views_handler_filter_media_status.
 */

/**
 * Media Status filter handler class.
 */
class kaltura_views_handler_filter_media_status extends views_handler_filter_in_operator {

  /**
   * We use only one function to supply our own set of values.
   */
  function get_value_options() {
    $kaltura_settings = new KalturaSettings();
    $this->value_options = $kaltura_settings->{'media_status_map'};
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    $form['kaltura_media_status']['#type'] = 'checkboxes';
    // $form['kaltura_media_type']['#default_value'] = 'All';
    // unset($form['kaltura_media_type']['#default_value']);
    unset($form['kaltura_media_status']['#multiple']);
    unset($form['kaltura_media_status']['#size']);
  }

  function store_exposed_input($input, $status) {
    parent::store_exposed_input($input, $status);

    // FAPI set 0 as a vlue for each unchecked checkbox so we need to unset it
    // so we woun't get a form error.
    $display_id = ($this->view->display_handler->is_defaulted('filters')) ? 'default' : $this->view->current_display;

    if ($status) {
      $session = &$_SESSION['views'][$this->view->name][$display_id];
      if (!empty($session[$this->field])) {
        foreach ($session[$this->field] as $key => $val) {
          if ($val === 0) {
            unset($session[$this->field][$key]);
          }
        }
      }
    }
  }
}
