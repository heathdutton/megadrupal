<?php
/**
 * @file
 * Define the "grid" views plugin.
 */

/**
 * Style plugin.
 */
class views_plugin_style_list_effect extends views_plugin_style_list {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['mode'] = array('default' => 'pageTop');
    $options['duration'] = array('default' => '600');
    $options['delay'] = array('default' => '300');
    $options['random'] = array('default' => 0);
    $options['waypoints'] = array('default' => 0);
    $options['instance_id'] = array('default' => '');
    return $options;
  }

  /**
   * Render the given style.
   */
  public function options_form(&$form, &$form_state) {
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => t('<div class="error messages">You need at least one field before you can configure your list effect settings</div>'),
      );
      return;
    }

    $modes = array(
      'fadeIn' => 'Fade In',
      'zoomIn' => 'Zoom In',
      'zoomOut' => 'Zoom Out',
      'slideLeft' => 'Slide Left',
      'slideRight' => 'Slide Right',
      'slideTop' => 'Slide Top',
      'slideBottom' => 'Slide Bottom',
      'flipInX' => 'Flip in horizontal',
      'flipInY' => 'Flip in vertical',
      'bounceIn' => 'Bounce In',
      'bounceInUp' => 'Bounce In Up',
      'bounceInRight' => 'Bounce In Right',
      'bounceInDown' => 'Bounce In Down',
      'bounceInLeft' => 'Bounce In Left',
      'pageTop' => 'Page Top',
      'pageTopBack' => 'Page Top (back)',
      'pageRight' => 'Page Right',
      'pageRightBack' => 'Page Right (back)',
      'pageBottom' => 'Page Bottom',
      'pageBottomBack' => 'Page Bottom (back)',
      'pageLeft' => 'Page Left',
      'pageLeftBack' => 'Page Left (back)',
      'starwars' => 'Star Wars',
    );
    
    $form['mode'] = array(
      '#type' => 'select',
      '#options' => $modes,
      '#title' => t('Choose an effect mode'),
      '#default_value' => $this->options['mode'],
      '#required' => TRUE,
    );

    $form['duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter a transition time (ms)'),
      '#default_value' => $this->options['duration'],
      '#required' => TRUE,
    );

    $form['delay'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter a delay between transitions (ms)'),
      '#default_value' => $this->options['delay'],
      '#required' => TRUE,
    );

    $form['random'] = array(
      '#type' => 'checkbox',
      '#title' => t('Random effect'),
      '#default_value' => $this->options['random'],
    );

    if (!module_exists('waypoints')) {
      $waypoints_description = t('Download and enable <a href="http://www.drupal.org/project/waypoints">waypoints</a> module to active this option');
      $waypoints_disabled = TRUE;
      $waypoints_value = 0;
    }
    else {
      $waypoints_description = '';
      $waypoints_disabled = FALSE;
      $waypoints_value = $this->options['waypoints'];
    }

    $form['waypoints'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use waypoints'),
      '#description' => $waypoints_description,
      '#default_value' => $waypoints_value,
      '#disabled' => $waypoints_disabled,
    );

  }

}
