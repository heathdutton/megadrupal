<?php
// @codingStandardsIgnoreStart

/**
 * @file
 * Blocksit style plugin for the Views module.
 */

/**
 * Implements a style type plugin for the Views module.
 */
class views_blocksit_plugin_style_blocksit extends views_plugin_style {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['blocksit_options']['numOfCol'] = array('default' => 4);
    $options['blocksit_options']['offsetX'] = array('default' => 5);
    $options['blocksit_options']['offsetY'] = array('default' => 5);
    $options['blocksit_options']['breakPoints'] = array('default' => "");
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $defaults = $this->options['blocksit_options'];

    $form['blocksit_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Blocksit Options'),
      'numOfCol' => array(
        '#title' => t('Number of Columns'),
        '#type' => 'textfield',
        '#default_value' => $defaults['numOfCol'],
        '#description' => t('How many columns of blocks should there be?'),
      ),
      'offsetX' => array(
        '#title' => t('Offset X'),
        '#type' => 'textfield',
        '#default_value' => $defaults['offsetX'],
        '#element_validate' => array('element_validate_integer_positive'),
        '#description' => t('The amount of horizontal space (in pixels) between the edge of a column and the start of a block on either side.'),
      ),
      'offsetY' => array(
        '#title' => t('Offset Y'),
        '#type' => 'textfield',
        '#default_value' => $defaults['offsetY'],
        '#element_validate' => array('element_validate_integer_positive'),
        '#description' => t('The amount of vertical space (in pixels) between the end of one block and the start of another.'),
      ),
      'breakPoints' => array(
        '#title' => t('Break Points'),
        '#type' => 'textfield',
        '#default_value' => $defaults['breakPoints'],
        '#description' => t('Add a responsive breakpoint. Use the format: [container_width,number_of_columns]. e.g. [1100, 3] [700, 2] etc.
        Please order the breakpoints from largest width first down to smaller screen widths. Whitespace will be removed.'),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    // Ensure we are loading the blocksit library and associated JS files.
    drupal_add_library('views_blocksit', 'blocksit');
    $options = $this->options['blocksit_options'];

    // Change the format of the breakpoints for the JS.
    // Remove all whitespace.
    $break_points = preg_replace('/\s+/', '', $options['breakPoints']);
    $break_points = explode('][', $break_points);

    if (!empty($break_points)) {
      foreach($break_points as &$break_point) {
        $break_point = str_replace('[', '', $break_point);
        $break_point = str_replace(']', '', $break_point);
      }
    }

    $settings = array(
      $this->view->name . ':' . $this->view->current_display => array(
        'selector' => '.view-dom-id-' . $this->view->dom_id,
        'numOfCol' => $options['numOfCol'],
        'offsetX' => $options['offsetX'],
        'offsetY' => $options['offsetY'],
        'breakPoints' => $break_points,
      )
    );
    // Ensure we add it to the page output and the defer the default views
    // rendering.
    drupal_add_js(array('views_blocksit' => $settings), 'setting');
    return parent::render();
  }
}

// @codingStandardsIgnoreEnd
