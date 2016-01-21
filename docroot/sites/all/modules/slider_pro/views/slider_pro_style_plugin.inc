<?php

/**
 * @file
 * Contains the slider pro style plugin.
 */

/**
 * Style plugin to render views as slider pro instance.
 *
 * @ingroup views_style_plugins
 */
class slider_pro_style_plugin extends views_plugin_style {

  /**
   * Provide default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['slider_pro'] = array('default' => array());

    return $options;
  }

  /**
   * Adds Slider Pro configuration form options.
   */
  function options_form(&$form, &$form_state) {
    //parent::options_form($form, $form_state);
    // Check if fields have been added.  Table style plugin will set
    // error_markup if fields are not added.
    // @see views_plugin_style::options_form()
    if (isset($form['error_markup'])) {
      return;
    }

    $field_names = $this->display->handler->get_field_labels();

    $form['slider_pro'] = array(
      '#type' => 'fieldset',
      '#title' => t('Slider Pro settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['slider_pro']['width'] = array(
      '#title' => t('Width'),
      '#type' => 'textfield',
      '#default_value' => isset($this->options['slider_pro']['width']) ? $this->options['slider_pro']['width'] : '100%',
      '#description' => t('The width of the slider. Eg 200px or 50%'),
      '#required' => TRUE,
    );

    $form['slider_pro']['visible_size'] = array(
      '#title' => t('Visible size'),
      '#type' => 'textfield',
      '#default_value' => isset($this->options['slider_pro']['visible_size']) ? $this->options['slider_pro']['visible_size'] : 'auto',
      '#description' => t('Sets the size (for example 100%) of the visible area, allowing for more slides to become visible near the selected slide.'
          . 'Be sure that the width of your slider is less than 100%. <a href="!url" target="_blank">Example</a>',
          array('!url' => 'http://bqworks.com/slider-pro/#example2')),
      '#required' => TRUE,
    );

    $form['slider_pro']['force_size'] = array(
      '#title' => t('Force size'),
      '#type' => 'select',
      '#options' => array('none' => t('None'), 'fullWidth' => t('Full width'), 'fullWindow' => t('Ful window')),
      '#default_value' => isset($this->options['slider_pro']['force_size']) ? $this->options['slider_pro']['force_size'] : 'none',
      '#description' => t('Indicates if the size of the slider will be forced to full width or full window.'),
      '#required' => TRUE,
    );

    $form['slider_pro']['height'] = array(
      '#title' => t('Height'),
      '#type' => 'textfield',
      '#default_value' => isset($this->options['slider_pro']['height']) ? $this->options['slider_pro']['height'] : '200px',
      '#description' => t('The height of the slider. Eg 200px or 50%'),
      '#required' => TRUE,
    );

    $form['slider_pro']['orientation'] = array(
      '#title' => t('Orientation'),
      '#type' => 'select',
      '#options' => array('horizontal' => t('Horizontal'), 'vertical' => t('Vertical')),
      '#default_value' => isset($this->options['slider_pro']['orientation']) ? $this->options['slider_pro']['orientation'] : 'horizontal',
      '#description' => t('Indicates whether the slides will be arranged horizontally or vertically.'),
      '#required' => TRUE,
    );

    $form['slider_pro']['buttons'] = array(
      '#title' => t('Buttons'),
      '#type' => 'select',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($this->options['slider_pro']['buttons']) ? $this->options['slider_pro']['buttons'] : 0,
      '#description' => t('Indicates whether the buttons will be created.'),
      '#required' => TRUE,
    );

    $form['slider_pro']['transition'] = array(
      '#title' => t('Transition'),
      '#type' => 'select',
      '#options' => array(0 => t('Slide'), 1 => t('Fade')),
      '#default_value' => isset($this->options['slider_pro']['transition']) ? $this->options['slider_pro']['transition'] : 0,
      '#description' => t('Indicates which transition will be used.'),
      '#required' => TRUE,
    );

    $form['slider_pro']['arrows'] = array(
      '#title' => t('Arrows'),
      '#type' => 'select',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($this->options['slider_pro']['arrows']) ? $this->options['slider_pro']['arrows'] : 0,
      '#description' => t('Indicates whether the arrow buttons will be created..'),
      '#required' => TRUE,
    );

    $form['slider_pro']['fields_on_slide'] = array(
      '#title' => t('Fields on slide'),
      '#type' => 'checkboxes',
      '#options' => $field_names,
      '#default_value' => isset($this->options['slider_pro']['fields_on_slide']) ? $this->options['slider_pro']['fields_on_slide'] : array(),
      '#description' => t('Select which fields you want to display on the slider.'),
      '#required' => TRUE,
    );

    // Or AJAX won't work!
    $form_state['no_cache'] = FALSE;
    $use_thumbnails = isset($form_state['values']['style_options']['slider_pro']['thumbnails']['thumbnails_position']) ? $form_state['values']['style_options']['slider_pro']['thumbnails']['thumbnails_position'] : $this->options['slider_pro']['thumbnails']['thumbnails_position'];

    $form['slider_pro']['thumbnails'] = array(
      '#prefix' => '<div id="slider-pro-thumbs">',
      '#suffix' => '</div>',
      '#type' => 'fieldset',
      '#title' => t('Thumbnails'),
      '#collapsible' => TRUE,
      '#collapsed' => !$use_thumbnails,
    );

    $form['slider_pro']['thumbnails']['thumbnails_position'] = array(
      '#title' => t('Thumbnail position'),
      '#type' => 'select',
      '#options' => array(0 => t('No thumbs'), 'left' => t('Left'), 'right' => t('Right'), 'top' => t('Top'), 'bottom' => t('Bottom')),
      '#default_value' => isset($this->options['slider_pro']['thumbnails']['thumbnails_position']) ? $this->options['slider_pro']['thumbnails']['thumbnails_position'] : 0,
      '#required' => TRUE,
      '#description' => t('Sets the position of the thumbnail scroller.'),
      '#ajax' => array(
        'wrapper' => 'slider-pro-thumbs',
        'callback' => '_slider_pro_plugin_style_refresh_thumbs'
      ),
    );

    if ($use_thumbnails) {
      $form['slider_pro']['thumbnails']['fields_on_thumbs'] = array(
        '#title' => t('Fields on thumb'),
        '#type' => 'checkboxes',
        '#options' => $field_names,
        '#default_value' => isset($this->options['slider_pro']['thumbnails']['fields_on_thumbs']) ? $this->options['slider_pro']['thumbnails']['fields_on_thumbs'] : array(),
        '#description' => t('Select which fields you want to display on the thumbs.'),
        '#required' => TRUE,
      );

      $form['slider_pro']['thumbnails']['width'] = array(
        '#title' => t('Width'),
        '#type' => 'textfield',
        '#default_value' => isset($this->options['slider_pro']['thumbnails']['width']) ? $this->options['slider_pro']['thumbnails']['width'] : '100px',
        '#description' => t('The width of each thumbnail. Eg 200px.'),
        '#required' => TRUE,
      );

      $form['slider_pro']['thumbnails']['height'] = array(
        '#title' => t('Height'),
        '#type' => 'textfield',
        '#default_value' => isset($this->options['slider_pro']['thumbnails']['height']) ? $this->options['slider_pro']['thumbnails']['height'] : '80px',
        '#description' => t('The height of each thumbnail. Eg 200px.'),
        '#required' => TRUE,
      );
    }

    $form['slider_pro']['layers'] = array(
      '#type' => 'fieldset',
      '#title' => t('Layers'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#prefix' => '<div id="slider-pro-layers">',
      '#suffix' => '</div>',
    );

    $form['slider_pro']['layers']['table'] = array(
      '#theme' => 'slider_pro_layers_table',
      '#tree' => TRUE,
    );

    $layers = !empty($this->options['slider_pro']['layers']['table']) ? $this->options['slider_pro']['layers']['table'] : FALSE;
    if (empty($form_state['layers']) && !$layers) {
      // This means the form is loaded for the first time.
      $form_state['layers'] = array();
    }
    else if (!empty($layers) && empty($form_state['layers'])) {
      // This means the form is loaded, but layers have already been saved.
      $form_state['layers'] = array_keys($layers);
    }

    $count = 0;
    foreach ($form_state['layers'] as $layer) {
      $form['slider_pro']['layers']['table']['layer-' . $layer]['fields'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Fields on layer'),
        '#options' => $field_names,
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['fields'] : NULL,
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['background'] = array(
        '#type' => 'select',
        '#title' => t('Background'),
        '#options' => array(
          '' => t('None'),
          'sp-white' => t('White transparant'),
          'sp-black' => t('Black transparant'),
        ),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['background'] : '',
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['data_position'] = array(
        '#type' => 'select',
        '#title' => t('Position'),
        '#options' => array(
          'topLeft' => t('Top left'),
          'topCenter' => t('Top center'),
          'topRight' => t('Top right'),
          'bottomLeft' => t('Bottom left'),
          'bottomCenter' => t('Bottom center'),
          'bottomRight' => t('Bottom right'),
          'centerLeft' => t('Center left'),
          'centerRight' => t('Center right'),
          'centerCenter' => t('Center center'),
        ),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['data_position'] : 'topLeft',
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['data_show_transition'] = array(
        '#type' => 'select',
        '#title' => t('Show transition'),
        '#options' => array(
          'left' => t('Left'),
          'right' => t('Right'),
          'up' => t('Up'),
          'down' => t('Down'),
        ),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['data_show_transition'] : 'right',
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['data_hide_transition'] = array(
        '#type' => 'select',
        '#title' => t('Hide transition'),
        '#options' => array(
          'left' => t('Left'),
          'right' => t('Right'),
          'up' => t('Up'),
          'down' => t('Bottom left'),
        ),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['data_hide_transition'] : 'left',
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['data_show_delay'] = array(
        '#type' => 'textfield',
        '#title' => t('Show delay'),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['data_show_delay'] : 0,
        '#size' => 5,
        '#required' => FALSE,
      );

      $form['slider_pro']['layers']['table']['layer-' . $layer]['data_stay_duration'] = array(
        '#type' => 'textfield',
        '#title' => t('Stay duration'),
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['data_stay_duration'] : 0,
        '#size' => 5,
        '#required' => FALSE,
      );

      if ($count > 0) {
        $form['slider_pro']['layers']['table']['layer-' . $layer]['remove'] = array(
          '#type' => 'submit',
          '#value' => t('Remove layer'),
          '#submit' => array('_slider_pro_plugin_style_remove_layer'),
          '#limit_validation_errors' => array(),
          '#ajax' => array(
            'wrapper' => 'slider-pro-layers',
            'callback' => '_slider_pro_plugin_style_refresh_layers'
          ),
          '#name' => 'remove-' . $count,
        );
      }

      $form['slider_pro']['layers']['table']['layer-' . $layer]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#title_display' => 'invisible',
        '#default_value' => !empty($layers[$layer]) ? $layers[$layer]['weight'] : 10,
        '#attributes' => array(
          'class' => array('order-weight'),
        ),
      );
      $count++;
    }

    $form['slider_pro']['layers']['add_layer'] = array(
      '#type' => 'submit',
      '#value' => t('Add layer'),
      '#submit' => array('_slider_pro_plugin_style_add_layer'),
      '#ajax' => array(
        'wrapper' => 'slider-pro-layers',
        'callback' => '_slider_pro_plugin_style_refresh_layers'
      ),
    );
  }

  /**
   * Add form validation for options.
   */
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    // Form_error($form['slider_pro']['height'], 'test');
  }

  function validate() {
    parent::validate();
  }

}

/**
 * Ajax callback in response to thumbnails.
 */
function _slider_pro_plugin_style_refresh_thumbs($form, &$form_state) {
  return $form['options']['style_options']['slider_pro']['thumbnails'];
}

/**
 * Ajax callback in response to layers.
 */
function _slider_pro_plugin_style_refresh_layers($form, &$form_state) {
  return $form['options']['style_options']['slider_pro']['layers'];
}

/**
 * Adds a layer.
 * @param type $form
 * @param array $form_state
 */
function _slider_pro_plugin_style_add_layer($form, &$form_state) {
  array_push($form_state['layers'], count($form_state['layers']));
  $form_state['rebuild'] = TRUE;
}

/**
 * Removes a layer.
 * @param type $form
 * @param array $form_state
 */
function _slider_pro_plugin_style_remove_layer($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $index = str_replace('remove-', '', $clicked_button);
  if ($index > 0) {
    unset($form_state['layers'][$index]);
  }
  $form_state['rebuild'] = TRUE;
}
