<?php

/**
 * Search service class using Lucene server.
 */
class ZendLuceneService extends SearchApiAbstractService {
  protected $lucene;

  public function configurationForm(array $form, array &$form_state) {
    $lucene_local_form = parent::configurationForm($form, $form_state);
    $lucene_local_form['lucene_database_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to database'),
      '#default_value' => empty($this->options['lucene_database_path']) ? file_default_scheme() . '://lucene_database' : $this->options['lucene_database_path'],
      '#required' => TRUE,
      '#description' => t('Directory where your Lucenen database will be created.  Specify a directory writable by your web server process.'),
    );
    return $lucene_local_form;
  }

  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    parent::configurationFormValidate($form, $values, $form_state);
  }

  protected function getLucene(SearchApiIndex $index) {
    if (!$this->lucene) {
      $filepath = $this->server->options['lucene_database_path'];
      // Zend will attempt to create the directory itself but failed. I'm not
      // sure why. So for now we step in and create it with Drupal.
      if (!file_exists($filepath) && drupal_mkdir($filepath, NULL, TRUE)) {
        file_create_htaccess($filepath);
      }

      // If the Zend Framework isn't availble return FALSE to avoid an error
      if (!zend_get_path()) {
        watchdog('zend_lucene', 'Zend Lucene can not index or search without the Zend Framework library.', array(),  WATCHDOG_CRITICAL);
        return FALSE;
      }

      try {
        $lucene = Zend_Search_Lucene::open($filepath);
      }
      catch (Zend_Exception $e) {
        // Failed to open the database so let's try creating one first
        try {
          $lucene = Zend_Search_Lucene::create($filepath);
        }
        catch (Zend_Exception $e) {
          // this bad because we can't get the database
        }
      }
      // Instantiates and sets the numeric, case insentive, UTF-8 analyzer.
      $analyzer = new Zend_Search_Lucene_Analysis_Analyzer_Common_Utf8Num_CaseInsensitive();
      Zend_Search_Lucene_Analysis_Analyzer::setDefault($analyzer);
      $this->lucene = $lucene;
    }
    return $this->lucene;
  }

  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    $lucene = $this->getLucene($index);
    for ($count = 0; $count < $lucene->maxDoc(); $count++) {
      $lucene->delete($count);
    }
    $lucene->optimize();
  }

  public function indexItems(SearchApiIndex $index, array $items) {
    $successful_ids = array();
    $index_id = $index->machine_name;
    // only continue if we have an instance of lucene to avoid an error
    if ($lucene = $this->getLucene($index)) {
      foreach ($items as $id => $item) { 
        $document = new Zend_Search_Lucene_Document();
        $id_field = Zend_Search_Lucene_Field::unIndexed('id', $this->createId($index_id, $id));
        $document->addField($id_field);
        $item_id_field = Zend_Search_Lucene_Field::unIndexed('item_id', $id);
        $document->addField($item_id_field);
        foreach ($item as $key => $field_data) {
          if (!$type = search_api_is_list_type($field_data['type'])) {
            $type = $field_data['type'];
            $field_data['value'] = array($field_data['value']);
          }
          foreach ($field_data['value'] as $value) {
            // Everything is stored as text which is tokenized, indexed and
            // stored.
            // Other options would be:
            // - keyword: indexed and stored
            // - unIndexed: stored
            // - binary: stored
            // - unStored: tokenized and indexed
            $field = Zend_Search_Lucene_Field::Text($key, (string)$value);
            $field->boost = $field_data['boost'];        
            $document->addField($field);
          }
        }
        $lucene->addDocument($document);
        $successful_ids[] = $id;
      }
      $lucene->optimize();
    }
    return $successful_ids;
  }

  /**
   * Creates an ID used as the unique identifier. This has to consist of both
   * index and item ID.
   * @todo check if this is required for the lucene or is just something needed
   *   by solr because it uses a single core for multiple indexes. 
   */
  protected function createId($index_id, $item_id) {
    return "$index_id-$item_id";
  }

  public function search(SearchApiQueryInterface $query) {
    $return = array();
    $index = $query->getIndex();
    $lucene = $this->getLucene($index);
    $keys = $query->getKeys();
    $options = $query->getOptions();
    $offset = isset($options['offset']) ? $options['offset'] : 0;
    $limit = isset($options['limit']) ? $options['limit'] : 1000000;

    // this is currently only taking the first search term and needs to be
    // built up to response to the $keys returned by Search API.
    $terms = $signs = array();
    foreach ($keys as $i => $key) {
      if (is_numeric($i)) {
        // The query is case senertive so we have used to strtolower() for
        // each key.
        $terms[] = new Zend_Search_Lucene_Index_Term(strtolower($key));
        $signs[] = TRUE;
      }
    }
    $query = new Zend_Search_Lucene_Search_Query_MultiTerm($terms, $signs);
    $lucene_results = $lucene->find($query);
    $count = count($lucene_results);
    $lucene_results = array_slice($lucene_results, $offset, $limit);
    foreach ($lucene_results as $lucene_result) {
      $item_id = (int)$lucene_result->item_id;
      $result = array(
        'id' => $item_id,
        'score' => $lucene_result->score,
      );
      $return['results'][$item_id] = $result;
    }
    $return['result count'] = $count;
    return $return;
  }

}
