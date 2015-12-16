<?php

class views_contact_form_handler extends views_handler_field_user_mail {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_user']['#options']['views_contact_form'] = t('Contact form');
  }

  function render($values) {
    if ($this->options['link_to_user'] == 'views_contact_form') {
      return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
    }
    return parent::render($values);
  }

  function views_form(&$form, &$form_state) {
    if ($this->options['link_to_user'] == 'views_contact_form') {
      // The view is empty, abort.
      if (empty($this->view->result)) {
        return;
      }

      $form[$this->options['id']] = array(
        '#tree' => TRUE,
      );

      // At this point, the query has already been run, so we can access the results
      // in order to get the base key value (for example, nid for nodes).
      $data = array();
      foreach ($this->view->result as $row_index => $row) {
        $data['emails'][] = $this->get_value($row);
      }

      $form['views_contact_form'] = array(
        '#weight' => 100
      );
      $form['views_contact_form']['form'] = drupal_get_form('views_contact_form_contact_form', $data);
    }
  }


}
