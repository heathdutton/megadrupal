<?php

/**
 * @file
 * class for searching lom within a sql database
 */

/**
 * SQL based lom ressource search handler
 *
 * @author Heiko Henning <h.henning@educa.ch>
 */
class ArchibaldLomSQLSearch {

  /**
   * sql joins
   * @var array
   */
  protected $joins = array();

  /**
   * sql where parts
   * @var array
   */
  protected $where = array();

  /**
   * sql params
   * @var array
   */
  protected $params = array();

  /**
   * search beginn offset
   * @var integer
   */
  protected $offest = 0;

  /**
   * search end limit
   * @var integer
   */
  protected $limit = 25;

  /**
   * total count of all records
   * @var integer
   */
  protected $totalRecordCount = 0;

  /**
   * FALSE = show only activ items
   * TRUE = show only deleted items
   * @var bollean
   */
  protected $showDeleted = FALSE;

  /**
   * set this option to 1 to display delted ressoruces also
   *
   * @param boolean $show_deleted
   *   FALSE = show only activ items
   *   TRUE = show only deleted items
   */
  public function showDeleted($show_deleted = TRUE) {
    $this->showDeleted = $show_deleted;
  }

  /**
   * search string
   * will be exploded by space and or`d
   * looks up all langstring fields
   *
   * @param string $query
   * @param string $language
   *   default:any
   *   only search in spezial language
   */
  public function setQuery($query, $language = 'any') {
    $possible_filter = array(
      'education_context',
      'intendet_enduserrole',
      'learning_resourcetype',
      'difficulty',
      'keywords',
      'coverage',
      'author',
      'editor',
      'publisher',
    );

    $query = trim($query);

    $language = drupal_strtolower($language);

    $use_query_words = FALSE;
    $filters = array();
    $langstrings = array();

    // cut query into words
    $words = array();

    // find concated words
    preg_match_all('/[^\s]*\"([^]]+)\"/', $query, $matches);
    foreach ($matches[0] as $match) {
      if (!empty($match)) {
        $words[] = $match;
      }
    }
    // find single words
    foreach (preg_split('/[^\s]*\"([^]]+)\"/', $query) as $query_part) {
      foreach (preg_split('/[\s]+/', trim($query_part)) as $word) {
        $word = trim($word);
        if (!empty($word)) {
          $words[] = $word;
        }
      }
    }

    foreach ($words as $word) {
      if (!empty($word)) {
        // match for filter
        if (preg_match('/^(' . implode('|', $possible_filter) . ')\:(.+)/i', $word, $matches)) {
          $filters[drupal_strtolower($matches[1])][] = trim($matches[2], '"\' ');
          continue;
        }

        $word = trim($word, '.,?!;:_-');

        $use_query_words = TRUE;
        $sh = db_select('archibald_langstring_terms', 'lt')
          ->fields('lt', array('langstring_terms_id'))
          ->condition('term', '%' . $word . '%', 'LIKE');
        if (!empty($language) && $language != 'any') {
          $sh->condition('language', $language, '=');
        }
        $result = $sh->execute();
        while ($row = $result->fetchAssoc()) {
          $langstrings[] = $row['langstring_terms_id'];
        }
      }
    }

    // process filters
    foreach ($filters as $filter => $filter_values) {
      $filter_methode = preg_replace_callback('/_([a-z]{1})/i', function($match) {
        return drupal_strtoupper($match[1]);
      }, $filter);

      $filter_methode = drupal_ucfirst($filter_methode);
      $filter_methode = 'setFilter' . $filter_methode;

      $this->$filter_methode($filter_values);
    }

    // if we found anny matching langstrings, match them agains lom object
    if (!empty($langstrings)) {
      // genrate IN () query
      $in_query           = array();
      $prefix             = ':qry_langs_';
      $where['qry_langs'] = array();
      foreach ($langstrings as $i => $langstring) {
        $this->params[$prefix . $i] = $langstring;
        $in_query[] = $prefix . $i;
      }
      $in_query = 'IN (' . implode(',', $in_query) . ')';

      // add search filters
      $this->joins['archibald_general'] = TRUE;
      $this->where['qry_langs'][] = 'g.title ' . $in_query;
      $this->where['qry_langs'][] = 'g.description ' . $in_query;

      $this->joins['archibald_general_coverage'] = TRUE;
      $this->where['qry_langs'][] = 'gc.coverage ' . $in_query;

      $this->joins['archibald_general_keywords'] = TRUE;
      $this->where['qry_langs'][] = 'gk.keyword ' . $in_query;

      $this->joins['archibald_education'] = TRUE;
      $this->where['qry_langs'][] = 'e.description ' . $in_query;

      return TRUE;
    }
    elseif ($use_query_words == TRUE) {
      // now we should find nothing
      $this->where['qry_langs'][] = '1=0';

      return FALSE;
    }
  }

