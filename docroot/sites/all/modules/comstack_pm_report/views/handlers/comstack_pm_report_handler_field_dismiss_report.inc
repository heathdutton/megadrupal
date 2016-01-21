<?php

/**
 * @file
 * Contains comstack_pm_report_handler_field_dismiss_report.
 *
 * Output an AJAXy link to dismiss a report using core JS.
 */

class comstack_pm_report_handler_field_dismiss_report extends views_handler_field {
  function construct() {
    parent::construct();
    $this->real_field = 'cspmr_id';
    $this->additional_fields['dismissed'] = array('table' => 'comstack_pm_report', 'field' => 'dismissed');
  }

  function pre_render(&$values) {
    parent::construct();
    drupal_add_library('system', 'drupal.ajax');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => t('Dismiss report'), 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  function render($values) {
    global $user;
    $id = $this->get_value($values);
    $dismissed = $this->get_value($values, 'dismissed');
    $text = check_plain($this->options['text']);

    if (!$dismissed) {
      $token = drupal_get_token($id . $user->uid);
      $url = "admin/ajax/cs-pm-r/dismiss/{$id}/{$token}/nojs";

      // Don't use ctools_ajax_text_button as we've already added the library.
      $link = l($text, $url, array('html' => TRUE, 'attributes' => array('class' => array('use-ajax'), 'title' => $text)));
      return '<div id="cs-pm-report-dismiss-' . $id . '" class="cs-pm-report-dismiss">' . $link . '</div>';
    }
    else {
      return '<span class="text-muted cs-pm-report-dismiss cs-pm-report-dismissed">' . $text . '</span>';
    }
  }
}
