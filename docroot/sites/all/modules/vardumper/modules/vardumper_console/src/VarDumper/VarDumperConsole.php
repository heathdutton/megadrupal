<?php
/**
 * @file
 * Contains Drupal\vardumper_console\VarDumper\VarDumperConsole.
 */

namespace Drupal\vardumper\VarDumper;

use Symfony\Component\VarDumper\Cloner\VarCloner;
use Symfony\Component\VarDumper\Dumper\CliDumper;

class VarDumperConsole extends VarDumperDebug {
  public function dump($var, $name = '') {
    $cloner = new VarCloner();
    $dumper = new CliDumper('php://stdout');

    $html = $this->border(strip_tags($this->getHeaders($name, $this->getDebugInformation()))) . "\n";
    file_put_contents ('php://stdout', $html);
    $dumper->dump($cloner->cloneVar($var));
  }

  private function border($string, $character = '*') {
    $string = $character . " " . $string . " " . $character;
    $line = str_repeat($character, strlen($string));
    return implode("\n", array($line, $string, $line));
  }
}
