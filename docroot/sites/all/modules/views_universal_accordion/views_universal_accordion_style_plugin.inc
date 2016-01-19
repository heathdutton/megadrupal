<?php
/**
 * @file
 * Provide an accordion style plugin for Views.
 * This file is autoloaded by views.
 */

/**
 * Implementation of views_plugin_style().
 */
class ViewsUniversalAccordionStylePlugin extends views_plugin_style {

  protected $libpath = FALSE;
  protected $effects = FALSE;
  protected $effect_options = NULL;

  /**
   * Init function.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
    $this->effect_options = array(
      'none' => t('None'),
    );
    $this->libpath = libraries_get_path('uAccordion');

    if ($this->libpath) {
      $this->effects = @file_exists($this->libpath . '/js/jquery.easing.min.js');
      if ($this->effects) {
        $this->effect_options += array(
          'none' => t('None'),
          'easeInQuart' => t('easeInQuart'),
          'easeOutQuart' => t('easeOutQuart'),
          'easeInOutQuart' => t('easeInOutQuart'),
          'easeInExpo' => t('easeInExpo'),
          'easeOutExpo' => t('easeOutExpo'),
          'easeInOutExpo' => t('easeInOutExpo'),
          'easeInBack' => t('easeInBack'),
          'easeOutBack' => t('easeOutBack'),
          'easeInOutBack' => t('easeInOutBack'),
          'easeInQuad' => t('easeInQuad'),
          'easeOutQuad' => t('easeOutQuad'),
          'easeInOutQuad' => t('easeInOutQuad'),
          'easeInQuint' => t('easeInQuint'),
          'easeOutQuint' => t('easeOutQuint'),
          'easeInOutQuint' => t('easeInOutQuint'),
          'easeInCirc' => t('easeInCirc'),
          'easeOutCirc' => t('easeOutCirc'),
          'easeInOutCirc' => t('easeInOutCirc'),
          'easeInBounce' => t('easeInBounce'),
          'easeOutBounce' => t('easeOutBounce'),
          'easeInOutBounce' => t('easeInOutBounce'),
          'easeInCubic' => t('easeInCubic'),
          'easeOutCubic' => t('easeOutCubic'),
          'easeInOutCubic' => t('easeInOutCubic'),
          'easeInSine' => t('easeInSine'),
          'easeOutSine' => t('easeOutSine'),
          'easeInOutSine' => t('easeInOutSine'),
          'easeInElastic' => t('easeInElastic'),
          'easeOutElastic' => t('easeOutElastic'),
          'easeInOutElastic' => t('easeInOutElastic'),
        );
      }
    }
  }

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['horizontal'] = array('default' => 1);
    $options['totalwidth'] = array('default' => '100%');
    $options['totalheight'] = array('default' => 500);
    $options['tabwidth'] = array('default' => '10%');
    $options['speed'] = array('default' => 1200);
    $options['timeout'] = array('default' => 6000);
    $options['trigger'] = array('default' => 'click');
    $options['autoplay'] = array('default' => 0);
    $options['errors'] = array('default' => 0);
    $options['invert'] = array('default' => 0);
    $options['pause'] = array('default' => 0);
    $options['easing'] = array('default' => 'none');
    return $options;
  }

  /**
   * Settings form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['horizontal'] = array(
      '#type' => 'select',
      '#title' => t('Accordion type'),
      '#default_value' => $this->options['horizontal'],
      '#description' => t('Accordion type'),
      '#options' => array(
        1 => t('Horizontal'),
        0 => t('Vertical'),
      ),
    );

    $form['totalwidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#required' => TRUE,
      '#default_value' => $this->options['totalwidth'],
      '#description' => t('Total width of the accordion. Percents or pixels can be specified.'),
    );

    $form['totalheight'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#required' => TRUE,
      '#default_value' => $this->options['totalheight'],
      '#description' => t('Total height of the accordion'),
    );

    $form['tabwidth'] = array(
      '#type' => 'textfield',
      '#title' => t('Tab offset'),
      '#required' => TRUE,
      '#default_value' => $this->options['tabwidth'],
      '#description' => t('Offset of the accordion "tab", a visible part used to trigger the accordion'),
    );

    $form['speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Sliding delay'),
      '#default_value' => $this->options['speed'],
      '#description' => t('Sliding speed, it is actually delay in ms.'),
    );

    $form['trigger'] = array(
      '#type' => 'select',
      '#title' => t('Event type'),
      '#default_value' => $this->options['trigger'],
      '#description' => t('Event type that will bind to the "tab" (click, mouseover, etc.)'),
      '#options' => array(
        'click' => t('Click'),
        'mouseover' => t('Mouse over'),
      ),
    );

    $form['easing'] = array(
      '#type' => 'select',
      '#title' => t('Animation effect to use'),
      '#default_value' => $this->options['easing'],
      '#description' => t('Animation effect to use'),
      '#options' => $this->effect_options,
    );

    $form['invert'] = array(
      '#type' => 'checkbox',
      '#title' => t('Invert the accordion'),
      '#default_value' => $this->options['invert'],
      '#description' => t('Whether or not the last slide stays in the same position, rather than the first slide.'),
    );

    $form['errors'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display errors'),
      '#default_value' => $this->options['errors'],
      '#description' => t('Whether or not display errors in the console.'),
    );

    $form['autoplay'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto play'),
      '#default_value' => $this->options['autoplay'],
      '#description' => t('Whether or not the slideshow should play automatically.'),
    );

    $form['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#default_value' => $this->options['timeout'],
      '#description' => t('Timeout between slides when autoplaying'),
      '#states' => array(
        'visible' => array(
          ':input[id="edit-style-options-autoplay"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['pause'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pause on hover'),
      '#default_value' => $this->options['pause'],
      '#description' => t('Pause on hover when autoplaying'),
      '#states' => array(
        'visible' => array(
          ':input[id="edit-style-options-autoplay"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  /**
   * Render the display in this style.
   */
  function render() {
    $output = '';
    // Prevent errors if users select content instead of fields as row style.
    // This forces them to use field as row style.
    if (parent::uses_fields()) {
      $output = parent::render();
    }
    else {
      drupal_set_message('Views accordion requires Fields as row style', 'error');
    }

    if (count($this->rendered_fields) < 2) {
      return $output;
    }

    // Add the appropiate effect library if necessary.
    $effect = $this->effects ? $this->options['easing'] : 'none';
    if ($effect !== 'none') {
      // For now we use additional library, which provides the easing effects.
      drupal_add_js($this->libpath . '/js/jquery.easing.min.js');
    }

    drupal_add_js($this->libpath . '/js/jquery.uaccordion.min.js');
    drupal_add_js(drupal_get_path('module', 'views_universal_accordion') . '/views-universal-accordion.js');

    $view_settings['horizontal'] = $this->options['horizontal'];
    $view_settings['totalwidth'] = $this->options['totalwidth'];
    $view_settings['totalheight'] = $this->options['totalheight'];
    $view_settings['tabwidth'] = $this->options['tabwidth'];
    $view_settings['speed'] = $this->options['speed'];

    $view_settings['trigger'] = $this->options['trigger'];
    $view_settings['easing'] = $effect;
    $view_settings['autoplay'] = $this->options['autoplay'];
    $view_settings['timeout'] = $this->options['timeout'];
    $view_settings['errors'] = $this->options['errors'];
    $view_settings['invert'] = $this->options['invert'];
    $view_settings['pause'] = $this->options['pause'];

    $accordion_id = '.view-id-' . $this->view->name . '.view-display-id-' . $this->view->current_display;

    drupal_add_js(array('views_universal_accordion' => array($accordion_id => $view_settings)), 'setting');

    return $output;
  }
}
