<?php
module_load_include('inc', 'factorydrone', 'lib/FactoryDrone');

class FactoryDroneTestCase extends DrupalWebTestCase {
  public function setUp($modules = array()) {
    parent::setUp($modules);
  }

  protected function load_factory_definitions() {
    FactoryDrone::addFactoryPath($this->factory_path);
    FactoryDrone::loadDefinitions();
  }

  protected function assertThrows($message, $cb) {
    try {
      $cb();
    } catch (Exception $e) {
      $this->assertEqual($e->getMessage(), $message);
    }
  }

  public function __get($name) {
    if ($name == 'factory_path') {
      return join(DIRECTORY_SEPARATOR, array(__DIR__, 'factories'));
    }
  }
}
