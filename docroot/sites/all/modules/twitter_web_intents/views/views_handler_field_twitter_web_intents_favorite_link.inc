<?php

/**
 * @file
 * Views handler to show Favorite link field.
 */

/**
 * Field handler for Favorite link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_twitter_web_intents_favorite_link extends views_handler_field_twitter_web_intents_link {

  // Overrides default link text.
  protected $defaultText = 'Favorite';

  /**
   * Information about options for all kinds of purposes will be held here.
   * @code
   * 'option_name' => array(
   *  - 'default' => default value,
   *  - 'translatable' => (optional) TRUE/FALSE (wrap in t() on export if true),
   *  - 'contains' => (optional) array of items this contains, with its own
   *      defaults, etc. If contains is set, the default will be ignored and
   *      assumed to be array().
   *  - 'bool' => (optional) TRUE/FALSE Is the value a boolean value. This will
   *      change the export format to TRUE/FALSE instead of 1/0.
   *  - 'export' => (optional) FALSE or a callback for special export handling
   *      if necessary.
   *  - 'unpack_translatable' => (optional) callback for special handling for
   *      translating data within the option, if necessary.
   *  ),
   *
   * @return array
   *   Returns the options of this handler/plugin.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['favorite_related'] = array('default' => '', 'translatable' => FALSE);
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['favorite_options'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Favorite options'),
    );
    $form['favorite_related'] = array(
      '#type' => 'textfield',
      '#title' => t('Favorite related'),
      '#default_value' => $this->options['favorite_related'] ? $this->options['favorite_related'] : '',
      '#fieldset' => 'favorite_options',
    );
  }

  /**
   * Render the field.
   *
   * @param stdClass $values
   *   The values retrieved from the database.
   */
  public function render($values) {
    $link = 'https://twitter.com/intent/favorite?tweet_id=' . $values->twitter_id;
    $link .= $this->build_parameters(
      array(
        'favorite_related' => array(
          'parameter' => 'related',
          'escape_mode' => 1,
        ),
      )
    );
    return parent::render_link($link);
  }

}
