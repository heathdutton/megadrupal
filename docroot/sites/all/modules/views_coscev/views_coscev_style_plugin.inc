<?php
/**
 * @file
 * Views settings, output and data validation
 */

/**
 * Extend views_plugin_style.
 */
class views_coscev_style_plugin extends views_plugin_style {

  /**
   * Default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['margins'] = array(
      'contains' => array(
        'margin_top' => array('default' => '10%'),
        'margin_right' => array('default' => "10%"),
        'margin_bottom' => array('default' => "20%"),
        'margin_left' => array('default' => "20%"),
      ),
    );

    $options['overlap'] = array('default' => 0);
    $options['center'] = array('default' => 1);

    $options['direction_type'] = array('default' => 'random');
    $options['manual'] = array(
      'contains' => array(
        'manual_directions' => array('default' => ''),
        'manual_next' => array('default' => "random"),
      ));

    $options['title'] = array(
      'contains' => array(
        'title_mode' => array('default' => 'fixed'),
        'title_space' => array('default' => 1),
        'title_effect' => array('default' => "fade"),
        'title_navigation' => array('default' => FALSE),
      ));

    $options['random_options'] = array('default' => array('same_direction' => 'same_direction'));
    $options['max_width'] = array('default' => '30%');

    return $options;
  }

  /**
   * Define fields.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['margins'] = array(
      '#type' => 'fieldset',
      '#title' => t('Margins'),
      '#description' => t('Here you can set the distance from the border where the item will appear.<br /> See !this_schema to know what parts are affected (green lines).<br /> Use % or number only for pixels', array(
        '!this_schema' => l(t('this schema'), drupal_get_path('module', 'views_coscev') . '/lines_position.gif'))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['margins']['margin_top'] = array(
      '#type' => 'textfield',
      '#title' => t('Margin Top'),
      '#size' => 7,
      '#required' => TRUE,
      '#description' => t('Distance between the top border and the top of the item.<br />Affect Top Horizontal Left and Top Horizontal Right lines.'),
      '#default_value' => $this->options['margins']['margin_top'],
    );

    $form['margins']['margin_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Margin Right'),
      '#size' => 7,
      '#required' => TRUE,
      '#description' => t('Distance between the right border and the right of the item.<br />Affect Top Vertical Right and Bottom Vertical Right lines.'),
      '#default_value' => $this->options['margins']['margin_right'],
    );

    $form['margins']['margin_bottom'] = array(
      '#type' => 'textfield',
      '#title' => t('Margin Bottom'),
      '#size' => 7,
      '#required' => TRUE,
      '#description' => t('Distance between the bottom border and the bottom of the item.<br />Affect Bottom Horizontal Left and Bottom Horizontal Right lines.'),
      '#default_value' => $this->options['margins']['margin_bottom'],
    );

    $form['margins']['margin_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Margin Left'),
      '#size' => 7,
      '#required' => TRUE,
      '#description' => t('Distance between the left border and the left of the item.<br />Affect Bottom Vertical Left and Bottom Vertical Right lines.'),
      '#default_value' => $this->options['margins']['margin_left'],
    );

    $form['title'] = array(
      '#type' => 'fieldset',
      '#title' => t('Title'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['title']['title_space'] = array(
      '#type' => 'textfield',
      '#title' => t('Space'),
      '#size' => 5,
      '#required' => TRUE,
      '#description' => t('a multiple of the window width. 2/3 of the space before, 1/3 after.<br />Use . for decimal: 1.2'),
      '#default_value' => $this->options['title']['title_space'],
    );

    $form['title']['title_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Mode'),
      '#description' => t('Inline: title will be integrated like other view items<br />Fixed: title will be displayed top left with an effect'),
      '#options' => array(
        'inline' => t('Inline'),
        'hidden' => t('Hidden'),
        'fixed' => t('Fixed'),
      ),
      '#default_value' => $this->options['title']['title_mode'],
    );

    $form['title']['title_effect'] = array(
      '#type' => 'select',
      '#title' => t('Effect'),
      '#options' => array(
        'fade' => t('Fade'),
        'popout' => t('Pop Out'),
        'rotate' => t('Rotate'),
      ),
      '#default_value' => $this->options['title']['title_effect'],
      '#dependency' => array('radio:style_options[title][title_mode]' => array('fixed')),
    );

    $form['title']['title_navigation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Navigation'),
      '#description' => t('Display the title in the top navigation bar'),
      '#default_value' => $this->options['title']['title_navigation'],
    );

    $form['overlap'] = array(
      '#type' => 'textfield',
      '#title' => t('Overlap'),
      '#required' => TRUE,
      '#size' => 7,
      '#description' => t('No overlap means the next item will appear just after the current item disappear.<br />Negative overlap will display several items at once.<br /> Use % or number only for pixels<br />sounds complicated but is easier when you try!!'),
      '#default_value' => $this->options['overlap'],
    );

    $form['max_width'] = array(
      '#type' => 'textfield',
      '#title' => t('View item width'),
      '#size' => 7,
      '#description' => t('It is recommended to set a width for text items.<br />Use % (ratio of the window width) or only numbers for pixels.<br />You can manually set the width in css as well.<br />'),
      '#default_value' => $this->options['max_width'],
    );

    $form['center'] = array(
      '#type' => 'checkbox',
      '#title' => t('Center first item'),
      '#default_value' => $this->options['center'],
      '#description' => t('This is only for the first item off the view, not for each attachment'),
    );

    // Directions.
    $form['direction_type'] = array(
      '#type' => 'radios',
      '#title' => t('Directions'),
      '#description' => t('Items come from 8 different locations and 4 directions, check !this_schema to know them', array(
        '!this_schema' => l(t('this schema'), drupal_get_path('module', 'views_coscev') . '/lines_position.gif'))),
      '#options' => array('random' => t('Random'), 'manual' => t('Manual')),
      '#default_value' => $this->options['direction_type'],
    );

    $form['manual'] = array(
      '#type' => 'fieldset',
      '#title' => t('Manual Options'),
      '#dependency' => array('radio:style_options[direction_type]' => array('manual')),
    );

    // Manuals line.
    $form['manual']['manual_directions'] = array(
      '#type' => 'textfield',
      '#title' => t('Directions'),
      '#description' => t('Refer to !this_schema to know what to use<br /> use / to separate numbers<br />example 6/3/4/2', array(
        '!this_schema' => l(t('this schema'), drupal_get_path('module', 'views_coscev') . '/lines_position.gif'))),
      '#default_value' => $this->options['manual']['manual_directions'],
      '#dependency' => array('radio:style_options[direction_type]' => array('manual')),
    );

    $form['manual']['manual_next'] = array(
      '#type' => 'radios',
      '#title' => t('Manual next'),
      '#description' => t('What to do for additional items (when last manual direction is reached)'),
      '#default_value' => $this->options['manual']['manual_next'],
      '#options' => array('loop' => t('Use manual again (loop)'), 'random' => t('Random')),
      '#dependency' => array('radio:style_options[direction_type]' => array('manual')),
    );

    $form['random_options'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Random options'),
      '#description' => t('Settings for the next item.<br />Same direction is exactly the same behavior for both.<br />Same way is the same movment (from left to right, top to bottom,...).<br />Clash occurs when an item disappear where the next one appear'),
      '#default_value' => $this->options['random_options'],
      '#options' => array(
        'same_direction' => t('Prevent same direction'),
        'same_way' => t('Prevent same way'),
        'clashes' => t('Prevent clashes'),
      ),
    );
  }

  /**
   * Validating the inputs.
   */
  public function options_validate(&$form, &$form_state) {

    parent::options_validate($form, $form_state);
    $options = &$form_state['values']['style_options'];

    if (!preg_match('#^[0-9]{1,4}%?$#', $options['margins']['margin_top'])) {
      form_error($form['margins']['margin_top'], t('Please use only numbers or add % for margin Top'));
    }

    if (!preg_match('#^[0-9]{1,4}%?$#', $options['margins']['margin_right'])) {
      form_error($form['margins']['margin_right'], t('Please use only numbers or add % for margin Right'));
    }

    if (!preg_match('#^[0-9]{1,4}%?$#', $options['margins']['margin_bottom'])) {
      form_error($form['margins']['margin_bottom'], t('Please use only numbers or add % for margin Bottom'));
    }

    if (!preg_match('#^[0-9]{1,4}%?$#', $options['margins']['margin_left'])) {
      form_error($form['margins']['margin_left'], t('Please use only numbers or add % for margin Left'));
    }

    if (!is_numeric($options['title']['title_space'])) {
      form_error($form['title']['title_space'], t('Please specify a numerical value for title space'));
    }

    if (!preg_match('#^-?[0-9]{1,4}%?$#', $options['overlap'])) {
      form_error($form['overlap'], t('Please use only numbers or add % for overlap'));
    }

    if ($options['direction_type'] == 'manual' && !preg_match('#^((1|2|3|4|5|6|7|8)\/?)+$#', $options['manual']['manual_directions'])) {
      form_error($form['manual']['manual_directions'], t('Please use only numbers from 1 to 8 separated by /'));
    }
  }
}
