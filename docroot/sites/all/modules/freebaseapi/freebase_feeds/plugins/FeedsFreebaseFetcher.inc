<?php

/**
 * @file
 * A Feeds Fetcher plugin that uses Freebase API to fetch data.
 *
 * author dman dan@coders.co.nz
 * @version 2013-03
 */

/**
 * Feeds plugin. Uses Freebase API to fetch data.
 */
class FeedsFreebaseFetcher extends FeedsFetcher {

  /**
   * List the fields we expect to find on the config form.
   * 
   * ...and therefore which values get saved as settings on the fetcher.
   *
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'mode' => 'simple',
      'simple' => array(
        'type' => '',
        'attribute' => '',
        'value' => '',
      ),
      'mql' => array(
        'query' => '',
      ),
    );
  }

  /**
   * Provides the configuration form.
   * 
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $importer = feeds_importer($this->id);

    $form['mode'] = array(
      '#type' => 'select',
      '#options' => array('simple' => 'simple', 'mql' => 'mql'),
      '#title' => t('Query mode'),
      '#default_value' => $this->config['mode'],
    );

    // Edit UI for either mode. Visibility is toggled by the above selector.

    $form['mql'] = array(
      '#type' => 'fieldset',
      '#title' => t('MQL Query mode'),
      '#tree' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="mode"]' => array('value' => 'mql'),
        ),
      ),
    );
    $form['mql']['query'] = array(
      '#type' => 'textarea',
      '#title' => t('MQL Query'),
      '#default_value' => !empty($form_state['values']['mql']['query']) ? $form_state['values']['mql']['query'] : $this->config['mql']['query'],
      '#description' => t(
        '<p>An <a href="http://mql.freebaseapps.com/ch03.html">MQL query</a>,
         such as one prepared and tested with the
         <a href="https://www.freebase.com/query">Freebase API tool</a>.
         It should return data topics in the first level of the array of
         results.
         </p>
         <p>
         It is recommended to use fully namespaced predicates
         (attribute identifiers) as this helps disambiguation and auto-mapping
         later. (eg, you <em>could</em> ask for a books genre with)
           <pre>"type": "/book/book", "genre": []</pre>
         but <b>it is better</b> to ask for
           <pre>"type": "/book/book", "/book/book/genre": []</pre>
         </p>
         <p>
         This is helpful as it avoids puzzlement when the shortcut
           <pre>"type": "/book/book", "author": []</pre>
         does <em>not</em> work. It must instead be
           <pre>"type": "/book/book", "/book/written_work/author": []</pre>
         </p>
         <p>
         Also, prefer the [] empty value over the "null" placeholder. Assume
         all values may be multiple. Drupal supports this internally anyway
         so it\'s a convenient convention. You don\'t want to be surprised when
         it turns out that a person may have more than one nationality ... or
         gender.
         </p>
         '
      ),
    );
    $form['mql']['examples'] = array(
      '#type' => 'fieldset',
      '#title' => t('Examples'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mql']['examples'][] = array(
      '#prefix' => '<pre>',
      '#suffix' => '</pre>',
      '#markup' => '
[{
  "name": null,
  "id": null,
  "/cvg/computer_videogame/platforms": [],
  "/cvg/computer_videogame/publisher": [],
  "type": "/cvg/computer_videogame",
  "limit": 5
}]
',
    );

    $form['mql']['examples'][] = array(
      '#prefix' => '<pre>',
      '#suffix' => '</pre>',
      '#markup' => '
[{
  "name": null,
  "id": null,
  "/people/person/gender": null,
  "/people/person/place_of_birth": null,
  "/people/person/date_of_birth": null,
  "type": "/people/person",
  "/government/politician/government_positions_held": [{
    "/government/government_position_held/office_position_or_title": [{
      "name": "Prime Minister of the United Kingdom"
    }]
  }],
  "limit": 5
}]
',
    );

    $form['simple'] = array(
      '#type' => 'fieldset',
      '#title' => t('Simple Lookup mode'),
      '#tree' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="mode"]' => array('value' => 'simple'),
        ),
      ),
    );
    $form['simple']['type'] = array(
      '#type' => 'textfield',
      '#title' => t('Topic Type'),
      '#default_value' => !empty($form_state['values']['simple']['type']) ? $form_state['values']['simple']['type'] : $this->config['simple']['type'],
      '#description' => t(
        'A Freebase type. such as /music/recording, /people/person .'
      ),
    );
    if ($freebase_type = freebase_feeds_topic_type($importer)) {
      $form['simple']['type']['#description'] .= " I can tell by the RDF semantics you are using that <b>This Drupal bundle matches with the Freebase Topic type <em>$freebase_type</em></em></b>";
    }

    $form['simple']['attribute'] = array(
      '#type' => 'textfield',
      '#title' => t('Attribute'),
      '#default_value' => !empty($form_state['values']['simple']['attribute']) ? $form_state['values']['simple']['attribute'] : $this->config['simple']['attribute'],
      '#description' => t(
        'Optional. A Freebase attribute to use as a filter. EG name, /music/genre, /people/person/profession'
      ),
    );
    $form['simple']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Attribute Value'),
      '#default_value' => !empty($form_state['values']['simple']['value']) ? $form_state['values']['simple']['value'] : $this->config['simple']['value'],
      '#description' => t(
        'Optional. A string value to filter the attribute by. EG "Rock and Roll", "Lawyer"'
      ),
    );

    $freebase_predicates_to_fetch = freebase_feeds_entity_field_mappings($importer);
    // Given that we know what entity the processor should map to,
    // we can fetch its fields, and their data mapping rules,
    // and thus the data fields we need to request.
    $form['simple']['details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Details'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if ($freebase_predicates_to_fetch) {
      $form['simple']['details']['predicates'] = array(
        '#markup' => t('
        Based on info set in the Feeds Processor, and the semantic RDF
        annotations on the entity fields, I know that the data fields we need to
        fetch from Freebase for this content type are: <pre>%fields</pre>.
        ', array('%fields' => implode("\n", array_keys($freebase_predicates_to_fetch)))),
      );
    }
    else {
      $form['simple']['details']['no_predicates'] = array(
        '#markup' => t('
        I can\'t tell enough from the content type schema enough about
        exactly what data to ask Freebase for.
        You will probably just get the basic values associated
        with the topic type. This is usually enough, but for complex multi-type
        data, more refinement is needed.
        You can use the MQL method, and define the fields and mappings yourself.
        Alternatively, use RDF field mapping rules, and assign Freebase URI
        predicates to your fields, directly on the content type.
        ', array()),
      );
    }
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    switch ($values['mode']) {
      case 'mql':
        $json = json_decode($values['mql']['query']);
        if (empty($json)) {
          $form_key = 'mql][query';
          form_set_error($form_key, t('Invalid JSON. Check your syntax', array()));
        }
        break;
    }
  }

  /**
   * Capture a settings change.
   *
   * TODO, no ajax, maybe can remove.
   */
  public function configFormSubmit(&$values) {

    // For unknown magic reasons, possibly caching, this object is no longer
    // associated with its parent container.
    // We need to squish our data into that object so when the parent saves
    // it saves right.
    feeds_importer($this->id)->fetcher->config = $this->config;

    // This was HORRIBLE to find, and still makes no sense.
    // It's not needed if not using AJAX, but is if you are.
    parent::configFormSubmit($values);
  }


