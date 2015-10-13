<?php
/**
 * @file
 * Contains Drupal\vardumper\VarDumper\VarDumperMessage.
 */

namespace Drupal\vardumper\VarDumper;

use Drupal\service_container\Legacy\Drupal7;
use Drupal\service_container\Messenger\MessengerInterface;
use Symfony\Component\VarDumper\Cloner\VarCloner;
use Symfony\Component\VarDumper\Dumper\CliDumper;
use Drupal\vardumper\VarDumper\Dumper\HtmlDrupalDumper;
use Symfony\Component\VarDumper\VarDumper;

class VarDumperMessage extends VarDumperDebug {

  /**
   * The Messenger service.
   *
   * @var \Drupal\service_container\Messenger\LegacyMessenger
   */
  protected $messenger;

  /**
   * The Drupal7 legacy service.
   *
   * @var \Drupal\service_container\Legacy\Drupal7
   */
  protected $drupal7;

  public function __construct(MessengerInterface $messenger, Drupal7 $drupal7) {
    $this->messenger = $messenger;
    $this->drupal7 = $drupal7;
  }

  public function dump($var, $name = '') {
    if (!$this->hasPermission()) {
      return;
    }
    $html = $this->getHeaders($name, $this->getDebugInformation()) . $this->getDebug($var);
    $this->messenger->addMessage($html, 'status', FALSE);
  }

}
