<?php

/**
 * @file
 * Contains comstack_pm_report_handler_field_view_conversation.
 */

class comstack_pm_report_handler_field_view_conversation extends views_handler_field {
  function construct() {
    parent::construct();

    $this->real_field = 'cspmr_id';
    $this->additional_fields['dismissed'] = array('table' => 'comstack_pm_report', 'field' => 'dismissed');
  }

  function pre_render(&$values) {
    parent::construct();
    ctools_include('ajax');
    ctools_include('modal');
    ctools_modal_add_js();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => t('View conversation'), 'translatable' => TRUE);
    $options['newline_prepend'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $this->options['text'],
    );
    $form['newline_prepend'] = array(
      '#type' => 'checkbox',
      '#title' => t('Output a newline before the link'),
      '#default_value' => $this->options['newline_prepend'],
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

    if ($id) {
      $text = check_plain($this->options['text']);
      $token = drupal_get_token($id . $dismissed . $user->uid);
      $url = "admin/ajax/cs-pm-r/view-conversation/$id/$token/nojs";

      $output = '';
      if ($this->options['newline_prepend']) {
        $output = '<br>';
      }
      $output = '<div id="cs-pm-report-view-conversation-' . $id . '">' . ctools_modal_text_button($text, $url, '') . '</div>';
      return $output;
    }
  }
}
