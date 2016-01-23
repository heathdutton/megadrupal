<?php
/**
 * @file
 * Search service class using Dataseed server.
 */

/**
 * Class DataseedService.
 */
class DataseedService extends SearchApiAbstractService {

  /**
   * A connection to the Dataseed server
   * @var SearchApiDataseedConnection
   */
  protected $dataseed;

  /**
   * Index the specified items.
   *
   * @param SearchApiIndex $index
   *   The search index for which items should be indexed.
   * @param array $items
   *   An array of items to be indexed, keyed by their id. The values are
   *   associative arrays of the fields to be stored, where each field is an
   *   array with the following keys:
   *   - type: One of the data types recognized by the Search API, or the
   *     special type "tokens" for fulltext fields.
   *   - original_type: The original type of the property, as defined by the
   *     datasource controller for the index's item type.
   *   - value: The value to index.
   *
   *   The special field "search_api_language" contains the item's language and
   *   should always be indexed.
   *
   *   The value of fields with the "tokens" type is an array of tokens. Each
   *   token is an array containing the following keys:
   *   - value: The word that the token represents.
   *   - score: A score for the importance of that word.
   *
   * @return array
   *   An array of the ids of all items that were successfully indexed.
   *
   * @throws SearchApiException
   *   If indexing was prevented by a fundamental configuration error.
   */
  public function indexItems(SearchApiIndex $index, array $items) {

    foreach ($items as &$item) {
      unset($item['search_api_language']);
    }

    $success = FALSE;

    try {
      $this->connect();
      $success = $this->dataseed->addObservations($items, $index->machine_name);
    }
    catch (SearchApiException $e) {
      watchdog_exception(
        'dataseed_search_api',
        $e,
        "%type while Indexing items: !message in %function (line %line of %file)
        .",
        array(),
        WATCHDOG_WARNING
      );
    }

    $ret = $success ? array_keys($items) : array();
    return $ret;
  }

  /**
   * Delete items from an index on this server.
   *
   * Might be either used to delete some items (given by their ids) from a
   * specified index, or all items from that index, or all items from all
   * indexes on this server.
   *
   * @param array $ids
   *   Either an array containing the ids of the items that should be deleted,
   *   or 'all' if all items should be deleted. Other formats might be
   *   recognized by implementing classes, but these are not standardized.
   * @param SearchApiIndex $index
   *   The index from which items should be deleted, or NULL if all indexes on
   *   this server should be cleared (then, $ids has to be 'all').
   */
  public function deleteItems($ids = 'all', SearchApiIndex $index = NULL) {
    try {
      $this->connect();

      if (is_array($ids)) {
        foreach ($ids as $id) {
          $this->dataseed->deleteObservation($index->machine_name, $id);
        }
      }
      else {
        $this->dataseed->deleteAllObservations($index->machine_name);
      }

    }
    catch (SearchApiException $e) {
      watchdog_exception(
        'dataseed_search_api',
        $e,
        "%type while Deleting items: !message in %function (line %line of %file)
        .",
        array(),
        WATCHDOG_WARNING
      );
    }
  }

  /**
   * Executes a search on the server represented by this object.
   *
   * @param SearchApiQueryInterface $query
   *   The SearchApiQueryInterface object to execute.
   *
   * @return array
   *   An associative array containing the search results, as required by
   *   SearchApiQueryInterface::execute().
   *
   * @throws SearchApiException
   *   If an error prevented the search from completing.
   */
  public function search(SearchApiQueryInterface $query) {
    /*
     * Actually we do not need this implementation as Dataseed module will be
     * used only for indexing stuff not for searching them through Drupal.
     *
     * This method must be implemented just because it is abstract in the parent
     * class
     */
  }

