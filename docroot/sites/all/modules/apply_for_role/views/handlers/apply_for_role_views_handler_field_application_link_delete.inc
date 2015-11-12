<?php
/**
 * Field handler for a link to delete an application.
 */
class apply_for_role_views_handler_field_application_link_delete extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users_roles_apply', 'field' => 'uid');
    $this->additional_fields['rid'] = array('table' => 'users_roles_apply', 'field' => 'rid');
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
    return user_access('delete role applications');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->aliases['uid']}), check_plain($values->{$this->aliases['rid']}), $values);
  }

  function render_link($data, $data, $values) {
    $uid = $values->{$this->aliases['uid']};
    $rid = $values->{$this->aliases['rid']};
    $this->options['alter']['make_link'] = TRUE;

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');

    $this->options['alter']['path'] = "admin/people/apply_for_role/remove/" . $uid . "/" . $rid;
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}
