<?php

/**
 * @file
 * Definition of watchcat_handler_field.
 */

/**
 * Field handler to present a value
 */
class watchcat_handler_field extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby
    if (!empty($this->options['link_to_detail'])) {
      $this->additional_fields['wid'] = array('table' => 'watchcat', 'field' => 'wid');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_detail'] = array('default' => isset($this->definition['link_to_detail default']) ? $this->definition['link_to_detail default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_detail'] = array(
      '#title' => t('Link this field to the watchcat detail page.'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_detail']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_detail']) && !empty($this->additional_fields['wid'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "watchcat/" . $this->get_value($values, 'wid');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    return $this->render_link($this->get_value($values), $values);
  }
}