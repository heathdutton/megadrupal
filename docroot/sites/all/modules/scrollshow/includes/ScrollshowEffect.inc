<?php

class ScrollshowEffect {
  public $name;
  public $title;

  function __construct($plugin) {
    $this->name = $plugin['name'];
    $this->title = $plugin['title'];
  }

  function getSettingsDefaults() {
    return array();
  }

  function getSettingsForm($settings) {
    return array();
  }

  function prepareSettingsForJavascript($settings, $items) {
    return $settings;
  }

  function getLibrary($settings) {
    return array();
  }

  function getJS($settings) {
    return array();
  }

  function getCSS($settings) {
    return array();
  }
}

