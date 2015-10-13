<?php
/**
 * @file
 * Contains the controller classes for Party entities.
 */

/**
 * The API controller class for the Party Hat entity.
 */
class PartyHatController extends EntityAPIControllerExportable {

}

/**
 * The class used for hat entities.
 */
class PartyHat extends Entity {

  /**
   * The internal numeric id of the hat.
   *
   * @var integer
   */
  public $hid;

  /**
   * The machine name of a hat.
   *
   * @var string
   */
  public $name;

  /**
   * The machine name of this hat's parent.
   *
   * @var string
   */
  public $parent;

  /**
   * The label of the hat.
   *
   * @var string
   */
  public $label;

  /**
   * The description for the hat.
   *
   * @var string
   */
  public $description;

  /**
   * A serialized array of hat settings.
   *
   * This array contains:
   *   - dat_sets: An array settings keyed by data set name.
   *     - has: Whether or not this data set is allowed.
   *     - multiple: Whether or not this data set is allowed multiple.
   *
   * @var string
   */
  public $data;

  /**
   * Whether this hat is required by all parties.
   *
   * @var integer
   */
  public $required;

  /**
   * The exportable status of the entity.
   *
   * @var integer
   */
  public $status;

  /**
   * The name of the providing module if the entity has been defined in code.
   *
   * @var string
   */
  public $module;

  /**
   * Constructor for a PartyHat.
   *
   * @see Entity::__construct()
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'party_hat');
  }

  /**
   * Return the label of the hat.
   */
  protected function defaultLabel() {
    return $this->label;
  }
}

/**
 * UI controller.
 */
class PartyHatUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = 'Manage hats';
    // Set the main hat admin page as a local task so it appears within the
    // main Community admin area.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Disambiguate the 'list' tab.
    $items[$this->path . '/list']['title'] = 'List hats';

    return $items;
  }
}

/**
 * Extends the Party UI controller to show a party's hats in the admin list.
 */
class PartyHatPartyUIController extends PartyUIController {

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    // Get the basic row from the parent class.
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    array_splice($header, 2, 0, t('Hats'));

    return $header;
  }

  protected function overviewTableRow($conditions, $id, $party, $additional_cols = array()) {
    // Get the basic row from the parent class.
    $row = parent::overviewTableRow($conditions, $id, $party, $additional_cols);

    $hats_labels = array();
    $hats = party_hat_get_hats($party);
    foreach ($hats as $hat) {
      $hats_labels[] = $hat->label;
    }
    if (count($hats_labels)) {
      $hats_text = check_plain(implode(', ', $hats_labels));
    }
    else {
      $hats_text = t('None');
    }

    array_splice($row, 2, 0, $hats_text);

    return $row;
  }

}
