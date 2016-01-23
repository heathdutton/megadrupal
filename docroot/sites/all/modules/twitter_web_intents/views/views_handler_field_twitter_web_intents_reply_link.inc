<?php

/**
 * @file
 * Views handler to show Reply link field.
 */

/**
 * Field handler for Reply link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_twitter_web_intents_reply_link extends views_handler_field_twitter_web_intents_link {

  // Overrides default link text.
  protected $defaultText = 'Reply';

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
    $options['reply_url'] = array('default' => '', 'translatable' => FALSE);
    $options['reply_via'] = array('default' => '', 'translatable' => FALSE);
    $options['reply_text'] = array('default' => '', 'translatable' => FALSE);
    $options['reply_hashtags'] = array('default' => '', 'translatable' => FALSE);
    $options['reply_related'] = array('default' => '', 'translatable' => FALSE);
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['reply_options'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Reply options'),
    );
    $form['reply_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Reply URL'),
      '#default_value' => $this->options['reply_url'] ? $this->options['reply_url'] : '',
      '#fieldset' => 'reply_options',
    );
    $form['reply_via'] = array(
      '#type' => 'textfield',
      '#title' => t('Reply via'),
      '#default_value' => $this->options['reply_via'] ? $this->options['reply_via'] : '',
      '#fieldset' => 'reply_options',
    );
    $form['reply_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Reply text'),
      '#default_value' => $this->options['reply_text'] ? $this->options['reply_text'] : '',
      '#fieldset' => 'reply_options',
    );
    $form['reply_hashtags'] = array(
      '#type' => 'textfield',
      '#title' => t('Reply hashtags'),
      '#default_value' => $this->options['reply_hashtags'] ? $this->options['reply_hashtags'] : '',
      '#fieldset' => 'reply_options',
    );
    $form['reply_related'] = array(
      '#type' => 'textfield',
      '#title' => t('Reply related'),
      '#default_value' => $this->options['reply_related'] ? $this->options['reply_related'] : '',
      '#fieldset' => 'reply_options',
    );
  }

  /**
   * Render the field.
   *
   * @param stdClass $values
   *   The values retrieved from the database.
   */
  public function render($values) {
    $link = 'https://twitter.com/intent/tweet?in_reply_to=' . $values->twitter_id;
    $link .= $this->build_parameters(
      array(
        'reply_url' => array(
          'parameter' => 'url',
          'escape_mode' => 2,
        ),
        'reply_via' => array(
          'parameter' => 'via',
          'escape_mode' => 0,
        ),
        'reply_text' => array(
          'parameter' => 'text',
          'escape_mode' => 2,
        ),
        'reply_hashtags' => array(
          'parameter' => 'hashtags',
          'escape_mode' => 0,
        ),
        'reply_related' => array(
          'parameter' => 'related',
          'escape_mode' => 1,
        ),
      )
    );
    return parent::render_link($link);
  }

}
