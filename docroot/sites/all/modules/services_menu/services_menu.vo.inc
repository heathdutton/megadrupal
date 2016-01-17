<?php

/**
 * @file
 * Services menu Value Object, returns menu
 * Filters out theming related values
 * Adds path alias values
 */

class ServicesMenu {
  public $name;
  public $tree = array();

  public function __construct($name) {
    $this->name = $name;
  }
}

class ServicesMenuItem {
  public $link;
  public $children;

  public function __construct() {
    $this->link = new ServicesMenuLink();
  }
}

class ServicesMenuLink {
  public $mlid;
  public $plid;

  public $title;
  public $description;

  public $path;
  public $path_alias;
  public $href;

  public $hidden;
  public $external;
  public $has_children;
  public $weight;
  public $depth;

  public $options;
  public $localized_options;
}