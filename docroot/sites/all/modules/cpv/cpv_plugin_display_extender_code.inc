<?php
/**
 * @file
 * Contains the class to extend views display plugins with CSS and JS fields.
 */

/**
 * The plugin that handles a full page with optional CSS and JS additions.
 *
 * @ingroup views_display_plugins
 */
class cpv_plugin_display_extender_code extends views_plugin_display_extender {
  /**
   * Provide a form to edit options for this plugin.
   */
  function option_definition(&$options) {
    $options['cpv_css'] = array('default' => '');
    $options['cpv_js'] = array('default' => '');
  }

  /**
   * Attaches any custom CSS and JS code to the view with the highest weight
   * before the view is executed.
   */
  function pre_execute() {
    $display = $this->display;
    $save = variable_get('cpv_save', 'inline');
    
    $css  = trim($display->options['cpv_css']);
    $js   = trim($display->options['cpv_js']);
    switch($save) {
      // Serve code inline.
      case 'inline':
        if (!empty($css)) {
          drupal_add_css($css, array('type' => 'inline', 'group' => CSS_THEME));
        }
        if (!empty($js)) {
          drupal_add_js($js, array('type' => 'inline', 'group' => JS_THEME));
        }
        break;
      // Serve code from file.
      case 'file' :
        $dir = variable_get('cpv_path', 'public://cpv');
        $filename = $display->view->name . '-' . $display->display->id;
        if (!empty($css)) {
          drupal_add_css($dir . '/' . $filename . '.css', array('group' => CSS_THEME));
        }
        if (!empty($js)) {
          drupal_add_js($dir . '/' . $filename . '.js', array('group' => CSS_THEME));
        }
        break;
    }
  }

  /**
   * Provide the form to set the CSS and JS code.
   */
  function options_form(&$form, &$form_state) {
    switch ($form_state['section']) {
      case 'cpv_css':
        $form['#title'] .= t('CSS Code');
        $form['cpv_css'] = array(
          '#type' => 'textarea',
          '#description' => t('Custom CSS rules for this display. Do not include @style tags.', array('@style' => '<style>')),
          '#default_value' => $this->display->get_option('cpv_css'),
        );
        break;
      case 'cpv_js':
        $form['#title'] .= t('Javascript Code');
        $form['cpv_js'] = array(
          '#type' => 'textarea',
          '#description' => t('Custom Javascript for this display. Do not include @script tags.', array('@script' => '<script>')),
          '#default_value' => $this->display->get_option('cpv_js'),
        );
        break;
    }
  }

  /**
   * Validate the inputted code.
   */
  function options_validate(&$form, &$form_state) {
    switch ($form_state['section']) {
      case 'cpv_css':
        $css = $form_state['values']['cpv_css'];
        if (cpv_validate_code($css, 'css')) {
          form_set_error('cpv_css', t('Do not include @style tags in the CSS.', array('@style' => '<style>')));
        }
        break;
      case 'cpv_js':
        $js = $form_state['values']['cpv_js'];
        if (cpv_validate_code($js, 'js')) {
          form_set_error('cpv_js', t('Do not include @script tags in the Javascript.', array('@script' => '<script>')));
        }
        break;
    }
  }

  /**
   * Inserts the code into the view display.
   */
  function options_submit(&$form, &$form_state) {
      $css  = $form_state['values']['cpv_css'];
      $js   = $form_state['values']['cpv_js'];
    switch ($form_state['section']) {
      case 'cpv_css':
        $this->display->set_option('cpv_css', $css);
        $empty = trim($css);
        $empty = empty($empty);
        
        if (variable_get('cpv_save', 'inline') == 'file') {
          $display = $this->display;
          $filename = $display->view->name . '-' . $display->display->id;
          if ($empty) {
            cpv_delete_file($filename . '.css');
          }
          else {
            cpv_save_file($css, $filename . '.css');
          }
        }
        break;
      case 'cpv_js':
        $this->display->set_option('cpv_js', $js);
        $empty = trim($js);
        $empty = empty($empty);
        
        if (variable_get('cpv_save', 'inline') == 'file') {
          $display = $this->display;
          $filename = $display->view->name . '-' . $display->display->id;
          if ($empty) {
            cpv_delete_file($filename . '.js');
          }
          else {
            cpv_save_file($js, $filename . '.js');
          }
        }
        break;
    }
  }

  /**
   * Summarizes the CSS and JS fields.
   *
   * Lists the fields as either 'Yes' if there is code or 'None' otherwise and
   * categorizes the fields under the 'Other' category.
   */
  function options_summary(&$categories, &$options) {
    $css = check_plain(trim($this->display->get_option('cpv_css')));
    if ($css) {
      $css = t('Yes');
    }
    else {
      $css = t('None');
    }

    $options['cpv_css'] = array(
      'category' => 'other',
      'title' => t('CSS Code'),
      'value' => $css,
      'desc' => t('Change custom CSS rules for this display.'),
    );

    $js = check_plain(trim($this->display->get_option('cpv_js')));
    if ($js) {
      $js = t('Yes');
    }
    else {
      $js = t('None');
    }

    $options['cpv_js'] = array(
      'category' => 'other',
      'title' => t('JS Code'),
      'value' => $js,
      'desc' => t('Change custom Javascript rules for this display.'),
    );
  }
}
