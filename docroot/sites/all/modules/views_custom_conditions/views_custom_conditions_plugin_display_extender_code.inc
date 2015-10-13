<?php
/**
 * @file
 * Contains the class to extend views display plugins browse all link.
 */

/**
 * The plugin that added additional setting to views edit form.
 *
 * @ingroup views_display_plugins
 */
class views_custom_conditions_plugin_display_extender_code extends views_plugin_display_extender {
  // Provide a form to edit options for this plugin.
  function options_definition() {
    $options['views_custom_conditions'] = array('default' => '');
  }

  /**
   * Provide the form to set new option.
   */
  function options_form(&$form, &$form_state) {

    switch ($form_state['section']) {
      case 'views_custom_conditions':
        $form['#title'] .= t('Setting');
        $form['views_custom_conditions'] = array(
          '#type' => 'textarea',
          '#description' => t('Please enter table,field,desired match value and operator name saprated by "|" symbol. eg:- A). node|title|%2%|LIKE,B). users|uid|arg1,arg2,arg3|IN. Please use next line for multiple index.'),
          '#default_value' => $this->display->get_option('views_custom_conditions'),
        );
        break;
    }
  }

  /**
   * Inserts the code into the view display.
   */
  function options_submit(&$form, &$form_state) {

    // Not sure I like this being here, but it seems (?) like a logical place.
    $new_option  = $form_state['values']['views_custom_conditions'];
    variable_set('views_custom_conditions_' . $form_state['view']->current_display .'_'. $form_state['view']->name, $new_option);
    switch ($form_state['section']) {
      case 'views_custom_conditions':
        variable_set('views_custom_conditions_' . $form_state['values']['override']['dropdown'], $new_option);
        wr($form_state['values']['override']);
        if($form_state['values']['override']['dropdown'] == 'default') {
          variable_set('views_custom_conditions_' . $form_state['view']->current_display .'_'. $form_state['view']->name, '');
        }
        $this->display->set_option('views_custom_conditions', $new_option);
        $empty = trim($new_option);
        $empty = empty($empty);
      break;
    }
    foreach ($this->extender as $extender) {
      $extender->options_submit($form, $form_state);
    }
  }

  /**
   * Summarizes new option.
   *
   * Lists the fields as either 'Yes' if there is text or 'None' otherwise and
   * categorizes the fields under the 'Other' category.
   */
  function options_summary(&$categories, &$options) {

    $new_option = check_plain(trim($this->display->get_option('views_custom_conditions')));

    if ($new_option) {
      $new_option = t('Set');
    }
    else {
      $new_option = t('Not Set');
    }
    $options['views_custom_conditions'] = array(
      'category' => 'other',
      'title'    => t('Views Custom Conditions'),
      'value'    => $new_option,
      'desc'     => t('Add some option.'),
    );
  }
}
