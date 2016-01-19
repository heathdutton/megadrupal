<?php
namespace Drupal\go1_base\Icon;

class FontAwesome implements IconSourceInterface {
  public function getName() {
    return 'FontAwesome';
  }

  public function __construct() {
    \go1_fn::drupal_add_css(go1_library('fontawesome', NULL, FALSE) . 'css/font-awesome.css');
  }

  /**
   * Get Icon instance with information to generate icon tag.
   *
   * @param type $name
   *   The name of icon in fontawesome.
   *   Browse available icons at http://fortawesome.github.io/Font-Awesome/icons/
   * @return \Drupal\go1_base\Icon\Icon
   *   Contain enough information to generate icon tag.
   */
  public function get($id) {
    if (strpos($id, '/')) {
      list(, $name) = explode('/', $id);
    }
    else {
      $name = $id;
    }

    return new Icon($class = "fa fa-{$name}");
  }

  public function getIconSets() {
    return array('default');
  }

  public function getIconList($set_name = 'default') {
    if ($v = go1_container('kv', 'aticon')->get('fontawsome.iconlist')) {
      return $v;
    }

    if ($v = $this->fetchIconList()) {
      go1_container('kv', 'aticon')->set('fontawsome.iconlist', $v);
    }

    return $v;
  }

  private function fetchIconList() {
    $css = go1_library('fontawesome') . '/css/font-awesome.css';
    $css = file_get_contents($css);
    preg_match_all('/\.fa-([a-z-_]+):before/', $css, $matches);
    return isset($matches[1]) ? $matches[1] : array();
  }
}
