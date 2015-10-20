<?php

/**
 * @file
 * collection of classes that provide ArchibaldLom
 * Learning Object Metadata
 * For further informations
 * see ArchibaldLom-CH Dokumentation
 */

/**
 * Definitions:
 *
 * char_string = <string>this is the text<string>
 * lang_string = <langstring lang="de-CH">this is the text</langstring>
 *               or Maybe ArchibaldLomDataLangString
 *
 * datetime = YYYY[-MM[-DD[Thh[:mm[:ss[.s[TZD]]]]]]] where:
 *   - YYYY: The 4-digit year (â‰¥ 0001)
 *   - MM: The 2-digit month (01 through 12 where 01=January, etc.)
 *   - DD: The 2-digit day of month (01 through 31,
 *         depending on the values of month and year)
 *   - hh: Two digits of hour (00 through 23) (am and pm are NOT allowed)
 *   - mm: Two digits of minute (00 through 59)
 *   - ss: Two digits of second (00 through 59)
 *   - s: One or more digits representing a decimal fraction of a second
 *   - TZD: The time zone designator ("Z" for coordinated
 *          universal time [UTC] or +hh,
 *   - hh, +hh:mm, or -hh:mm, where hh is two digits of hour
 *         and mm is two digits of minute)
 * duration = P[yY][mM][dD][T[hH][nM][s[.s]S]] where:
 *   - y: The number of years (integer, â‰¥ 0)
 *   - m: The number of months (integer, â‰¥ 0, not restricted,
 *        e.g., > 12 is acceptable)
 *   - d: The number of days (integer, â‰¥ 0, not restricted,
 *        e.g., > 31 is acceptable)
 *   - h: The number of hours (integer, â‰¥ 0, not restricted,
 *        e.g., > 23 is acceptable)
 *   - n: The number of minutes (integer, â‰¥ 0, not restricted,
 *        e.g., > 59 is acceptable)
 *   - s: The number of seconds or fraction of seconds
 *        (integer, â‰¥ 0, not restricted, e.g.,
 *   - > 59 is acceptable)
 *  P must be present if not all values are zeros,
 *  If the "time" is complety zero, T should not be present
 *
 * general_langauge = lang key string (en, fr, ch) or "none"
 * contribute->entity = String valid vCard IETF RFC 2426:1998
 * format = all MIME-Types (RFC 2048) or 'non-digital'
 */
require_once 'DbStruct.class.php';

/**
 * The ArchibaldLom Class
 * Learning Object Metadata
 */
class ArchibaldLom extends ArchibaldAbstractDataManager {

  /**
   * Possible general STRUCTURE values for vocabular
   * How the document is structured
   */
  const GENERAL_STRUCTURE_ATOMIC = 'atomic';
  const GENERAL_STRUCTURE_COLLECTION = 'collection';
  const GENERAL_STRUCTURE_NETWORKED = 'networked';
  const GENERAL_STRUCTURE_HIERARCHICAL = 'hierarchical';
  const GENERAL_STRUCTURE_LINEAR = 'linear';

  /**
   * Possible general AGGREGATIONLEVEL values for vocabular
   * what this resource is
   */
  //raw media or fragments
  const GENERAL_AGGREGATIONLEVEL_1 = '1';

  //a lesson
  const GENERAL_AGGREGATIONLEVEL_2 = '2';

  //a course
  const GENERAL_AGGREGATIONLEVEL_3 = '3';

  //learnign objects or can contain other lvl4 resources
  const GENERAL_AGGREGATIONLEVEL_4 = '4';

  /**
   *
   * @var string
   */
  public $lomId = '';

  /**
   *
   * @var string
   */
  public $version = '';

  /**
   * ArchibaldLomDataGeneral required, max 1
   * @var ArchibaldLomDataGeneral
   */
  public $general = NULL;

  /**
   * ArchibaldLomDataLifeCycle optional, max 1
   * @var ArchibaldLomDataLifeCycle
   */
  public $lifeCycle = NULL;

  /**
   * ArchibaldLomDataMetaMetadata optional, max 10
   * @var ArchibaldLomDataMetaMetadata
   */
  public $metaMetadata = NULL;

  /**
   * ArchibaldLomDataTechnical optional, max 1
   * @var ArchibaldLomDataTechnical
   */
  public $technical = NULL;

  /**
   * ArchibaldLomDataEducation optional, max 1
   * @var ArchibaldLomDataEducation
   */
  public $education = NULL;

  /**
   * ArchibaldLomDataRights optional, max 1
   * @var ArchibaldLomDataRights
   */
  public $rights = NULL;

  /**
   * ArchibaldLomDataRelation optional, max 100
   * @var array
   */
  public $relation = array();

  /**
   * ArchibaldLomDataAnnotation optional, max 30
   * @var array
   */
  public $annotation = array();

  /**
   * ArchibaldLomDataClassification optional, max 40
   * @var array
   */
  public $classification = array();

  /**
   * ArchibaldLomDataCurriculum optional, max 40
   * @var array
   */
  public $curriculum = array();

  public function getLomId() {
    return $this->lomId;
  }

  public function setLomId($lom_id) {
    $this->lomId = $lom_id;
  }

  public function getVersion() {
    return $this->version;
  }

  public function setVersion($version) {
    $this->version = $version;
  }


  /**
   *
   * @var int
   */
  // The variables and functions below are used when adding / editing a description
  /**
   *
   * @return ArchibaldLomDataGeneral
   */
  public function getGeneral() {
    if (!($this->general instanceof ArchibaldLomDataGeneral)) {
      return new ArchibaldLomDataGeneral();
    }
    return $this->general;
  }

  public function setGeneral($general) {
    $this->general = $general;
  }

  /**
   *
   * @return ArchibaldLomDataLifeCycle
   */
  public function getLifeCycle() {
    if (!($this->lifeCycle instanceof ArchibaldLomDataLifeCycle)) {
      return new ArchibaldLomDataLifeCycle();
    }
    return $this->lifeCycle;
  }

  public function setLifeCycle($life_cycle) {
    $this->lifeCycle = $life_cycle;
  }

  /**
   *
   * @return ArchibaldLomDataMetaMetadata
   */
  public function getMetaMetadata() {
    if (!($this->metaMetadata instanceof ArchibaldLomDataMetaMetadata)) {
      return new ArchibaldLomDataMetaMetadata();
    }
    return $this->metaMetadata;
  }

  public function setMetaMetadata($meta_metadata) {
    $this->metaMetadata = $meta_metadata;
  }

  /**
   *
   * @return ArchibaldLomDataTechnical
   */
  public function getTechnical() {
    if (!($this->technical instanceof ArchibaldLomDataTechnical)) {
      return new ArchibaldLomDataTechnical();
    }
    return $this->technical;
  }

  public function setTechnical($technical) {
    $this->technical = $technical;
  }

  /**
   *
   * @return ArchibaldLomDataEducation
   */
  public function getEducation() {
    if (!($this->education instanceof ArchibaldLomDataEducation)) {
      return new ArchibaldLomDataEducation();
    }
    return $this->education;
  }

  public function setEducation($education) {
    $this->education = $education;
  }

  /**
   *
   * @return ArchibaldLomDataRights
   */
  public function getRights() {
    if (!($this->rights instanceof ArchibaldLomDataRights)) {
      return new ArchibaldLomDataRights();
    }
    return $this->rights;
  }

  public function setRights($rights) {
    $this->rights = $rights;
  }

  /*
    .########..########.##..........###....########.####..#######..##....##..######.
    .##.....##.##.......##.........##.##......##.....##..##.....##.###...##.##....##
    .##.....##.##.......##........##...##.....##.....##..##.....##.####..##.##......
    .########..######...##.......##.....##....##.....##..##.....##.##.##.##..######.
    .##...##...##.......##.......#########....##.....##..##.....##.##..####.......##
    .##....##..##.......##.......##.....##....##.....##..##.....##.##...###.##....##
    .##.....##.########.########.##.....##....##....####..#######..##....##..######.
  */
  /**
   *
   * @return Array with ArchibaldLomDataRelation objects
   */
  public function getRelation($id = NULL) {
    if ($id !== NULL && isset($this->relation[$id])) {
      return $this->relation[$id];
    }
    return $this->relation;
  }

  public function setRelation($relation, $id = NULL) {
    if ($id !== NULL) {
      $this->relation[$id] = $relation;
      return;
    }
    $this->relation = $relation;
  }

  public function addRelation($relation) {
    if (!is_array($this->relation)) {
      $this->relation = array();
    }
    if (is_array($relation)) {
      foreach ($relation as $item) {
        if (is_null($this->addRelation($relation))) {
          return NULL;
        }
      }
    }

    if ($relation instanceof ArchibaldLomDataRelation) {
      $this->relation[] = $relation;
    }
  }

  /**
   *
   * @return Array with ArchibaldLomDataAnnotation objects
   */
  public function getAnnotation($id = NULL) {
    if ($id !== NULL && isset($this->annotation[$id])) {
      return $this->annotation[$id];
    }
    return $this->annotation;
  }

  public function setAnnotation($annotation, $id = NULL) {
    if ($id !== NULL) {
      $this->annotation[$id] = $annotation;
      return;
    }
    $this->annotation = $annotation;
  }

  public function addAnnotation($annotation) {
    if (!is_array($this->annotation)) {
      $this->annotation = array();
    }
    if (is_array($annotation)) {
      foreach ($annotation as $item) {
        if (is_null($this->addAnnotation($annotation))) {
          return NULL;
        }
      }
    }
    $this->annotation[] = $annotation;
  }

  /**
   *
   * @return Array with ArchibaldLomDataClassification objects
   */
  public function getClassification($id = NULL) {
    if ($id !== NULL && isset($this->classification[$id])) {
      return $this->classification[$id];
    }
    return $this->classification;
  }

  public function setClassification($classification, $id = NULL) {
    if ($id !== NULL) {
      $this->classification[$id] = $classification;
      return;
    }
    $this->classification = $classification;
  }

  public function addClassification($classification) {
    if (!is_array($this->classification)) {
      $this->classification = array();
    }
    if (is_array($classification)) {
      foreach ($classification as $item) {
        if (is_null($this->addClassification($classification))) {
          return NULL;
        }
      }
    }
    $this->classification[] = $classification;
  }

  /**
   *
   * @return Array with ArchibaldLomDataCurriculum objects
   */
  public function getCurriculum($id = NULL) {
    if ($id !== NULL && isset($this->curriculum[$id])) {
      return $this->curriculum[$id];
    }
    return $this->curriculum;
  }

  public function setCurriculum($curriculum, $id = NULL) {
    if ($id !== NULL) {
      $this->curriculum[$id] = $curriculum;
      return;
    }
    $this->curriculum = $curriculum;
  }

  public function addCurriculum($curriculum) {
    if (!is_array($this->curriculum)) {
      $this->curriculum = array();
    }
    if (is_array($curriculum)) {
      foreach ($curriculum as $item) {
        if (is_null($this->addCurriculum($curriculum))) {
          return NULL;
        }
      }
    }

    $this->curriculum[] = $curriculum;
  }

