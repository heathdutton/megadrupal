<?php

/**
 * Search service class using elasticsearch server.
 */
class ElasticsearchService extends SearchApiAbstractService {
  protected $client;

  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);

    if (empty($this->options)) {
      // First configuring this server
      $this->options = array(
        'host' => 'localhost',
        'port' => '9200',
      );
    }

    $form['host'] = array(
      '#type' => 'textfield',
      '#title' => t('elasticsearch host'),
      '#description' => t('The host name or IP of your elasticsearch server, e.g. <code>localhost</code> or <code>www.example.com</code>.'),
      '#default_value' => $this->options['host'],
      '#required' => TRUE,
    );
    $form['port'] = array(
      '#type' => 'textfield',
      '#title' => t('elasticsearch port'),
      '#description' => t('For example 9200.'),
      '#default_value' => $this->options['port'],
      '#required' => TRUE,
    );

    return $form;
  }

  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    parent::configurationFormValidate($form, $values, $form_state);
    if (isset($values['port']) && (!is_numeric($values['port']) || $values['port'] < 0 || $values['port'] > 65535)) {
      form_error($form['port'], t('The port has to be an integer between 0 and 65535.'));
    }
  }


  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    if ($ids == 'all' && $index) {
      $client = new Elastica_Client();
      $index = $client->getIndex($index->machine_name);
      $index->delete();
    }
  }

  public function indexItems(SearchApiIndex $index, array $items) {
    $successful_ids = array();
    $index_id = $index->machine_name;

    $type = $this->getIndexType($index);

    foreach ($items as $id => $item) {
      $doc_fields = array();
      foreach ($item as $key => $field) {
        $doc_fields[$key] = $field['value'];
      }
      $doc = new Elastica_Document($id, $doc_fields);

      try {
        $type->addDocument($doc);
      } catch (Exception $e) {
        $message = 'Could not index item @id for @index_id. Error: @msg';
        $variables = array(
          '@id' => $id,
          '@index_id' => $index_id,
          '@msg' => $e->getMessage(),
        );
        watchdog('elasticsearch', $message, $variables);
        // continue to the next item because this one has failed
        continue;
      }
      $successful_ids[] = $id;
    }

    return $successful_ids;
  }

  public function search(SearchApiQueryInterface $query) {
    $return = array(
      'results' => array(),
    );
    $index = $query->getIndex();
    $index_id = $index->machine_name;
    $type = $this->getIndexType($index);
    $keys = $query->getKeys();
    $options = $query->getOptions();
    $offset = isset($options['offset']) ? $options['offset'] : 0;
    $limit = isset($options['limit']) ? $options['limit'] : 1000000;

    try {
      $resultSet = $type->search($keys[0]);
      $return['result count'] = $resultSet->getTotalHits();

      foreach ($resultSet->getResults() as $result) {
        $id = $result->getId();
        $source = $result->getSource();
        $return['results'][$id] = array(
          'fields' => $source,
          'id' => $id,
          'score' => $result->getScore(),
        );
      }

    }
    catch (Exception $e) {
      $message = 'Could not search index @index_id. Error: @msg';
      $variables = array(
        '@index_id' => $index_id,
        '@msg' => $e->getMessage(),
      );
      watchdog('elasticsearch', $message, $variables);
      throw new SearchApiException($e->getMessage());
    }
    return $return;
  }

  public function getIndexType($index) {
    $client = new Elastica_Client();
    $index = $client->getIndex($index->machine_name);
    return $index->getType('entity');
  }

}
