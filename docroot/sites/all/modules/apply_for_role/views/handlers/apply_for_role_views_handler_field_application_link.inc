<?php
/**
 * Field handler to present a link to the role application page.
 */
class apply_for_role_views_handler_field_application_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users_roles_apply', 'field' => 'uid');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  // An example of field level access control.
  function access() {
    return user_access('apply for roles');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->aliases['uid']}), $values);
  }

  function render_link($data, $values) {
    $uid = $values->{$this->aliases['uid']};
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Apply');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "user/" . $uid . "/apply_for_role";

    return $text;
  }
}
