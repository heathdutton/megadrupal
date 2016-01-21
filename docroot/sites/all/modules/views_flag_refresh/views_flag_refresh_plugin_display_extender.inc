<?php
/**
 * @file
 * Views field view field handler class.
 *
 */

/**
 * This plugin adds additional settings to the Views AJAX options.
 *
 * @ingroup views_display_plugins
 */
class views_flag_refresh_plugin_display_extender extends views_plugin_display_extender {

  /**
   *  List of new options added by plugin.
   */
  protected function add_options(&$options = array()) {
    $options['use_ajax_flags'] = array('default' => FALSE);
    $options['use_ajax_flags_noscrolltop'] = array('default' => 0);
    $options['use_ajax_flags_widget'] = array('default' => 'default');
  }

  /**
   * Provide a form to edit options for this plugin.
   *
   * Added in Views 3.5.
   */
  function options_definition_alter(&$options = array()) {
    $this->add_options($options);
  }

  /**
   * Provide a form to edit options for this plugin.
   *
   * Used in Views prior to 3.5.
   */
  function option_definition(&$options = array()) {
    $this->add_options($options);
    return $options;
  }

  /**
   * Provide the form to set new option.
   */
  function options_form(&$form, &$form_state) {
    switch ($form_state['section']) {
      case 'views_flag_refresh':
        $form['use_ajax_flags'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Refresh display on flags'),
          '#options' => views_flag_refresh_flag_options_get(),
          '#default_value' => (array) $this->display->get_option('use_ajax_flags'),
          '#description' => t('Refreshes the display via AJAX whenever a user clicks one of the selected flags. This will only take effect if the <em>Use AJAX</em> option is set to <em>Yes</em>.'),
        );

        $form['use_ajax_flags_noscrolltop'] = array(
          '#type' => 'checkbox',
          '#title' => t('Disable scroll to top of this view.'),
          '#default_value' => $this->display->get_option('use_ajax_flags_noscrolltop'),
          '#description' => t('Check if you want disable scroll to the top of this view after AJAX update.'),
        );

        $default_value = $this->display->get_option('use_ajax_flags_widget');
        $form['use_ajax_flags_widget'] = array(
          '#type' => 'radios',
          '#title' => t('Refresh widget'),
          '#options' => views_flag_refresh_widget_options_get(),
          '#default_value' => $default_value ? $default_value : 'default',
          '#description' => t('The widget alters the display of the view as it is refreshing, for example by adding a throbber or textual message.'),
        );

        break;
    }
  }

  /**
   * Inserts the code into the view display.
   */
  function options_submit(&$form, &$form_state) {
    switch ($form_state['section']) {
      case 'views_flag_refresh':
        $this->display->set_option('use_ajax_flags', $form_state['values']['use_ajax_flags']);
        $this->display->set_option('use_ajax_flags_noscrolltop', $form_state['values']['use_ajax_flags_noscrolltop']);
        $this->display->set_option('use_ajax_flags_widget', $form_state['values']['use_ajax_flags_widget']);
        break;
    }
  }

  /**
   * Summarizes new option.
   *
   * Lists the fields as either 'Yes' if there is text or 'None' otherwise and
   * categorizes the fields under the 'Other' category.
   */
  function options_summary(&$categories, &$options) {
    $flags = $this->display->get_option('use_ajax_flags');
    $flags_selected = array();
    if (is_array($flags)) {
      $flags_names = views_flag_refresh_flag_options_get();
      foreach ($flags as $flag) {
        if ($flag) {
          $flags_selected[] = $flags_names[$flag];
        }
      }
    }

    if ($flags_selected) {
      $flags_selected = implode(', ', $flags_selected);
    }
    else {
      $flags_selected = t('None');
    }

    $options['views_flag_refresh'] = array(
      'category' => 'other',
      'title'    => t('Flag refresh'),
      'value'    => $flags_selected,
      'desc'     => t('Choose flags that can refresh display.'),
    );
  }
}
