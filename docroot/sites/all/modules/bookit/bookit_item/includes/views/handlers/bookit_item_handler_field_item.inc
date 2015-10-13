<?php

/**
 * @file
 * Contains the basic item field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a bookable item.
 */
class bookit_item_handler_field_item extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_item'])) {
      $this->additional_fields['item_id'] = 'item_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_item'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to item option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_item'] = array(
      '#title' => t("Link this field to the bookable item's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_item']),
    );
  }

  /**
   * Render whatever the data is as a link to the bookable item.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_item']) && $data !== NULL && $data !== '') {
      $item_id = $this->get_value($values, 'item_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/bookit/items/' . $item_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
