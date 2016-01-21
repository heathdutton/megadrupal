<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\Tests\Parser;

use Drupal\config\Parser\ParserManager;

/**
 * Class MockParserManager
 * @package Drupal\config\Tests\Parser
 */
class MockParserManager extends ParserManager {

  /**
   * {@inheritdoc}
   */
  protected function getParserImplementers() {

    // We only return 'config', and we do it manually to avoid having to
    // enable it for testing.
    return array('config' => 'config');
  }

  /**
   * {@inheritdoc}
   */
  protected function getParserModuleConfig($module_name) {

    return array(
      CONFIG_PARSER_PHP => array(
        'name' => t('PHP'),
        'class' => '\Drupal\config\Parser\PhpParser',
      ),
      CONFIG_PARSER_JSON => array(
        'name' => t('JSON'),
        'class' => '\Drupal\config\Parser\JsonParser',
      ),
    );
  }

}