  /**
   * Defines a configuration form for this service class.
   */
  public function configurationForm(array $form, array &$form_state) {
    if (is_null(variable_get('dataseed_host'))) {
      drupal_set_message(
        t("You have to configure the !dataseed_settings_url before creating a Dataseed server.",
          array(
            '!dataseed_settings_url' => l(
              t('Dataseed module'),
              'admin/config/system/dataseed'
            ),
          )
        ), 'warning');
    }

    /*
     * This form must contain at least one field: if we leave this form empty,
     * we'll get a lot of errors a dataseed server is created/edited
     */
    $form['dummy'] = array('#type' => 'hidden');

    return $form;
  }


  /**
   * Overrides SearchApiAbstractService::configurationFormValidate().
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    if (is_null(variable_get('dataseed_host'))) {
      form_error($form,
        t("Cannot create a Dataset server: !dataseed_settings_url is not configured yet.",
          array(
            '!dataseed_settings_url' => l(
              t('Dataseed module'),
              'admin/config/system/dataseed'
            ),
          )
        )
      );
    }
  }

  /**
   * Creates a search api Index.
   */
  public function addIndex(SearchApiIndex $index) {
    /*
     * If the index was already added to the server, the object should treat
     * this as if removeIndex() and then addIndex() were called.
     * @see SearchApiServiceInterface
     */
    $this->removeIndex($index);

    try {
      $this->connect();
      $this->dataseed->createDataset($this->createModelByIndex($index));
    }
    catch (SearchApiException $e) {
      /*
       * If we get a SearchApiException we should not create the index at all
       * but actually we can't do that: this method is invoked within the
       * postCreate() method of the SearchApiIndex entity which, therefore, is
       * saved even if the service class addIndex() implementation raises an
       * error. Even raising a new Exception here wouldn't be useful:
       * search_api_admin_add_index form submit/validate handlers wouldn't take
       * it into account.
       */
      drupal_set_message(
        t("The index has been created but something went wrong with the Dataseed
          Server: the dataset might have not been properly created on Dataseed.
          Check logs for details"),
        'warning'
      );
      watchdog_exception(
        'dataseed_search_api  ',
        $e,
        "%type while creating Dataseed index: !message in %function (line %line
        of %file). This may be caused by network problems or wrong Dataseed
        credentials provided",
        array(),
        WATCHDOG_WARNING
      );
    }

  }

