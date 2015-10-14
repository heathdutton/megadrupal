<?php
/**
 * @file
 * Contains the TrelloBoard class.
 */

/**
 * Class for the TrelloBoard.
 */
class TrelloBoard extends Entity {

  // Make cards and lists private.
  protected $cards = array();
  protected $labels = array();
  protected $lists = array();
  protected $archived_lists = array();
  protected $archived_cards = array();

  /**
   * Make sure that the TrelloBoard has all related external data.
   *
   * @return $this
   */
  public function fetchActiveExternal() {
    $this->getLabels();
    $this->getLists();
    $this->getCards();
    return $this;
  }

  /**
   * Make sure that the TrelloBoard has all archived external data.
   *
   * @return $this
   */
  public function fetchArchivedExternal() {
    $this->getLabels();
    $this->getArchivedCards();
    $this->getArchivedLists();
    return $this;
  }

  /**
   * Get the cards for this board.
   *
   * @return TrelloCard[]
   *   List of trello cards attached to this board.
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
    return $this->setCards($api->getCardsForBoard($this->trello_id))->cards;
  }

  /**
   * Get the archived cards for this board.
   *
   * @return TrelloCard[]
   *   List of trello cards attached to this board.
   */
  public function getArchivedCards() {
    if (!empty($this->archived_cards)) {
      return $this->archived_cards;
    }
    try {
      $api = trello_api_get_api();
    }
    catch (TrelloAPIMalconfiguredException $e) {
      return array();
    }
    return $this->setArchivedCards($api->getArchivedCardsForBoard($this->trello_id))->archived_cards;
  }

  /**
   * Get the labels for this board.
   *
   * @return TrelloLabel[]
   *   List of trello labels attached to this board.
   */
  public function getLabels() {
    if (!empty($this->labels)) {
      return $this->labels;
    }
    try {
      $api = trello_api_get_api();
    }
    catch (TrelloAPIMalconfiguredException $e) {
      return array();
    }
    return $this->setLabels($api->getLabelsForBoard($this->trello_id))->labels;
  }

  /**
   * Get the lists for this board.
   *
   * @return TrelloList[]
   *   List of trello lists attached to this board.
   */
  public function getLists() {
    if (!empty($this->lists)) {
      return $this->lists;
    }
    try {
      $api = trello_api_get_api();
    }
    catch (TrelloAPIMalconfiguredException $e) {
      return array();
    }
    return $this->setLists($api->getListsForBoard($this->trello_id))->lists;
  }

  /**
   * Get the lists for this board.
   *
   * @return TrelloList[]
   *   List of trello lists attached to this board.
   */
  public function getArchivedLists() {
    if (!empty($this->archived_lists)) {
      return $this->archived_lists;
    }
    try {
      $api = trello_api_get_api();
    }
    catch (TrelloAPIMalconfiguredException $e) {
      return array();
    }
    return $this->setArchivedLists($api->getArchivedListsForBoard($this->trello_id))->archived_lists;
  }

  /**
   * Sets the cards for this board.
   *
   * @param TrelloCard[] $cards
   *   List of trello cards attached to this board.
   *
   * @return $this
   */
  public function setCards($cards) {
    // Add labels to the cards if we have them.
    if ($this->labels) {
      foreach ($cards as $card) {
        foreach ($card->label_trello_ids as $label_id) {
          if (!empty($this->labels[$label_id])) {
            $card->labels[] = $this->labels[$label_id];
          }
        }
      }
    }

    $this->cards = $cards;

    // If lists are already loaded on the board, attach the cards to them.
    if ($this->lists) {
      $card_list_cards = array();
      foreach ($cards as $card) {
        $card_list_cards[$card->list_trello_id][] = $card;
      }
      foreach ($card_list_cards as $list_id => $card_list) {
        if (!empty($this->lists[$list_id])) {
          $this->lists[$list_id]->setCards($card_list);
        }
      }
    }

    return $this;
  }

  /**
   * Sets the archived cards for this board.
   *
   * @param TrelloCard[] $cards
   *   List of trello cards attached to this board.
   *
   * @return $this
   */
  public function setArchivedCards($cards) {
    // Add labels to the cards if we have them.
    if ($this->labels) {
      foreach ($cards as $card) {
        foreach ($card->label_trello_ids as $label_id) {
          if (!empty($this->labels[$label_id])) {
            $card->labels[] = $this->labels[$label_id];
          }
        }
      }
    }

    $this->archived_cards = $cards;

    // If lists are already loaded on the board, attach the cards to them.
    if ($this->archived_lists) {
      $card_list_cards = array();
      foreach ($cards as $card) {
        $card_list_cards[$card->list_trello_id][] = $card;
      }
      foreach ($card_list_cards as $list_id => $card_list) {
        if (!empty($this->archived_lists[$list_id])) {
          $this->archived_lists[$list_id]->setCards($card_list);
        }
      }
    }

    return $this;
  }

  /**
   * Sets the labels for this board.
   *
   * @param TrelloLabel[] $labels
   *   List of trello labels attached to this board.
   *
   * @return $this
   */
  public function setLabels($labels) {
    $this->labels = $labels;
    if ($this->cards) {
      $this->setCards($this->cards);
    }
    return $this;
  }

  /**
   * Sets the lists for this board.
   *
   * @param TrelloList[] $lists
   *   List of trello lists attached to this board.
   *
   * @return $this
   */
  public function setLists($lists) {
    $this->lists = $lists;
    if ($this->cards) {
      $this->setCards($this->cards);
    }
    return $this;
  }

  /**
   * Sets the lists for this board.
   *
   * @param TrelloList[] $lists
   *   List of trello lists attached to this board.
   *
   * @return $this
   */
  public function setArchivedLists($lists) {
    $this->archived_lists = $lists;
    if ($this->archived_cards) {
      $this->setArchivedCards($this->archived_cards);
    }
    return $this;
  }
}
