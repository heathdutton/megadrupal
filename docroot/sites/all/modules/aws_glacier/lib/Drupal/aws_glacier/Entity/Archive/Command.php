<?php

namespace Drupal\aws_glacier\Entity\Archive;

use Drupal\aws_glacier\Command as CommandWrapper;
use Drupal\aws_glacier\Entity\Vault\Vault;

/**
 * Class Command
 * @package Drupal\aws_glacier\Entity\Archive
 */
class Command extends CommandWrapper {

  /**
   * Sets the vaultName for the command.
   *
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $vault
   *
   * @return $this
   */
  public function setVault(Vault $vault) {
    $this->setArgs(array('vaultName' => $vault->VaultName));
    return $this;
  }

  /**
   * Sets the archiveId for the command.
   *
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   *
   * @return $this
   */
  public function setArchive(Archive $archive) {
    $this->setArgs(array('archiveId' => $archive->archiveId));
    return $this;
  }

}
