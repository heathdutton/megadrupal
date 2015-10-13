<?php

namespace Drupal\share_light;

class Twitter extends ChannelBase {
  public static function title() { return t('Twitter'); }
  public static function defaults() { return array('text' => '') + parent::defaults(); }
  public static function optionsWidget(array &$element, array $options) {
    $title = static::title();
    $element['text'] =  array(
      '#title' => t('Tweet text for ' . $title . '.'),
      '#description' => t('Tweet text for ' . $title . '.'),
      '#maxlength' => 116, // = 140 - 1 - 23 (tweet max-length - space - url in https)
      '#type' => 'textarea',
      '#cols' => 60,
      '#rows' => 2,
      '#attributes' => array(),
      '#default_value' => $options['text'],
    );
  }
  public function render() {
    $url = $this->block->getUrl();
    return array(
      'title' => 'Twitter',
      'href' => 'http://twitter.com/share',
      'query' => array('text' => $this->options['text'], 'url' => $url),
      'attributes' => array(
        'title' => t('Share this via Twitter!'),
        'data-share' => 'twitter',
      ),
    );
  }
}
