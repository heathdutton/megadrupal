<?php

/**
 * @file
 * Defines the View Style Plugins for Timeline module.
 */

/**
 * Extending the view_plugin_style class to provide a timeline view style.
 */
class timelinr_views_plugin_style extends views_plugin_style {
  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    unset($options['grouping']);

    $options['orientation'] = array('default' => 'horizontal');
    $options['issues_speed'] = array('default' => 'normal');
    $options['issues_transparency'] = array('default' => '0.2');
    $options['issues_transparency_speed'] = array('default' => '500');
    $options['prev_button'] = array('default' => '#prev');
    $options['next_button'] = array('default' => '#next');
    $options['arrow_keys'] = array('default' => 'false');
    $options['start_at'] = array('default' => '1');
    $options['auto_play'] = array('default' => 'false');
    $options['auto_play_direction'] = array('default' => 'forward');
    $options['auto_play_pause'] = array('default' => '2000');

    return $options;
  }

  /**
   * Show a form to edit the style options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['grouping']);

    // Appearance settings.
    $timelinr_orientation_select = array(
      'horizontal' => t('Horizontal Style'),
      'vertical' => t('Vertical Style'),
    );

    $form['orientation'] = array(
      '#type' => 'select',
      '#title' => t('Orientation'),
      '#default_value' => $this->options['orientation'],
      '#options' => $timelinr_orientation_select,
      '#required' => TRUE,
      '#description' => t('Select a orientation for slider.'),
    );

    $form['reset_style'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset styles'),
      '#default_value' => $this->options['reset_style'],
      '#description' => t('Adds a reset-style to timelinr plugin for overwriting plugins styles.'),
    );
    
    $form['adv'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Configure advanced settings'),
    );

    $timelinr_issues_speed_select = array(
      'slow' => 'slow',
      'normal' => 'normal',
      'fast' => 'fast',
    );

    $form['adv']['issues_speed'] = array(
      '#type' => 'select',
      '#title' => t('issues_speed'),
      '#default_value' => $this->options['issues_speed'],
      '#options' => $timelinr_issues_speed_select,
      '#description' => t('Select a speed for issues.'),
    );

    $timelinr_issues_transparency_select = array(
      '0.1' => '0.1',
      '0.2' => '0.2',
      '0.3' => '0.3',
      '0.4' => '0.4',
      '0.5' => '0.5',
      '0.6' => '0.6',
      '0.6' => '0.6',
      '0.7' => '0.7',
      '0.8' => '0.8',
      '0.9' => '0.8',
      '1' => '1',
    );

    $form['adv']['issues_transparency'] = array(
      '#type' => 'select',
      '#title' => t('issues_transparency'),
      '#default_value' => $this->options['issues_transparency'],
      '#options' => $timelinr_issues_transparency_select,
      '#description' => t('Select a transparency for issues.'),
    );

    $timelinr_issues_transparency_speed = array(
      '100' => '100',
      '200' => '200',
      '300' => '300',
      '400' => '400',
      '500' => '500',
      '600' => '600',
      '700' => '700',
      '800' => '800',
      '900' => '900',
      '1000' => '1000',
    );

    $form['adv']['issues_transparency_speed'] = array(
      '#type' => 'select',
      '#title' => t('issues_transparency_speed'),
      '#default_value' => $this->options['issues_transparency_speed'],
      '#options' => $timelinr_issues_transparency_speed,
      '#description' => t('Select a issues_transparency_speed for issues.'),
    );

    $form['adv']['prev_button'] = array(
      '#type' => 'textfield',
      '#title' => t('prevButton'),
      '#default_value' => $this->options['prev_button'],
      '#description' => t('Select a prevButton-Id for issues.'),
    );

    $form['adv']['next_button'] = array(
      '#type' => 'textfield',
      '#title' => t('nextButton'),
      '#default_value' => $this->options['next_button'],
      '#description' => t('Select a nextButton-Id for issues.'),
    );

    $timelinr_arrow_keys = array(
      'true' => 'true',
      'false' => 'false',
    );

    $form['adv']['arrow_keys'] = array(
      '#type' => 'select',
      '#title' => t('issues_arrow_keys'),
      '#default_value' => $this->options['arrow_keys'],
      '#options' => $timelinr_arrow_keys,
      '#description' => t('Select a issues_arrow_keys for issues.'),
    );

    $form['adv']['start_at'] = array(
      '#type' => 'textfield',
      '#title' => t('start_at'),
      '#default_value' => $this->options['start_at'],
      '#description' => t('Select a start_at for issues.'),
    );

    $timelinr_autoplay = array(
      'true' => 'true',
      'false' => 'false',
    );

    $form['adv']['auto_play'] = array(
      '#type' => 'select',
      '#title' => t('auto_play'),
      '#default_value' => $this->options['auto_play'],
      '#options' => $timelinr_autoplay,
      '#description' => t('Select a issues_arrow_keys for issues.'),
    );

    $timelinr_auto_play_direction = array(
      'forward' => 'forward',
      'backward' => 'backward',
    );

    $form['adv']['auto_play_direction'] = array(
      '#type' => 'select',
      '#title' => t('auto_play_direction'),
      '#default_value' => $this->options['auto_play_direction'],
      '#options' => $timelinr_auto_play_direction,
      '#description' => t('Select a auto_play_direction for issues.'),
    );

    $timelinr_auto_play_pause_select = array(
      '500' => '500',
      '1000' => '1000',
      '1500' => '1500',
      '2000' => '2000',
      '2500' => '2500',
    );

    $form['adv']['auto_play_pause'] = array(
      '#type' => 'select',
      '#title' => t('auto_play_pause'),
      '#default_value' => $this->options['auto_play_pause'],
      '#options' => $timelinr_auto_play_pause_select,
      '#description' => t('Select a auto_play_pause for issues.'),
    );
  }
}
