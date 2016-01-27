<?php
/**
 * @file
 * Define service clasess for xapian search engine.
 */

include_once('xapian.php');

/**
 * Basic search service class using Xapian library.
 */
abstract class SearchApiAbstractXapianService extends SearchApiAbstractService {

  /**
   * The main prefix used on xapian documents.
   */
  const DOCID_PREFIX = 'Q';

  /**
   * Xapian matches strategy: Best estimate.
   */
  const MATCHES_BEST_ESTIMATE = 0;

  /**
   * Xapian matches strategy: Lower bound.
   */
  const MATCHES_LOWER_BOUND = 1;

  /**
   * Xapian matches strategy: Upper bound.
   */
  const MATCHES_UPPER_BOUND = 2;

  /**
   * A read only Xapian database.
   *
   * @var XapianDatabase
   */
  protected $database = NULL;

  /**
   * A writable Xapian database.
   *
   * @var XapianWritableDatabase
   */
  protected $writable_database = NULL;

  /**
   * A Xapian indexer.
   *
   * @var XapianTermGenerator
   */
  protected $indexer = NULL;

  /**
   * A Xapian stemmer.
   *
   * @var XapianStem
   */
  protected $stemmer = NULL;

  /**
   * Helper to get the right xapian database object.
   *
   * @param boolean $writable
   *   TRUE to obtain a XapianWritableDatabase object, FALSE to get a
   *   XapianDatabase object.
   */
  public function getDatabase($writable = FALSE) {
    if (!$writable && is_object($this->database)) {
      return $this->database;
    }
    if ($writable && is_object($this->writable_database)) {
      return $this->writable_database;
    }
  }

  /**
   * Helper to get the right xapian stemmer object.
   * TODO make sure non-stem is handled correctly.
   */
  public function getStemmer() {
    if (!is_object($this->stemmer)) {
      if (isset($this->options['xapian_stem_language'])) {
        $language = $this->options['xapian_stem_language'];
      }
      else {
        $language = 'english';
      }
      try {
        $this->stemmer = new XapianStem($language);
      }
      catch (Exception $e) {
        watchdog('search_api_xapian', t('An error occurred getting the stem with language !language: !msg.', array('!msg' => $e->getMessage(), '!language' => $language)));
        return NULL;
      }
    }
    return $this->stemmer;
  }

  /**
   * Helper to get the right xapian term generator(aka indexer) object.
   */
  public function getIndexer() {
    if (!is_object($this->indexer)) {
      try {
        $this->indexer = new XapianTermGenerator();
      }
      catch (Exception $e) {
        watchdog('search_api_xapian', t('An error occurred getting the indexer: !msg.', array('!msg' => $e->getMessage())));
        return NULL;
      }
    }
    try {
      $this->indexer->set_stemmer($this->getStemmer());
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error occurred setting the stemmer: !msg.', array('!msg' => $e->getMessage())));
      return NULL;
    }
    return $this->indexer;
  }

  /**
   * Write all the pending changes to the xapian database.
   */
  public function flushDatabase() {
    if (is_object($this->writable_database)) {
      try {
        $this->writable_database->flush();
      }
      catch (Exception $e) {
        watchdog('search_api_xapian', t('An error flushing the database: !msg.', array('!msg' => $e->getMessage())));
        return NULL;
      }
    }
  }

  /**
   * Convinience method to get a unique item identifier inside the xapian
   * database.
   *
   * @param string $index_machine_name
   *   The actual index machine name.
   * @param int $item_id
   *   The unique item identifier.
   */
  public function getUniqueId($index_machine_name, $item_id) {
    return sprintf('%sindex:%s;item_id:%d', self::DOCID_PREFIX, $index_machine_name, $item_id);
  }

