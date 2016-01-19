<?php

/**
 * @file
 * Class kaltura_views_handler_filter_media_type.
 */

/**
 * Media Type filter handler class.
 */
class kaltura_views_handler_filter_media_type extends views_handler_filter_in_operator {

  /**
   * We use only one function to supply our own set of values.
   */
  function get_value_options() {
    $this->value_options = array(
      KALTURA_MEDIA_TYPE_VIDEO => t('Video'),
      KALTURA_MEDIA_TYPE_IMAGE => t('Image'),
      KALTURA_MEDIA_TYPE_AUDIO => t('Audio'),
    );
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    $form['kaltura_media_type']['#type'] = 'checkboxes';
    $form['kaltura_media_type']['#default_value'] = array(
      KALTURA_MEDIA_TYPE_VIDEO => KALTURA_MEDIA_TYPE_VIDEO,
      KALTURA_MEDIA_TYPE_IMAGE => KALTURA_MEDIA_TYPE_IMAGE,
      KALTURA_MEDIA_TYPE_AUDIO => KALTURA_MEDIA_TYPE_AUDIO,
    );
    unset($form['kaltura_media_type']['#multiple']);
    unset($form['kaltura_media_type']['#size']);
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
