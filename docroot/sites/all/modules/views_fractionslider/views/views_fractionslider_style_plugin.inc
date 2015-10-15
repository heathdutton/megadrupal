<?php
/**
 * @file
 * Provide an fractionslider style plugin for Views. This file is autoloaded by views.
 */

/**
  * Implementation of views_plugin_style().
  */
class views_fractionslider_style_plugin extends views_plugin_style {
  /**
   * Set default options
   */
function option_definition() {
  $options = parent::option_definition();
    return $options;
  }

/**
  * Render the given style.
  */
function options_form(&$form, &$form_state) {
  parent::options_form($form, $form_state);

  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['class'] = array(
    '#title' => t('Div class'),
    '#description' => t('The class to provide on the Div element itself. In addition with "slide" class.'),
    '#type' => 'textfield',
    '#size' => '30',
    '#default_value' => $this->options['class'],
    '#fieldset' => 'general_settings',
  );
  $form['pager'] = array(
    '#type' => 'select',
    '#title' => t('Pager'),
    '#options' => array(
       'true' => t('True'),
       'false' => t('False'),
     ),
    '#default_value' => $this->options['pager'],
    '#description' => t('Set Pager True or False. Default is True.'),
    '#fieldset' => 'general_settings',
  );
  $form['controls'] = array(
    '#type' => 'select',
    '#title' => t('Controls'),
    '#options' => array(
      'true' => t('True'),
      'false' => t('False'),
    ),
    '#default_value' => $this->options['controls'],
    '#description' => t('Set Controls True or False. Default is True.'),
    '#fieldset' => 'general_settings',
  );
  $form['views_dimensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Dimensions'),
    '#default_value' => ($this->options['views_dimensions']) ? $this->options['views_dimensions'] : '1000, 400',
    '#description' => t('Default: 1000, 400'),
    '#fieldset' => 'general_settings',
  );
  $form['views_fullwidth'] = array(
    '#type' => 'select',
    '#title' => t('Full width'),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#default_value' => $this->options['views_fullwidth'],
    '#description' => t('Default: false'),
    '#fieldset' => 'general_settings',
  );
  $form['views_responsive'] = array(
    '#type' => 'select',
    '#title' => t('Responsive'),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#default_value' => $this->options['views_responsive'],
    '#description' => t('Default: true'),
    '#fieldset' => 'general_settings',
  );
  $form['views_increase'] = array(
    '#type' => 'select',
    '#title' => t('Increase'),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#default_value' => $this->options['views_increase'],
    '#description' => t('Default: false'),
    '#fieldset' => 'general_settings',
  );
  $form['fields_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
   
    $fields = $this->view->display['default']->display_options['fields'];
    foreach ($fields as $key => $field) {
        $form[$field['id']] = array(
          '#type' => 'fieldset',
          '#title' => check_plain(t($field['field'])),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#fieldset' => 'fields_settings',
        );
        $form[$field['id']]['data-in'] = array(
            '#type' => 'select',
            '#title' => t('data-in'),
            '#options' => array(
              'left' => t('left'),
              'fade' => t('fade'),
              'none' => t('none'),
              'right' => t('right'),
              'top' => t('top'),
              'bottom' => t('bottom'),
              'bottomLeft' => t('bottomLeft'),
              'bottomRight' => t('bottomRight'),
              'topLeft' => t('topLeft'),
              'topRight' => t('topRight'),
            ),
            '#default_value' => $this->options[$field['id']]['data-in'],
            '#description' => t('Type of the in-animation (default is left).'),
            '#fieldset' => $field['id'],
        );
        $form[$field['id']]['data-out'] = array(
            '#type' => 'select',
            '#title' => t('data-out'),
            '#options' => array(
              'fade' => t('fade'),
              'none' => t('none'),
              'left' => t('left'),
              'right' => t('right'),
              'top' => t('top'),
              'bottom' => t('bottom'),
              'bottomLeft' => t('bottomLeft'),
              'bottomRight' => t('bottomRight'),
              'topLeft' => t('topLeft'),
              'topRight' => t('topRight'),
            ),
            '#default_value' => $this->options[$field['id']]['data-out'],
            '#description' => t('Type of the out-animation (default is fade).'),
            '#fieldset' => $field['id'],
        );
        $form[$field['id']]['data-step'] = array(
            '#type' => 'select',
            '#title' => t('data-step'),
            '#options' => array(
              0 => t('0'),
              1 => t('1'),
              2 => t('2'),
              3 => t('3'),
              4 => t('4'),
              5 => t('5'),
              6 => t('6'),
              7 => t('7'),
              8 => t('8'),
              9 => t('9'),
            ),
            '#default_value' => $this->options[$field['id']]['data-step'],
            '#description' => t('You can group your elements in different steps. All animation of one step will start at the same time (maybe with your choosen element-specific data-delay). Elements of the next step will not start before the previous step is finished. If an element has no data-step attribute the plugin will thread it as having data-step=”0″.'),
            '#fieldset' => $field['id'],
        );
        $form[$field['id']]['data-ease-in'] = array(
            '#type' => 'select',
            '#title' => t('data-ease-in'),
            '#options' => array(
                '' => t('-None-'),
                'linear' => t('linear'),
                'swing' => t('swing'),
                'easeInQuad' => t('easeInQuad'),
                'easeOutQuad' => t('easeOutQuad'),
                'easeInOutQuad' => t('easeInOutQuad'),
                'easeInCubic' => t('easeInCubic'),
                'easeOutCubic' => t('easeOutCubic'),
                'easeInOutCubic' => t('easeInOutCubic'),
                'easeInQuart' => t('easeInQuart'),
                'easeOutQuart' => t('easeOutQuart'),
                'easeInOutQuart' => t('easeInOutQuart'),
                'easeInQuint' => t('easeInQuint'),
                'easeOutQuint' => t('easeOutQuint'),
                'easeInOutQuint' => t('easeInOutQuint'),
                'easeInExpo' => t('easeInExpo'),
                'easeOutExpo' => t('easeOutExpo'),
                'easeInOutExpo' => t('easeInOutExpo'),
                'easeInSine' => t('easeInSine'),
                'easeOutSine' => t('easeOutSine'),
                'easeInOutSine' => t('easeInOutSine'),
                'easeInCirc' => t('easeInCirc'),
                'easeOutCirc' => t('easeOutCirc'),
                'easeInOutCirc' => t('easeInOutCirc'),
                'easeInElastic' => t('easeInElastic'),
                'easeOutElastic' => t('easeOutElastic'),
                'easeInOutElastic' => t('easeInOutElastic'),
                'easeInBack' => t('easeInBack'),
                'easeOutBack' => t('easeOutBack'),
                'easeInOutBack' => t('easeInOutBack'),
                'easeInBounce' => t('easeInBounce'),
                'easeOutBounce' => t('easeOutBounce'),
                'easeInOutBounce' => t('easeInOutBounce'),
            ),
            '#default_value' => $this->options[$field['id']]['data-ease-in'],
            '#description' => t('easing for the animations (you can use all in jquery-ui contained <a target="_blank" href="http://jqueryui.com/effect/#easing">easing methods)</a>.'),
            '#fieldset' => $field['id'],
        );
        $form[$field['id']]['data-ease-out'] = array(
            '#type' => 'select',
            '#title' => t('data-ease-out'),
            '#options' => array(
                '' => t('-None-'),
                'linear' => t('linear'),
                'swing' => t('swing'),
                'easeInQuad' => t('easeInQuad'),
                'easeOutQuad' => t('easeOutQuad'),
                'easeInOutQuad' => t('easeInOutQuad'),
                'easeInCubic' => t('easeInCubic'),
                'easeOutCubic' => t('easeOutCubic'),
                'easeInOutCubic' => t('easeInOutCubic'),
                'easeInQuart' => t('easeInQuart'),
                'easeOutQuart' => t('easeOutQuart'),
                'easeInOutQuart' => t('easeInOutQuart'),
                'easeInQuint' => t('easeInQuint'),
                'easeOutQuint' => t('easeOutQuint'),
                'easeInOutQuint' => t('easeInOutQuint'),
                'easeInExpo' => t('easeInExpo'),
                'easeOutExpo' => t('easeOutExpo'),
                'easeInOutExpo' => t('easeInOutExpo'),
                'easeInSine' => t('easeInSine'),
                'easeOutSine' => t('easeOutSine'),
                'easeInOutSine' => t('easeInOutSine'),
                'easeInCirc' => t('easeInCirc'),
                'easeOutCirc' => t('easeOutCirc'),
                'easeInOutCirc' => t('easeInOutCirc'),
                'easeInElastic' => t('easeInElastic'),
                'easeOutElastic' => t('easeOutElastic'),
                'easeInOutElastic' => t('easeInOutElastic'),
                'easeInBack' => t('easeInBack'),
                'easeOutBack' => t('easeOutBack'),
                'easeInOutBack' => t('easeInOutBack'),
                'easeInBounce' => t('easeInBounce'),
                'easeOutBounce' => t('easeOutBounce'),
                'easeInOutBounce' => t('easeInOutBounce'),
            ),
            '#default_value' => $this->options[$field['id']]['data-ease-out'],
            '#description' => t('easing for the animations (you can use all in jquery-ui contained <a target="_blank" href="http://jqueryui.com/effect/#easing">easing methods)</a>.'),
            '#fieldset' => $field['id'],
        );
        /*$form[$field['id']]['datadelay'] = array(
          '#title' => t('Data Delay'),
          '#description' => t('Time in ms before the in transition starts (in the current step / see steps). Default is 1000.'),
          '#type' => 'textfield',
          '#size' => '10',
          '#default_value' => ($this->options[$field['id']]['datadelay']) ? $this->options[$field['id']]['datadelay'] : '',
          '#fieldset' => $field['id'],
        );*/
        $form[$field['id']]['data-time'] = array(
          '#title' => t('Data Time'),
          '#description' => t('Time after which the elements animation is complete. It will start at the beginning of the slide/step, or after its specificed delay. Default is 1000. Add 00 for none/instead of 0.'),
          '#type' => 'textfield',
          '#size' => '10',
          '#default_value' => ($this->options[$field['id']]['data-time']) ? $this->options[$field['id']]['data-time'] : '1000',
          '#fieldset' => $field['id'],
        );
        $form[$field['id']]['space'] = array(
          '#title' => t('Field Spacing Top'),
          '#description' => t('The Space/Gap between fields while sliding. Similar padding-top. Default is 30. Add 00 for none/instead of 0.'),
          '#type' => 'textfield',
          '#size' => '10',
          '#default_value' => ($this->options[$field['id']]['space']) ? $this->options[$field['id']]['space'] : '30',
          '#fieldset' => $field['id'],
        );
        $form[$field['id']]['lspace'] = array(
          '#title' => t('Field Spacing Left'),
          '#description' => t('The Space/Gap between fields while sliding. Similar padding-left. Default is 30. Add 00 for none/instead of 0.'),
          '#type' => 'textfield',
          '#size' => '10',
          '#default_value' => ($this->options[$field['id']]['lspace']) ? $this->options[$field['id']]['lspace'] : '30',
          '#fieldset' => $field['id'],
        );

    }
    
  }
}