  /**
   * at filter for lom lifecyle.status
   *
   * @param string $status
   *   The status tring to filter for
   */
  public function setStatusFilter($status) {
    $this->joins['archibald_lifecycle'] = TRUE;
    $this->where['flt_status'][] = 'lc.status=:lc_status';
    $this->params[':lc_status'] = $status;
  }

  /**
   * Filter for LOM objects which need to be republished
   */
  public function setRepublicationRequiredFilter() {
    $this->joins['archibald_lom_stats'] = TRUE;
    $this->where['flt_republication_required'][] = 'ls.republication_required=:lc_republication_required';
    $this->params[':lc_republication_required'] = 1;
  }


  /**
   * at filter for lom object responsible/owner
   *
   * @param integer $uid
   *   the userId of the resposible editor to filter for
   */
  public function setResponsibleFilter($uid) {
    $this->joins['archibald_lom_stats'] = TRUE;
    $this->where['flt_responsible_uid'][] = 'ls.responsible_uid=:responsible_uid';
    $this->params[':responsible_uid'] = $uid;
  }

  /**
   * at filter for lom object local published
   *
   * @param int $local_published
   *   set to TRUE to get published resource
   *   set to FALSE to not published
   */
  public function setLocalPublishedFilter($local_published) {
    $this->joins['archibald_lom_stats'] = TRUE;
    if ($local_published == TRUE) {
      $this->where['flt_local_published'][0] = "ls.local_published!='' AND ls.local_published != '0'";
    }
    else {
      $this->where['flt_local_published'][0] = "lc.status='final' AND (ls.local_published = '' OR  ls.local_published = '0' OR ls.local_published!=l.version)";
    }
  }

  /**
   * get lom object in local published version
   */
  public function getLocalPublishedVersion() {
    $this->setLocalPublishedFilter(TRUE);
    $this->where['l_getLocalPublishedVersion'][0] = 'ls.local_published=l.version';
  }

  /**
   * at filter for lom object publishing status
   *
   * @param boolean $only_national
   *   If set to true only national published descriptions will be shown.
   */
  public function addWasPublishedFilter($only_national = FALSE) {
    $this->joins['archibald_lom_stats'] = TRUE;
    $filter = '(ls.publication_progress=100 AND publication_version IS NOT NULL)';
    if (!$only_national) {
     $filter .= ' OR (ls.local_published!=\'\' AND ls.local_published != \'0\')';
    }
    $this->where['flt_publication_progress'][] = $filter;
  }

  /**
   * at filter for lom object with could be published
   */
  public function addCanPublishedFilter() {
    $this->where['flt_publication_progress'][] = '(lc.status=\'final\' AND (' . 'ls.publication_progress IS NULL OR ' . 'ls.publication_progress!=100)) OR publication_version IS NULL';

    $this->joins['archibald_lom_stats'] = TRUE;
    $this->where['flt_publication_progress'][] = '(ls.publication_progress IS NOT NULL AND ' . 'ls.publication_progress!=100) OR publication_version IS NULL';
  }


