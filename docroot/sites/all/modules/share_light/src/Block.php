<?php

namespace Drupal\share_light;

class Block {
  public static function defaults() {
    $default = array(
      'subject' => t('Share this page!'),
      'link' => array('path' => ''),
      'node' => NULL,
      'counter_toggle' => variable_get('share_light_default_counter_toggle', 1),
      'channels' => Loader::instance()->defaults(),
      'image' => array('fid' => 0),
    );
    return $default;
  }

  protected $options;
  public function __construct($options = array()) {
    if (empty($options['subject'])) {
      unset($options['subject']);
    }
    $this->options = drupal_array_merge_deep(static::defaults(), $options);

    // overrides based on the current page / share $_GET-parameter.
    if (empty($this->options['link']['path'])) {
      $this->options['link']['path'] = !empty($_GET['share']) ? $_GET['share'] : current_path();
    }
  }

  public function channelLinks() {
    $links = array();
    $options = $this->options['channels'];
    $loader = Loader::instance();
    $enabled_channels = array_keys($loader->channelOptions());
    foreach ($enabled_channels as $channel_name) {
      $channel_options = isset($options[$channel_name]) ? $options[$channel_name] : array();
      $channel = $loader->channel($channel_name, $this, $channel_options);
      if($channel->enabled() && ($channel_link = $channel->render())) {
        $links[$channel_name] = $channel_link;
      }
    }
    return $links;
  }

  public function getUrl($absolute = TRUE) {
    $link = $this->options['link'];
    return url($link['path'], $link + array('absolute' => $absolute));
  }

  public function getLink() {
    return $this->options['link'];
  }

  public function getNode() {
    return $this->options['node'];
  }

  public function getImageUrl() {
    if (!empty($this->options['image']->type)) {
      $img = $this->options['image'];
      if ($img->type == 'image' && ($image = image_load($img->uri))) {
        return file_create_url($image->source);
      }
    }
    return FALSE;
  }

  public function render() {
    drupal_alter('share_light_options', $this->options);

    // add tracking for GA if googleanalytics module is enabled
    // and share tracking is enabled (default: enabled)
    $tracking_enabled = module_exists('googleanalytics') && variable_get('share_light_tracking_enabled', 1);
    if ($tracking_enabled) {
      drupal_add_js(drupal_get_path('module', 'share_light') . '/tracking.js');
      drupal_add_js(array('share_light' => array(
        'share_url' => $this->getUrl(FALSE),
      )), 'setting');
    }

    $links = $this->channelLinks();

    foreach ($links as &$x) {
      $x['title'] = "<span>{$x['title']}</span>";
      $x['html'] = TRUE;
      $x['attributes']['target'] = '_blank';
    }

    $block['subject'] = $this->options['subject'];
    $block['content'] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#attributes' => array('class' => array('share-light')),
    );
    return $block;
  }

  public static function from_current_path($options = array()) {
    $node = NULL;
    if (!($node = menu_get_object())) {
      $count = 0;
      $nid = preg_replace('/^.*\/(\d+)$/', '$1', current_path(), -1, $count);
      if ($count) {
        $node = node_load($nid);
      }
    }
    if ($node) {
      $options['node'] = $node;
      if ($item = self::config_by_node($node, 'share_light')) {
        if ($item['toggle'] == '0') {
          return NULL;
        }
        $options += $item['options'];
      }
    }
    return new static($options);
  }

  private static function config_by_node($node) {
    $instances = field_info_instances('node', $node->type);
    foreach ($instances as $instance) {
      $field_info = field_info_field($instance['field_name']);
      if ($field_info['type'] == 'share_light') {
        $item = field_get_items('node', $node, $instance['field_name']);
        $data = array();
        if ($item) {
          $data += $item[0];
        }
        if (empty($data['options']['subject'])) {
          unset($data['options']['subject']);
        }
        if (count($instance['default_value'])) {
          $data += $instance['default_value'][0];
        }
        if (empty($data['options']['subject'])) {
          unset($data['options']['subject']);
        }
        return $data;
      }
    }
    return array();
  }
}
