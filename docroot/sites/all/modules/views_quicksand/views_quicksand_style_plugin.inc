<?php

/**
 * @file
 * Provide an accordion style plugin for Views. This file is autoloaded by
 * views.
 */

/**
 * Implementation of views_plugin_style().
 */
class views_quicksand_style_plugin extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['adjustHeight'] = array('default' => 'auto');
    $options['element'] = array('default' => 'views-row');
    $options['attribute'] = array('default' => 'data-id');
    $options['duration'] = array('default' => 750);
    $options['easing'] = array('default' => 'swing');
    $options['css'] = array('default' => '1');


    /*
     * @TODO:
     * Provide support for missing quicksand options:
     * enhancement
     * useScaling
     *
     * See http://razorjack.net/quicksand/docs-and-demos.html
     */
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $heightOptions = array(
      'auto' => t('Auto'),
      'dynamic' => t('Dynamic'),
      'false' => t('False')
    );

    $form['adjustHeight'] = array(
      '#type' => 'select',
      '#title' => t('Adjust height'),
      '#default_value' => $this->options['adjustHeight'],
      '#description' => t("Adjusts the height of container to fit all the "
          . "items, 'auto' for automatically adjusting before or after "
          . "the animation (determined automatically), 'dynamic' for "
          . "height adjustment animation, false for keeping the height "
          . "constant."),
      '#options' => $heightOptions,
    );
    $form['element'] = array(
      '#type' => 'textfield',
      '#title' => t('ID Element'),
      '#default_value' => $this->options['element'],
      '#description' => t(
        'Specify which element contains the attribute that identifies your '
        . 'distinct elements. This is any valid jquery selector whose search '
        . 'will be scoped inside your view.'),
    );
    $form['attribute'] = array(
      '#type' => 'textfield',
      '#title' => t('ID attribute'),
      '#default_value' => $this->options['attribute'],
      '#description' => t('Attribute used to match items in collections. '
          . 'This can be the id or class attribute or even a custom '
          . 'attribute like data-id, which you would have to integrate '
          . 'manually into your view results.'),
    );
    $form['duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Duration'),
      '#default_value' => $this->options['duration'],
      '#description' => t('How long the animation will take. In milliseconds..'),
    );
    $form['easing'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['easing'],
    );
    $form['easing_select'] = array(
      '#type' => 'select',
      '#title' => t('Easing animation'),
      '#description' => t("Select the animation to use for transitions.")
      . t("For examples how these easings will behave, have a look at !link", 
          array(
            '!link' => l('jQuery UI easing', 
                'http://jqueryui.com/demos/effect/#easing'))),
      '#attached' => array('js' => array(
          drupal_get_path('module', 'views_quicksand')
          . '/views_quicksand_settings.js',
      )),
    );
    $form['css'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use default css'),
      '#default_value' => $this->options['css'],
      '#description' => t("By default Views Quicksand defines a few styles to"
          . " make your view results look like a grid. You can disable "
          . "this behaviour here."),
    );
  }

  /**
   * Render the display in this style.
   */
  function render() {
    $output = parent::render();

    // Add the javascript for quicksand if it exists in the correct place.
    if ($path = libraries_get_path('jquery.quicksand')) {
      if (file_exists($path . '/jquery.quicksand.js')) {
        drupal_add_js($path . '/jquery.quicksand.js');
      }
      else {
        // Otherwise inform the user that the library couldn't be located.
        drupal_set_message(
            'The jQuery Quicksand library is missing under path: "'
            . $path . '/jquery.quicksand.js"', 'error');
      }
    }
    drupal_add_library('system', 'effects');

    // Scope our javascript to the footer to allow us to access Drupal.settings
    // before document ready has fired.
    $path = drupal_get_path('module', 'views_quicksand');
    drupal_add_js($path . '/views_quicksand.js', array('scope' => 'footer'));

    // add css
    if ($this->options['css'] == true) {
      drupal_add_css($path . '/views_quicksand.css');
    }

    // Preparing the js variables and adding the js to our display.
    $view_settings['adjustHeight'] = $this->options['adjustHeight'];
    $view_settings['element'] = $this->options['element'];
    $view_settings['attribute'] = $this->options['attribute'];
    $view_settings['duration'] = $this->options['duration'];
    $view_settings['easing'] = $this->options['easing'];

    $view_settings['display'] = $this->view->current_display;
    $view_settings['viewname'] = $this->view->name;
    if (isset($this->view->dom_id)) {
      $view_settings['dom_id'] = $this->view->dom_id;
    }

    $view_settings['row_plugin'] = get_class($this->row_plugin);


    $quicksand_id = 'views-quicksand-' . $this->view->name . '-'
        . $this->view->current_display;

    drupal_add_js(array(
      'views_quicksand' => array(
        $quicksand_id => $view_settings)
        ), 'setting');

    return $output;
  }

}
