<?php

/**
 * @file
 * Zoundation orbit slideshow style plugin.
 *
 */

/**
 * Style plugin to render rows in an orbit slideshow.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_zoundation_orbit_slideshow extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['timer_speed'] = array('default' => 10000);
    $options['animation_speed'] = array('default' => 500);
    $options['bullets'] = array('default' => 'true');
    $options['stack_on_small'] = array('default' => 'true');
    $options['navigation_arrows'] = array('default' => 'true');
    $options['slide_number'] = array('default' => 'true');
    $options['container_class'] = array('default' => 'orbit-container');
    $options['stack_on_small_class'] = array('default' => 'orbit-stack-on-small');
    $options['next_class'] = array('default' => 'orbit-next');
    $options['prev_class'] = array('default' => 'orbit-prev');
    $options['timer_container_class'] = array('default' => 'orbit-timer');
    $options['timer_paused_class'] = array('default' => 'paused');
    $options['timer_progress_class'] = array('default' => 'orbit-progress');
    $options['slides_container_class'] = array('default' => 'orbit-slides-container');
    $options['bullets_container_class'] = array('default' => 'orbit-bullets');
    $options['bullets_active_class'] = array('default' => 'active');
    $options['slide_number_class'] = array('default' => 'orbit-slide-number');
    $options['caption_class'] = array('default' => 'orbit-caption');
    $options['active_slide_class'] = array('default' => 'active');
    $options['orbit_transition_class'] = array('default' => 'orbit-transitioning');
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['timer_speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Timer speed'),
      '#default_value' => $this->options['timer_speed'],
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $form['animation_speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Aniimation speed'),
      '#default_value' => $this->options['animation_speed'],
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $options = array(
      'false' => t('No'),
      'true' => t('Yes'),
    );

    $form['bullets'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Bullets'),
      '#default_value' => $this->options['bullets'],
    );

    $form['stack_on_small'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Stack on small'),
      '#default_value' => $this->options['stack_on_small'],
    );

    $form['navigation_arrows'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Navigation arrows'),
      '#default_value' => $this->options['navigation_arrows'],
    );

    $form['slide_number'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Slide number'),
      '#default_value' => $this->options['slide_number'],
    );

    $form['container_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Container class'),
      '#default_value' => $this->options['container_class'],
    );

    $form['stack_on_small_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Stack on small class'),
      '#default_value' => $this->options['stack_on_small_class'],
    );

    $form['next_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Next class'),
      '#default_value' => $this->options['next_class'],
    );

    $form['prev_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Previous class'),
      '#default_value' => $this->options['prev_class'],
    );

    $form['timer_container_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Timer container class'),
      '#default_value' => $this->options['timer_container_class'],
    );

    $form['timer_paused_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Timer paused class'),
      '#default_value' => $this->options['timer_paused_class'],
    );

    $form['timer_progress_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Timer progress class'),
      '#default_value' => $this->options['timer_progress_class'],
    );

    $form['slides_container_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Slides container class'),
      '#default_value' => $this->options['slides_container_class'],
    );

    $form['bullets_container_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Bullets container class'),
      '#default_value' => $this->options['bullets_container_class'],
    );

    $form['bullets_active_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Bullets active class'),
      '#default_value' => $this->options['bullets_active_class'],
    );

    $form['slide_number_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Slide number class'),
      '#default_value' => $this->options['slide_number_class'],
    );

    $form['caption_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption class'),
      '#default_value' => $this->options['caption_class'],
    );

    $form['active_slide_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Active slide class'),
      '#default_value' => $this->options['active_slide_class'],
    );

    $form['orbit_transition_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Orbit transition class'),
      '#default_value' => $this->options['orbit_transition_class'],
    );

  }
}