  /**
   * Helper for getting the right count.
   *
   * @param XapianMSet $matches
   */
  public function getMatchesCount(XapianMSet $matches) {
    $count_type = NULL;
    if (isset($this->options['xapian_count_type'])) {
      $count_type = $this->options['xapian_count_type'];
    }

    try {
      switch ($count_type) {
        case self::MATCHES_LOWER_BOUND:
          $count = $matches->get_matches_lower_bound();
          break;

        case self::MATCHES_UPPER_BOUND:
          $count = $matches->get_matches_upper_bound();
          break;

        case self::MATCHES_BEST_ESTIMATE:
        default:
          $count = $matches->get_matches_estimated();
          break;
      }
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error getting the matches count: !msg.', array('!msg' => $e->getMessage())));
      return NULL;
    }

    return  $count;
  }

  /**
   * Helper to generate select form about xapian languages.
   */
  function getXapianLanguagesOptions() {
    try {
      $list = explode(' ', XapianStem::get_available_languages());
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error getting the language options: !msg.', array('!msg' => $e->getMessage())));
      return NULL;
    }

    foreach ($list as $language) {
      $languages[$language] = drupal_ucfirst($language);
    }
    return $languages;
  }

  /**
   * Helper to generate select form about xapian match count types.
   */
  function getXapianMatchCountTypes() {
    return array(
        self::MATCHES_BEST_ESTIMATE => t('Best estimate'),
        self::MATCHES_LOWER_BOUND => t('Lower bound'),
        self::MATCHES_UPPER_BOUND => t('Upper bound'),
      );
  }

  function getDefaultQueryOperator($query) {
    $operator = XapianQuery::OP_OR;
    $conjunction = $query->getOption('conjunction');
    switch ($conjunction) {
      case 'AND':
        return XapianQuery::OP_AND;
      case 'OR':
      default:
        return XapianQuery::OP_OR;
    }
  }

  /**
   * Flatten a keys array into a single search string.
   *
   * @param array $keys
   *   The keys array to flatten, formatted as specified by
   *   SearchApiQueryInterface::getKeys().
   *
   * @return string
   *   A Xapian query string representing the same keys.
   */
  protected function flattenKeys(array $keys) {
    $k = array();
    foreach (element_children($keys) as $i) {
      $key = $keys[$i];
      if (!$key) {
        continue;
      }
      if (is_array($key)) {
        $k[] = $this->flattenKeys($key);
      }
      else {
        $key = trim($key);
        if (strpos($key, ' ') !== FALSE) {
          $key = '"' . $key .'"';
        }
        $k[] = $key;
      }
    }
    if (!$k) {
      return '';
    }
    if ($keys['#conjunction'] == 'OR') {
      $k = '((' . implode(') OR (', $k) . '))';
      return empty($keys['#negation']) ? $k : '-' . $k;
    }
    if ($keys['#conjunction'] == 'AND') {
      $k = '((' . implode(') AND (', $k) . '))';
      return empty($keys['#negation']) ? $k : '-' . $k;
    }

    $k = implode(' ', $k);
    return empty($keys['#negation']) ? $k : '-(' . $k . ')';
  }

  /**
   * Helper method for indexing a field into a document.
   *
   * @param XapianDocument $document
   *   A xapian document object passed by reference where the field belongs to.
   * @param XapianTermGenerator $indexer
   *   The xapian indexer that is going to be used to index the field.
   * @param array $field
   *   A field array as received inside each item of $items parameter on SearchApiServiceInterface::indexItems().
   */
  protected function indexField(&$document, &$indexer, $field) {
    $value = $field['value'];
    $type = $field['type'];
    if (search_api_is_list_type($type)) {
      $type = substr($type, 5, -1);
      foreach ($value as $v) {
        $this->indexField($document, $indexer, $v);
      }
      return;
    }
    $weight = (int)($field['boost']);
    try {
      switch ($type) {
        case 'tokens':
          foreach ($value as $v) {
            $indexer->index_text($v['value'], $weight);
          }
          return;
        case 'boolean':
          $value = $value ? 'true' : 'false';
          break;
        case 'date':
          $value = is_numeric($value) ? (int) $value : strtotime($value);
          if ($value === FALSE) {
            return;
          }
          // xapian require a string
          $value = (string) $value;
          break;
        default:
          // if not explicitely supported, use string casted value
          $value = (string) $value;
      }
      $document->add_term($value, $weight);
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error indexing the field: !msg.', array('!msg' => $e->getMessage())));
    }
  }

