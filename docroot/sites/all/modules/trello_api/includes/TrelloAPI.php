<?php
/**
 * @file
 * Contains the TrelloAPI class.
 */

/**
 * Class for the API integration
 */
class TrelloAPI {

  /**
   * The trello url we use for api integrations.
   */
  const BASE_URL = 'https://api.trello.com/1/';

  /**
   * The request timeout, so we don't have requests hanging for too long.
   */
  const REQUEST_TIMEOUT = 5;

  /**
   * The api key used for Trello intergration.
   *
   * @var string
   */
  protected $apiKey;

  /**
   * The api token used for Trello intergration.
   *
   * @var string
   */
  protected $apiToken;

  /**
   * Make the new TrelloAPI class ready for use.
   *
   * @param array $settings
   *   Array containing the config for Trello, containing the following keys
   *    - api_key => The API key to use, fallback to the global api key
   *    - api_token => The API token to use, fallback to the global api token
   *
   * @throws TrelloAPIMalconfiguredException
   *   If could now be instanciated with required settings.
   */
  public function __construct(array $settings = array()) {
    $this->apiKey = !empty($settings['api_key']) ? $settings['api_key'] : variable_get('trello_api_key', '');
    $this->apiToken = !empty($settings['api_token']) ? $settings['api_token'] : variable_get('trello_api_token', '');
    if (!$this->apiKey) {
      throw new TrelloAPIMalconfiguredException('API key missing');
    }
    if (!$this->apiToken) {
      throw new TrelloAPIMalconfiguredException('API token missing');
    }
  }

  /**
   * Get a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get.
   *
   * @return NULL|TrelloBoard
   *   The loaded board or NULL if data could not be fetched.
   */
  public function getBoard($trello_id) {
    $data = $this->getBoardData($trello_id);
    if (!$data) {
      return NULL;
    }
    // Convert to Drupal style array values.
    $values = array(
      'trello_id' => $data['id'],
      'name' => $data['name'],
      'url' => $data['url'],
      'closed' => $data['closed'],
      'organization_trello_id' => $data['idOrganization'],
    );
    $board = entity_get_controller('trello_api_board')->create($values);
    if (variable_get('trello_api_auto_save', FALSE)) {
      $board->save();
    }
    return $board;
  }

  /**
   * Get the data for a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get.
   *
   * @return NULL|array
   *   The data for the board or NULL if data could not be fetched.
   */
  public function getBoardData($trello_id) {
    return $this->makeCall('boards/' . $trello_id);
  }

  /**
   * Get actions data related a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the cards for.
   * @param string $filter
   *   Optional filter string.
   * @param int $limit
   *   Optional limit, defaults to 50.
   *
   * @return array
   *   List of action objects sorted by date created or empty if no actions
   *   could be found.
   */
  public function getActionsDataForBoard($trello_id, $filter = '', $limit = 50) {
    $query = array('limit' => $limit);
    if ($filter) {
      $query['filter'] = $filter;
    }
    $data = $this->makeCall('boards/' . $trello_id . '/actions', $query);
    if (!$data) {
      return array();
    }
    return $data;
  }

  /**
   * Get a Trello Card.
   *
   * @param string $trello_id
   *   The long id of for the trello card to get.
   *
   * @return NULL|TrelloCard
   *   The loaded card or NULL if data could not be fetched.
   */
  public function getCard($trello_id) {
    $data = $this->getCardData($trello_id);
    if (!$data) {
      return NULL;
    }
    $cards = $this->prepareCards(array($data));
    return array_shift($cards);
  }

  /**
   * Get the data for a Trello card.
   *
   * @param string $trello_id
   *   The long id of for the trello card to get.
   *
   * @return NULL|array
   *   The data for the board or NULL if data could not be fetched.
   */
  public function getCardData($trello_id) {
    return $this->makeCall('cards/' . $trello_id);
  }

  /**
   * Get the cards related a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the cards for.
   *
   * @return TrelloCard[]
   *   List of trello cards or empty list if fail or no cards.
   */
  public function getCardsForBoard($trello_id) {
    $data = $this->getCardsForBoardData($trello_id);
    if (!$data) {
      return array();
    }
    return $this->prepareCards($data);
  }

  /**
   * Get the cards related a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the cards for.
   *
   * @return TrelloCard[]
   *   List of trello cards or empty list if fail or no cards.
   */
  public function getArchivedCardsForBoard($trello_id) {
    $data = $this->getCardsForBoardData($trello_id, array('filter' => 'closed'));
    if (!$data) {
      return array();
    }
    return $this->prepareCards($data);
  }

  /**
   * Get the data for cards related to a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the cards for.
   * @param array $query
   *   List of filters to be added to the API call to Trello.
   *
   * @return NULL|array[]
   *   The data for the cards or NULL if data could not be fetched.
   */
  public function getCardsForBoardData($trello_id, $query = array()) {
    return $this->makeCall('boards/' . $trello_id . '/cards', $query);
  }

  /**
   * Get the cards related a Trello list.
   *
   * @param string $trello_id
   *   The long id of for the trello list to get the cards for.
   *
   * @return TrelloCard[]
   *   List of trello cards or empty list if fail or no cards.
   */
  public function getCardsForList($trello_id) {
    $data = $this->getCardsForListData($trello_id);
    if (!$data) {
      return array();
    }
    return $this->prepareCards($data);
  }

