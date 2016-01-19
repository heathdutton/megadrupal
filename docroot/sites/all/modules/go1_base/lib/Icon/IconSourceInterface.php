<?php
namespace Drupal\go1_base\Icon;

interface IconSourceInterface {
  /**
   * Return the name of source.
   * @return string
   */
  public function getName();

  /**
   * Get Icon instance with information to generate icon tag.
   *
   * @param string $css_code
   * @return \Drupal\go1_base\Icon\Icon
   */
  public function get($css_code);

  /**
   * Get icon-sets.
   *
   * @return [type] [description]
   */
  public function getIconSets();

  public function getIconList($set_name);
}
