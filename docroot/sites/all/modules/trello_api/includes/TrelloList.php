<?php
/**
 * @file
 * Contains the TrelloList class.
 */

/**
 * Class for the TrelloList.
 */
class TrelloList extends Entity {

  // Make cards private.
  protected $cards = array();

  /**
   * Get the cards for this list.
   *
   * @return TrelloCard[]
   *   List of trello cards attached to this list.
   */
  public function getCards() {
    if (!empty($this->cards)) {
      return $this->cards;
    }
    try {
      $api = trello_api_get_api();
    }
    catch (TrelloAPIMalconfiguredException $e) {
      return array();
    }
    return $this->setCards($api->getCardsForList($this->trello_id))->cards;
  }

  /**
   * Sets the cards for this list.
   *
   * @param TrelloCard[] $cards
   *   List of trello cards attached to this list.
   *
   * @return $this
   */
  public function setCards($cards) {
    $this->cards = $cards;
    return $this;
  }

}
