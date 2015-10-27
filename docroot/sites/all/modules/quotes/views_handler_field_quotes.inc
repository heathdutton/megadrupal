<?php
/**
 * @file
 * Provide views data and handlers for quotes.module
 */

/**
 * Field handler to provide an embedded image.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_quotes extends views_handler_field {
  /**
   * Define options available for this field.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['author_link'] = array('default' => 'text');
    return $options;
  }

  /**
   * Build option configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['author_link'] = array(
      '#title' => t('Show author as'),
      '#type' => 'radios',
      '#options' => array(
        'text' => 'Text',
        'author' => 'Link to quotes',
      ),
      '#default_value' => $this->options['author_link'],
    );
  }

  /**
   * Render field output to the browser.
   */
  function render($values) {
    //   drupal_set_message(print_r($values, true));
    $author = $values->{$this->field_alias};
    $type = $this->options['author_link'];
    //   drupal_set_message("\$this->field_alias='$this->field_alias', author='$author', type='$type'.");

    switch ($type) {
      case 'author':
        return l($author, 'quotes/author/' . check_plain($author), array('attributes' => array('rel' => 'tag')));

      default: // Also 'text'
        return check_plain($author);
    }
  }
}