  /**
   * Get the data for cards related to a Trello list.
   *
   * @param string $trello_id
   *   The long id of for the trello list to get the cards for.
   *
   * @return NULL|array[]
   *   The data for the cards or NULL if data could not be fetched.
   */
  public function getCardsForListData($trello_id) {
    return $this->makeCall('lists/' . $trello_id . '/cards');
  }

  /**
   * Get the labels related Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the labels for.
   *
   * @return TrelloLabel[]
   *   List of trello labels or empty list if fail or no labels.
   */
  public function getLabelsForBoard($trello_id) {
    $data = $this->getLabelsForBoardData($trello_id);
    if (!$data) {
      return array();
    }
    $labels = array();
    foreach ($data as $label_data) {
      $values = array(
        'trello_id' => $label_data['id'],
        'board_trello_id' => $label_data['idBoard'],
        'name' => $label_data['name'],
        'color' => $label_data['color'],
      );
      $label = entity_get_controller('trello_api_label')->create($values);
      if (variable_get('trello_api_auto_save', FALSE)) {
        $label->save();
      }
      $labels[$label_data['id']] = $label;
    }
    return $labels;
  }

  /**
   * Get the data for lists related to a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the lists for.
   *
   * @return NULL|array[]
   *   The data for the lists or NULL if data could not be fetched.
   */
  public function getLabelsForBoardData($trello_id) {
    return $this->makeCall('boards/' . $trello_id . '/labels');
  }

  /**
   * Get the lists related Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the lists for.
   *
   * @return TrelloList[]
   *   List of trello lists or empty list if fail or no lists.
   */
  public function getListsForBoard($trello_id) {
    $data = $this->getListsForBoardData($trello_id);
    if (!$data) {
      return array();
    }
    $lists = array();
    foreach ($data as $list_data) {
      $values = array(
        'trello_id' => $list_data['id'],
        'board_trello_id' => $list_data['idBoard'],
        'name' => $list_data['name'],
        'pos' => $list_data['pos'],
        'closed' => $list_data['closed'],
      );
      $list = entity_get_controller('trello_api_list')->create($values);
      if (variable_get('trello_api_auto_save', FALSE)) {
        $list->save();
      }
      $lists[$list_data['id']] = $list;
    }
    return $lists;
  }

  /**
   * Get the data for lists related to a Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the lists for.
   * @param array $query
   *   List of filters to be added to the API call to Trello.
   *
   * @return NULL|array[]
   *   The data for the lists or NULL if data could not be fetched.
   */
  public function getListsForBoardData($trello_id, $query = array()) {
    return $this->makeCall('boards/' . $trello_id . '/lists', $query);
  }

  /**
   * Get the archived lists related Trello board.
   *
   * @param string $trello_id
   *   The long id of for the trello board to get the lists for.
   *
   * @return TrelloList[]
   *   List of trello lists or empty list if fail or no lists.
   */
  public function getArchivedListsForBoard($trello_id) {
    $data = $this->getListsForBoardData($trello_id, array('filter' => 'closed'));
    if (!$data) {
      return array();
    }
    $lists = array();
    foreach ($data as $list_data) {
      $values = array(
        'trello_id' => $list_data['id'],
        'board_trello_id' => $list_data['idBoard'],
        'name' => $list_data['name'],
        'pos' => $list_data['pos'],
        'closed' => $list_data['closed'],
      );
      $list = entity_get_controller('trello_api_list')->create($values);
      if (variable_get('trello_api_auto_save', FALSE)) {
        $list->save();
      }
      $lists[$list_data['id']] = $list;
    }
    return $lists;
  }

  /**
   * Internal helper function to create an array of TrelloCard from JSON data.
   *
   * @param object $data
   *   JSON data from a card call
   *
   * @return TrelloCard[]
   *   List of cards created from data.
   */
  protected function prepareCards($data) {
    foreach ($data as $card_data) {
      $values = array(
        'trello_id' => $card_data['id'],
        'board_trello_id' => $card_data['idBoard'],
        'label_trello_ids' => $card_data['idLabels'],
        'list_trello_id' => $card_data['idList'],
        'description' => $card_data['desc'],
        'name' => $card_data['name'],
        'url' => $card_data['url'],
        'labels' => array(),
      );
      $card = entity_get_controller('trello_api_card')->create($values);
      if (variable_get('trello_api_auto_save', FALSE)) {
        $card->save();
      }
      $cards[$card_data['id']] = $card;
    }

    return $cards;
  }

  /**
   * Helper function to make calls and handle errors.
   *
   * @param string $url_part
   *   The url to call, relative to the base api url.
   * @param array $query
   *   The url to call, relative to the base api url.
   *
   * @return NULL|array
   *   JSON decoded response from trello or NULL if error.
   */
  protected function makeCall($url_part, $query = array()) {
    $query = array(
      'key' => $this->apiKey,
      'token' => $this->apiToken,
    ) + $query;
    $url = url(static::BASE_URL . $url_part, array('query' => $query));
    $response = drupal_http_request($url, array(
      'timeout' => static::REQUEST_TIMEOUT,
    ));
    if ($response->code != 200) {
      // Log the failure and return empty.
      watchdog('trello_api', 'Failed to call Trello API. Url: @url, response code: @response_code, response: @response', array(
        '@url' => $url,
        '@response_code' => $response->code,
        '@response' => !empty($response->data) ? $response->data : $response->error,
      ), WATCHDOG_ERROR);
      return NULL;
    }
    return json_decode($response->data, TRUE);
  }

}
