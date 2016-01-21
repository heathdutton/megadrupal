<?php

/**
 * @file
 * Views plugin handler. Contains all relevant options and related logic.
 * Implements the Views Form API.
 */
class vbo_hide_plugin_display_extender extends views_plugin_display_extender {

  /**
   * Returns setting options.
   */
  function option_definition() {
    // Call parent method so that default functionality not override.
    $options = parent::option_definition();
    $options['vbo_hide'] = array('default' => '');
    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    //Call parent method so that default functionality not override.
    parent::options_form($form, $form_state);
    $vbo_value = $this->display->get_option('vbo_hide');
    switch ($form_state['section']) {
      case 'vbo_hide':
        $form['#title'] .= t('Hide VBO when no data in view');
        $form['vbo_hide'] = array(
          '#type' => 'radios',
          '#options' => array(0 => t('No'), 1 => t('Yes')),
          '#default_value' => isset($vbo_value) ? $this->display->get_option('vbo_hide') : 0,
          '#description' => t('Hide the view bulk operation options if no data in view ?'),
        );
        break;
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    //Call parent method so that default functionality not override.
    parent::options_submit($form, $form_state);
    $vbo_hide = isset($form_state['values']['vbo_hide']) ? $form_state['values']['vbo_hide'] : 0;
    //If no value in any how for VBO hide, then will store 0.
    switch ($form_state['section']) {
      case 'vbo_hide':
        $this->display->set_option('vbo_hide', $vbo_hide);
        break;
    }
  }

  /**
   * Provide the summary for attachment options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    //Call parent method so that default functionality not override.
    parent::options_summary($categories, $options);
    $vbo_hide = check_plain(trim($this->display->get_option('vbo_hide')));
    $options['vbo_hide'] = array(
      'category' => 'other',
      'title' => t('Hide VBO'),
      'value' => $vbo_hide == 1 ? t('Yes') : t('No'),
      'desc' => t('Change whether or not to display view bulk operation options for this view or not.'),
    );
  }
}