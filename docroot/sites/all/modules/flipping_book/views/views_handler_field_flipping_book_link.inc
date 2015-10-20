<?php

/**
 * @file
 * Definition of views_handler_field_flipping_book_link.
 */

/**
 * Field handler to present a link to the flipping_book.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_flipping_book_link extends views_handler_field_entity {

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Options Form that provides the text to display.
   */
  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * Render the field.
   */
  function render($values) {
    if ($entity = flipping_book_load($values->fbid)) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * Render whatever the data is as a link to the flipping_book.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($flipping_book, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $flipping_book->url;
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    return $text;
  }
}
