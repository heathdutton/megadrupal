<?php

namespace Drupal\go1_base\Drush\Command;

use \Drupal\go1_base\Drush\Command\Go1Require\DependencyFetcher;

class Go1Require {
  private $module;

  public function __construct($module = 'all') {
    $this->module = $module;
  }

  public function execute() {
    $modules = array($this->module);

    if ($this->module === 'all') {
      $modules = array('go1_base' => 'go1_base') + go1_modules('go1_base', 'require');
    }

    foreach ($modules as $module) {
      $this->fetchDependencies($module);
    }
  }

  private function fetchDependencies($module) {
    $data = go1_config($module, 'require')->get('projects');

    foreach ($data as $name => $info) {
      go1_id(new DependencyFetcher($name, $info))->fetch();
    }
  }
}