  public function configurationForm(array $form, array &$form_state) {
    // See http://xapian.org/docs/stemming.html
    $xapian_form['xapian_stem_language'] = array(
      '#type' => 'select',
      '#title' => t('Stemming language'),
      '#options' => $this->getXapianLanguagesOptions(),
      '#default_value' => empty($this->options['xapian_stem_language']) ? 'english' : $this->options['xapian_stem_language'],
      '#description' => t('Select the language that Xapian should use when deriving the stem of each word when building an index.'),
    );
    $xapian_form['xapian_count_type'] = array(
      '#type' => 'radios',
      '#title' => t('Result count'),
      '#description' => t('This setting determines the value that xapian returns for the result count returned from queries (used for number of pages in pagers, etc.)'),
      '#default_value' => empty($this->options['xapian_count_type']) ? self::MATCHES_BEST_ESTIMATE : $this->options['xapian_count_type'],
      '#options' => $this->getXapianMatchCountTypes(),
    );
    return $xapian_form;
  }

  public function viewSettings() {
    $output = '';
    $form = $form_state = array();
    $option_form = $this->configurationForm($form, $form_state);
    $option_names = array();
    foreach ($option_form as $key => $element) {
      if (isset($element['#title']) && isset($this->options[$key])) {
        $option_names[$key] = $element['#title'];
      }
    }

    foreach ($option_names as $key => $name) {
      $value = $this->options[$key];
      if ($key == 'xapian_stem_language' || $key == 'xapian_count_type') {
        $value = $option_form[$key]['#options'][$value];
      }
      $output .= '<dt>' . check_plain($name) . '</dt>' . "\n";
      $output .= '<dd>' . nl2br(check_plain(print_r($value, TRUE))) . '</dd>' . "\n";
    }

    return $output ? "<dl>\n$output</dl>" : '';
  }