  /**
   * Filter for the language which is defined in the archibald_meta_metadata table.
   *
   * @param string $language the language code (ISO-639: de, fr, it, en, ...)
   */
  public function setFilterLanguage($language) {
    $this->joins['archibald_meta'] = TRUE;
    $name = ':flt_language';
    $this->params[$name] = $language;
    $this->where['flt_language'][] = 'm.language = ' . $name;
  }


  /**
   * add filter for lifecycle contributor author.
   *
   * @param array $values
   *   list of values to filter for
   *
   * @return boolean
   *   status, error or success
   */
  public function setFilterAuthor($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_contributors_author'] = TRUE;
    $prefix = ':flt_contributor_author_';
    $this->where['flt_contributor_author'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_contributor_author'][] = "(c.firstname||' '||c.lastname) = " . $prefix . $i;
      $this->where['flt_contributor_author'][] = "(c.firstname||' '||c.lastname||' '||c.organisation) = " . $prefix . $i;
      $this->where['flt_contributor_author'][] = "c.organisation = " . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for lifecycle contributor editor.
   *
   * @param array $values
   *   list of values to filter for
   *
   * @return boolean
   *   status, error or success
   */
  public function setFilterEditor($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_contributors_editor'] = TRUE;

    $prefix = ':flt_contributor_editor_';
    $this->where['flt_contributor_editor'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_contributor_editor'][] = "(c.firstname||' '||c.lastname) = " . $prefix . $i;
      $this->where['flt_contributor_editor'][] = "(c.firstname||' '||c.lastname||' '||c.organisation) = " . $prefix . $i;
      $this->where['flt_contributor_editor'][] = "c.organisation = " . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for context
   *
   * @param array $values
   *   list of values to filter for
   *
   * @return boolean
   *   status, error or success
   */
  public function setFilterEducationContext($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_education'] = TRUE;
    $this->joins['archibald_education_context'] = TRUE;

    $prefix = ':flt_context_';
    $this->where['flt_context'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_context'][] = "ec.context_id LIKE " . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for difficulty
   *
   * @param array $values
   *   list of values to filter for
   *
   * @return boolean
   *   status, error or success
   */
  public function setFilterDifficulty($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_education'] = TRUE;

    $prefix = ':flt_difficult_';
    $this->where['flt_difficult'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_difficult'][] = "e.difficult LIKE " . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for intendedEndUserRole
   *
   * @param array $values
   *   list of values to filter for
   *
   * @return boolean
   *   status, error or success
   */
  public function setFilterIntendetEnduserrole($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_education'] = TRUE;
    $this->joins['archibald_education_intendedenduserrole'] = TRUE;

    $prefix = ':flt_intendedenduserrole_';
    $this->where['flt_intendedenduserrole'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_intendedenduserrole'][] = 'ei.intendedEndUserRole_id LIKE ' . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for learningResourceType
   *
   * @param array $values
   *
   * @return boolean status
   */
  public function setFilterLearningResourcetype($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_education'] = TRUE;
    $this->joins['archibald_education_learningresourcetype'] = TRUE;

    $prefix = ':flt_learningresourcetype_';
    $this->where['flt_learningresourcetype'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_learningresourcetype'][] = 'elr.learningResourceType_id LIKE ' . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for keywords
   *
   * @param array $values
   *
   * @return boolean status
   */
  public function setFilterKeywords($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_general'] = TRUE;
    $this->joins['archibald_general_keywords'] = TRUE;

    $prefix = ':flt_keywords_';
    $this->where['flt_keywords'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_keywords'][] = 'gkls.term LIKE ' . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * add filter for coverage
   *
   * @param array $values
   *
   * @return boolean status
   */
  public function setFilterCoverage($values) {
    if (empty($values)) {
      return FALSE;
    }

    if (!is_array($values)) {
      $values = array($values);
    }

    $this->joins['archibald_general'] = TRUE;
    $this->joins['archibald_general_coverage'] = TRUE;

    $prefix = ':flt_coverage_';
    $this->where['flt_coverage'] = array();
    foreach ($values as $i => $value) {
      $this->params[$prefix . $i] = $value;
      $this->where['flt_coverage'][] = 'gcls.term LIKE ' . $prefix . $i;
    }

    return TRUE;
  }

  /**
   * execute the search by given parameters
   *
   * @param boolean $enable_order_by
   *   If set to true it sort by revision edit time if option is enabled. (optional, default = FALSE)
   *
   * @return array
   *  list of result objects (lom_id, version, title)
   */
  public function search($enable_order_by = FALSE) {
    $fields = array(
      'version' => 'l.version',
      'title' => 'g.title',
      'description' => 'g.description',
      'default_language' => 'm.language',
      'status' => 'lc.status',
      'preview_image' => 't.preview_image',
      'publication_version' => 'ls.publication_version',
      'publication_progress' => 'ls.publication_progress',
      'publication_log' => 'ls.publication_log',
      'responsible_uid' => 'ls.responsible_uid',
      'local_published' => 'ls.local_published',
      'republication_required' => 'ls.republication_required',
      'content_partner_id' => 'ls.content_partner_id',
      'user_name' => 'u.name',
    );

    if (db_driver() == 'pgsql') {
      $sql_prefix = 'SELECT l.lom_id';
      foreach ($fields as $ident => $selector) {
        $sql_prefix .= ', FIRST(' . $selector . ') AS ' . $ident;
      }
    }
    else {
      $sql_prefix = 'SELECT l.lom_id';
      foreach ($fields as $ident => $selector) {
        $sql_prefix .= ', ' . $selector . ' as ' . $ident;
      }
    }
    $sql_prefix .= ' ';

    // Uncomment for debug:
    // $pp = array();foreach($this->params AS $k => $vv) {$pp[$k] = "'" . $vv . "'";}echo strtr(strtr($sql_prefix . "\n" . $this->generateSql() . ' GROUP BY l.lom_id ', $pp), array('{' => '', '}' => '')) . "\n\n";

    $order_by = "";
    if ($enable_order_by && variable_get('archibald_search_order_latest', 1)) {
      $order_by = " ORDER BY l.save_time DESC";
    }

    $q_res = db_query_range($sql_prefix . $this->generateSql() . ' GROUP BY l.lom_id ' . $order_by, $this->offset, $this->limit, $this->params);
    $result = array();
    while ($row = $q_res->fetchObject()) {
      if (!empty($row->title)) {
        // get langstring by id
        $row->title = ArchibaldLomSaveHandler::requestLangStringObj($row->title);
      }

      if (!empty($row->description)) {
        // get langstring by id
        $row->description = ArchibaldLomSaveHandler::requestLangStringObj($row->description);
      }

      if (empty($row->publication_log)) {
        $row->publication_log = new stdClass();
      }
      else {
        $row->publication_log = unserialize($row->publication_log);
      }

      $result[] = $row;
    }

    return $result;
  }

  /**
   * generate the sql starting with FROM ...
   *
   * @return string
   */
  protected function generateSql() {

    /**
     * Table alias definition
     *   l    > archibald_lom
     *   la   > archibald_lom  (for last version)
     *   u    > users
     *   g    > archibald_general
     *   gk   > archibald_general_keywords
     *   gkls > archibald_langstring_terms
     *   gc   > archibald_general_coverage
     *   gcls > archibald_langstring_terms
     *   lc   > archibald_lifecycle
     *   m    > archibald_meta_metadata
     *   t    > archibald_technical
     *   ls   > archibald_lom_stats
     *   e    > archibald_education
     *   ec   > archibald_education_context
     *   ei   > archibald_education_intendedenduserrole
     *   elr  > archibald_education_learningresourcetype
     */
    if ($this->showDeleted == TRUE) {
      $this->where['l_deleted'] = 'l.deleted IS NOT NULL';
    }
    else {
      $this->where['l_deleted'] = 'l.deleted IS NULL';
    }

    if (empty($this->where['l_getLocalPublishedVersion'])) {
      $this->where['l_get_onyl_newest'] = 'l.save_time = (SELECT la.save_time FROM {archibald_lom} la WHERE l.lom_id=la.lom_id ORDER BY la.save_time DESC LIMIT 1)';
    }

    if (!isset($this->joins['archibald_general']) || $this->joins['archibald_general'] === TRUE) {
      $this->joins['archibald_general'] = '{archibald_general} g ON (g.general_id=l.general_id)';
    }

    if (isset($this->joins['archibald_general_keywords']) && $this->joins['archibald_general_keywords'] === TRUE) {
      $this->joins['archibald_general_keywords'] = '{archibald_general_keywords} gk ON (gk.general_id=l.general_id)';
      $this->joins['archibald_general_keywords_ls'] = '{archibald_langstring_terms} gkls ON ' . '(gk.keyword=gkls.langstring_terms_id)';
    }

    if (isset($this->joins['archibald_general_coverage']) && $this->joins['archibald_general_coverage'] === TRUE) {
      $this->joins['archibald_general_coverage'] = '{archibald_general_coverage} gc ON (gc.general_id=l.general_id)';
      $this->joins['archibald_general_coverage_ls'] = '{archibald_langstring_terms} gcls ON ' . '(gc.coverage=gcls.langstring_terms_id)';
    }

    if (!isset($this->joins['archibald_lifecycle']) || $this->joins['archibald_lifecycle'] === TRUE) {
      $this->joins['archibald_lifecycle'] = '{archibald_lifecycle} lc ON (lc.lifecycle_id=l.lifecycle_id)';
    }

    if (!isset($this->joins['archibald_meta']) || $this->joins['archibald_meta'] === TRUE) {
      $this->joins['archibald_meta'] = '{archibald_meta_metadata} m ON (m.meta_metadata_id=l.meta_metadata_id)';
    }

    if (!isset($this->joins['archibald_technical']) || $this->joins['archibald_technical'] === TRUE) {
      $this->joins['archibald_technical'] = '{archibald_technical} t ON (t.technical_id=l.technical_id)';
    }

    if (!isset($this->joins['archibald_lom_stats']) || $this->joins['archibald_lom_stats'] === TRUE) {
      $this->joins['archibald_lom_stats'] = '{archibald_lom_stats} ls ON (ls.lom_id=l.lom_id)';
    }

    if (isset($this->joins['archibald_education']) && $this->joins['archibald_education'] === TRUE) {
      $this->joins['archibald_education'] = '{archibald_education} e ON (e.education_id=l.education_id)';
    }

    if (isset($this->joins['archibald_education_context']) && $this->joins['archibald_education_context'] === TRUE) {
      $this->joins['archibald_education_context'] = '{archibald_education_context} ec ON (e.education_id=ec.education_id)';
    }

    if (isset($this->joins['archibald_education_intendedenduserrole']) && $this->joins['archibald_education_intendedenduserrole'] === TRUE) {
      $this->joins['archibald_education_intendedenduserrole'] = '{archibald_education_intendedenduserrole} ei ON ' . '(e.education_id=ei.education_id)';
    }

    if (isset($this->joins['archibald_education_learningresourcetype']) && $this->joins['archibald_education_learningresourcetype'] === TRUE) {
      $this->joins['archibald_education_learningresourcetype'] = '{archibald_education_learningresourcetype} elr ON ' . '(e.education_id=elr.education_id)';
    }

    $author_search = isset($this->joins['archibald_contributors_author']) && $this->joins['archibald_contributors_author'] === TRUE;
    $editor_search = isset($this->joins['archibald_contributors_editor']) && $this->joins['archibald_contributors_editor'] === TRUE;

    $lc_contributor_s_c = 0;
    if ($author_search || $editor_search) {
      $this->joins['zz_lifecycle_contributor_search' . $lc_contributor_s_c++] = '{archibald_lifecycle_contributes} lcc ON (lcc.lifecycle_id=l.lifecycle_id)';
    }

    if ($author_search) {
      unset($this->joins['archibald_contributors_author']);
      $this->joins['zz_lifecycle_contributor_search' . $lc_contributor_s_c++] = '{archibald_contribute} contribute ON (contribute.role = \'author\' AND contribute.contribute_id=lcc.contribute_id)';
    }

    if ($editor_search) {
      unset($this->joins['archibald_contributors_editor']);
      $this->joins['zz_lifecycle_contributor_search' . $lc_contributor_s_c++] = '{archibald_contribute} contribute ON (contribute.role = \'editor\' AND contribute.contribute_id=lcc.contribute_id)';
    }

    if ($author_search || $editor_search) {
      $this->joins['zz_lifecycle_contributor_search' . $lc_contributor_s_c++] = '{archibald_contribute_entity} c_entity ON (c_entity.contribute_id=contribute.contribute_id)';
      $this->joins['zz_lifecycle_contributor_search' . $lc_contributor_s_c++] = '{archibald_contributors} c ON (c.contributor_id=c_entity.entity_id)';
    }

    if (!isset($this->joins['users']) || $this->joins['users'] === TRUE) {
      $this->joins['users'] = '{users} u ON (ls.responsible_uid=u.uid)';
    }

    $sql = ' FROM {archibald_lom} l ';


    foreach ($this->joins as $jkey => $join) {
      if (in_array($jkey, array('archibald_general', 'archibald_meta'))) {
        $join_type = 'INNER';
      }
      else {
        $join_type = 'LEFT';
      }

      $sql .= $join_type . ' JOIN ' . $join . ' ' . "\n";
    }

    if (!empty($this->where)) {
      foreach ($this->where as $i => $where) {
        if (is_array($where)) {
          $this->where[$i] = '(' . implode(' OR ', $where) . ')';
        }
      }

      $sql .= 'WHERE ' . implode(' AND ' . "\n", $this->where) . ' ';
    }
    return $sql;
  }

  /**
   * return the ammount of all records with the last search filters affected
   *
   * @return integer
   */
  public function getTotalRecordCount() {
    $this->totalRecordCount = db_query('SELECT COUNT(DISTINCT l.lom_id) ' . $this->generateSql(), $this->params)
      ->fetchColumn(0);
    return $this->totalRecordCount;
  }

  /**
   * set sql limit
   *
   * @param integer $limit
   */
  public function setLimit($limit) {
    $this->limit = (INT) $limit;
  }

  /**
   * set sql limit offset
   *
   * @param integer $offset
   */
  public function setOffset($offset) {
    $this->offset = (INT) $offset;;
  }

  /**
   * get lom object title by lom id
   *
   * @param string $lom_id
   *
   * @return string
   */
  public function getLomTitle($lom_id) {
    $this->where['lom_id'] = 'l.lom_id = :lom_id';
    $this->params[':lom_id'] = $lom_id;

    if (db_driver() == 'pgsql') {
      $sql_prefix = 'SELECT FIRST(g.title) AS title, ' . 'FIRST(m.language) AS default_language ';
    }
    else {
      $sql_prefix = 'SELECT g.title, m.language AS default_language ';
    }

    $row = db_query($sql_prefix . $this->generateSql() . ' GROUP BY l.lom_id ', $this->params )
      ->fetchObject();
    if (empty($row)) {
      return NULL;
    }

    $archibald_lom = new ArchibaldLom();
    return $archibald_lom->determinTitle(ArchibaldLomSaveHandler::requestLangStringObj($row->title), $row->default_language);
  }
}

