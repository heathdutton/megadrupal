<?php

/**
 * @file
 * Search API service class.
 */

/**
 * Search API service class for Enterprise Search.
 */
class SearchApiEnterpriseSearchService extends SearchApiSolrService {

  /**
   * Overrides SearchApiSolrService::connect().
   */
  protected function connect() {
    if (!$this->solr) {
      if (!class_exists('Apache_Solr_Service')) {
        throw new Exception(t('SolrPhpClient library not found! Please follow the instructions in search_api_solr/INSTALL.txxt for installing the Solr search module.'));
      }
      $this->solr = new SearchApiEnterpriseSearchConnection($this->options);
    }
  }

  /**
   * Overrides SearchApiSolrService::configurationForm().
   *
   * Populates the Solr configs with Enterprise Search Information.
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);

    $options = $this->options += array(
      'edismax' => 0,
      'host' => 'find.axis12.com',
      'port' => '80',
      
      // @todo add our host grabber
      'path' => '/solr/' . enterprise_agent_settings('enterprise_identifier'),
    );
    $form['http']['#access'] = FALSE;

    $form['edismax'] = array(
      '#type' => 'checkbox',
      '#title' => t('Always allow advanced syntax for Enterprise Search'),
      '#default_value' => $options['edismax'],
      '#description' => t('If enabled, all Enterprise Search keyword searches may use advanced <a href="@url">Lucene syntax</a> such as wildcard searches, fuzzy searches, proximity searches, boolean operators and more via the Extended Dismax parser. If not enabled, this syntax wll only be used when needed to enable wildcard searches.', array('@url' => 'http://lucene.apache.org/java/2_9_3/queryparsersyntax.html')),
      '#weight' => -30,
    );

    $form['modify_enterprise_connection'] = array(
      '#type' => 'checkbox',
      '#title' => 'Modify Enterprise Search Connection Parameters',
      '#default_value' => $options['modify_enterprise_connection'],
      '#description' => t('Only check this box if you are absolutely certain about what you are doing. Any misconfigurations will most likely break your site\'s connection to Enterprise Search.'),
      '#weight' => -20,
    );

    // Re-sets defaults with Enterprise information.
    $form['host']['#default_value'] = $options['host'];
    $form['port']['#default_value'] = $options['port'];
    $form['path']['#default_value'] = $options['path'];

    // Only display fields if we are modifying the connection parameters to the
    // Enterprise Search service.
    $states = array(
      'visible' => array(
        ':input[name="options[form][modify_enterprise_connection]"]' => array('checked' => TRUE),
      ),
    );
    $form['host']['#states'] = $states;
    $form['port']['#states'] = $states;
    $form['path']['#states'] = $states;

    // We cannot connect directly to the Solr instance, so don't make it a link.
    if (isset($form['server_description'])) {
      $url = 'http://' . $this->options['host'] . ':' . $this->options['port'] . $this->options['path'];
      $form['server_description'] = array(
        '#type' => 'item',
        '#title' => t('Enterprise Search URI'),
        '#description' => check_plain($url),
        '#weight' => 10,
      );
    }

    return $form;
  }

  /**
   * Overrides SearchApiSolrService::preQuery().
   *
   * Sets the eDisMax parameters if certain conditions are met.
   */
  protected function preQuery(array &$call_args, SearchApiQueryInterface $query) {
    $params = &$call_args['params'];

    // Bails if this is a 'mlt' query or something else custom.
    if (!empty($params['qt']) || !empty($params['defType'])) {
      return;
    }

    // The Search API module adds default "fl" parameters in solrconfig.xml
    // that are not present in Enterprise Search's solrconfig.xml file. Add them
    // and others here as a backwards compatible solution.
    // @see http://drupal.org/node/1619770
    $params += array(
      'echoParams' => 'none',
      'fl' => 'item_id,score',
      'q.op' => 'AND',
      'q.alt' => '*:*',
      'spellcheck' => 'false',
      'spellcheck.onlyMorePopular' => 'true',
      'spellcheck.extendedResults' => 'false',
      'spellcheck.count' => '1',
      'hl' => 'false',
      'hl.fl' => 'spell',
      'hl.simple.pre' => '[HIGHLIGHT]',
      'hl.simple.post' => '[/HIGHLIGHT]',
      'hl.snippets' => '3',
      'hl.fragsize' => '70',
      'hl.mergeContiguous' => 'true',
    );

    // Set the qt to eDisMax if we have keywords and either the configuration
    // is set to always use eDisMax or the keys contain a wildcard (* or ?).
    $keys = $query->getOriginalKeys();
    if ($keys && (($wildcard = preg_match('/\S+[*?]/', $keys)) || $this->options['edismax'])) {
      $params['defType'] = 'edismax';
      if ($wildcard) {
        // Converts keys to lower case, reset keys in query and replaces param.
        $new_keys = preg_replace_callback('/(\S+[*?]\S*)/', array($this, 'toLower'), $keys);
        $query->keys($new_keys);
        $call_args['query'] = $new_keys;
      }
    }
  }

  /**
   * Convert to lower-case any keywords containing a wildcard.
   *
   * @see _enterprise_search_lower()
   */
  public function toLower($matches) {
    return drupal_strtolower($matches[1]);
  }
}