  public function indexItems(SearchApiIndex $index, array $items) {

    $ret = array();

    try {
      $database = $this->getDatabase(TRUE);
      foreach ($items as $id => $item) {
        $document = new XapianDocument();
        $indexer = $this->getIndexer();
        $indexer->set_document($document);
        // some ways to identify this document: all, index and item
        $unique_id = $this->getUniqueId($index->machine_name, $id);
        $document->add_boolean_term('search_api_xapian_server');
        $document->add_term('index' . $index->machine_name);
        $document->add_term('item_id' . $id);
        $document->add_term($unique_id);
        // set the right data
        $document->set_data($id);

        foreach ($item as $key => $field) {
          $this->indexField($document, $indexer, $field);
        }

        $database->replace_document($unique_id, $document);
        $ret[] = $id;
      }
      $this->flushDatabase();
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error occurred while indexing: !msg.', array('!msg' => $e->getMessage())));
      return array();
    }

    return $ret;
  }

  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    $database = $this->getDatabase(TRUE);
    try {
      if ($index) {
        if (is_array($ids)) {
          // delete mentioned items
          foreach ($ids as $id) {
            $database->delete_document($this->getUniqueId($index->machine_name, $id));
          }
        }
        else {
          // delete all
          $database->delete_document('index' . $index->machine_name);
        }
      }
      else {
        // delete all
        $database->delete_document('search_api_xapian_server');
      }
      $this->flushDatabase();
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error occurred while deleting: !msg.', array('!msg' => $e->getMessage())));
    }
  }

  //FIXME implement filters correctly
  public function search(SearchApiQueryInterface $query) {
    $start_time = microtime(TRUE);

    try {
      // 1. Prepare the search
      $preprocess_start = microtime(TRUE);
      $keys = $query->getKeys();
      $stemmer = $this->getStemmer();
      $database = $this->getDatabase();
      $operator = $this->getDefaultQueryOperator($query);
      $enquire = new XapianEnquire($database);
      $query_parser = new XapianQueryParser();
      $query_parser->set_stemmer($stemmer);
      $query_parser->set_database($database);
      $query_parser->set_default_op($operator);
      $query_parser->set_stemming_strategy(XapianQueryParser::STEM_SOME); //TODO this should be an option
      if (is_array($keys)) {
        $keys = $this->flattenKeys($keys);
      }
      // avoid FLAG_DEFAULT to support versions less than 1.0.11
      // maybe convert this into an option in the future or autodetect to provide a best attempt
      $parser_options = XapianQueryParser::FLAG_PHRASE | XapianQueryParser::FLAG_BOOLEAN | XapianQueryParser::FLAG_LOVEHATE | XapianQueryParser::FLAG_PURE_NOT;
      $real_query = $query_parser->parse_query($keys, $parser_options);
      $start = $query->getOption('offset') === NULL ? 0 : $query->getOption('offset');
      // a lenght = 0 gives us an empty MSet with valid statistics calculated
      // without looking at any postings, which is very quick, but means the
      // estimates may be more approximate and the bounds may be much looser.
      $length = $query->getOption('limit') === NULL ? 0 : $query->getOption('limit');
      $preprocess_end = microtime(TRUE);

      // 2. Execute the search
      $execution_start = microtime(TRUE);
      $enquire->set_query($real_query);
      $matches = $enquire->get_mset((int)$start, (int)$length);
      $execution_end = microtime(TRUE);

      // 3. Extract results
      $postprocessing_start = microtime(TRUE);
      $results = array();
      $results['result count'] = $this->getMatchesCount($matches);
      $results['results'] = array();

      $i = $matches->begin();
      while (!$i->equals($matches->end())) {
        $document = $i->get_document();
        $id = $document->get_data();
        $results['results'][$id] = array(
          'id' => $id,
          'score' => (int)($i->get_percent()),
        );
        $i->next();
      }
      $postprocessing_end = microtime(TRUE);

      // Compute performance
      $end_time = microtime(TRUE);
      $results['performance'] = array(
        'complete' => $end_time - $start_time,
        'preprocessing' => $preprocess_end - $preprocess_start,
        'execution' => $execution_end - $execution_end,
        'postprocessing' => $postprocessing_end - $postprocessing_start,
      );

      return $results;
    }
    catch (Exception $e) {
      throw new SearchApiException($e->getMessage());
    }
  }

}

/**
 * Search service class using Xapian library on a local directory.
 */
class SearchApiXapianLocalService extends SearchApiAbstractXapianService {

  /**
   * Local directory where xapian database is stored.
   */
  protected $path = NULL;

  public function getDatabase($writable = FALSE) {
    $ret_database = parent::getDatabase($writable);
    if (is_object($ret_database)) {
      return $ret_database;
    }

    if (is_null($this->path)) {
      if (isset($this->options['xapian_database_path'])) {
        $this->path = drupal_realpath($this->options['xapian_database_path']);
      }
      else {
        watchdog('search_api_xapian', 'No database path given for server %server.', array('%server' => $this->server->machine_name));
        return NULL;
      }
    }

    if (!is_dir($this->path)) {
      // Create it if needed.
      $this->writable_database = new XapianWritableDatabase($this->path, Xapian::DB_CREATE_OR_OPEN);
    }

    try {
      if ($writable) {
        $this->writable_database = new XapianWritableDatabase($this->path, Xapian::DB_CREATE_OR_OPEN);
        return $this->writable_database;
      }
      else {
        $this->database = new XapianDatabase($this->path);
        return $this->database;
      }
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error occurred getting the !writable database : !msg.', array('!msg' => $e->getMessage(), '!writable' => ($writable ? 'writable': 'non-writable'))));
      return NULL;
    }
  }

