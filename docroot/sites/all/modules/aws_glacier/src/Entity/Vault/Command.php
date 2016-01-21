<?php

namespace Drupal\aws_glacier\Entity\Vault;

use Drupal\aws_glacier\Command as CommandWrapper;

/**
 * Class Command
 * @package Drupal\aws_glacier\Entity\Vault
 */
class Command extends CommandWrapper {

  /**
   * @var Vault
   */
  protected $vault;

  /**
   * Sets the required vault name for the command.
   *
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $vault
   *
   * @return $this
   */
  public function setVault(Vault $vault) {
    $this->vault = $vault;
    $this->setArgs(array('vaultName' => $vault->VaultName));
    return $this;
  }
}
