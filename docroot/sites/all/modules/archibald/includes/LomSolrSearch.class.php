<?php

/**
 * @file
 * class for searching lom within a solr index
 */
module_load_include('module', 'apachesolr', 'apachesolr');

/**
 ! class for searching lom within a solr index
 * This class completly depends on drupal and its apchesolr module
 *
 * @author Heiko Henning h.henning@educa.ch
 */
class ArchibaldLomSolrSearch {

  /**
   * solr service object
   * @var DrupalApacheSolrService
   */
  protected $solr;

  /**
   * total number of found records
   * @var integer
   */
  protected $total = 0;

  /**
   * result set of search request
   * @var stdClass
   */
  protected $response;

  /**
   * filter query conditions
   * @var array
   */
  protected $conditions = array();

  /**
   * mapping between options and faceting fields
   * @var array
   */
  protected $optionsFacetMapping = array();

  /**
   * stored by setFacetFilter() and used from getFacetOptions() to set activ bit in on options
   * @var array
   */
  protected $choosenFacetOptions = array();

  public function __construct() {
    // maximum number of results per page
    $this->conditions['rows'] = 25;
    $this->conditions['start'] = 0;

    // query type
    $this->conditions['qt'] = 'standard';

    // the basic filter will set here cause otherwise we dont have any results with empty query
    $this->conditions['q'] = 'entity_type:lom';

    // filed list to return
    $this->conditions['fl'] = array(
      'id',
      'ss_entity_id',
      'teaser',
      'hs_preview_image',
      'path',
      'label',
      'hts_collection',
      'hs_company_logo',
      'score',
    );

    // define query parser
    // with dismax we dont can have empty querys,  see: self::setQuery()
    $this->conditions['defType'] = 'lucene';
    // disable debug options, safe traffix
    $this->conditions['echoParams'] = 'none';
  }

