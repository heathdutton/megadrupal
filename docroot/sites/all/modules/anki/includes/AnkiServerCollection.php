<?php
/**
 * @file
 * Contains AnkiServerCollection class.
 */

/**
 * Represents a collection on the Anki server.
 */
class AnkiServerCollection {
  private $connection;
  private $name;

  /**
   * Construct a new AnkiServerCollection..
   *
   * @param AnkiServerConnection $connection
   *   Connection to the server.
   * @param string $name
   *   Name of the collection.
   */
  public function __construct(AnkiServerConnection $connection, $name) {
    $this->connection = $connection;
    $this->name = $name;
  }

  /**
   * Get the connection.
   *
   * @return AnkiServerConnection
   *   Connection to the server.
   */
  public function getConnection() {
    return $this->connection;
  }

  /**
   * Get the name.
   *
   * @return string
   *   Name of the collection.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Make a request to the AnkiServer for this collection.
   */
  public function request($path, $args = array()) {
    return $this->connection->request('collection/' . $this->name . '/' . $path, $args);
  }

  /**
   * Get a list of the decks in this collection.
   *
   * @todo: Document the deck objects!
   *
   * @return array
   *   An array of objects.
   */
  public function listDecks() {
    return $this->request('list_decks');
  }

  /**
   * Get the cards in the collection.
   *
   * @param array $options
   *   (Optional) An associative array with the following keys (all optional):
   *   - query: An Anki card query (see http://ankisrs.net/docs/manual.html#searching)
   *   - order: A property to order by
   *   - limit: The number of cards to retreive (an integer)
   *   - offset: The number of cards to skip in the result set (an integer)
   *   - preload: Whether or not to fully load the card object (a boolean)
   *
   * @return array
   *   An array of card objects.
   */
  public function findCards(array $options = array()) {
    return $this->request('find_cards', $options);
  }

  /**
   * Add a note.
   *
   * @param object $note
   *   An object representing a note.
   */
  public function addNote($note) {
    return $this->request('add_note', $note);
  }

  /**
   * Update a note.
   *
   * @param object $note
   *   An object representing a note.
   */
  public function updateNote($note) {
    return $this->request('note/' . $note->id . '/update', $note);
  }

  /**
   * Delete a note.
   *
   * @param object $note
   *   An object representing a note.
   */
  public function deleteNote($note) {
    return $this->request('note/' . $note->id . '/delete', $note);
  }

  /**
   * Reset scheduler.
   *
   * @todo: Document the options!
   *
   * @param array $options
   *   An associative array of options.
   * @return array
   *   An associative array with the following keys:
   *   - new_cards: The number of new cards (integer).
   *   - learning_cards: The number of learning cards (integer).
   *   - review_cards: The number of review cards (integer).
   */
  public function resetScheduler(array $options = array()) {
    return (array)$this->request('reset_scheduler', $options);
  }

  /**
   * Create a dynamic deck.
   *
   * @todo: Document the options!
   *
   * @param array $options
   *   An associative array of options.
   */
  public function createDynamicDeck(array $options = array()) {
    return $this->request('create_dynamic_deck', $options);
  }

  /**
   * Set the Anki server's language.
   *
   * @param string $code
   *   The language code.
   */
  public function setLanguage($code) {
    return $this->request('set_language', array('code' => $code));
  }

  /**
   * Get the next card to be reviewed.
   *
   * @todo: Document the options!
   * @todo: Document the return value!
   *
   * @param array $options
   *   An associative array of options.
   *
   * @return array
   *   An array containing the next card and some meta-data.
   */
  public function getNextCard(array $options = array()) {
    return (array)$this->request('next_card', $options);
  }

  /**
   * Answer a card.
   *
   * @param string $card_id
   *   The card id.
   * @param int $ease
   *   The ease that the card was answered.
   * @param int $timer_started
   *   (Optional) When the user started reviewing this card in UNIX time.
   */
  public function answerCard($card_id, $ease, $timer_started = NULL) {
    return $this->request('answer_card', array(
      'id' => $card_id,
      'ease' => $ease,
      'timerStarted' => $timer_started,
    ));
  }
}