  public function configurationForm(array $form, array &$form_state) {
    $xapian_local_form = parent::configurationForm($form, $form_state);
    $xapian_local_form['xapian_database_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to database'),
      '#default_value' => empty($this->options['xapian_database_path']) ? file_default_scheme() . '://xapian_database' : $this->options['xapian_database_path'],
      '#required' => TRUE,
      '#description' => t('Directory where your local Xapian database will be created.  Specify a directory writable by your web server process.'),
    );
    return $xapian_local_form;
  }

  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    parent::configurationFormValidate($form, $values, $form_state);
    // TODO check a valid drupal schema path
  }

}

/**
 * Search service class using Xapian library on a remote server.
 */
class SearchApiXapianServerService extends SearchApiAbstractXapianService {

  /**
   * Hostname where xapian-tcpsrv is running.
   */
  public $hostname = NULL;

  /**
   * Port where xapian-tcpsrv is running.
   */
  public $port = NULL;

  public function getDatabase($writable = FALSE) {
    $ret_database = parent::getDatabase($writable);
    if (is_object($ret_database)) {
      return $ret_database;
    }

    if (is_null($this->hostname) || is_null($this->port)) {
      if (isset($this->options['xapian_database_hostname']) && isset($this->options['xapian_database_port'])) {
        $this->hostname = $this->options['xapian_database_hostname'];
        $this->port = $this->options['xapian_database_port'];
      }
      else {
        watchdog('search_api_xapian', 'No database hostname or port given for server %server.', array('%server' => $this->server->machine_name));
        return NULL;
      }
    }

    try {
      if ($writable) {
        $this->writable_database = Xapian::remote_open_writable($this->hostname, (int)$this->port);
        return $this->writable_database;
      }
      else {
        $db_source = Xapian::remote_open($this->hostname, (int)$this->port);
        $this->database = new XapianDatabase($db_source);
        return $this->database;
      }
    }
    catch (Exception $e) {
      watchdog('search_api_xapian', t('An error occurred getting the !writable database : !msg.', array('!msg' => $e->getMessage(), '!writable' => ($writable ? 'writable': 'non-writable'))));
      return NULL;
    }

  }

  public function configurationForm(array $form, array &$form_state) {
    $xapian_daemon_form = parent::configurationForm($form, $form_state);
    $xapian_daemon_form['xapian_database_hostname'] = array(
      '#type' => 'textfield',
      '#title' => t('Database server'),
      '#default_value' => empty($this->options['xapian_database_hostname']) ? '' : $this->options['xapian_database_hostname'],
      '#required' => TRUE,
      '#description' => t('IP address or host name of remote server running xapian-tcpsrv.'),
    );
    $xapian_daemon_form['xapian_database_port'] = array(
      '#type' => 'textfield',
      '#title' => t('Database port'),
      '#default_value' => empty($this->options['xapian_database_port']) ? '6431' : $this->options['xapian_database_port'],
      '#required' => TRUE,
      '#description' => t('Remote port that xapian-tcpsrv is listening on.'),
    );
    return $xapian_daemon_form;
  }

  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    parent::configurationFormValidate($form, $values, $form_state);
    if (!is_numeric($values['xapian_database_port']) || (int)$values['xapian_database_port'] < 1 || (int)$values['xapian_database_port'] > 65535) {
      //FIXME hightlight correctly the port field?
      form_set_error('xapian_database_port', t('%value is not a valid port.', array('%value' => $values['xapian_database_port'])));
    }
  }

}

//TODO support Xapian Remote Backend with Prog Method on another class
//TODO implement a facets
//TODO implement a special facet for XapianESet
