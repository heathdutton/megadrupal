<?php

/**
 * @file
 * Contains the basic 'flipping_book' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a flipping_book.
 * Definition terms:
 * - link_to_flipping_book default: Should this field have the checkbox
 * "link to flipping_book" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_flipping_book extends views_handler_field {

  /**
   * Overrides views_handler::init().
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby.
    if (!empty($this->options['link_to_flipping_book'])) {
      $this->additional_fields['fbid'] = array(
        'table' => 'flipping_book',
        'field' => 'fbid',
      );
    }
  }

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_flipping_book'] = array(
      'default' => isset($this->definition['link_to_flipping_book default']) ? $this->definition['link_to_flipping_book default'] : FALSE,
      'bool' => TRUE,
    );
    return $options;
  }

  /**
   * Provide link to flipping_book option.
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_flipping_book'] = array(
      '#title' => t('Link this field to the original piece of content'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_flipping_book']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the flipping_book.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_flipping_book']) && !empty($this->additional_fields['fbid'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $flipping_book = flipping_book_load($this->get_value($values, 'fbid'));
        $this->options['alter']['path'] = $flipping_book->url;
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  /**
   * Render the field.
   */
  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
