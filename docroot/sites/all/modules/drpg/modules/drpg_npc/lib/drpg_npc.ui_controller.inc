<?php

/**
 * @file
 * The UI controller for the NPC instance entity.
 */

class DrpgNPCUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage NPCs.');
    return $items;
  }

  /**
   * Overrides default table row to add JSON game data link.
   */
  public function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    global $base_url;

    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('JSON'), $base_url . '/drpg/data/npc/' . $id);

    return $row;
  }
}
