<?php

namespace Drupal\share_light;

class Pinterest extends ChannelBase {
  public static function title() { return t('Pinterest'); }
  public static function defaults() { return array('text' => '') + parent::defaults(); }
  public static function optionsWidget(array &$element, array $options) {
    $title = static::title();
    $element['text'] =  array(
      '#title' => t('Description text for ' . $title . '.'),
      '#description' => t('Description text for ' . $title . '.'),
      '#maxlength' => 500, // the pinterest max-length for descriptions
      '#type' => 'textarea',
      '#cols' => 60,
      '#rows' => 2,
      '#attributes' => array(),
      '#default_value' => $options['text'],
    );
  }
  public function render() {
    $url = $this->block->getUrl();
    if ($media_url = $this->block->getImageUrl()) {
    // get the url from the media object
      return array(
        'title' => 'Pinterest',
        'href' => 'http://www.pinterest.com/pin/create/button/',
        'query' => array('url' => $url, 'media' => $media_url, 'description' => $this->options['text']),
        'attributes' => array(
          'title' => t('Share this via Pinterest!'),
          'data-share' => 'pinterest',
        ),
      );
    }
  }
}
