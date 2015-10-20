<?php

/**
 * @file
 * Provides a link to view extended profile data.
 * module.
 */
class lr_social_login_handler_field_view_link extends views_handler_field {

  /**
   * Define our additional configuration setting.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   * option defination that display in view admin page.
   */
  public function optionDefinition() {
    $options = parent::optionDefinition();
    $options['text'] = array(
      'default' => t('View User Profile'),
      'translatable' => TRUE,
    );
    $options['text2'] = array(
      'default' => t('Show User Profile'),
      'translatable' => TRUE,
    );
    return $options;
  }

  /**
   * Define the configuration form for our textfield.
   *
   * @param array $form Nested array of form elements that comprise the form.
   * @param array $form_state A keyed array containing the current state of the form.
   *
   */
  public function optionsForm(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => isset($this->options['text']) ? $this->options['text'] : '',
      '#description' => t('The title for this field'),
    );
    $form['text2'] = array(
      '#type' => 'textfield',
      '#title' => t('Label of the field'),
      '#default_value' => isset($this->options['text2']) ? $this->options['text2'] : '',
      '#description' => t('The label for this field'),
    );
  }

  /**
   * Add additional fields into view page.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render fields and display link
   *
   * @param string $values The element value to be rendered.
   * @return string  The rendered element.
   */
  public function render($values) {
    $value = $this->get_value($values, 'uid');
    return $this->rendeLink($this->sanitize_value($value), $values);
  }

  /**
   * Renders our field, displays a link if the user is allowed to.
   *
   * @param array $data
   * @param array $values
   * @return string
   */
  public function rendeLink($data, $values) {
    if (variable_get('lr_social_profile_data_show_profile') == 1) {
      drupal_add_library('system', 'ui.tabs');
      $options = array(
        'attributes' => array(
          'class' => 'lr_social_login',
          'rel' => 'nofollow',
          'title' => $this->options['text'],
        ),
      );
      $this->options['label'] = $this->options['text2'];
      $user_profile = user_load($data);
      $admin_access = 0;
      foreach ($user_profile->roles as $keyd => $value) {
        if (isset($value) && $value == "administrator") {
          $admin_access = 1;
        }
      }
      if ($admin_access != 1) {
        return $links = l(t('View'), 'lr_social_profile_data/ajax/userprofile/' . $data, $options);
      }
      else {
        return '';
      }
    }
  }
}
