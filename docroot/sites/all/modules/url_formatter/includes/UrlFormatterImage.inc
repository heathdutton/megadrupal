<?php

/**
 * @file
 * Main class for Url formatter image.
 */

class UrlFormatterImage extends UrlFormatter {

  /**
   * The human name of formatter.
   *
   * @see UrlFormatter::name.
   */
  public $name = 'Image';


  /**
   * Implements UrlFormatter::form().
   */
  public function form($field, $instance, $view_mode, $form, &$form_state) {
    $element = parent::form($field, $instance, $view_mode, $form, $form_state);

    $element['enable'] = array(
      '#title' => t('Image'),
      '#type' => 'checkbox',
      '#default_value' => $this->settings['enable'],
    );

    $element['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of image'),
      '#default_value' => $this->settings['width'],
      '#states' => $this->visibility($field['field_name'])
    );

    $element['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height of image'),
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
        $img = array(
          'path' => $url,
          'width' => $this->settings['width'],
          'height' => $this->settings['height'],
          'attributes' => array('class' => __CLASS__),
        );
        $attributes = array('style' => array('display: block'));
        $img = l(theme('image', $img), $url, array('html' => TRUE, 'attributes' => $attributes));
        $output = str_replace($url, $img, $output);
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
    return '!https?://([a-z0-9\-\.\/\_]+\.(?:jp?g|png|gif)\??[^\s]*?)!Ui';
  }

}
