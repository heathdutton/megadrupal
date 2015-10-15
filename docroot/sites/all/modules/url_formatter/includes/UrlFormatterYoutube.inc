<?php

/**
 * @file
 * Main class for Url formatter Youtube.
 */

class UrlFormatterYoutube extends UrlFormatter {

  /**
   * The human name of formatter.
   *
   * @see UrlFormatter::name.
   */
  public $name = 'Youtube';


  /**
   * Implements UrlFormatter::form().
   */
  public function form($field, $instance, $view_mode, $form, &$form_state) {
    $element = parent::form($field, $instance, $view_mode, $form, $form_state);

    $element['enable'] = array(
      '#title' => t('Youtube'),
      '#type' => 'checkbox',
      '#default_value' => $this->settings['enable'],
    );

    $element['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of Video'),
      '#default_value' => $this->settings['width'],
      '#states' => $this->visibility($field['field_name'])
    );

    $element['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height of Video'),
      '#default_value' => $this->settings['height'],
      '#states' => $this->visibility($field['field_name']),
    );

    return $element;
  }


  /**
   * Implements UrlFormatter::view().
   */
  public function view($entity_type, $entity, $field, $instance, $langcode, $items, $display, $delta, $output) {
    $output = parent::view($entity_type, $entity, $field, $instance, $langcode, $items, $display, $delta, $output);
    preg_match_all($this->regExp(), $output, $matches);
    if (!empty($matches[0])) {
      foreach (array_unique($matches[0]) as $url) {
        $options = drupal_parse_url($url);
        $video = array(
          'element' => array(
            '#tag' => 'iframe',
            '#attributes' => array(
              'width' => $this->settings['width'],
              'height' => $this->settings['height'],
              'src' => '//www.youtube.com/embed/' . $options['query']['v'],
              'frameborder' => '0',
              'allowfullscreen' => 1,
              'style' => array('display: block'),
            ),
            '#value' => '',
          ),
        );
        $video = theme('html_tag', $video);
        $output = str_replace($url, $video, $output);
      }
    }
    return $output;
  }


  /**
   * Implements UrlFormatter::defaultValue().
   */
  static public function defaultValue() {
    return array(
      'enable' => 1,
      'width' => '250px',
      'height' => '100%',
    );
  }


  /**
   * Implements UrlFormatter::regExp().
   */
  public function regExp() {
    return '!http://\w{0,3}.?youtube+\.\w{2,3}/watch\?v=[\w-]{11}!Ui';
  }

}