  /**
   * determine title from a lom object
   * @global object $language
   *
   * @param ArchibaldLomDataLangString $title
   * @param string $default_language
   *
   * @return string
   */
  public static function determinTitle($title, $default_language = '') {
    global $language;

    // a lom_id was given, get title directly from db
    if (is_string($title)) {
      $version_subquery = 'SELECT la.version
        FROM archibald_lom la
        WHERE lom.lom_id=la.lom_id
        ORDER BY la.save_time DESC
        LIMIT 1';

      $sql = 'SELECT l.language, l.term
        FROM  {archibald_lom} AS lom
        INNER JOIN {archibald_general} g ' . 'ON (lom.general_id = g.general_id)
        INNER JOIN {archibald_langstring_terms} l ' . 'ON (g.title = l.langstring_terms_id)
        WHERE lom.lom_id = :lom_id
        AND lom.version = (' . $version_subquery . ') ';

      $params = array(':lom_id' => $title);
      $q_res  = db_query($sql, $params);
      $titles = array();
      while ($row = $q_res->fetchObject()) {
        $titles[$row->language] = $row->term;
      }

      if (!empty($titles[$language->language])) {
        return $titles[$language->language];
      }
      elseif (!empty($default_language) && !empty($titles[$default_language])) {
        return $titles[$default_language];
      }
      else {
        return reset($titles);
      }
    }

    // if only the first parameter is given as lom object
    if ($title instanceof ArchibaldLom) {
      $general = $title->getGeneral();
      $meta    = $title->getMetaMetadata();
      $title   = '';

      if ($general instanceof ArchibaldLomDataGeneral) {
        $title = $general->getTitle();
      }

      if ($meta instanceof ArchibaldLomDataMetaMetadata) {
        $default_language = $meta->getLanguage();
      }
    }

    // determine title
    if ($title instanceof ArchibaldLomDataLangString) {
      $ret_title = $title->getString($language->language);
      if (empty($ret_title) && !empty($default_language)) {
        $ret_title = $title->getString($default_language);
      }

      if (empty($ret_title)) {
        $tmp = $title->getStrings();
        $ret_title = reset($tmp);
      }

      return $ret_title;
    }
    else {
      return (isset($res_row->title)) ? $res_row->title : NULL;
    }
  }

  /**
   * extract a string from a ArchibaldLomDataLangString object
   * @global object $language
   *
   * @param ArchibaldLomDataLangString $langstring
   * @param string $default_language
   * @param boolaan $no_fallback_language
   *   dont return otherwise language if no match
   *
   * @return string
   */
  public static function getLangstringText($langstring, $default_language = '', $no_fallback_language = FALSE) {
    global $language;

    if ($langstring instanceof ArchibaldLomDataLangString) {
      $ret_title = $langstring->getString($language->language);
      if (empty($ret_title) && !empty($default_language)) {
        $ret_title = $langstring->getString($default_language);
      }

      if (empty($ret_title)) {
        if ($no_fallback_language == TRUE) {
          return '';
        }

        $tmp = $langstring->getStrings();
        $ret_title = reset($tmp);
      }

      return $ret_title;
    }
    else {
      return "";
    }
  }
}

/**
 * ArchibaldLomDataGeneral, required, max 1
 */
class ArchibaldLomDataGeneral extends ArchibaldAbstractDataManager {

  /**
   *
   * @var int
   */
  public $generalId = 0;

  /**
   * ArchibaldLomDataIdentifier optional, max 10
   * @var array
   */
  public $identifier = array();

  /**
   * ArchibaldLomDataLangString optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $title = NULL;

  /**
   * char_string, optional, max 10 @def: general_langauge
   * @var array
   */
  public $language = array();

  /**
   * ArchibaldLomDataLangString,  optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $description = NULL;

  /**
   * ArchibaldLomDataLangString,  optional, max 10
   * @var array
   */
  public $keyword = array();

  /**
   * ArchibaldLomDataLangString,  optional, max 10
   * @var array
   */
  public $coverage = array();

