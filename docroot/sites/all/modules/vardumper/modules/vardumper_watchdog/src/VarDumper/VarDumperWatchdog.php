<?php
/**
 * @file
 * Contains Drupal\vardumper_watchdog\VarDumper\VarDumperWatchdog.
 */

namespace Drupal\vardumper\VarDumper;

use Psr\Log\LoggerInterface;
use Symfony\Component\VarDumper\Cloner\VarCloner;
use Symfony\Component\VarDumper\Dumper\CliDumper;
use Drupal\vardumper\VarDumper\Dumper\HtmlDrupalDumper;
use Symfony\Component\VarDumper\VarDumper;

class VarDumperWatchdog extends VarDumperDebug {

  /**
   * The Logger service.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  public function __construct(LoggerInterface $logger) {
    $this->logger = $logger;
  }

  public function dump($var, $name = '') {
    // Permission are not checked in this submodule because permissions
    // are set on the module dblog from Drupal 7.
    $this->logger->debug($this->getHeaders($name, $this->getDebugInformation()) . $this->getDebug($var));
  }

}
