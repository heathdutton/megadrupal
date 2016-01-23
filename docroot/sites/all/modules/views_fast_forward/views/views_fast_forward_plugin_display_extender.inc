<?php

/**
 * @file
 * The actual Views display extender implementation.
 *
 * Here we extend the Views display extender class to add
 * the Fast Forward functionality under the "other" heading
 * of the "advanced" section.
 */

/**
 * Extends the Views Plugin Display Extender.
 */
class views_fast_forward_plugin_display_extender extends views_plugin_display_extender {

  /**
   * Provide Views with info on how to handle our options.
   */
  function option_definition() {
    return array(
      'ff_enabled' => array(
        'default' => 0,
        'bool' => TRUE,
      ),
      'ff_manual' => array(
        'default' => 0,
        'bool' => TRUE,
      ),
      'ff_path' => array(
        'default' => '',
      ),
      'ff_tokenize' => array(
        'default' => 0,
        'bool' => TRUE,
      ),
    );
  }

  /**
   * Allow these options to have defaults.
   *
   * Seems redundant, but necessary in order for these options
   * to be available when you wish to export a View.
   */
  function options_definition_alter(&$options) {
    $options = $options + $this->option_definition();
    return $options;
  }

  /**
   * Provide a list of options for this plugin.
   */
  public function options_summary(&$categories, &$options) {
    $options['views_fast_forward'] = array(
      'category' => 'other',
      'title' => t('Fast forward'),
      'value' => $this->display->get_option('ff_enabled') ? t('Enabled') : t('Disabled'),
      'desc' => t('If this view only outputs one result, fast forward to that result.'),
    );
  }

  /**
   * Provide a simple public interface to pass the current display's options.
   */
  public function get_options() {
    return array(
      'ff_enabled' => $this->display->get_option('ff_enabled'),
      'ff_manual' => $this->display->get_option('ff_manual'),
      'ff_path' => $this->display->get_option('ff_path'),
      'ff_tokenize' => $this->display->get_option('ff_tokenize'),
    );
  }


  /**
   * Generate the form to set the redirection path.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    switch ($form_state['section']) {
      case 'views_fast_forward':
        // Build the basic form.
        $form['#title'] .= t('Fast forward path for this display');
        $description = <<<'EOT'
          Enable  to directly skip to an entity if this display only returns a single result.
          You can choose to automatically detect the path or to manually construct it.
EOT;

        // Build the general help text.
        $form['ff_help_1'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="description">' . t($description) . '</div>',
        );

        // Build the enable/disable checkbox.
        $form['ff_enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable fast forward'),
          '#default_value' => $this->display->get_option('ff_enabled'),
          '#class' => 'edit-ff-enabled',
        );

        // Build the enable/disable checkbox.
        $form['ff_manual'] = array(
          '#type' => 'checkbox',
          '#title' => t('Manually construct path'),
          '#default_value' => $this->display->get_option('ff_manual'),
          '#class' => 'edit-ff-manual',
        );

        // Setup a simple wrapper.
        $form['ff_wrapper'] = array(
          '#type' => 'fieldset',
          '#title' => 'Manual path',
          '#dependency' => array(
            'edit-ff-manual' => array(1),
          ),
        );

        // Build the path input.
        $form['ff_wrapper']['ff_path'] = array(
          '#type' => 'textfield',
          '#description' => t('The path to which to redirect.'),
          '#default_value' => $this->display->get_option('ff_path'),
          '#dependency' => array(
            'edit-ff-manual' => array(1),
          ),
        );

        // Build the tokonized input checkbox.
        $form['ff_wrapper']['ff_tokenize'] = array(
          '#type' => 'checkbox',
          '#title' => t('Use replacement tokens from the first (and only) row.'),
          '#default_value' => $this->display->get_option('ff_tokenize'),
          '#dependency' => array(
            'edit-ff-manual' => array(1),
          ),
        );

        // Fetch the available tokens.
        $options = $this->_getTokenItems();

        // Create the collapsible "Replacement Pattern" overview block.
        if (!empty($options)) {
          $form['ff_wrapper']['token_help'] = array(
            '#type' => 'fieldset',
            '#title' => t('Replacement patterns'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#description' => t('The following tokens are available to dynamically construct your path.'),
            'content' => $this->_createTokenRenderArray($options),
            '#id' => 'ff-options-token-help',
            '#dependency' => array(
              'edit-ff-tokenize' => array(1),
            ),
          );
        }

        // Setup a simple wrapper.
        $form['ff_status_wrapper'] = array(
          '#type' => 'fieldset',
          '#title' => 'Automatic path',
          '#description' => t('You have chosen to automatically detect the path.'),
          '#dependency' => array(
            'edit-ff-manual' => array(0),
          ),
        );

    }

  }

  /**
   * Save the different options.
   */
  public function options_submit(&$form, &$form_state) {
    switch ($form_state['section']) {
      case 'views_fast_forward':
        $this->display->set_option('ff_enabled', $form_state['values']['ff_enabled']);
        $this->display->set_option('ff_path', $form_state['values']['ff_path']);
        $this->display->set_option('ff_tokenize', $form_state['values']['ff_tokenize']);
        $this->display->set_option('ff_manual', $form_state['values']['ff_manual']);
    }
  }

  /**
   * Get a list of the available fields for token replacement.
   */
  private function _getTokenItems() {
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options["[$field]"] = $handler->ui_name();
    }

    return $options;
  }

  /**
   * Build a Drupal render array from given tokens.
   */
  private function _createTokenRenderArray($options) {
    $options_ra = array(
      '#theme' => 'item_list',
      '#items' => array(),
      '#type' => 'ul',
    );

    foreach ($options as $token => $field) {
      array_push($options_ra['#items'], $token . ' == ' . $field);
    }

    return $options_ra;
  }

}
