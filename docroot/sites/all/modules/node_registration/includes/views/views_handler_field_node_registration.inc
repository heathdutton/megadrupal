<?php

/**
 * @file
 * Views handler for Node registration fields
 */

class views_handler_field_node_registration extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    //if (!empty($this->options['link_to_node_registration'])) {
      $this->additional_fields['registration_id'] = array('table' => 'node_registration', 'field' => 'registration_id');
      $this->additional_fields['email'] = array('table' => 'node_registration', 'field' => 'email');
    //}
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_node_registration'] = array('default' => isset($this->definition['link_to_node_registration default']) ? $this->definition['link_to_node_registration default'] : FALSE);
    $options['link_to_node_registration_edit'] = array('default' => FALSE);
    $options['link_to_node_registration_email'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_node_registration'] = array(
      '#title' => t('Link this field to the Node registration page'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node_registration']),
    );

    $form['link_to_node_registration_edit'] = array(
      '#title' => t('Link this field to the Node registration edit form'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node_registration_edit']),
    );

    $form['link_to_node_registration_email'] = array(
      '#title' => t('Link this field to the Node registration e-mail address'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node_registration_email']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    $this->options['alter']['make_link'] = FALSE;

    if (!empty($this->options['link_to_node_registration'])) {
      if ($data !== NULL && $data !== '') {
        $uri = node_registration_uri($this->get_value($values, 'registration_id'));
        $path = $uri['path'];
        $this->options['alter']['path'] = $path;

        $this->options['alter']['make_link'] = TRUE;
      }
    }

    if (!empty($this->options['link_to_node_registration_edit'])) {
      if ($data !== NULL && $data !== '') {
        $uri = node_registration_uri($this->get_value($values, 'registration_id'));
        $path = $uri['path'] . '/edit';
        $this->options['alter']['path'] = $path;

        $this->options['alter']['make_link'] = TRUE;
      }
    }

    if (!empty($this->options['link_to_node_registration_email'])) {
      if ($data !== NULL && $data !== '') {
        $email = $this->get_value($values, 'email');
        $this->options['alter']['path'] = 'mailto:' . $email;

        $this->options['alter']['make_link'] = TRUE;
      }
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