  /**
   * ArchibaldLomDataVocabulary,  optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $structure = NULL;

  /**
   * ArchibaldLomDataVocabulary,  optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $aggregationLevel = NULL;

  public function __construct() {
    parent::__construct();
    //@Schema ArchibaldLomDataGeneral
  }

  public function getIdentifier($id = NULL) {
    if ($id !== NULL && isset($this->identifier[$id])) {
      return $this->identifier[$id];
    }
    return $this->identifier;
  }

  public function setIdentifier($identifier, $id = NULL) {
    if ($id !== NULL) {
      $this->identifier[$id] = $identifier;
      return;
    }
    $this->identifier = $identifier;
  }

  public function addIdentifier($identifier) {
    if (!is_array($this->identifier)) {
      $this->identifier = array();
    }
    if (is_array($identifier)) {
      foreach ($identifier as $item) {
        if (is_null($this->addIdentifier($identifier))) {
          return NULL;
        }
      }
    }

    if ($identifier instanceof ArchibaldLomDataIdentifier) {
      $key = $identifier->getId();
      $this->identifier[$key] = $identifier;
    }
  }

  /**
   * drop a identifer from this resource
   *
   * @param ArchibaldLomDataIdentifier $identifier
   *
   * @return boolean status
   */
  public function removeIdentifier($identifier) {
    if (!($identifier instanceof ArchibaldLomDataIdentifier)) {
      return FALSE;
    }

    $key = $identifier->getId();

    foreach ($this->identifier as $i => $cur_identifier) {
      $cur_key = $cur_identifier->getId();

      if ($key == $cur_key) {
        unset($this->identifier[$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  public function getGeneralId() {
    return $this->generalId;
  }

  public function getTitle() {
    return $this->title;
  }

  /**
   * Set the title
   *
   * @param ArchibaldLomDataLangString $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  public function getLanguage($id = NULL) {
    if ($id !== NULL && isset($this->language[$id])) {
      return $this->language[$id];
    }
    return $this->language;
  }

  public function setLanguage($language, $id = NULL) {
    if ($id !== NULL) {
      $this->language[$id] = $language;
      return;
    }
    $this->language = $language;
  }

  public function addLanguage($language) {
    if (!is_array($this->language)) {
      $this->language = array();
    }
    if (is_array($language)) {
      foreach ($language as $item) {
        if (is_null($this->addLanguage($item))) {
          return NULL;
        }
      }
    }
    $this->language[] = $language;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {

    $this->description = $description;
  }

  public function getKeyword($id = NULL) {
    if ($id !== NULL && isset($this->keyword[$id])) {
      return $this->keyword[$id];
    }
    return $this->keyword;
  }

  public function setKeyword($keyword, $id = NULL) {
    if ($id !== NULL) {
      $this->keyword[$id] = $keyword;
      return;
    }
    $this->keyword = $keyword;
  }

  public function addKeyword($keyword) {
    if (!is_array($this->keyword)) {
      $this->keyword = array();
    }
    if (is_array($keyword)) {
      foreach ($keyword as $item) {
        if (is_null($this->addKeyword($keyword))) {
          return NULL;
        }
      }
    }
    $this->keyword[] = $keyword;
  }

  /**
   * remove a keyword by ArchibaldLomDataLangString or integer array id
   *
   * @param ArchibaldLomDataLangString $keyword
   *
   * @return boolean status
   */
  public function removeKeyword($keyword) {
    if (!($keyword instanceof ArchibaldLomDataLangString)) {
      if (is_numeric($keyword) && isset($this->keyword[$keyword])) {
        unset($this->keyword[$keyword]);
        return TRUE;
      }
      return FALSE;
    }

    $key = $keyword->getId();

    foreach ($this->keyword as $i => $cur_keyword) {
      $cur_key = $cur_keyword->getId();

      if ($key == $cur_key) {
        unset($this->keyword[$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  public function getCoverage($id = NULL) {
    if ($id !== NULL && isset($this->coverage[$id])) {
      return $this->coverage[$id];
    }
    return $this->coverage;
  }

  public function setCoverage($coverage, $id = NULL) {
    if ($id !== NULL) {
      $this->coverage[$id] = $coverage;
      return;
    }
    $this->coverage = $coverage;
  }

  public function addCoverage($coverage) {
    if (!is_array($this->coverage)) {
      $this->coverage = array();
    }
    if (is_array($coverage)) {
      foreach ($coverage as $item) {
        if (is_null($this->addCoverage($coverage))) {
          return NULL;
        }
      }
    }
    $this->coverage[] = $coverage;
  }

  /**
   * remove a coverage by ArchibaldLomDataLangString or integer array id
   *
   * @param ArchibaldLomDataLangString $coverage
   *
   * @return boolean status
   */
  public function removeCoverage($coverage) {
    if (!($coverage instanceof ArchibaldLomDataLangString)) {
      if (is_numeric($coverage) && isset($this->coverage[$coverage])) {
        unset($this->coverage[$coverage]);
        return TRUE;
      }
      return FALSE;
    }

    $key = $coverage->getId();

    foreach ($this->coverage as $i => $cur_coverage) {
      $cur_key = $cur_coverage->getId();

      if ($key == $cur_key) {
        unset($this->coverage[$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Getter
   *
   * @return ArchibaldLomDataVocabulary
   */
  public function getStructure() {
    return $this->structure;
  }

  /**
   * Setter
   * Use ArchibaldLom::GENERAL_STRUCTUR_* for vocabular
   *
   * @param mixed $structure
   *  preferred ArchibaldLomDataVocabulary,
   *  if string provied, it creates ArchibaldLomDataVocabulary($structure)
   */
  public function setStructure($structure) {
    if (!is_object($structure)) {
      $structure = new ArchibaldLomDataVocabulary($structure);
    }
    $this->structure = $structure;
  }

  /**
   * Getter
   *
   * @return ArchibaldLomDataVocabulary
   */
  public function getAggregationLevel() {
    return $this->aggregationLevel;
  }

  /**
   * Setter
   * Use ArchibaldLom::GENERAL_AGGREGRATIONLEVEL_* for vocabular
   *
   * @param mixed $aggregation_level
   *  preferred ArchibaldLomDataVocabulary,
   *  if string provied, it creates ArchibaldLomDataVocabulary($aggregation_level)
   */
  public function setAggregationLevel($aggregation_level) {
    if (!is_object($aggregation_level)) {
      $aggregation_level = new ArchibaldLomDataVocabulary($aggregation_level);
    }
    $this->aggregationLevel = $aggregation_level;
  }
}

/**
 * ArchibaldLomDataLifeCycle
 */
class ArchibaldLomDataLifeCycle extends ArchibaldAbstractDataManager {

  /**
   *
   * @var int
   */
  public $lifecycleId = 0;

  /**
   * contribute roles (the person who changed something)
   */
  const CONTRIBUTE_ROLE_AUTHOR = "author";
  const CONTRIBUTE_ROLE_PUBLISHER = "publisher";
  const CONTRIBUTE_ROLE_UNKNOWN = "unknown";
  const CONTRIBUTE_ROLE_INITIATOR = "initiator";
  const CONTRIBUTE_ROLE_TERMINATOR = "terminator";
  const CONTRIBUTE_ROLE_VALIDATOR = "validator";
  const CONTRIBUTE_ROLE_EDITOR = "editor";
  const CONTRIBUTE_ROLE_GRAPHICAL_DESIGNER = "graphical designer";
  const CONTRIBUTE_ROLE_TECHNICAL_IMPLEMENTER = "technical implementer";
  const CONTRIBUTE_ROLE_CONTENT_PROVIDER = "content provider";
  const CONTRIBUTE_ROLE_TECHNICAL_VALIDATOR = "technical validator";
  const CONTRIBUTE_ROLE_EDUCATIONAL_VALIDATOR = "educational validator";
  const CONTRIBUTE_ROLE_SCRIPT_WRITER = "script writer";
  const CONTRIBUTE_ROLE_INSTRUCTION_DESIGNER = "instructional designer";
  const CONTRIBUTE_ROLE_SUBJECT_MATTER_EXPERT = "subject matter expert";

  /**
   * Possible lifeCycle STATUS values for vocabular
   * status of this document
   */
  const LIFECYCLE_STATUS_DRAFT = "draft";
  const LIFECYCLE_STATUS_FINALE = "final";
  const LIFECYCLE_STATUS_REVISED = "revised";
  const LIFECYCLE_STATUS_UNAVAILABLE = "unavailable";

  /**
   * ArchibaldLomDataLangString optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $version = NULL;

  /**
   * ArchibaldLomDataVocabulary optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $status = NULL;

  /**
   * ArchibaldLomDataContribute, optional, max 30 ordered
   * @var array
   */
  public $contribute = array();

  public function __construct() {
    parent::__construct();
    $this->version = new ArchibaldLomDataLangString();
  }

  public function getVersion() {
    if (!($this->version instanceof ArchibaldLomDataLangString)) {
      return new ArchibaldLomDataLangString();
    }
    return $this->version;
  }

  public function setVersion($version) {
    $this->version = $version;
  }

  public function getStatus() {
    return $this->status;
  }

  public function setStatus($status) {
    $this->status = $status;
  }

  public function getContribute($id = NULL) {
    if ($id !== NULL && isset($this->contribute[$id])) {
      return $this->contribute[$id];
    }
    return $this->contribute;
  }

  public function setContribute($contribute, $id = NULL) {
    if ($id !== NULL) {
      $this->contribute[$id] = $contribute;
      return;
    }
    $this->contribute = $contribute;
  }

  public function addContribute($contribute) {
    if (!is_array($this->contribute)) {
      $this->contribute = array();
    }
    if (is_array($contribute)) {
      foreach ($contribute as $item) {
        if (is_null($this->addContribute($contribute))) {
          return NULL;
        }
      }
    }

    if ($contribute instanceof ArchibaldLomDataContribute) {
      $this->contribute[] = $contribute;
      return;
    }
    $this->contribute[] = new ArchibaldLomDataContribute($contribute);
  }
}


/**
 * ArchibaldLomDataContribute
 */
class ArchibaldLomDataContribute extends ArchibaldAbstractDataManager {

  /**
   *
   * @var int
   */
  public $contributeId = NULL;

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $role = NULL;

  /**
   * char_string, optional, max 40 @def: contribute->entity ordered
   * @var array
   */
  public $entity = array();

  /**
   * ArchibaldLomDataDateTime, optional, max 1
   * @var ArchibaldLomDataDateTime
   */
  public $date = NULL;

  public function __construct() {
    parent::__construct();
    $this->date = new ArchibaldLomDataDateTime();
  }

  public function getRole() {
    return $this->role;
  }

  public function setRole($role) {
    if ($role instanceof ArchibaldLomDataVocabulary) {
      $this->role = $role;
      return;
    }
    $this->role = new ArchibaldLomDataVocabulary($role);
  }

  public function getEntity($id = NULL) {
    if ($id !== NULL && isset($this->entity[$id])) {
      return $this->entity[$id];
    }
    return $this->entity;
  }

  public function setEntity($entity, $id = NULL) {
    if ($id !== NULL) {
      $this->entity[$id] = $entity;
      return;
    }
    $this->entity = $entity;
  }

  public function addEntity($entity) {
    if (!is_array($this->entity)) {
      $this->entity = array();
    }
    if (is_array($entity)) {
      foreach ($entity as $item) {
        if (is_null($this->addEntity($entity))) {
          return NULL;
        }
      }
    }
    $this->entity[] = $entity;
  }

  public function getDate() {
    return $this->date;
  }

  public function setDate($date) {
    $this->date = $date;
  }
}

/**
 * ArchibaldLomDataVocabulary
 */
class ArchibaldLomDataVocabulary extends ArchibaldAbstractDataManager {

  /**
   * char_string, optional, max 1"
   * @var string
   */
  public $source = '';

  /**
   * char_string, optional, max 1"
   * @var string
   */
  public $value = '';

  public function __construct($value = "", $source = "LOM-CHv1.1") {
    parent::__construct();
    $this->setSource($source);
    $this->setValue($value);
  }

  public function getSource() {
    return $this->source;
  }

  public function setSource($source) {
    $this->source = $source;
  }

  public function getValue() {
    return $this->value;
  }

  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * get full key for this identifier
   *
   * @return string
   */
  public function getFullKey() {
    return $this->source . '|' . $this->value;
  }

  /**
   * get uniqui id/key for this identifier
   *
   * @return string
   */
  public function getId() {
    return md5($this->source . '|' . $this->value);
  }
}

/**
 * ArchibaldLomDataIdentifier
 */
class ArchibaldLomDataIdentifier extends ArchibaldAbstractDataManager {

  /**
   * char_string, optional, max 1"
   * @var string
   */
  public $catalog = '';

  /**
   * char_string, optional, max 1"
   * @var string
   */
  public $entry = '';

  /**
   * ArchibaldLomDataLangString optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $title = '';

  public function __construct($catalog = '', $entry = '', $title = '') {
    parent::__construct();
    $this->setCatalog($catalog);
    $this->setEntry($entry);

    if ($title instanceof ArchibaldLomDataLangString) {
      $this->setTitle($title);
    }
  }

  public function getCatalog() {
    return $this->catalog;
  }

  public function setCatalog($catalog) {
    $this->catalog = $catalog;
  }

  public function getEntry() {
    return $this->entry;
  }

  public function setEntry($entry) {
    $this->entry = $entry;
  }

  public function getTitle() {
    return $this->title;
  }

  /**
   * Set the title
   *
   * @param ArchibaldLomDataLangString $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * get uniqui id/key for this identifier
   *
   * @return string
   */
  public function getId() {
    return md5($this->catalog . '|' . $this->entry);
  }
}

/**
 * ArchibaldLomDataDuration
 *
 * duration = P[yY][mM][dD][T[hH][nM][s[.s]S]] where:
 *   - y: The number of years (integer, â‰¥ 0)
 *   - m: The number of months (integer, â‰¥ 0, not restricted,
 *        e.g., > 12 is acceptable)
 *   - d: The number of days (integer, â‰¥ 0, not restricted,
 *        e.g., > 31 is acceptable)
 *   - h: The number of hours (integer, â‰¥ 0, not restricted,
 *        e.g., > 23 is acceptable)
 *   - n: The number of minutes (integer, â‰¥ 0, not restricted,
 *        e.g., > 59 is acceptable)
 *   - s: The number of seconds or fraction of seconds
 *        (integer, â‰¥ 0, not restricted, e.g.,
 *   - > 59 is acceptable)
 *  P must be present if not all values are zeros,
 *  If the "time" is complety zero, T should not be present
 */
class ArchibaldLomDataDuration extends ArchibaldAbstractDataManager {

  /**
   * char_string, optional, max 1"
   * @var string
   */
  public $duration = '';

  /**
   * ArchibaldLomDataLangString, optional, max 1"
   * @var ArchibaldLomDataLangString
   */
  public $description = NULL;

  /**
   *
   * @param String $duration
   * @param ArchibaldLomDataLangString $description
   */
  public function __construct($duration = "", $description = NULL) {
    parent::__construct();
    if ($description == NULL) {
      $description = new ArchibaldLomDataLangString();
    }
    $this->setDescription($description);
    $this->setDuration($duration);
  }

  /**
   * can return a string which is formated as defined in
   *  http://www.w3.org/TR/xmlschema-2/#duration
   *  or an array with parameters
   *
   * int year
   * int month
   * int days
   * int hours
   * int minutes
   * float seconds
   *
   * @param boolean $as_array
   *
   * @return mixed (string or array)
   */
  public function getDuration($as_array = FALSE) {
    if ($as_array == TRUE) {
      if( empty($this->duration) ) {
        return array('hours' => 0, 'minutes' => 0, 'seconds' => 0);
      }

      $duration = drupal_strtoupper($this->duration);
      $regexp =
          '/^[-]?P(?!$)' . //Have date entries?
          '(?:(?<year>\d+)+Y)?' . //Years
          '(?:(?<month>\d+)+M)?' .  //Months
          '(?:(?<days>\d+)+D)?' .  //Days
          '(T(?!$)(?:(?<hours>\d+)+H)?' . //Hours
          '(?:(?<minutes>\d+)+M)?' .  //Minutes
          '(?:(?<seconds>\d+' .   //Seconds
          '(?:\.\d+)?)+S)?)?$/i'; //Have time entries?

      if (preg_match($regexp, $duration, $result)) {
        foreach ($result as $k => $v) {
          if (intval($k) === $k) {
            unset($result[$k]);
          }
        }
        return $result;
      }
    }
    return $this->duration;
  }


  /**
   * return current duration as a human-readable value
   *
   * @param string $unit (year,month,days,hours,minutes,seconds)
   *
   * @return string
   */
  public function getDurationHuman() {
    if( !strlen($this->duration) ) return NULL;

    $duration = $this->getDuration(TRUE);

    $ret = '';
    if (!empty($duration['hours'])) {
      $ret .= sprintf("%02d", $duration['hours']);
    } else {
      $ret .= '00';
    }
    $ret .= ':';

    if (!empty($duration['minutes'])) {
      $ret .= sprintf("%02d", $duration['minutes']);
    } else {
      $ret .= '00';
    }
    $ret .= ':';

    if (!empty($duration['seconds'])) {
      $ret .= sprintf("%02d", $duration['seconds']);
    } else {
      $ret .= '00';
    }

    return $ret;
  }

  /**
   * return current duration as integer
   *
   * @param string $unit (year,month,days,hours,minutes,seconds)
   *
   * @return integer
   */
  public function getDurationIn($unit = 'seconds') {
    $duration = $this->getDuration(TRUE);

    $seconds = 0;

    if (!empty($duration['year'])) {
      $seconds += $duration['year'] * 60 * 60 * 24 * 365;
    }

    if (!empty($duration['month'])) {
      $seconds += $duration['month'] * 60 * 60 * 24 * 30;
    }

    if (!empty($duration['days'])) {
      $seconds += $duration['days'] * 60 * 60 * 24;
    }

    if (!empty($duration['hours'])) {
      $seconds += $duration['hours'] * 60 * 60;
    }

    if (!empty($duration['minutes'])) {
      $seconds += $duration['minutes'] * 60;
    }

    if (!empty($duration['seconds'])) {
      $seconds += $duration['seconds'];
    }

    switch ($unit) {
      case 'year':
        return $seconds / (60 * 60 * 24 * 365);
      case 'month':
        return $seconds / (60 * 60 * 24 * 30);
      case 'days':
        return $seconds / (60 * 60 * 24);
      case 'hours':
        return $seconds / (60 * 60);
      case 'minutes':
        return $seconds / (60);

      case 'seconds':
      default:
        return $seconds;
    }
  }

  /**
   * duration can be setted as string
   * http://www.w3.org/TR/xmlschema-2/#duration
   * or array
   *
   * int year
   * int month
   * int days
   * int hours
   * int minutes
   * float seconds
   *
   * @param mixed $duration
   */
  public function setDuration($duration) {
    if (is_array($duration)) {
      $temp = 'P';
      if (!empty($duration['year'])) {
        $temp .= (INT) $duration['year'] . 'Y';
      }
      if (!empty($duration['month'])) {
        $temp .= (INT) $duration['month'] . 'M';
      }
      if (!empty($duration['days'])) {
        $temp .= (INT) $duration['days'] . 'D';
      }

      if (!empty($duration['hours']) || !empty($duration['minutes']) || !empty($duration['seconds'])) {
        $temp .= 'T';
        if (!empty($duration['hours'])) {
          $temp .= (INT) $duration['hours'] . 'H';
        }
        if (!empty($duration['minutes'])) {
          $temp .= (INT) $duration['minutes'] . 'M';
        }
        if (!empty($duration['seconds'])) {
          $temp .= (FLOAT) $duration['seconds'] . 'S';
        }
      }

      if ($temp != 'P') {
        $this->duration = $temp;
      }
    }
    else {
      $this->duration = $duration;
    }
  }

  /**
   * get description
   *
   * @return ArchibaldLomDataLangString
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * set description
   *
   * @param ArchibaldLomDataLangString $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}

/**
 * ArchibaldLomDataDateTime
 *
 * datetime = YYYY[-MM[-DD[Thh[:mm[:ss[.s[TZD]]]]]]] where:
 *   - YYYY: The 4-digit year (â‰¥ 0001)
 *   - MM: The 2-digit month
 *         (01 through 12 where 01=January, etc.)
 *   - DD: The 2-digit day of month
 *         (01 through 31, depending on the values of month and year)
 *   - hh: Two digits of hour (00 through 23) (am and pm are NOT allowed)
 *   - mm: Two digits of minute (00 through 59)
 *   - ss: Two digits of second (00 through 59)
 *   - s: One or more digits representing a decimal fraction of a second
 *   - TZD: The time zone designator
 *          ("Z" for coordinated universal time [UTC] or +hh,
 *   - hh, +hh:mm, or -hh:mm,
 *         where hh is two digits of hour and mm is two digits of
 *   minute)
 */
class ArchibaldLomDataDateTime extends ArchibaldAbstractDataManager {

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $datetime = '';

  /**
   * ArchibaldLomDataLangString, optional, max 1
   *
   * @param ArchibaldLomDataLangString $description
   */
  public $description = NULL;

  public function __construct($datetime = '', $description = NULL) {
    parent::__construct();
    if (empty($datetime)) {
      $datetime = time();
    }
    $this->setDatetime($datetime);
    if ($description === NULL) {
      $description = new ArchibaldLomDataLangString();
    }
    $this->setDescription($description);
  }

  public function getDatetime($as_unixtimestamp = FALSE) {
    if ($as_unixtimestamp == TRUE) {
      $regexp =
        '/^(?<year>\d+)' .
        '?-(?<month>\d+)?-' .
        '(?<day>\d+)?' .
        '(T(?<hours>\d+)?:' .
        '(?<minutes>\d+)?:' .
        '(?<seconds>\d+)?' .
        '(\+(?<zone_h>\d+)' .
        '(\:(?<zone_m>\d+))?)?)?$/i';

      if (preg_match($regexp, $this->datetime, $result)) {
        return gmmktime($result['hours'], $result['minutes'], $result['seconds'], $result['month'], $result['day'], $result['year']);
      }
    }
    return $this->datetime;
  }

  public function setDatetime($datetime) {
    if (!empty($datetime) && is_numeric($datetime)) {
      $datetime = gmdate('Y-m-d\TH:i:sP', $datetime);
    }
    $this->datetime = $datetime;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }
}

/**
 * ArchibaldLomDataLangString default max 10
 *
 * A lang string contains textual content and its transaltions
 * For this each containing stering needs a language identifier
 */
class ArchibaldLomDataLangString extends ArchibaldAbstractDataManager {

  /**
   * array of strings
   * @var array
   */
  public $strings = array();

  /**
   * uuid for identifcation
   * @var string
   */
  public $id = '';

  public function __construct($string = "", $language = "en") {
    parent::__construct();
    if (!empty($string) && !empty($language)) {
      $this->setString($string, $language);
    }
  }

  public function getString($language) {
    if (empty($language) || empty($this->strings)) {
      return NULL;
    }

    if (!isset($this->strings[$language])) {
      return NULL;
    }
    else {
      return $this->strings[$language];
    }
  }

  public function setString($string, $language) {
    if (empty($string)) {
      unset($this->strings[$language]);
      return NULL;
    }

    $this->strings[$language] = strtr(html_entity_decode($string), array('&#039;' => '\''));
    ksort($this->strings);
  }

  public function getStrings() {
    if (!is_array($this->strings)) {
      $this->strings = array();
    }
    return $this->strings;
  }

  /**
   * return unique identifer for class instance
   *
   * @return varchar
   */
  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }
}

/**
 * ArchibaldLomDataMetaMetadata
 */
class ArchibaldLomDataMetaMetadata extends ArchibaldAbstractDataManager {

  /**
   *
   * @var int
   */
  public $metaMetadataId = 0;

  /**
   * who created a meta data ? (role)
   */
  const CONTRIBUTE_ROLE_CREATOR = 'creator';
  const CONTRIBUTE_ROLE_VALIDATOR = 'validator';

  /**
   * ArchibaldLomDataDateTime, optional, max 1
   * @var ArchibaldLomDataDateTime
   */
  public $lastModifiedDate = NULL;

  /**
   * ArchibaldLomDataDateTime, optional, max 1
   * @var ArchibaldLomDataDateTime
   */
  public $creationDate = NULL;

  /**
   * ArchibaldLomDataIdentifier, optional, max 10
   * @var array
   */
  public $identifier = array();

  /**
   * ArchibaldLomDataContribute, optional ordered, max 10
   * (also ArchibaldLomDataContribute:entity max 10 instead of 40
   * @var array
   */
  public $contribute = array();

  /**
   * char_string, optional, max 10 (STATIC: 'LOM-CHv1.1')
   * @var string
   */
  public $metadataSchema = 'LOM-CHv1.1';

  /**
   * char_string, optional, max 1 @def: general_langauge"
   * @var string
   */
  public $language = '';

  public function __construct() {
    parent::__construct();
    $this->creationDate = new ArchibaldLomDataDateTime();
    $this->lastModifiedDate = new ArchibaldLomDataDateTime();
  }

  public function getIdentifier($id = NULL) {
    if ($id !== NULL && isset($this->identifier[$id])) {
      return $this->identifier[$id];
    }
    return $this->identifier;
  }

  public function setIdentifier($identifier, $id = NULL) {
    if ($id !== NULL) {
      $this->identifier[$id] = $identifier;
      return;
    }
    $this->identifier = $identifier;
  }

  public function addIdentifier($identifier) {
    if (!is_array($this->identifier)) {
      $this->identifier = array();
    }
    if (is_array($identifier)) {
      foreach ($identifier as $item) {
        if (is_null($this->addIdentifier($identifier))) {
          return NULL;
        }
      }
    }
    if ($identifier instanceof ArchibaldLomDataIdentifier) {
      $key = $identifier->getId();
      $this->identifier[$key] = $identifier;
    }
  }

  /**
   *
   * @return ArchibaldLomDataDateTime
   */
  public function getLastModifiedDate() {
    return $this->lastModifiedDate;
  }

  public function setLastModifiedDate($date) {
    $this->lastModifiedDate = $date;
  }

  /**
   *
   * @return ArchibaldLomDataDateTime
   */
  public function getCreationDate() {
    return $this->creationDate;
  }

  public function setCreationDate($date) {
    $this->creationDate = $date;
  }

  public function getContribute($id = NULL) {
    if ($id !== NULL && isset($this->contribute[$id])) {
      return $this->contribute[$id];
    }
    return $this->contribute;
  }

  public function setContribute($contribute, $id = NULL) {
    if ($id !== NULL) {
      $this->contribute[$id] = $contribute;
      return;
    }
    $this->contribute = $contribute;

    $this->filterDuplicatedContributor();
  }

  public function addContribute($contribute) {
    if (!is_array($this->contribute)) {
      $this->contribute = array();
    }
    if (is_array($contribute)) {
      foreach ($contribute as $item) {
        if (is_null($this->addContribute($contribute))) {
          return NULL;
        }
      }
    }
    if ($contribute instanceof ArchibaldLomDataContribute) {
      $this->contribute[] = $contribute;
      return;
    }
    $this->contribute[] = new ArchibaldLomDataContribute($contribute);
  }

  /**
   * sort contributes by date
   */
  public function sortContribute($method = 'date', $order = 'asc') {
    if ($method == 'date') {
      usort($this->contribute, array('ArchibaldLomDataMetaMetadata', 'sortContributeByDate'));
    }
    if ($order == 'desc') {
      $this->contribute = array_reverse($this->contribute);
    }
  }

  /**
   * callback for usort() when sorting by date
   *
   * @param ArchibaldLomDataContribute $a
   * @param ArchibaldLomDataContribute $b
   *
   * @return integer
   */
  public static function sortContributeByDate($a, $b) {
    $date_a = $a->getDate();
    $date_b = $b->getDate();
    if( empty($date_a) || empty($date_b) ) {
      return -1;
    }

    $date_a = $date_a->getDatetime(TRUE);
    $date_b = $date_b->getDatetime(TRUE);

    if ($date_a == $date_b) {
      return 0;
    }
    return ($date_a > $date_b) ? +1 : -1;
  }

  /**
   * walking trough all contributor and remove douplets with same role and vcard
   */
  protected function filterDuplicatedContributor() {
    $seen_ids = array();
    foreach ($this->contribute as $cid => $contribute) {
      $id = md5($contribute->getRole()->getValue() . '__' . $contribute->getEntity(0));

      if (!empty($id) && isset($seen_ids[$id])) {
        unset($this->contribute[$cid]);
      }

      $seen_ids[$id] = TRUE;
    }
  }

  public function getMetadataSchema() {
    return $this->metadataSchema;
  }

  public function setMetadataSchema($metadata_schema) {
    if (!is_array($this->metadataSchema)) {
      $this->metadataSchema = array();
    }
    if (is_array($metadata_schema)) {
      foreach ($metadata_schema as $item) {
        $this->metadataSchema[] = $metadata_schema;
      }
    }
    $this->metadataSchema[] = $metadata_schema;
  }

  public function getLanguage() {
    return $this->language;
  }

  public function setLanguage($language) {
    $this->language = $language;
  }

}

/**
 * ArchibaldLomDataTechnical
 */
class ArchibaldLomDataTechnical extends ArchibaldAbstractDataManager {

  /**
   *
   * @var int
   */
  public $technicalId = 0;

  /**
   * char_string, optional
   * @var string
   */
  public $format = '';

  /**
   * char_string, optional, max 1 (just digits, non-negative, integer)
   * @var integer
   */
  public $size = 0;

  /**
   * char_string, optional, max 10 ordered
   * @var array
   */
  public $location = array();

  /**
   * ArchibaldLomDataRequirement, optional, max 40
   * @var array
   */
  public $requirement = array();

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $installationRemarks = NULL;

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $otherPlattformRequirements = NULL;

  /**
   * ArchibaldLomDataDuration, optional, max 1
   * @var ArchibaldLomDataDuration
   */
  public $duration = '';

  /**
   * ArchibaldLomDataPreviewImage, optional, max 1
   * @var ArchibaldLomDataPreviewImage
   */
  public $previewImage = array();

  public function __construct() {
    parent::__construct();
    $this->duration = new ArchibaldLomDataDuration();
    $this->installationRemarks = new ArchibaldLomDataLangString();
    $this->otherPlattformRequirements = new ArchibaldLomDataLangString();
    $this->previewImage = new ArchibaldLomDataPreviewImage();
  }

  public function getFormat($id = NULL) {
    return $this->format;
  }

  public function setFormat($format, $id = NULL) {
    if ($id !== NULL) {
      $this->format[$id] = $format;
      return;
    }
    $this->format = $format;
  }


  /**
   * return in human readable file size format
   *
   * @return string
   */
  public function getSizeHuman($as_array=FALSE) {

    if (empty($this->size)) {
      return ($as_array ? array(NULL, NULL) : NULL);
    }



    $multiplier = 1024;
    $unit = 'b';
    $size = $this->size;

    if ($this->size >= $multiplier * $multiplier * $multiplier * $multiplier * $multiplier) {
      $unit = 'pb';
      $size = round(($size / ($multiplier * $multiplier * $multiplier * $multiplier * $multiplier)) * 100) / 100;
    }
    elseif ($this->size >= $multiplier * $multiplier * $multiplier * $multiplier) {
      $unit = 'tb';
      $size = round(($size / ($multiplier * $multiplier * $multiplier * $multiplier)) * 100) / 100;
    }
    elseif ($this->size >= $multiplier * $multiplier * $multiplier) {
      $unit = 'gb';
      $size = round(($size / ($multiplier * $multiplier * $multiplier)) * 100) / 100;
    }
    elseif ($this->size >= $multiplier * $multiplier) {
      $unit = 'mb';
      $size = round(($size / ($multiplier * $multiplier)) * 100) / 100;
    }
    elseif ($this->size >= $multiplier) {
      $unit = 'kb';
      $size = round(($size / $multiplier) * 100) / 100;
    }
    $ret = ($as_array ? array($size, $unit) : $size . ' ' . strtoupper($unit));
    return $ret;
  }

  public function getSize() {
    return $this->size;
  }

  public function setSize($size) {
    $this->size = (INT) $size;
  }

  public function getLocation($id = NULL) {
    if ($id !== NULL) {
      if (isset($this->location[$id])) {
        return $this->location[$id];
      }
      else {
        return "";
      }
    }
    if (!is_array($this->location)) {
      return array();
    }
    return $this->location;
  }

  public function setLocation($location, $id = NULL) {
    if ($id !== NULL) {
      $this->location[$id] = $location;
      return;
    }

    if (empty($location)) {
      $location = array();
    }
    if (!is_array($location)) {
      $location = array($location);
    }
    $this->location = $location;
  }

  public function addLocation($location) {
    if (!is_array($this->location)) {
      $this->location = array();
    }
    if (is_array($location)) {
      foreach ($location as $item) {
        if (is_null($this->addLocation($location))) {
          return NULL;
        }
      }
    }
    $this->location[] = $location;
  }

  public function getRequirement($id = NULL) {
    if ($id !== NULL && isset($this->requirement[$id])) {
      return $this->requirement[$id];
    }
    return $this->requirement;
  }

  public function addRequirement($requirement) {
    if (!is_array($this->requirement)) {
      $this->requirement = array();
    }
    if (is_array($requirement)) {
      foreach ($requirement as $item) {
        if (is_null($this->addRequirement($requirement))) {
          return array();
        }
      }
    }
    $this->requirement[] = $requirement;
  }

  public function getInstallationRemarks() {
    return $this->installationRemarks;
  }

  public function setInstallationRemarks($installation_remarks) {
    $this->installationRemarks = $installation_remarks;
  }

  public function getOtherPlattformRequirements() {
    return $this->otherPlattformRequirements;
  }

  public function setOtherPlattformRequirements($other_plattform_requirements) {
    $this->otherPlattformRequirements = $other_plattform_requirements;
  }

  /**
   *
   * @return ArchibaldLomDataDuration
   */
  public function getDuration() {
    return $this->duration;
  }

  public function setDuration($duration) {
    $this->duration = $duration;
  }

  /**
   * get preview image
   *
   * @return string
   */
  public function getPreviewImage() {
    return $this->previewImage;
  }

  /**
   * set preview image
   *
   * @param string $preview_image
   */
  public function setPreviewImage($preview_image) {
    $this->previewImage = $preview_image;
  }
}

/**
 * ArchibaldLomDataRequirement default max 40
 */
class ArchibaldLomDataRequirement extends ArchibaldAbstractDataManager {

  /**
   * ArchibaldLomDataOrComposite, optional, max 40
   * @var array
   */
  public $orComposite = array();

  public function getOrComposite($id = NULL) {
    if ($id !== NULL && isset($this->orComposite[$id])) {
      return $this->orComposite[$id];
    }
    return $this->orComposite;
  }

  public function addOrComposite($or_composite) {
    if (!is_array($this->orComposite)) {
      $this->orComposite = array();
    }
    if (is_array($or_composite)) {
      foreach ($or_composite as $item) {
        if (is_null($this->addOrComposite($or_composite))) {
          return NULL;
        }
      }
    }
    $this->orComposite[] = $or_composite;
  }
}

/**
 * ArchibaldLomDataPreviewImage
 */
class ArchibaldLomDataPreviewImage extends ArchibaldAbstractDataManager {

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $image = "";

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $copyrightDescription = "";

  /**
   *
   * @param String $image
   * @param ArchibaldLomDataLangString $description
   */
  public function __construct($image = "", $description = NULL) {
    parent::__construct();
    $this->setImage($image);
    $this->setCopyrightDescription($description);
  }

  public function getImage() {
    return $this->image;
  }

  public function setImage($image) {
    $this->image = $image;
  }

  public function getCopyrightDescription() {
    return $this->copyrightDescription;
  }

  public function setCopyrightDescription($description) {
    $this->copyrightDescription = $description;
  }

}


/**
 * ArchibaldLomDataOrComposite default max 40
 */
class ArchibaldLomDataOrComposite extends ArchibaldAbstractDataManager {

  /**
   * what requirement is needed ? (type)
   */
  const TTYPE_OPERATING_SYSTEM = 'operating system';
  const TYPE_BROWSER = 'browser';

  /**
   * requirement key if type = operating system
   */
  const NAME_OPERATING_SYSTEM_PC_DOS = 'pc-dos';
  const NAME_OPERATING_SYSTEM_MS_WINDOWS = 'ms-windows';
  const NAME_OPERATING_SYSTEM_MACOS = 'macos';
  const NAME_OPERATING_SYSTEM_MULTI_IS = 'multi-os';
  const NAME_OPERATING_SYSTEM_NONE = 'none';

  /**
   * requirement key if type = browser
   */
  const NAME_BROWSER_ANY = 'any';
  const NAME_BROWSER_NETSCAPE_COMMUNICATOR = 'netscape communicator';
  const NAME_BROWSER_MS_IE = 'ms-internet explorer';
  const NAME_BROWSER_OPERA = 'opera';
  const NAME_BROWSER_AMAYA = 'amaya';

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $type = NULL;

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $name = NULL;

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $minimumVersion = "";

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $maximumVersion = "";

  public function __construct() {
    parent::__construct();
    $this->type = new ArchibaldLomDataVocabulary();
    $this->name = new ArchibaldLomDataVocabulary();
  }

  public function getType() {
    return $this->type;
  }

  public function setType($type) {
    $this->type = $type;
  }

  public function getName() {
    return $this->name;
  }

  public function setName($name) {
    $this->name = $name;
  }

  public function getMinimumVersion() {
    return $this->minimumVersion;
  }

  public function setMinimumVersion($minimum_version) {
    $this->minimumVersion = $minimum_version;
  }

  public function getMaximumVersion() {
    return $this->maximumVersion;
  }

  public function setMaximumVersion($maximum_version) {
    $this->maximumVersion = $maximum_version;
  }
}

/**
 * ArchibaldLomDataEducation default max 100
 */
class ArchibaldLomDataEducation extends ArchibaldAbstractDataManager {

  /**
   * the interactivity type, need someone to just listen,
   * or should he do something? maybe both?
   */
  const INTERACTIVITY_TYPE_ACTIVE = 'active';
  const INTERACTIVITY_TYPE_EXPOSITIVE = 'expositive';
  const INTERACTIVITY_TYPE_MIXED = 'mixed';

  /**
   * which type of resource is it?
   */
  const LEARNING_RESOURCE_TYPE_REFERENCE = 'reference';
  const LEARNING_RESOURCE_TYPE_GLOSSARY = 'glossary';
  const LEARNING_RESOURCE_TYPE_BROADCAST = 'broadcast';
  const LEARNING_RESOURCE_TYPE_APPLICATION = 'application';
  const LEARNING_RESOURCE_TYPE_TOOL = 'tool';
  const LEARNING_RESOURCE_TYPE_LEARNING_ASSETS = 'learning assets';
  const LEARNING_RESOURCE_TYPE_IMAGE = 'image';
  const LEARNING_RESOURCE_TYPE_MODEL = 'model';
  const LEARNING_RESOURCE_TYPE_DATA = 'data';
  const LEARNING_RESOURCE_TYPE_TEXT = 'text';
  const LEARNING_RESOURCE_TYPE_AUDIO = 'audio';
  const LEARNING_RESOURCE_TYPE_VIDEO = 'video';
  const LEARNING_RESOURCE_TYPE_WEBLOG = 'Weblog';
  const LEARNING_RESOURCE_TYPE_WEB_PAGE = 'web page';
  const LEARNING_RESOURCE_TYPE_WIKI = 'wiki';
  const LEARNING_RESOURCE_TYPE_OTHER_WEB_RESOURCE = 'other web resource';
  const LEARNING_RESOURCE_TYPE_EXPLORATION = 'exploration';
  const LEARNING_RESOURCE_TYPE_CASE_STUDY = 'case study';
  const LEARNING_RESOURCE_TYPE_OPEN_ACTIVITY = 'open activity';
  const LEARNING_RESOURCE_TYPE_ASSESSMENT = 'assessment';
  const LEARNING_RESOURCE_TYPE_EDUCATIONAL_GAME = 'educational game';
  const LEARNING_RESOURCE_TYPE_PROJECT = 'project';
  const LEARNING_RESOURCE_TYPE_ROLE_PLAY = 'role play';
  const LEARNING_RESOURCE_TYPE_SIMULATION = 'simluation';
  const LEARNING_RESOURCE_TYPE_DRILL_AND_PRACTICE = 'drill and practice';
  const LEARNING_RESOURCE_TYPE_DEMONSTRATION = 'demonstration';
  const LEARNING_RESOURCE_TYPE_ENQUIRY_ORIENTED_ACTIVITY = 'enquiry oriented activity';
  const LEARNING_RESOURCE_TYPE_COURSE = 'course';
  const LEARNING_RESOURCE_TYPE_LESSON_PLAN = 'lesson plan';
  const LEARNING_RESOURCE_TYPE_PRESENATION = 'presentation';
  const LEARNING_RESOURCE_TYPE_EXERCISE = 'exercise';
  const LEARNING_RESOURCE_TYPE_QUESTIONNAIRE = 'questionnaire';
  const LEARNING_RESOURCE_TYPE_DIAGRAM = 'diagram';
  const LEARNING_RESOURCE_TYPE_FIGURE = 'figure';
  const LEARNING_RESOURCE_TYPE_GRAPH = 'graph';
  const LEARNING_RESOURCE_TYPE_INDEX = 'index';
  const LEARNING_RESOURCE_TYPE_SLIDE = 'slide';
  const LEARNING_RESOURCE_TYPE_TABLE = 'table';
  const LEARNING_RESOURCE_TYPE_NARRATIVE_TEXT = 'narrative text';
  const LEARNING_RESOURCE_TYPE_EXAM = 'exam';
  const LEARNING_RESOURCE_TYPE_EXPERIMENT = 'experiment';
  const LEARNING_RESOURCE_TYPE_PROBLEM_STATEMENT = 'problem statement';
  const LEARNING_RESOURCE_TYPE_SELF_ASSESSMENT = 'self assessment';
  const LEARNING_RESOURCE_TYPE_LECTURE = 'lecture';

  /**
   * how intensive is the interactivity?
   */
  const INTERACTIVITY_LEVEL_VERY_LOW = 'very low';
  const INTERACTIVITY_LEVEL_LOW = 'low';
  const INTERACTIVITY_LEVEL_MEDIUM = 'medium';
  const INTERACTIVITY_LEVEL_HIGH = 'high';
  const INTERACTIVITY_LEVEL_VERY_HIGH = 'very high';

  /**
   * how intensive is the semantic?
   */
  const SEMANTIC_LEVEL_VERY_LOW = 'very low';
  const SEMANTIC_LEVEL_LOW = 'low';
  const SEMANTIC_LEVEL_MEDIUM = 'medium';
  const SEMANTIC_LEVEL_HIGH = 'high';
  const SEMANTIC_LEVEL_VERY_HIGH = 'very high';

  /**
   * for who is this resource?
   */
  const INTENDED_END_USER_ROLE_TEACHER = 'teacher';
  const INTENDED_END_USER_ROLE_AUTHOR = 'author';
  const INTENDED_END_USER_ROLE_LEARNER = 'learner';
  const INTENDED_END_USER_ROLE_MANAGER = 'manager';
  const INTENDED_END_USER_ROLE_OTHER = 'other';

  /**
   * pre choose the school level
   */
  const CONTEXT_SCHOOL = 'school';
  const CONTEXT_HIGHER_EDUCATION = 'higher education';
  const CONTEXT_TRAINING = 'training';
  const CONTEXT_OTHER = 'other';

  /**
   * self explained
   */
  const DIFFICULT_LEVEL_VERY_EASY = 'very easy';
  const DIFFICULT_LEVEL_EASY = 'easy';
  const DIFFICULT_LEVEL_MEDIUM = 'medium';
  const DIFFICULT_LEVEL_DIFFICULT = 'difficult';
  const DIFFICULT_LEVEL_VERY_DIFFICULT = 'very difficult';

  /**
   * @var int
   */
  public $educationId = '';

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $interactiveType = NULL;

  /**
   * ArchibaldLomDataVocabulary, optional, max 10 ordered
   * @var array
   */
  public $learningResourceType = array(
    'documentary' => array(),
    'pedagogical' => array()
  );

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $interactivityLevel = NULL;

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $semanticDensity = NULL;

  /**
   * ArchibaldLomDataVocabulary, optional, max 10 ordered
   * @var array
   */
  public $intendedEndUserRole = array();

  /**
   * ArchibaldLomDataVocabulary, optional, max 10
   * @var array
   */
  public $context = array();

  /**
   * ArchibaldLomDataLangString, optional, max 5
   * @var array
   */
  public $typicalAgeRange = array();

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $difficult = NULL;

  /**
   * ArchibaldLomDataDuration, optional, max 1
   * @var ArchibaldLomDataDuration
   */
  public $typicalLearningTime = NULL;

  /**
   * ArchibaldLomDataLangString, optional, max 10
   * @var array
   */
  public $description = array();

  /**
   * char_string, optional, max 10
   * @var array
   */
  public $language = array();

  public function __construct() {
    parent::__construct();
    $this->typicalLearningTime = new ArchibaldLomDataVocabulary();
    $this->difficult = new ArchibaldLomDataVocabulary();
    $this->semanticDensity = new ArchibaldLomDataVocabulary();
    $this->interactiveType = new ArchibaldLomDataVocabulary();
    $this->interactivityLevel = new ArchibaldLomDataVocabulary();
  }

  public function getInteractiveType() {
    if (!($this->interactiveType instanceof ArchibaldLomDataVocabulary)) {
      return new ArchibaldLomDataVocabulary();
    }
    return $this->interactiveType;
  }

  public function setInteractiveType($interactive_type) {
    if (!is_object($interactive_type)) {
      $interactive_type = new ArchibaldLomDataVocabulary($interactive_type);
    }
    $this->interactiveType = $interactive_type;
  }

  // Learning Resource Type
  /*
    .##.......########..########
    .##.......##.....##....##...
    .##.......##.....##....##...
    .##.......########.....##...
    .##.......##...##......##...
    .##.......##....##.....##...
    .########.##.....##....##...
  */

  public function getLearningResourceType($type, $id = NULL) {
    if ($id !== NULL && isset($this->learningResourceType[$type][$id])) {
      return $this->learningResourceType[$type][$id];
    }
    return $this->learningResourceType[$type];
  }

  public function setLearningResourceType($type, $learning_resource_type, $id = NULL) {
    if ($id !== NULL) {
      $this->learningResourceType[$type][$id] = $learning_resource_type;
      return;
    }
    $this->learningResourceType[$type] = $learning_resource_type;
  }

  public function addLearningResourceType($type, $learning_resource_type) {
    if (!is_array($this->learningResourceType[$type])) {
      $this->learningResourceType[$type] = array();
    }
    if (is_array($learning_resource_type)) {
      foreach ($learning_resource_type as $item) {
        if (is_null($this->addLearningResourceType($type, $learning_resource_type))) {
          return NULL;
        }
      }
      return;
    }
    if (!is_object($learning_resource_type)) {
      $learning_resource_type = new ArchibaldLomDataVocabulary($learning_resource_type);
    }
    $this->learningResourceType[$type][] = $learning_resource_type;
  }

  /**
   * remove a keyword by ArchibaldLomDataVocabulary or integer array id
   *
   * @param ArchibaldLomDataVocabulary $keyword
   *
   * @return boolean status
   */
  public function removeLearningResourceType($type, $learning_resource_type) {
    if (!($learning_resource_type instanceof ArchibaldLomDataVocabulary)) {
      if (is_numeric($learning_resource_type) && isset($this->learningResourceType[$type][$learning_resource_type])) {

        unset($this->learningResourceType[$type][$learning_resource_type]);
        return TRUE;
      }
      return FALSE;
    }

    $key = $learning_resource_type->getId();

    foreach ($this->learningResourceType[$type] as $i => $cur_learning_resource_type) {
      $cur_key = $cur_learning_resource_type->getId();

      if ($key == $cur_key) {
        unset($this->learningResourceType[$type][$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  // Interactivity
  /*
    .......####.##....##.########.########.########.....###.....######..########.####.##.....##.####.########.##....##
    ........##..###...##....##....##.......##.....##...##.##...##....##....##.....##..##.....##..##.....##.....##..##.
    ........##..####..##....##....##.......##.....##..##...##..##..........##.....##..##.....##..##.....##......####..
    ........##..##.##.##....##....######...########..##.....##.##..........##.....##..##.....##..##.....##.......##...
    ........##..##..####....##....##.......##...##...#########.##..........##.....##...##...##...##.....##.......##...
    ........##..##...###....##....##.......##....##..##.....##.##....##....##.....##....##.##....##.....##.......##...
    .......####.##....##....##....########.##.....##.##.....##..######.....##....####....###....####....##.......##...
  */

  public function getInteractivityLevel() {
    return $this->interactivityLevel;
  }

  public function setInteractivityLevel($interactivity_level) {
    if (!is_object($interactivity_level)) {
      $interactivity_level = new ArchibaldLomDataVocabulary($interactivity_level);
    }
    $this->interactivityLevel = $interactivity_level;
  }

  public function getSemanticDensity() {
    return $this->semanticDensity;
  }

  public function setSemanticDensity($semantic_density) {
    if (!is_object($semantic_density)) {
      $semantic_density = new ArchibaldLomDataVocabulary($semantic_density);
    }
    $this->semanticDensity = $semantic_density;
  }

  public function getIntendedEndUserRole($id = NULL) {
    if ($id !== NULL && isset($this->intendedEndUserRole[$id])) {
      return $this->intendedEndUserRole[$id];
    }
    return $this->intendedEndUserRole;
  }

  public function setIntendedEndUserRole($intended_end_user_role, $id = NULL) {
    if ($id !== NULL) {
      $this->intendedEndUserRole[$id] = $intended_end_user_role;
      return;
    }
    $this->intendedEndUserRole = $intended_end_user_role;
  }

  public function addIntendedEndUserRole($intended_end_user_role) {
    if (!is_array($this->intendedEndUserRole)) {
      $this->intendedEndUserRole = array();
    }
    if (is_array($intended_end_user_role)) {
      foreach ($intended_end_user_role as $item) {
        if (is_null($this->addIntendedEndUserRole($intended_end_user_role))) {
          return NULL;
        }
      }
      return;
    }
    if (!is_object($intended_end_user_role)) {
      $intended_end_user_role = new ArchibaldLomDataVocabulary($intended_end_user_role);
    }
    $this->intendedEndUserRole[] = $intended_end_user_role;
  }

  /**
   * remove a keyword by ArchibaldLomDataVocabulary or integer array id
   *
   * @param ArchibaldLomDataVocabulary $keyword
   *
   * @return boolean status
   */
  public function removeIntendedEndUserRole($intended_end_user_role) {
    if (!($intended_end_user_role instanceof ArchibaldLomDataVocabulary)) {
      if (is_numeric($intended_end_user_role) && isset($this->intendedEndUserRole[$intended_end_user_role])) {

        unset($this->intendedEndUserRole[$intended_end_user_role]);
        return TRUE;
      }
      return FALSE;
    }

    $key = $intended_end_user_role->getId();

    foreach ($this->intendedEndUserRole as $i => $cur_intended_end_user_role) {
      $cur_key = $cur_intended_end_user_role->getId();

      if ($key == $cur_key) {
        unset($this->intendedEndUserRole[$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  public function getContext($id = NULL) {
    if ($id !== NULL && isset($this->context[$id])) {
      return $this->context[$id];
    }
    return $this->context;
  }

  public function setContext($context, $id = NULL) {
    if ($id !== NULL) {
      $this->context[$id] = $context;
      return;
    }
    $this->context = $context;
  }

  public function addContext($context) {
    if (!is_array($this->context)) {
      $this->context = array();
    }

    if (is_array($context)) {
      foreach ($context as $item) {
        if (is_null($this->addContext($context))) {
          return NULL;
        }
      }
      return;
    }

    if (!is_object($context)) {
      $context = new ArchibaldLomDataVocabulary($context);
    }

    $this->context[] = $context;
  }

  /**
   * remove a context by ArchibaldLomDataVocabulary or integer array id
   *
   * @param ArchibaldLomDataVocabulary $context
   *
   * @return boolean status
   */
  public function removeContext($context) {
    if (!($context instanceof ArchibaldLomDataVocabulary)) {
      if (is_numeric($context) && isset($this->context[$context])) {
        unset($this->context[$context]);
        return TRUE;
      }
      return FALSE;
    }

    $key = $context->getId();

    foreach ($this->context as $i => $cur_context) {
      $cur_key = $cur_context->getId();

      if ($key == $cur_key) {
        unset($this->context[$i]);
        return TRUE;
      }
    }

    return FALSE;
  }

  public function getTypicalAgeRange($id = NULL) {
    if ($id !== NULL && isset($this->typicalAgeRange[$id])) {
      return $this->typicalAgeRange[$id];
    }
    return $this->typicalAgeRange;
  }

  public function setTypicalAgeRange($typical_age_range, $id = NULL) {
    if ($id !== NULL) {
      $this->typicalAgeRange[$id] = $typical_age_range;
      return;
    }
    $this->typicalAgeRange = $typical_age_range;
  }

  public function addTypicalAgeRange($typical_age_range) {
    if (!is_array($this->typicalAgeRange)) {
      $this->typicalAgeRange = array();
    }
    if (is_array($typical_age_range)) {
      foreach ($typical_age_range as $item) {
        if (is_null($this->addTypicalAgeRange($item))) {
          return NULL;
        }
      }
    }
    $this->typicalAgeRange[] = $typical_age_range;
  }

  public function getDifficult() {
    return $this->difficult;
  }

  public function setDifficult($difficult) {
    if (!is_object($difficult)) {
      $difficult = new ArchibaldLomDataVocabulary($difficult);
    }
    $this->difficult = $difficult;
  }

  public function getTypicalLearningTime() {
    return $this->typicalLearningTime;
  }

  public function setTypicalLearningTime($typicalLearningTime) {
    if (!is_object($typicalLearningTime)) {
      $typicalLearningTime = new ArchibaldLomDataVocabulary($typicalLearningTime);
    }
    $this->typicalLearningTime = $typicalLearningTime;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }

  public function getLanguage($id = NULL) {
    if ($id !== NULL && isset($this->language[$id])) {
      return $this->language[$id];
    }
    return $this->language;
  }

  public function setLanguage($language, $id = NULL) {
    if ($id !== NULL) {
      $this->language[$id] = $language;
      return;
    }
    $this->language = $language;
  }

  /**
   * adds a language as a 5 digit char code
   *
   * @param String $language
   */
  public function addLanguage($language) {
    if (!is_array($this->language)) {
      $this->language = array();
    }
    if (is_array($language)) {
      foreach ($language as $item) {
        if (is_null($this->addLanguage($language))) {
          return NULL;
        }
      }
    }
    $this->language[] = $language;
  }
}

/**
 * ArchibaldLomDataRights default max 1
 */
class ArchibaldLomDataRights extends ArchibaldAbstractDataManager {

  /**
   * does the resource cost something?
   */
  const COST_YES = 'yes';
  const COST_NO = 'no';

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $cost = ArchibaldLomDataRights::COST_NO;

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $description = NULL;

  public function __construct() {
    parent::__construct();
    $this->cost = new ArchibaldLomDataVocabulary(ArchibaldLomDataRights::COST_NO);
    $this->description = new ArchibaldLomDataLangString();
  }

  public function getCost() {
    return $this->cost;
  }

  public function setCost($cost) {
    if ($cost instanceof ArchibaldLomDataVocabulary) {
      $this->cost = $cost;
      return;
    }
    $this->cost = new ArchibaldLomDataVocabulary($cost);
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }
}

/*
  .########..########.##..........###....########.####..#######..##....##..######.
  .##.....##.##.......##.........##.##......##.....##..##.....##.###...##.##....##
  .##.....##.##.......##........##...##.....##.....##..##.....##.####..##.##......
  .########..######...##.......##.....##....##.....##..##.....##.##.##.##..######.
  .##...##...##.......##.......#########....##.....##..##.....##.##..####.......##
  .##....##..##.......##.......##.....##....##.....##..##.....##.##...###.##....##
  .##.....##.########.########.##.....##....##....####..#######..##....##..######.
*/

/**
 * ArchibaldLomDataRelation default max 100
 */
class ArchibaldLomDataRelation extends ArchibaldAbstractDataManager {


  /**
   * @var String
   */
  public $relationId = NULL;

  /**
   * ArchibaldLomDataVocabulary, mandatory, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $kind = NULL;

  /**
   * ArchibaldLomDataVocabulary, mandatory, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $catalog = NULL;

  /**
   * @var String
   */
  public $value = NULL;

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var String
   */
  public $description = NULL;

  public function __construct() {
    parent::__construct();
    $this->kind = new ArchibaldLomDataVocabulary();
    $this->catalog = new ArchibaldLomDataVocabulary();
    $this->description = new ArchibaldLomDataLangString();
  }

  public function getRelationId() {
    return $this->relationId;
  }

  public function setRelationId($relationId) {
    $this->relationId = $relationId;
  }

  public function getKind() {
    return $this->kind;
  }

  public function setKind($kind) {
    $this->kind = $kind;
  }

  public function getValue() {
    return $this->value;
  }

  public function setValue($value) {
    $this->value = $value;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }

  public function getCatalog() {
    return $this->catalog;
  }

  public function setCatalog($catalog) {
    $this->catalog = $catalog;
  }
}

/**
 * ArchibaldLomDataResource default max 1
 */
class ArchibaldLomDataResource extends ArchibaldAbstractDataManager {

  /**
   * ArchibaldLomDataIdentifier, optional, max 10
   * @var array
   */
  public $identifier = array();

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var String
   */
  public $description = NULL;

  public function __construct() {
    parent::__construct();
    $this->description = new ArchibaldLomDataLangString();
  }


  public function getIdentifier($id = NULL) {
    if ($id !== NULL && isset($this->identifier[$id])) {
      return $this->identifier[$id];
    }
    return $this->identifier;
  }

  public function setIdentifier($identifier, $id = NULL) {
    if ($id !== NULL) {
      $this->identifier[$id] = $identifier;
      return;
    }
    $this->identifier = $identifier;
  }

  public function addIdentifier($identifier) {
    if (!is_array($this->identifier)) {
      $this->identifier = array();
    }
    if (is_array($identifier)) {
      foreach ($identifier as $item) {
        if (is_null($this->addIdentifier($identifier))) {
          return NULL;
        }
      }
    }

    $this->identifier[] = $identifier;
  }

  public function getDescription($id = NULL) {
    if ($id !== NULL && isset($this->description[$id])) {
      return $this->description[$id];
    }
    return $this->description;
  }

  public function setDescription($description, $id = NULL) {
    if ($id !== NULL) {
      $this->description[$id] = $description;
      return;
    }
    $this->description = $description;
  }

  public function addDescription($description) {
    if (!is_array($this->description)) {
      $this->description = array();
    }
    if (is_array($description)) {
      foreach ($description as $item) {
        if (is_null($this->addDescription($description))) {
          return NULL;
        }
      }
    }
    $this->description[] = $description;
  }
}

/**
 * ArchibaldLomDataAnnotation default max 30
 */
class ArchibaldLomDataAnnotation extends ArchibaldAbstractDataManager {

  /**
   * @var int
   */
  public $annotationId = '';

  /**
   * @var int
   */
  public $lomId = '';

  /**
   * char_string, optional, max 1 @as vcard
   * @var string
   */
  public $entity = '';

  /**
   * ArchibaldLomDataDateTime, optional, max 1
   * @var ArchibaldLomDataDateTime
   */
  public $date = NULL;

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $description = NULL;

  public function __construct() {
    parent::__construct();
    $this->date = new ArchibaldLomDataDateTime();
    $this->description = new ArchibaldLomDataLangString();
  }

  public function getEntity() {
    return $this->entity;
  }

  public function setEntity($entitiy) {
    $this->entity = $entitiy;
  }

  /**
   *
   * @return ArchibaldLomDataDateTime
   */
  public function getDate() {
    return $this->date;
  }

  public function setDate($date) {
    $this->date = $date;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }
}

/**
 * ArchibaldLomDataClassification default max 40
 */
class ArchibaldLomDataClassification extends ArchibaldAbstractDataManager {

  /**
   * which purpose should this resource have after completing it?
   */
  const PURPOSE_DISCIPLINE = 'discipline';
  const PURPOSE_IDEA = 'idea';
  const PURPOSE_PREREQUISITE = 'prerequisite';
  const PURPOSE_EDUCATIONAL_OBJECTIVE = 'educational objective';
  const PURPOSE_ACCESSIBILITY_RESTRICTIONS = 'accessibility restrictions';
  const PURPOSE_EDUCATIONAL_LEVEL = 'educational level';
  const PURPOSE_SKILL_LEVEL = 'skill level';
  const PURPOSE_SECURITY_LEVEL = 'securitiy level';
  const PURPOSE_COMPETENCY = 'competency';

  /**
   * @var int
   */
  public $classificationId = '';

  /**
   * @var int
   */
  public $lomId = '';

  /**
   * ArchibaldLomDataVocabulary, optional, max 1
   * @var ArchibaldLomDataVocabulary
   */
  public $purpose = NULL;

  /**
   * ArchibaldLomDataTaxonPath, optional, max 15
   * @var array
   */
  public $taxonPath = array();

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $description = NULL;

  /**
   * ArchibaldLomDataLangString, optional, max 40 ordered
   * @var array
   */
  public $keyword = array();

  public function __construct() {
    parent::__construct();
    $this->purpose = new ArchibaldLomDataVocabulary();
    $this->description = new ArchibaldLomDataLangString();
  }

  public function getPurpose() {
    return $this->purpose;
  }

  public function setPurpose($purpose) {
    if ($purpose instanceof ArchibaldLomDataVocabulary) {
      $this->purpose = $purpose;
      return;
    }
    $this->purpose = new ArchibaldLomDataVocabulary($purpose);
  }

  public function getTaxonPath($id = NULL) {
    if ($id !== NULL && isset($this->taxonPath[$id])) {
      return $this->taxonPath[$id];
    }
    return $this->taxonPath;
  }

  public function setTaxonPath($taxon_path, $id = NULL) {
    if ($id !== NULL) {
      $this->taxonPath[$id] = $taxon_path;
      return;
    }
    $this->taxonPath = $taxon_path;
  }

  public function addTaxonPath($taxon_path) {
    if (!is_array($this->taxonPath)) {
      $this->taxonPath = array();
    }
    if (is_array($taxon_path)) {
      foreach ($taxon_path as $item) {
        if (is_null($this->addTaxonPath($taxon_path))) {
          return NULL;
        }
      }
    }
    $this->taxonPath[] = $taxon_path;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setDescription($description) {
    $this->description = $description;
  }

  public function getKeyword($id = NULL) {
    if ($id !== NULL && isset($this->keyword[$id])) {
      return $this->keyword[$id];
    }
    return $this->keyword;
  }

  public function setKeyword($keyword, $id = NULL) {
    if ($id !== NULL) {
      $this->keyword[$id] = $keyword;
      return;
    }
    $this->keyword = $keyword;
  }

  public function addKeyword($keyword) {
    if (!is_array($this->keyword)) {
      $this->keyword = array();
    }
    if (is_array($keyword)) {
      foreach ($keyword as $item) {
        if (is_null($this->addKeyword($keyword))) {
          return NULL;
        }
      }
    }
    $this->keyword[] = $keyword;
  }
}

/**
 * ArchibaldLomDataTaxonPath default max 15
 */
class ArchibaldLomDataTaxonPath extends ArchibaldAbstractDataManager {

  /**
   * @var int
   */
  public $taxonPathId = '';

  /**
   * ArchibaldLomDataLangString, optional, max 1
   * @var ArchibaldLomDataLangString
   */
  public $source = NULL;

  /**
   * ArchibaldLomDataTaxon, optional, max 15 ordered
   * @var array
   */
  public $taxon = array();

  public function __construct() {
    parent::__construct();
    $this->source = new ArchibaldLomDataLangString();
  }

  public function getSource() {
    return $this->source;
  }

  public function setSource($source) {
    $this->source = $source;
  }

  public function getTaxon($id = NULL) {
    if ($id !== NULL && isset($this->taxon[$id])) {
      return $this->taxon[$id];
    }
    return $this->taxon;
  }

  public function setTaxon($taxon, $id = NULL) {
    if ($id !== NULL) {
      $this->taxon[$id] = $taxon;
      return;
    }
    $this->taxon = $taxon;
  }

  public function addTaxon($taxon) {
    if (!is_array($this->taxon)) {
      $this->taxon = array();
    }
    if (is_array($taxon)) {
      foreach ($taxon as $item) {
        if (is_null($this->addTaxon($taxon))) {
          return NULL;
        }
      }
    }
    $this->taxon[] = $taxon;
  }
}

/**
 * ArchibaldLomDataTaxon default max 15
 */
class ArchibaldLomDataTaxon extends ArchibaldAbstractDataManager {

  /**
   * @var int
   */
  public $taxonPathId = '';

  /**
   * char_string, optional, max 1
   * @var string
   */
  public $id = '';

  /**
   * ArchibaldLomDataLangString, required, max 1
   * @var ArchibaldLomDataLangString
   */
  public $entry = NULL;

  public function __construct($id = '', $entry = '') {
    parent::__construct();
    $this->entry = new ArchibaldLomDataLangString();
    if (!is_object($entry)) {
      $this->entry->setString($entry, 'en');
    }

    $this->setId($id);
  }

  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }

  public function getEntry() {
    return $this->entry;
  }

  public function setEntry($entry) {
    $this->entry = $entry;
  }
}

/**
 * ArchibaldLomDataCurriculum default max 40
 */
class ArchibaldLomDataCurriculum extends ArchibaldAbstractDataManager {

  /**
   * identifier in database
   * @var int
   */
  public $curriculumId = '';

  /**
   * id of alloced ArchibaldLom object
   * @var int
   */
  public $lomId = '';

  /**
   * char_string, required, max 1
   * source of curriculum
   * example:
   *  per
   *  lp21
   *  ....
   * @var string
   */
  public $source = NULL;

  /**
   * char_string, required, max 1
   * json encoded curriculum paylod data
   * see all
   *  includes/curriculum/curriculum_*.class.php
   *  extending ArchibaldCurriculumAdvanced
   * @var string
   */
  public $entity = NULL;

  public function __construct() {
    parent::__construct();
  }

  /**
   * get source string
   * example:
   *  per
   *  lp21
   *  ....
   *
   * @return string
   */
  public function getSource() {
    return $this->source;
  }

  /**
   * set source string
   * example:
   *  per
   *  lp21
   *  ....
   *
   * @param string $source
   */
  public function setSource($source) {
    $this->source = $source;
  }

  /**
   * get entity string
   * returning json enjoded curriculum payload
   *
   * @return string
   */
  public function getEntity() {
    return $this->entity;
  }

  /**
   * set entity string
   * exepect json enjoded curriculum payload
   *
   * @param string $entity
   */
  public function setEntity($entity) {
    $this->entity = $entity;
  }
}

/**
 * ArchibaldLomDataLocation
 *
 */
class ArchibaldLomDataLocation extends ArchibaldAbstractDataManager {

  /**
   * value
   * @var string
   */
  public $value = '';

  /**
   * type of value (is it a URL or a simple text?)
   * @var string
   */
  public $type = '';

  public function __construct($value = "", $type = NULL) {
    parent::__construct();
    $this->value = $value;
    $this->type = $this->getType();
  }

  public function getType() {
    return ( filter_var($this->getValue(), FILTER_VALIDATE_URL) ) ? 'url' : 'text';
  }

  public function setType($type) {
    $this->type = $type;
  }

  public function getValue() {
    return $this->value;
  }

  public function setValue($value) {
    $this->value = $value;
  }
}


/**
 * abstract class for initing ArchibaldLom and sub objects
 */
class ArchibaldAbstractDataManager {

  protected $fields = array();

  public function __construct() {
    $this->fields = $this->getFields();
  }

  /**
   * get list of all values of current object instance
   *
   * @return array
   */
  public function getValues() {
    $values = array();
    foreach ($this->fields as $field) {

      $field_underline = NULL;
      if (preg_match('/[A-Z]/', $field)) {
        // convert lower cammelCase to underline
        $field_underline = preg_replace_callback(
          '/([A-Z]{1}[a-z]{1})/',
          array('ArchibaldLom', 'convertPregReplaceResultToUnderlinePlusLowerCase'),
          $field
        );
        $field_underline = drupal_strtolower($field_underline);
      }


      $tmp_value = $this->{$field};
      if (is_array($tmp_value)) {
        foreach ($tmp_value as $k => $tmp_val) {
          if (is_object($tmp_val)) {
            $values[$field][$k] = $tmp_val->getValues();
            if (!empty($field_underline)) {
              $values[$field_underline][$k] = $tmp_val->getValues();
            }
            continue;
          }

          $values[$field][$k] = $tmp_val;
          if (!empty($field_underline)) {
            $values[$field_underline][$k] = $tmp_val;
          }
        }
        continue;
      }
      if (is_object($tmp_value)) {
        $values[$field] = $tmp_value->getValues();
        if (!empty($field_underline)) {
          $values[$field_underline][$k] = $tmp_value->getValues();
        }

        continue;
      }

      $values[$field] = $tmp_value;
      if (!empty($field_underline)) {
        $values[$field_underline][$k] = $tmp_val;
      }
    }


    return $values;
  }

  /**
   * set all data fields of current object instance
   * with data from array
   *
   * @param array $values
   */
  public function setFields($values) {
    if ($values instanceof stdClass) {
      $values = get_object_vars($values);
    }
    if (!is_array($values)) {
      $values = array();
    }
    foreach ($values as $field => $value) {

      if (strpos($field, '_') !== FALSE) {
        // convert underline to lower cammelCase
        $field = preg_replace_callback(
          '/_([a-z]{1})/i',
          array('ArchibaldLom', 'convertPregReplaceResultToUpperCase'),
          $field
        );
        $field = lcfirst($field);
      }

      if (!isset($this->fields[$field])) {
        continue;
      }

      $this->{$field} = $value;
    }
  }

  /**
   * Helper function for setFields() called by preg_replace_callback().
   *
   * @param array $match
   *   Array returned from preg_replace_callback()
   *
   * @return string
   *   The first elemtnet of given array, converted to uppercase.
   */
  public static function convertPregReplaceResultToUpperCase($match) {
    return drupal_strtoupper($match[1]);
  }


  /**
   * Helper function for getValues() called by preg_replace_callback().
   *
   * @param array $match
   *   Array returned from preg_replace_callback()
   *
   * @return string
   *   The first elemtnet of given array, converted to lovercase,
   *   prefixed with a "_".
   */
  public static function convertPregReplaceResultToUnderlinePlusLowerCase($match) {
    return '_' . drupal_strtolower($match[1]);
  }

  /**
   * get a list of all exiting data field of current object instance
   *
   * @return array
   */
  public function getFields() {
    $reflection  = new ReflectionClass($this);
    $vars        = array_keys($reflection->getdefaultProperties());
    $reflection  = new ReflectionClass(__CLASS__);
    $parent_vars = array_keys($reflection->getdefaultProperties());

    $my_child_vars = array();
    foreach ($vars as $key) {
      if (!in_array($key, $parent_vars)) {
        $my_child_vars[$key] = $key;
      }
    }

    return $my_child_vars;
  }

  /**
   * magic get methode
   * @param string $field
   * @return mixed
   */
  public function __get($field) {
    if (strpos($field, '_') !== FALSE) {
      // convert underline to lower cammelCase
      $field = preg_replace_callback(
        '/_([a-z]{1})/i',
        array('ArchibaldLom', 'convertPregReplaceResultToUpperCase'),
        $field
      );
      $field = lcfirst($field);
    }

    return (isset($this->$field)) ? $this->$field : '';
  }

  /**
   * magic get methode
   * @param string $field
   * @param mixed $value
   * @return NULL
   */
  public function __set($field, $value) {
    if (strpos($field, '_') !== FALSE) {
      // convert underline to lower cammelCase
      $field = preg_replace_callback(
        '/_([a-z]{1})/i',
        array('ArchibaldLom', 'convertPregReplaceResultToUpperCase'),
        $field
      );
      $field = lcfirst($field);
    }

    $this->$field = $value;
  }

  /**
   * convert data type
   *
   * @param mixed $value
   *   input value
   * @param string $type
   *   type to convert
   *   see ArchibaldDbStruct::T_*
   *
   * @return mixed
   */
  public function parseValue($value, $type) {
    switch (intval($type)) {
      case ArchibaldDbStruct::T_TEXT:
      case ArchibaldDbStruct::T_DATE:
      case ArchibaldDbStruct::T_VARCHAR:
      case ArchibaldDbStruct::T_STRING:
        return '' . $value;

      case ArchibaldDbStruct::T_TINYINT:
      case ArchibaldDbStruct::T_BIGINT:
      case ArchibaldDbStruct::T_MEDIUMINT:
      case ArchibaldDbStruct::T_SMALLINT:
      case ArchibaldDbStruct::T_INT:
        return intval($value);

      case ArchibaldDbStruct::T_DECIMAL:
      case ArchibaldDbStruct::T_FLOAT:
        return floatval(str_replace(",", ".", $value));

      default:
        return $value;
    }
  }
}