  /**
   * Removes a search api index.
   */
  public function removeIndex($index) {
    try {
      $this->connect();
      $this->dataseed->removeDataset($index->machine_name);
    }
    catch (SearchApiException $e) {
      drupal_set_message(
        t(
          "The index has been deleted but something went wrong with the Dataseed
          Server: the dataset might have not been properly removed on Dataseed.
          Check logs for details"
        ),
        'warning'
      );
      watchdog_exception(
        'dataseed_search_api',
        $e,
        "%type while removing Dataseed index: !message in %function (line %line
        of %file). This may be caused by 1) network problems 2) the fact that
        the dataset actually does not exist on Dataseed 3) wrong Dataseed
        credentials provided.",
        array(),
        WATCHDOG_WARNING
      );
    }
  }

  /**
   * Deletes and then creates again the $index.
   *
   * This makes the corresponding Dataseed dataset being re-created: this is
   * needed when the Dataseed model changes (the dataset privacy option is
   * toggled, indexed fields change and so on).
   *
   * @param SearchApiIndex $index
   *   the index that needs to be re-created
   * @param bool $do_reindex
   *   TRUE if we want to schedule a re-index. The default value is FALSE
   *   because, tipically, the method is invoked within $this->fieldsUpdated()
   *   which returns true (which make a reindex being scheduled)
   *
   * @throws SearchApiException
   *   If an error prevented the index creation from completing.
   */
  public function recreateIndex(SearchApiIndex $index, $do_reindex = FALSE) {
    $this->removeIndex($index);
    $this->connect();
    $this->dataseed->createDataset($this->createModelByIndex($index));

    if ($do_reindex) {
      $index->clear();
      $index->reindex();
    }
  }

  /**
   * Invoked on change of the fields associated with an index.
   */
  public function fieldsUpdated(SearchApiIndex $index) {
    try {
      $this->recreateIndex($index);
    }
    catch (SearchApiException $e) {
      drupal_set_message(
        t("The fields have been updated but something went wrong with the
        Dataseed Server: the dataset might have not been properly updated on
        Dataseed . Check logs for details"),
        'warning'
      );
      watchdog_exception(
        'dataseed_search_api',
        $e,
        "%type Dataset creation failed during a fields update for a Dataseed
        index: !message in %function
        (line %line of %file).",
        array(),
        WATCHDOG_WARNING
      );
    }
    // Data must be re-indexed.
    return TRUE;
  }

  /**
   * Generate a Dataseed model starting from a SearchApiIndex.
   *
   * For testing this method:
   *  - make it public
   *  - use this snippet:
   *
   * $index = entity_load('search_api_index', array(<SEARCH_API_INDEX_ID>));
   * $index = array_shift($index);
   * $server = entity_load('search_api_server', array(<SEARCH_API_SERVER_ID>));
   * $server = array_shift($server);
   * $dataseed_service = new DataseedService($server);
   * dpm($dataseed_service->createModelByIndex($index));
   *
   * @param SearchApiIndex $index
   *   the index the model fields must be taken from
   *
   * @return string
   *   the Dataseed model in JSON format. For further information on the model
   *   format check the Dataseed documentation at
   *   http://dataseedapp.com/documentation
   */
  public function createModelByIndex(SearchApiIndex $index) {
    $public_dataset = FALSE;

    if (isset($index->options['dataseed_search_api'])) {
      $public_dataset = ($index->options['dataseed_search_api']['dataseed_search_api_privacy'] == 'public') ? TRUE : FALSE;
    }

    $dataseed_model         = new stdClass();
    $dataseed_model->id     = $index->machine_name;
    $dataseed_model->label  = strlen($index->name) ? $index->name : $index->machine_name;
    $dataseed_model->public = $public_dataset;
    $dataseed_model->fields = array();

    $index_fields = $index->getFields();

    $chart_obj_bar       = new stdClass();
    $chart_obj_bar->type = 'bar';

    $chart_obj_bubble       = new stdClass();
    $chart_obj_bubble->type = 'bubble';

    $chart_obj_table       = new stdClass();
    $chart_obj_table->type = 'table';

    $chart_obj_line       = new stdClass();
    $chart_obj_line->type = 'line';

    // search_api_language is added by the search framework. We don't need it.
    unset($index_fields['search_api_language']);
    foreach ($index_fields as $fkey => $f) {
      $model_field              = new stdClass();
      $model_field->id          = $fkey;
      $model_field->label       = $f['name'];
      $model_field->description = $f['description'];

      $model_field->charts = array(
        $chart_obj_bar,
        $chart_obj_bubble,
        $chart_obj_table,
        $chart_obj_line,
      );

      $model_field->aggregations = array();

      // Set model type.
      switch ($f['type']) {
        case 'date':
          $model_field->type = 'date';
          break;

        case 'integer':
          $model_field->type = 'integer';
          break;

        case 'decimal':
          $model_field->type = 'float';
          break;

        case 'string':
        default:
          $model_field->type = 'string';
          break;
      }

      $dataseed_model->fields[] = $model_field;
    }

    // Set default visualisations, if provided.
    if (!empty($index->options['dataseed_search_api']['dataseed_search_api_visualisations'])) {
      $dataseed_model->visualisations = drupal_json_decode($index->options['dataseed_search_api']['dataseed_search_api_visualisations']);
    }

    return drupal_json_encode($dataseed_model);
  }

  /**
   * Create a connection to the Dataseed server as configured in $this->options.
   *
   * Basically it implements a (kind of) factory for SearchApiDataseedConnection
   */
  protected function connect() {
    if (!$this->dataseed) {
      $options        = dataseed_get_settings() + array('server' => $this->server->machine_name);
      $this->dataseed = new SearchApiDataseedConnection($options);
    }
  }

}