  /**
   * init solar session
   * returns TRUE if connection to solr could be intited
   *
   * @param $do_ping boolean check if server is available
   *
   * @return boolean status
   */
  public function init($do_ping = TRUE) {
    try {
      // Get the $solr object
      $this->solr = apachesolr_get_solr();
      // If there is no server available, don't continue.
      if ($do_ping == TRUE) {
        if (!$this->solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
          throw new Exception(t('No Solr instance available.'));
        }
      }
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    return TRUE;
  }

  /**
   * cron for indexin in solr
   *
   * @return boolean status
   */
  public function cron() {
    $processed_list = array();
    if ($this->init() != TRUE) {
      return FALSE;
    }


    /*
     * When we need to index:
     *
     * if (empty(ls.indexed_version)) {
     *   need index
     *   be sure that it is really not indexed and delete it from solr
     * }
     * elseif !empty(ls.local_published) {
     *   if (ls.indexed_version!=ls.local_published) {
     *       // if local published version is not equal indexed version
     *      need reindex
     *      update solr index
     *   }
     * }
     * elseif empty(ls.local_published)) {
     *   if (ls.indexed_version!=l.version) {
     *       // if not newest version is indexed
     *      need reindex
     *      its is not local published and newest version is
     *      not equal indexed version
     *      delete it from index
     *   }
     *   else if(ls.local_published != ls.indexed_version) {
     *     We unpublished it, we need to remove it now from the index.
     *   }
     * }
     *
     */


    $res = db_query(" SELECT l.lom_id, l.version
                      FROM {archibald_lom} l
                      LEFT JOIN {archibald_lom_stats} ls ON (l.lom_id=ls.lom_id)
                      WHERE
                      l.save_time = (
                        SELECT la.save_time
                        FROM {archibald_lom} la
                        WHERE la.lom_id=l.lom_id
                        ORDER BY la.save_time DESC LIMIT 1
                        )
                      AND (
                            (
                              ls.local_published != '' AND
                              ls.local_published != ls.indexed_version
                            )
                            OR
                            (
                              ls.local_published='' AND ls.local_published != ls.indexed_version
                            )
                      )");

    $maintenance_max_execution_time = ini_get('max_execution_time') == 0 ? 240 : ini_get('max_execution_time');

    if ($maintenance_max_execution_time > 20) {
      $maintenance_max_execution_time -= 10;
    }

    $maintenance_start_time = time();
    while ($elm = $res->fetchObject()) {
      if (!empty($processed_list[$elm->lom_id])) {
        // already up to date, don't need to index again
        continue;
      }

      if (($maintenance_start_time + ($maintenance_max_execution_time / 2)) < time()) {
        // the max execution time reached : stop
        break;
      }

      $indexer = new ArchibaldLomSolr();
      switch ($indexer->index($elm->lom_id, $documents_to_delete)) {
        case 'delete':
          $this->solr->deleteByQuery('path:archibald/' . $elm->lom_id);
          $indexer->processed[$elm->lom_id] = '';
          break;

        case 'insert':
        default:
          $this->solr->deleteByQuery('path:archibald/' . $elm->lom_id);
          $docs = $indexer->getDocs();
          if (!empty($docs)) {
            $this->solr->addDocuments($docs);
          }
          if (!empty($documents_to_delete)) {
            foreach ($documents_to_delete as $lom_id_to_delete) {
              $this->solr->deleteByQuery('path:archibald/' . $lom_id_to_delete);
            }
          }
          /* debug version
            foreach ($indexer->getDocs() as $i=>$doc) {
            print_r($doc);
            $this->solr->addDocuments(array($doc));
            }
           */
          break;
      }

      $processed = $indexer->getProcessed();
      foreach ($processed as $lom_id => $version) {
        $processed_list[$lom_id] = $version;

        db_update('archibald_lom_stats')
          ->condition('lom_id', $lom_id)
          ->fields(array('indexed_version' => $version))
          ->execute();
      }
    }

    if (!empty($processed_list)) {
      $this->solr->commit();
    }
  }


  public function deleteFromIndex($lom_id) {
    if ($this->init() != TRUE) {
      return FALSE;
    }

    $this->solr->deleteByQuery('path:archibald/' . $lom_id);
    $this->solr->commit();
  }

  /**
   * configure the maximum number of results per page
   *
   * @param integer $limit
   *
   * @return boolean status
   */
  public function setLimit($limit) {
    $limit = (INT) $limit;

    if ($limit < 1) {
      return FALSE;
    }

    $this->conditions['rows'] = $limit;
    return TRUE;
  }

  /**
   * configure the result ofset
   *
   * @param integer $start
   *
   * @return boolean status
   */
  public function setStart($start) {
    $start = (INT) $start;

    if ($start < 0) {
      return FALSE;
    }

    $this->conditions['start'] = $start;
    return TRUE;
  }

  /**
   * set filter condition
   *
   * @param string $key    name of field to look up in
   * @param string $value  if empty it will remove the filter
   *                       if a array is given, then it all values will or`ed
   *
   * @example
   *  setFilter('ss_language', $language);
   *  setFilter('hts_collection', array(1, 2));
   */
  public function setFilter($key, $value, $operator = 'OR') {
    if (empty($value)) {
      foreach ($this->conditions as $k => $v) {
        if (drupal_substr($v, 0, drupal_strlen($key) + 1) == $key . ':') {
          unset($this->conditions[$k]);
        }
      }
    }
    else {
      // convert std class to array
      if (is_object($value)) {
        $value = (array) $value;
      }

      // convert array to ored string
      if (is_array($value)) {
        $value = '("' . implode('" ' . $operator . ' "', $value) . '")';
      }
      $this->conditions['fq'][] = $key . ':' . $value;
    }
  }

  /**
   * field to look for search keywords, can contain boost options
   *
   * boost options will attached as folat value after a ^
   * default boost options is 1.0
   *
   * @param string $qf if array given it will converted to string
   *
   * @example
   * setQueryFields('label content tm_keywords')
   *
   * @example
   * setQueryFields('label^3 content^2.5  tm_keywords^3')
   *
   * @example
   * setQueryFields(array('label^3', 'content^2.5', 'tm_keywords^3'))
   */
  public function setQueryFields($qf) {
    if (!is_array($qf)) {
      $qf = explode(' ', $qf);
    }
    $this->conditions['qf'] = $qf;
  }

  /**
   * set query words
   *
   * @param string $query
   *
   * @return boolean status
   */
  public function setQuery($query) {
    if (empty($query) || !is_string($query)) {
      return FALSE;
    }

    // ### Filter in query filters out ###
    // find concated words
    preg_match_all('/[^\s\.\,\?\!]*\"([^]]+)\"/', $query, $matches);
    foreach ($matches[0] as $match) {
      if (!empty($match)) {
        $words[] = $match;
      }
    }

    // find single words
    foreach (preg_split('/[^\s\.\,\?\!]*\"([^]]+)\"/', $query) as $query_part) {
      foreach (preg_split('/[\s\.\?\!]+/', trim($query_part)) as $word) {
        $word = trim($word);
        if (!empty($word)) {
          $words[] = $word;
        }
      }
    }
    foreach ($words as $wid => $word) {
      if (preg_match('/^([a-z0-9\_]+)\:\"+(.+)\"+$/Ui', $word, $filter_matches)) {
        unset($words[$wid]);
        $this->setFilter($filter_matches[1], $filter_matches[2]);
      }
    }
    $query = implode(' ', $words);

    if (empty($query) || !is_string($query)) {
      return TRUE;
    }

    $query = htmlspecialchars($query, ENT_NOQUOTES, 'UTF-8');
    $query = str_replace("'", '&#039;', $query);

    if ($this->conditions['q'] == 'entity_type:lom') {
      $this->conditions['fq'][] = 'entity_type:lom';
    }
    $this->conditions['q'] = $query;

    // define query parser
    $this->conditions['defType'] = 'dismax';

    return TRUE;
  }

  /**
   * this will perform the search
   * and set the total value
   *
   * @param boolean $enable_order_by
   *   If set to true it sort by revision edit time if option is enabled. (optional, default = FALSE)
   *
   * @return array list of found documents or FALSE in error case
   */
  public function search($enable_order_by = FALSE) {
    $fq = array();
    if (isset($this->conditions['fq'])) {
      $fq = $this->conditions['fq'];
      unset($this->conditions['fq']);
    }
    $query = apachesolr_drupal_query('apachesolr', $this->conditions, '', '', $this->solr);
    foreach ($fq as $expression) {
      list($field, $values) = explode(':', $expression, 2);
      $query->addFilter($field, $values, FALSE);
    }

    if ($enable_order_by && variable_get('archibald_search_order_latest', 1)) {
      $query->setAvailableSort('ds_changed', 'desc');
      $query->setSolrsort('ds_changed', 'desc');
    }
    $this->response = $query->search();

    if ($this->response->response) {
      $this->total = $this->response->response->numFound;

      $docs = array();
      foreach ($this->response->response->docs as $doc) {
        $docs[] = $this->processDoc($doc);
      }

      return $docs;
    }

    return FALSE;
  }

  /**
   * get number of total ammount of documents found
   *
   * @return integer
   */
  public function getTotal() {
    return $this->total;
  }

  /**
   * process found document
   * convert it to output style from sql db
   *
   * @param object $doc
   *
   * @return object
   */
  protected function processDoc($doc) {
    global $language;

    $new_doc = new stdClass();

    $new_doc->title = $doc->label;
    $new_doc->description = $doc->teaser;
    // dont have that here
    $new_doc->preview_image = '';
    $new_doc->preview_image_fid = $doc->hs_preview_image;
    $new_doc->lom_id = $doc->ss_entity_id;
    // we only index final
    $new_doc->status = 'final';
    // dont have that here
    $new_doc->version = '';
    // dont have that here
    $new_doc->default_language = '';
    // dont have that here
    $new_doc->publication_log = new stdClass();
    $new_doc->company_logo = @$doc->hs_company_logo;
    return $new_doc;
  }

  /**
   * set facet filter condition
   *
   * @param string $key    name of facet field (defined by getFacetOptions() ) to look up in
   * @param string $value  if empty it will remove the filter
   *                       if a array is given, then it all values will or`ed
   *
   * @example
   *  setFacetFilter('learning_resourcetype', array(pedagogical, exploration, demonstration));
   */
  public function setFacetFilter($key, $value) {
    if (empty($this->optionsFacetMapping[$key])) {
      return FALSE;
    }

    $this->setFilter($this->optionsFacetMapping[$key], $value, 'OR');
    $this->choosenFacetOptions[$key] = $value;
  }

  /**
   * get a list of possible filter options
   */
  public function getFacetOptions() {
    $options = array();

    $opts = array('educa_school_levels', 'educa_school_subjects', 'learning_resourcetype');

    foreach ($opts as $opt) {
      $cur_options = $this->getTaxonomyOptions($opt);
      $facet_field = $this->optionsFacetMapping[$opt];

      // set count option from solr facet options
      if (!empty($this->response->facet_counts->facet_fields->$facet_field)) {
        foreach ($this->response->facet_counts->facet_fields->$facet_field as $key => $count) {
          if (isset($cur_options[$key])) {
            $cur_options[$key]->count = $count;
          }
        }
      }

      // set activ options by the checked options in form
      if (!empty($this->choosenFacetOptions[$opt])) {
        foreach ($this->choosenFacetOptions[$opt] as $key) {
          if (isset($cur_options[$key])) {
            $cur_options[$key]->activ = TRUE;
          }
        }
      }

      $options[$opt] = $this->buildTaxonomyOptionsTree($cur_options);
    }

    return $options;
  }

  /**
   * get by taxonomy vocabulary machine_name a flat select options list
   * @staticvar string $vocabularies
   *
   * @param string $vocab
   *
   * @return array
   */
  private function getTaxonomyOptions($vocab) {
    $options = array();

    if ($vocabulary = taxonomy_vocabulary_machine_name_load($vocab)) {
      if ($terms = taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE)) {
        $key_list = array();
        foreach ($terms as $term) {
          $taxon_key = @$term->field_taxon_key['und'][0]['value'];
          if (@$term->field_taxon_deprecated['und'][0]['value']) {
            continue;
          }

          $options[$taxon_key] = (object)array(
            'tid' => $term->tid,
            'key' => $taxon_key,
            'name' => i18n_taxonomy_term_name($term),
            'parent' => $term->parents[0],
            'count' => 0,
            'activ' => FALSE,
          );
        }
      }
    }
    return $options;
  }

  /**
   * build up recursiv array from flat list with parent parameter
   *
   * @param array $options
   * @param integer $parent
   *
   * @return array recursiv
   */
  private function buildTaxonomyOptionsTree($options, $parent = 0) {
    $tree = array();
    foreach ($options as $opt) {
      if (isset($opt->parent) && $opt->parent == $parent) {
        $opt->vals = $this->buildTaxonomyOptionsTree($options, $opt->tid);
        unset($opt->parent, $opt->tid);
        $tree[] = $opt;
      }
    }

    return $tree;
  }

  /**
   * activate the faceting system
   * and enable faceting files
   */
  public function enableFacets() {
    $this->conditions['facet'] = 'true';
    $this->conditions['facet.mincount'] = '1';

    $this->conditions['facet.field'][] = 'sm_learning_resourcetype';
    $this->conditions['facet.field'][] = 'sm_education_level';
    $this->conditions['facet.field'][] = 'sm_discipline';

    $this->optionsFacetMapping['learning_resourcetype'] = 'sm_learning_resourcetype';
    $this->optionsFacetMapping['educa_school_levels'] = 'sm_education_level';
    $this->optionsFacetMapping['educa_school_subjects'] = 'sm_discipline';
  }
}

