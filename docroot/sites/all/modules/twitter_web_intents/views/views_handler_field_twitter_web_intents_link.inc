<?php

/**
 * @file
 * Views handler to show a link field, to be inherited from others.
 */

/**
 * Main field handler for links.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_twitter_web_intents_link extends views_handler_field {

  // Default link text (overrided from other handlers).
  protected $defaultText = 'Link';

  // Required column from Twitter table (overrided from other handlers).
  protected $requiredTwitterField = 'twitter_id';

  /**
   * Construct a new field handler.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields[$this->requiredTwitterField] = $this->requiredTwitterField;
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

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
    $options['text'] = array('default' => $this->defaultText, 'translatable' => TRUE);
    $options['link_title'] = array('default' => '', 'translatable' => TRUE);
    $options['link_name'] = array('default' => '', 'translatable' => FALSE);
    $options['link_rel'] = array('default' => '', 'translatable' => FALSE);
    $options['link_classes'] = array('default' => '', 'translatable' => FALSE);
    $options['link_style'] = array('default' => '', 'translatable' => FALSE);
    $options['link_target'] = array('default' => '', 'translatable' => FALSE);
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'] ? $this->options['text'] : $this->defaultText,
    );
    $form['link_options'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Link options'),
    );
    $form['link_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('The description displayed when hovering over the link.'),
      '#default_value' => $this->options['link_title'] ? $this->options['link_title'] : '',
      '#fieldset' => 'link_options',
    );
    $form['link_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#fieldset' => 'link_options',
      '#default_value' => $this->options['link_name'] ? $this->options['link_name'] : '',
    );
    $form['link_rel'] = array(
      '#type' => 'textfield',
      '#title' => t('Relationship'),
      '#description' => t("Specifies the relationship between the current page and the link. Enter 'nofollow' here to nofollow this link."),
      '#fieldset' => 'link_options',
      '#default_value' => $this->options['link_rel'] ? $this->options['link_rel'] : '',
    );
    $form['link_classes'] = array(
      '#type' => 'textfield',
      '#title' => t('Classes'),
      '#description' => t('Enter additional classes to be added to the link.'),
      '#fieldset' => 'link_options',
      '#default_value' => $this->options['link_classes'] ? $this->options['link_classes'] : '',
    );
    $form['link_style'] = array(
      '#type' => 'textfield',
      '#title' => t('Style'),
      '#description' => t('Enter additional styles to be applied to the link.'),
      '#fieldset' => 'link_options',
      '#default_value' => $this->options['link_style'] ? $this->options['link_style'] : '',
    );
    $form['link_target'] = array(
      '#type' => 'select',
      '#title' => t('Target'),
      '#required' => FALSE,
      '#options' => array(
        '' => 'None (i.e. same window)',
        '_blank' => 'New window (_blank)',
        '_top' => 'Top window (_top)',
        '_self' => 'Same window (_self)',
        '_parent' => 'Parent window (_parent)',
      ),
      '#description' => t('Specifies where to open the link. Using this attribute breaks XHTML validation.'),
      '#fieldset' => 'link_options',
      '#default_value' => $this->options['link_target'] ? $this->options['link_target'] : '',
    );
  }

  /**
   * Auxiliar function to get link attributes from options.
   */
  public function get_link_options() {

    $cached_attributes = &drupal_static('twitter_web_intents_attrs_' . $this->view->dom_id . '_' . $this->options['id'], FALSE);

    if ($cached_attributes === FALSE) {
      $given_options = array();
      $given_options['attributes'] = array();

      $option_keys = array(
        'title' => 'link_title',
        'name' => 'link_name',
        'rel' => 'link_rel',
        'class' => 'link_classes',
        'style' => 'link_style',
        'target' => 'link_target',
      );

      foreach ($option_keys as $attribute => $option_key) {
        if (isset($this->options[$option_key]) && $this->options[$option_key]) {
          $given_options['attributes'][$attribute] = $this->options[$option_key];
        }
      }

      $cached_attributes = $given_options;

      return $given_options;

    }
    else {

      return $cached_attributes;

    }

  }

  /**
   * Auxiliar function to build URL parameters from options.
   */
  public function build_parameters($parameters) {
    $cached_parameters = &drupal_static('twitter_web_intents_params_' . $this->view->dom_id . '_' . $this->options['id'], FALSE);

    if ($cached_parameters === FALSE) {
      $params = '';
      foreach ($parameters as $param => $configuration) {
        $escape_mode = isset($configuration['escape_mode']) ? $configuration['escape_mode'] : 0;
        $param_id = isset($configuration['parameter']) ? $configuration['parameter'] : 'dummy';
        if (isset($this->options[$param]) && $this->options[$param]) {
          if ($escape_mode === 1) {
            $val = str_replace(
              array(':', ' '),
              array('%3A', '%20'),
              $this->options[$param]
            );
          }
          elseif ($escape_mode === 2) {
            $val = urlencode($this->options[$param]);
          }
          else {
            $val = $this->options[$param];
          }
          $params .= ('&' . $param_id . '=' . $val);
        }
      }
      $cached_parameters = $params;
      return $params;
    }
    else {
      return $cached_parameters;
    }

  }

  /**
   * Auxiliar function to help render the link from child classes.
   */
  public function render_link($link) {
    $added_js = &drupal_static('twitter_web_intents_added_js', FALSE);
    if (!$added_js) {
      $added_js = TRUE;
      drupal_add_js('http://platform.twitter.com/widgets.js');
    }
    if (!(isset($this->options['text'])) || !($this->options['text'])) {
      return l(t($this->defaultText), $link, $this->get_link_options());
    }
    else {
      return l(t($this->options['text']), $link, $this->get_link_options());
    }
  }

}
