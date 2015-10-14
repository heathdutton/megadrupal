<?php

/**
 * @file
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\Tests\File;

use Drupal\config\Exception\ConfigException;
use Drupal\config\File\FileLoader;
use Drupal\config\Tests\ConfigTestBase;

class FileLoaderTest extends ConfigTestBase {

  /**
   * The FileLoader
   *
   * @var FileLoader
   */
  public $fileLoader;

  public $configPath;

  protected function setUp() {

    $this->fileLoader = FileLoader::create();
  }

  public function testGetFolderPath() {
    $this->configPath = $this->fileLoader->getFolderPath('config', 'src/Test/config');

    $this->assertEquals('sites/all/modules/config/src/Test/config', $this->configPath);
  }

  public function testGetFilePath() {
    $base_config_path = $this->fileLoader->getFilePath('sites/all/modules/config/src/Test/config', 'config.json');

    $this->assertEquals('sites/all/modules/config/src/Test/config/config.json', $base_config_path);
  }

  public function testLoadFile() {

    try {
      $file = $this->fileLoader->loadFile(DRUPAL_ROOT . '/sites/all/modules/config/src/Tests/config/config.json');
    }
    catch(ConfigException $e) {
      $this->fail($e->getMessage());
    }

    $this->assertNotEmpty($file, 'File exists');
  }
}
