<?php

/**
 * @file
 * This file holds style plugin for Views
 *
 */

/**
 * @class
 * Extension of the Views Plugin Syle
 *
 */
class views_contact_form_style extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['to'] = array('default' => '');
    return $options;
  }

  /**
   * Options form
   *
   * Overrides views_plugin_style->options_form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get list of fields in this view & flag available geodata fields
    $handlers = $this->display->handler->get_handlers('field');

    $fields = array();
    foreach ($handlers as $field_id => $handler) {
      $fields[$field_id] = $handler->ui_name();
    }

    $form['to'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Mapping'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['to']['email'] = array(
      '#type' => 'select',
      '#title' => t('Recipient(s) mapping'),
      '#description' => t('This field will be used to get the recipient email address.'),
      '#options' => $fields,
      '#default_value' => $this->options['to']['email'],
    );
  }

  /**
   * Renders views (map)
   */
  function render() {
    $view = $this->view;

    if (count($view->result) < 1) {
      return;
    }

    $view->style_plugin->render_fields($this->view->result);
    $email_field = isset($this->options['to']['email']) ? $this->options['to']['email'] : NULL;

    if (is_null($email_field)) {
      return;
    }

    $emails = array();
    foreach ($view->result as $count => $row) {
      $email = $view->style_plugin->rendered_fields[$count][$email_field];
      if (valid_email_address($email)) {
        $emails[] = $email;
      }
    }

    if (empty($emails)) {
      return;
    }

    $data['emails'] = $emails;
    $form = drupal_get_form('views_contact_form_contact_form', $data);

    return drupal_render($form);
  }
}
