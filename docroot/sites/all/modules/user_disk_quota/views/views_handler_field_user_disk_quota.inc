<?php
/**
 * @file
 * User Disk Quota Views field class.
 */

/**
 * User disk quota field handler for Views.
 */
class views_handler_field_user_disk_quota extends views_handler_field {

  /**
   * Add user id to field initialization.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   * Define field options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['show_default_role_limit'] = array('default' => TRUE, 'bool' => TRUE);
    $options['format_size'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Define field options form.
   */
  function options_form(&$form, &$form_state) {
    $form['show_default_role_limit'] = array(
      '#title' => t('Add role default value if limit not set'),
      '#description' => t("If user has no limit set, show his role's default limit"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['show_default_role_limit'],
    );
    $form['format_size'] = array(
      '#title' => t('Output formatted limit'),
      '#description' => t("Output limit as a formatted value (i.e. 2 MB, 512 KB)."),
      '#type' => 'checkbox',
      '#default_value' => $this->options['format_size'],
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Render field.
   */
  function render($values) {
    $value = $this->get_value($values);
    $value = $this->sanitize_value($value);
    if (empty($value) && $this->options['show_default_role_limit']) {
      if (isset($values->uid)) {
        $account = user_load($values->uid);
        if ($account) {
          $value = _user_disk_quota_get_user_role_max_quota($account, FALSE);
        }
      }
    }
    if (!empty($value) && $this->options['format_size']) {
      $value = format_size($value);
    }
    return $value;
  }
}