  /**
   * Actually call the service now, with the given params.
   * 
   * Sort out the response.
   *
   * Implementation of FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $importer = feeds_importer($this->id);
    $config = $this->config;
    // $source_config = $source->getConfigFor($this);

    // Query may be either a string or an struct, both are fine.
    switch ($config['mode']) {
      case 'mql':
        // Go off and run the query, assume it's legal!
        $query = $config['mql']['query'];
        break;

      case 'simple':
        $query = array(
          (object) array(
            "name" => NULL,
            "id" => NULL,
            "type" => $config['simple']['type'],
            "limit" => 2,
            "*" => array(),
          ),
        );
        // About wildcard expansion:
        // When used, the attributes come back without a namespace.
        // When used on a query that defines a 'type', then only type/*
        // attributes come back.
        // A wildcard search on an id with no type only returns base data.

        // But if we know enough about the semantics of the target from
        // RDF mapping rules, we know exactly what data to request.
        if ($freebase_predicates_to_fetch = freebase_feeds_entity_field_mappings($importer)) {
          foreach ($freebase_predicates_to_fetch as $predicate_id => $field_id) {
            $query[0]->$predicate_id = array();
          }
        }

        if (!empty($config['simple']['attribute'])) {
          $attribute = $config['simple']['attribute'];
          $value = $config['simple']['value'];
          $query[0]->$attribute = $value;
        }
        break;

      default:
        // Should not happen.
        $query = "";
    }

    // $query_string = json_encode($query);
    // dpm(get_defined_vars(), __FUNCTION__);

    $response = freebase_api_run_mql_query($query);

    // dpm(get_defined_vars(), __FUNCTION__);

    // $response is actually an already-decoded json struct now,
    // But other basic Feeds parsers may expect the raw string.
    // Let them have it... by RE-encoding it.
    $result = new FeedsFreebaseFetcherResult(json_encode($response));
    $result->setData($response);
    return $result;
  }


  /**
   * Source form.
   *
   * Shows all of the arguments this operation expects, Used on the
   * fetcher config form, and also shown on per-import pages.
   *
   * @param array $source_config
   *   Settings.
   *
   * @return array
   *   FAPI form array.
   */
  public function sourceForm($source_config) {
    $form = array();
    // $config = $this->getConfig();
    $form['operation_arguments']['#tree'] = TRUE;

    // This notice doesn't show if the operation takes *useless* parameters,
    // such as an array of nothing - which some WSDLs seem to do,
    if (empty($operation['parameter'])) {
      $form['operation_arguments']['#description'] = t('
        This operation takes no parameters.
        To set global parameters that apply to all operations, see the
        wsclient settings.
      ');
    }

    return $form;
  }

}


/**
 * A slightly extended result of FeedsHTTPFetcher::fetch().
 *
 * Most Feeds processors expect the ->raw data returned by
 * fetchers to be strings.
 * When using JSON fetching, it's already parsed object structures once
 * we've fetched it using freebase_api_run_mql_query().
 *
 * For conformity, the $result->raw will be the raw string, but we'll
 * internally keep a handle on the $result->data also.
 */
class FeedsFreebaseFetcherResult extends FeedsFetcherResult {
  protected $data;

  /**
   * Set the data array.
   */
  public function setData($data) {
    $this->data = $data;
  }
  /**
   * Return the data array.
   */
  public function getData() {
    return $this->data;
  }
}
