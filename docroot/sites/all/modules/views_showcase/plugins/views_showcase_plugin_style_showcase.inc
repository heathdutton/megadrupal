<?php
/**
 * @file
 * Contains the showcase style plugin.
 */

/**
 * Style plugin to render each item in a grid cell.
 *
 * @ingroup views_style_plugins
 */
class views_showcase_plugin_style_showcase extends views_plugin_style_list {
  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['cycle']     = array('default' => 'scrollRight');
    $options['timeout']   = array('default' => '4000');
    $options['sync']      = array('default' => 'true');
    $options['pause']     = array('default' => 'false');
    $options['listpause'] = array('default' => 'false');
    $options['easing']    = array('default' => 'easeOutBounce');
    $options['skin']      = array('default' => 'right');

    $options['navigation_box_field'] = array();
    $options['big_box_field']        = array();
    $options['link_anchor_field']    = array();
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['grouping']);

    $form['skin'] = array(
      '#type' => 'select',
      '#title' => t('Skin'),
      '#default_value' => $this->options['skin'],
      '#options' => array('right' => t('Right'), 'left' => t('Left')),
    );
    $form['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#default_value' => $this->options['timeout'],
      '#description' => t('The timeout between each slide. In miliseconds.'),
    );
    $form['cycle'] = array(
      '#type' => 'select',
      '#title' => t('Cycle Plugin Effect'),
      '#description' => t('Choose the desired cycle plugin effect.'),
      '#default_value' => $this->options['cycle'],
      '#options' => $this->get_cycle_options(),
    );
    $form['sync'] = array(
      '#type' => 'select',
      '#title' => t('Sync setting'),
      '#description' => t('Choose if the sycle effect will be synchronized or not.'),
      '#default_value' => $this->options['sync'],
      '#options' => array('true' => t('Yes'), 'false' => t('No')),
    );
    $form['pause'] = array(
      '#type' => 'select',
      '#title' => t('Pause on hover the slide'),
      '#description' => t('Choose if the transition stops when the mouse is over the slide.'),
      '#default_value' => $this->options['pause'],
      '#options' => array('true' => t('Yes'), 'false' => t('No')),
    );
    $form['listpause'] = array(
      '#type' => 'select',
      '#title' => t('Pause on hover the list'),
      '#description' => t('Choose if the transition stops when the mouse is over the slide title (in the navigation list).'),
      '#default_value' => $this->options['listpause'],
      '#options' => array('true' => t('Yes'), 'false' => t('No')),
    );
    $form['easing'] = array(
      '#type' => 'select',
      '#title' => t('Easing Plugin Effec'),
      '#description' => t('Choose the desired easing plugin effect.'),
      '#default_value' => $this->options['easing'],
      '#options' => $this->get_easing_options(),
    );

    // Pre-build all of our option lists for the dials and switches that follow.
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label;
      }
      else {
        $fields[$field] = $handler->ui_name();
      }
    }

    $form['showcase_display_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Showcase Display Options'),
      '#theme' => 'views_showcase_checkbox_table',
    );

    //Setting the default value of the showcase_display_options
    if (isset($this->options['showcase_display_options'])) {
      $navigation_box_default = $this->options['showcase_display_options']['navigation_box_field'];
      $big_box_default        = $this->options['showcase_display_options']['big_box_field'];
      $link_anchor_default    = $this->options['showcase_display_options']['link_anchor_field'];
    }
    else {
      $navigation_box_default = array();
      $big_box_default        = array();
      $link_anchor_default    = array();
    }

    $form['showcase_display_options']['navigation_box_field'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Navigation Box'),
      '#options' => $fields,
      '#default_value' => $navigation_box_default,
    );

    $form['showcase_display_options']['big_box_field'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Big Box'),
      '#options' => $fields,
      '#default_value' => $big_box_default,
    );

    $form['showcase_display_options']['link_anchor_field'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Link to Anchor'),
      '#options' => $fields,
      '#default_value' => $link_anchor_default,
    );
  }

  function get_cycle_options() {
    return array(
      'all' => t('Random'),
      'blindX' => 'blindX',
      'blindY' => 'blindY',
      'blindZ' => 'blindZ',
      'cover' => 'cover',
      'curtainX' => 'curtainX',
      'curtainY' => 'curtainY',
      'fade' => 'fade',
      'fadeZoom' => 'fadeZoom',
      'growX' => 'growX',
      'growY' => 'growY',
      'scrollUp' => 'scrollUp',
      'scrollDown' => 'scrollDown',
      'scrollLeft' => 'scrollLeft',
      'scrollRight' => 'scrollRight',
      'scrollHorz' => 'scrollHorz',
      'scrollVert' => 'scrollVert',
      'shuffle' => 'shuffle',
      'slideX' => 'slideX',
      'slideY' => 'slideY',
      'toss' => 'toss',
      'turnUp' => 'turnUp',
      'turnDown' => 'turnDown',
      'turnLeft' => 'turnLeft',
      'turnRight' => 'turnRight',
      'uncover' => 'uncover',
      'wipe' => 'wipe',
      'zoom' => 'zoom'
    );
  }

  function get_easing_options() {
    return array(
      "" => t('None'),
      "jswing" => "jswing",
      "easeInQuad" => "easeInQuad",
      "easeOutQuad" => "easeOutQuad",
      "easeInOutQuad" => "easeInOutQuad",
      "easeInCubic" => "easeInCubic",
      "easeOutCubic" => "easeOutCubic",
      "easeInOutCubic" => "easeInOutCubic",
      "easeInQuart" => "easeInQuart",
      "easeOutQuart" => "easeOutQuart",
      "easeInOutQuart" => "easeInOutQuart",
      "easeInQuint" => "easeInQuint",
      "easeOutQuint" => "easeOutQuint",
      "easeInOutQuint" => "easeInOutQuint",
      "easeInSine" => "easeInSine",
      "easeOutSine" => "easeOutSine",
      "easeInOutSine" => "easeInOutSine",
      "easeInExpo" => "easeInExpo",
      "easeOutExpo" => "easeOutExpo",
      "easeInOutExpo" => "easeInOutExpo",
      "easeInCirc" => "easeInCirc",
      "easeOutCirc" => "easeOutCirc",
      "easeInOutCirc" => "easeInOutCirc",
      "easeInElastic" => "easeInElastic",
      "easeOutElastic" => "easeOutElastic",
      "easeInOutElastic" => "easeInOutElastic",
      "easeInBack" => "easeInBack",
      "easeOutBack" => "easeOutBack",
      "easeInOutBack" => "easeInOutBack",
      "easeInBounce" => "easeInBounce",
      "easeOutBounce" => "easeOutBounce",
      "easeInOutBounce" => "easeInOutBounce"
    );
  }
}
