<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * flexslider style plugin for the Views module.
 */

/**
 * Implements a style type plugin for the Views module.
 */
class views_swipeview_plugin_style_swipeview extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['swipeview_options']['snap_threshold'] = array('default' => NULL);
    $options['swipeview_options']['hasty_page_flip'] = array('default' => FALSE, 'bool' => TRUE);
    $options['swipeview_options']['loop'] = array('default' => TRUE, 'bool' => TRUE);
    $options['swipeview_options']['navigation'] = array('default' => TRUE, 'bool' => TRUE);
    $options['swipeview_options']['pager'] = array('default' => TRUE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Show a form to edit the style options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $defaults = $this->options['swipeview_options'];
    $form['swipeview_options'] = array(
      '#type' => 'fieldset',
      '#title' => 'SwipeView Options',
      'snap_threshold' => array(
        '#title' => t('Snap Threshold'),
        '#type' => 'textfield',
        '#default_value' => $defaults['snap_threshold'],
        '#description' => t('How much horizontal space should be required for mouse drags to count as a navigation action.'),
      ),
      'hasty_page_flip' => array(
        '#title' => t('Hasty Page Flip'),
        '#type' => 'checkbox',
        '#default_value' => $defaults['hasty_page_flip'],
        '#description' => t('Should the page flip be hasty?'),
      ),
      'loop' => array(
        '#title' => t('Loop'),
        '#type' => 'checkbox',
        '#default_value' => $defaults['loop'],
        '#description' => t('Should the slideshow loop each slide, allowing users to navigate forward and backward indefinitely?'),
      ),
      'navigation' => array(
        '#title' => t('Slide Navigation'),
        '#type' => 'checkbox',
        '#default_value' => $defaults['navigation'],
        '#description' => t('Enable a some arrows to navigate slides with.'),
      ),
      'pager' => array(
        '#title' => t('Pager'),
        '#type' => 'checkbox',
        '#default_value' => $defaults['navigation'],
        '#description' => t('Enable a pager to jump directly to slides.'),
      ),
    );
  }

  /**
   * Validate our plugin settings.
   */
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    $values = $form_state['values']['style_options']['swipeview_options'];
    if ($values['snap_threshold'] != '' && !is_numeric($values['snap_threshold'])) {
      form_set_error('style_options][swipeview_options][snap_threshold', 'Must be int.');
    }
  }

  /**
   * Render the display in this style.
   */
  public function render() {
    // Ensure we are loading the swipeview library and associated JS files.
    drupal_add_library('views_swipeview', 'swipeview');
    $options = $this->options['swipeview_options'];
    // Build a settings array of everything needed to render a slideshow.
    $settings = array(
      $this->view->name . ':' . $this->view->current_display => array(
        'selector' => '.view-dom-id-' . $this->view->dom_id,
        'hasty_page_flip' => $options['hasty_page_flip'],
        'loop' => $options['loop'] ? TRUE : FALSE,
        'snap_threshold' => $options['snap_threshold'],
      )
    );
    // Ensure we add it to the page output and the defer the default views
    // rendering.
    drupal_add_js(array('views_swipeview' => $settings), 'setting');
    return parent::render();
  }
}
// @codingStandardsIgnoreEnd
