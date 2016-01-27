<?php

/**
 * @file
 * class for indexing lom resources into the solr index
 */

/**
 * require ArchibaldLomContributor
 * for rendering vCards to plain data
 *
 * this class could used without drupal
 * only replace ArchibaldLomSolrDrupal with abstraction for your system
 *
 */
require_once dirname(__FILE__) . '/LomContributor.class.php';

/**
 * require ArchibaldLomPublish
 * for loading lom object from database
 */
require_once dirname(__FILE__) . '/LomPublish.class.php';

/**
 * require curriculum_educa
 * for getting default curriculum values
 */
require_once dirname(__FILE__) . '/curriculum/curriculum_educa.class.php';

/**
 * require curriculum_per
 * for getting per curriculum values
 */
require_once dirname(__FILE__) . '/curriculum/curriculum_per.class.php';

/**
 * lom to solr document converter
 *
 * @author Heiko Henning <h.henning@educa.ch>
 */
class ArchibaldLomSolr extends ArchibaldLomSolrDrupal {

  /**
   * a list of processed lom objects
   *  array( lom_id=>version,
   *         lom_id=>version,
   *         .....
   *       )
   * @var array
   */
  public $processed = array();

  /**
   * a list apache solr documents
   * @var array
   */
  protected $solrDocs = array();

  /**
   * start generating solr objects form lom object
   *
   * @param string $lom_id
   * @param array $documents_to_delete
   *   called by refference
   *   list lom_id`s with are to delte from index
   *
   * @return boolean status
   */
  public function index($lom_id, &$documents_to_delete = array()) {
    $version_to_index = ArchibaldLomSaveHandler::getLocalPublished($lom_id);

    // if it is not published, dont index it
    if (!empty($version_to_index)) {
      $lom = $this->loadLom($lom_id, $version_to_index);
      if (!($lom instanceof ArchibaldLom)) {
        return FALSE;
      }

      if ($lom->getLifeCycle()->getStatus() == 'deleted') {
        $status_valid_for_indexing = FALSE;
      }
      else {
        $status_valid_for_indexing = TRUE;
      }
    }

    //Resource is not published locally so do not index it
    else {
      $status_valid_for_indexing = FALSE;
      $lom = $this->loadLom($lom_id);
      if (!($lom instanceof ArchibaldLom)) {
        return FALSE;
      }
    }

    if (!$status_valid_for_indexing) {
      $status_val = $lom->getLifeCycle()->getStatus();
      $this->log('Delete @lom_id for @status',  array('@lom_id' => $lom_id, '@status' => $status_val), 'ok');

      $this->processed[$lom->getLomId()] = $lom->getVersion();

      return 'delete';
    }

    // determine all possible languages for this object
    $langs = array_intersect(
      array_keys($lom->getGeneral()->getTitle()->getStrings()),
      array_keys($lom->getGeneral()->getDescription()->getStrings())
    );

    if (empty($langs)) {
      $this->log('No processable languages found for @lom_id', array('@lom_id' => $lom_id), 'error');
    }

    foreach ($langs as $lang) {
      $this->log('Process @lom_id for language: @lang', array('@lom_id' => $lom_id, '@lang' => $lang), 'ok');

      $document = $this->indexCollectionHead($lom, $lang, 0);

      $this->processed[$lom->getLomId()] = $lom->getVersion();

      $this->postprocess($document);
      $this->solrDocs[] = $document;
    }

    return 'insert';
  }

  /**
   * create a new solr object and index it
   *
   * @param ArchibaldLom $lom
   * @param string $lang
   * @param integer $collection_type
   *
   * ***** Solr Object Structur *****
   * attribute name prefixes:
   *
   * ss = string single
   * sm = string multi

   * ts = text singe
   * tm = text multi
   *
   * hs = integer single
   * hm = integer multi
   *
   * hts = tiny integer single
   * htm = tiny integer multi
   *
   * For further informations
   * @see sites/all/moduels/apachesolr/schema.xml
   *
   * *******************************
   *
   * entity_type = lom
   * ss_entity_id  = lom_id
   * label = general.title
   * path = archibald/$lom_id
   * content = general.description
   * teaser = substr(general.description, 0, 300)...
   *
   * tm_keywords
   * tm_coverage
   * tags_keywords = implode(' ', tm_keywords);
   * tags_coverage = implode(' ', tm_coverage);
   *
   * ss_version
   * ss_location
   *
   * twm_languages
   *
   * hs_preview_image
   *
   * ts_other_plattform_requirements
   * ts_education_description
   *
   * tm_contributor
   *
   * sm_learning_resourcetype
   * sm_intendet_enduserrole
   *
   * htm_typical_age_range
   *      here we store each single year with is in selcted range
   *      infinity is 99
   * ss_typical_age_range    here we store the value like in lom object
   *
   * ss_difficulty here the search need to search for multi values
   *
   * hs_typical_learningtime  value in minutes
   *
   * hts_cost  1/0
   *
   * sm_context
   *
   * sm_education_level
   * sm_discipline
   *      Both from default curriculum and reverse selected
   *
   * Dont store: rights_description
   *
   * hts_collection
   *      0 = not in collection
   *      1 = in a collection
   *      2 = in a collection and collection head
   *
   */
  protected function indexCollectionHead($lom, $lang, $collection_type) {
    $document = new ApacheSolrDocument();
    $document->id = apachesolr_document_id(
      $lom->getLomId() . '/' . $lang, 'lom'
    );

    $lom_version_date = db_select('archibald_lom', 'l')
      ->fields('l', array('save_time'))
      ->condition('lom_id', $lom->getLomId())
      ->condition('version', $lom->getVersion())
      ->execute()->fetchAssoc();

    $document->site = url(NULL, array('absolute' => TRUE));
    $document->hash = apachesolr_site_hash();
    $document->ds_changed = date("Y-m-d", $lom_version_date['save_time']) . 'T' . date('H:i:s', $lom_version_date['save_time']) . 'Z';
    $document->entity_type = 'lom';
    // here we only can store numeric long values
    $document->entity_id = 0;
    $document->ss_entity_id = $lom->getLomId();

    $document->hts_collection = $collection_type;

    // process title
    $lom_title = $lom->getGeneral()->getTitle()->getString($lang);
    $document->label = $this->cleanText($lom_title);

    $path           = 'archibald/' . $lom->getLomId();
    $document->url  = url($path, array('absolute' => TRUE));
    $document->path = $path;

    // process description
    $lom_text          = $lom->getGeneral()->getDescription()->getString($lang);
    $document->content = $this->cleanText($lom_text);
    $document->teaser  = truncate_utf8(strtr($document->content, array("\n" => ' ', "\r" => '')), 300, TRUE);

    $document->ss_language = $lang;

    $this->processKeywords($lom, $document, $lang);

    // process version and location
    $value = ArchibaldLom::getLangstringText($lom->getLifeCycle()->getVersion());
    if (!empty($value)) {
      $document->setField('ss_version', $value);
    }

    $value = (array) $lom->getTechnical()->getLocation(0);
    if (!empty($value)) {
      $document->setField('ss_location', $value['value']);
    }

    // process languages
    foreach ($lom->getGeneral()->getLanguage() as $general_language) {
      $document->setMultiValue('twm_languages', $general_language);
    }

    // process other Plattform Requirements
    $lang_string = $lom->getTechnical()->getOtherPlattformRequirements();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $text = $this->cleanText($lang_string->getString($lang));
      if (!empty($text)) {
        $document->setField('ts_other_plattform_requirements', $text);
      }
    }

    // process educational description
    $lang_string = $lom->getEducation()->getDescription();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $text = $this->cleanText($lang_string->getString($lang));
      if (!empty($text)) {
        $document->setField('ts_education_description', $text);
      }
    }

    // process contributor
    foreach ($lom->getLifeCycle()->getContribute() as $contributor_id => $contribute) {
      $tm_contributor = '';
      /*
       * $role = archibald_get_term_by_key($contribute->getRole(), 'lc_cont_role', TRUE);
       * $tm_contributor  .= $role[$lang].' ';
       */


      $contributor = new ArchibaldLomContributor($contribute->entity[0]);
      $contributor_data = $contributor->getData();
      foreach ($contributor_data as $entety) {
        $tm_contributor .= $entety . ' ';
      }
      $author_name = '';
      if (!empty($contributor_data['firstname'])) {
        $author_name .= ' ' . $contributor_data['firstname'];
      }
      if (!empty($contributor_data['lastname'])) {
        $author_name .= ' ' . $contributor_data['lastname'];
      }
      if (!empty($contributor_data['organisation'])) {
        $author_name .= ' ' . $contributor_data['organisation'];
      }
      $author_name = trim($author_name);
      if (!empty($author_name)) {
        $document->setMultiValue('sm_author', $this->cleanText($author_name));
      }

      $document->setMultiValue('tm_contributor', $this->cleanText($tm_contributor));
    }

    // process learning resource types
    $opts = array('documentary', 'pedagogical');
    foreach ($opts as $key => $type) {
      $this->processVocabular(
        $document, 'sm_learning_resourcetype', $lom->getEducation()->getLearningResourceType($type), 'learning_resourcetype', $lang
      );
    }

    // process intended end user role
    $this->processVocabular(
      $document, 'sm_intendet_enduserrole', $lom->getEducation()->getIntendedEndUserRole(), 'intendet_enduserrole', $lang
    );

    // process preview image
    $default_value = $lom->getTechnical()->getPreviewImage()->getImage();
    preg_match('/archibald_file\/([0-9]+)\//', $default_value, $old_fid);
    $document->setField('hs_preview_image', (isset($old_fid[1])) ? $old_fid[1] : 0);

    // process typical age range
    $lang_string = $lom->getEducation()->getTypicalAgeRange(0);
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $document->setField('ss_typical_age_range', $lang_string->getString('en'));

      if (preg_match('/^([\-U0-9]{0,3})\-([\-U0-9]{0,3})$/i', (string) $lang_string->getString('en'), $hits)) {
        $default_value_from = min((INT) $hits[1], (INT) $hits[2]);
        $default_value_to = max((INT) $hits[1], (INT) $hits[2]);
      }
      if (empty($default_value_from)) {
        $default_value_from = 0;
      }
      if (empty($default_value_to)) {
        $default_value_to = 99;
      }
      for ($j = $default_value_from; $j <= $default_value_to; $j++) {
        $document->setMultiValue('htm_typical_age_range', $j);
      }
    }

    // process educational difficulty
    $vocabulary = $lom->getEducation()->getDifficult();
    if ($vocabulary instanceof ArchibaldLomDataVocabulary) {
      $document->setField('ss_difficulty', $vocabulary->getValue());
      $human_value = archibald_get_term_by_key($vocabulary->getValue(), 'difficulty', FALSE, $lang);

      if (!empty($human_value)) {
        $document->setMultiValue('tm_vocabular_text', $human_value);
      }
    }

    // process typical lerning time
    $typical_learning_time = $lom->getEducation()->getTypicalLearningTime();
    if ($typical_learning_time instanceof ArchibaldLomDataDuration) {
      $document->setField('hs_typical_learningtime', $typical_learning_time->getDurationIn('minutes'));
    }

    // process rights cost
    $vocabulary = $lom->getRights()->getCost();
    if ($vocabulary instanceof ArchibaldLomDataVocabulary) {
      $document->setField('hts_cost', ($vocabulary->getValue() == 'yes') ? 1 : 0);
    }

    // process context
    $document->sm_context = array();
    $this->processVocabular($document, 'sm_context', $lom->getEducation()->getContext(), 'education_context', $lang);

    // process education level and dicipline
    $curr_handler = new ArchibaldCurriculumEduca();
    $curr_handler->setLomClassification($lom->getClassification());
    foreach ($curr_handler->getEntryObjects() as $entry) {
      $vocabular_text = '';
      foreach ($entry->educationalLevelIds as $educational_level) {
        $document->setMultiValue('sm_education_level', $educational_level);

        $human_value = archibald_get_term_by_key($educational_level, 'educa_school_levels', FALSE, $lang);

        if (!empty($human_value)) {
          $vocabular_text .= ' ' . $human_value;
        }
      }
      if (!empty($vocabular_text)) {
        $document->setMultiValue('tm_vocabular_text', trim($vocabular_text));
      }

      $vocabular_text = '';
      foreach ($entry->disciplineIds as $discipline) {
        $document->setMultiValue('sm_discipline', $discipline);

        $human_value = archibald_get_term_by_key($discipline, 'educa_school_subjects', FALSE, $lang);

        if (!empty($human_value)) {
          $vocabular_text .= ' ' . $human_value;
        }
      }
      if (!empty($vocabular_text)) {
        $document->setMultiValue('tm_vocabular_text', trim($vocabular_text));
      }
    }


    // process per curriculum
    $curr_handler = new ArchibaldCurriculumPer();
    $curr_handler->setLomCurriculum($lom->getCurriculum());
    $curr_handler->solrIndex($document);


    // get company logo if it is not default
    $variables['lom'] = &$lom;
    archibald_get_lom_for_template_company_logo($variables);
    if (!empty($variables['company_logo'])) {
      $document->setMultiValue('hs_company_logo', (INT) $variables['company_logo']);
    }

    return $document;
  }


  /**
   * return a list of processed lom objects
   *  array( lom_id=>version,
   *         lom_id=>version,
   *         .....
   *       )
   *
   * @return array
   */
  public function getProcessed() {
    return $this->processed;
  }

  /**
   * return a list solr index documents
   *
   * @return array
   */
  public function getDocs() {
    return $this->solrDocs;
  }

  /**
   * extract keywords from lom object and add it to apache solr object
   *
   * @param ArchibaldLom $lom
   * @param ApacheSolrDocument $document
   * @param string $lang
   */
  protected function processKeywords(ArchibaldLom &$lom, ApacheSolrDocument &$document, $lang) {
    $lom_general = $lom->getGeneral();
    foreach ($lom_general->getKeyword() as $keyword) {
      if ($keyword instanceof ArchibaldLomDataLangString) {
        $string = $keyword->getString($lang);
        if (!empty($string)) {
          $document->setMultiValue('tm_keywords', $string);
        }
      }
    }

    foreach ($lom_general->getCoverage() as $keyword) {
      if ($keyword instanceof ArchibaldLomDataLangString) {
        $string = $keyword->getString($lang);
        if (!empty($string)) {
          $document->setMultiValue('tm_coverage', $string);
        }
      }
    }
  }

  /**
   * post process solr document
   * fill up tags_* variables
   *
   * @param ApacheSolrDocument $document
   */
  protected function postprocess(ApacheSolrDocument &$document) {
    $keywords = $document->getField('tm_keywords');
    if ($keywords != FALSE) {
      $document->setField('tags_keywords', implode(' ', $keywords['value']));
    }

    $keywords = $document->getField('tm_coverage');
    if ($keywords != FALSE) {
      $document->setField('tags_coverage', implode(' ', $keywords['value']));
    }

    $multi_fields = array('sm_label',
      'tm_content',
      'tm_keywords',
      'tm_coverage',
      'tm_contributor',
      'sm_learning_resourcetype',
      'sm_intendet_enduserrole',
      'htm_typical_age_range',
      'sm_context',
      'sm_education_level',
      'sm_discipline',
      'tm_vocabular_text',
      'sm_author',
    );

    // uniqui field values
    foreach ($multi_fields as $multi_field) {
      $data = $document->getField($multi_field);
      if (!empty($data)) {
        $values = array();
        foreach ($data['value'] as $val) {
          $key = md5($val);
          if (isset($values[$key])) {
            $values[$key]['ammount']++;
          }
          else {
            $values[$key] = array('value' => $val, 'ammount' => 1);
          }
        }
        unset($document->$multi_field);
        foreach ($values as $value) {
          // we dont set boost here, cause:
          // An Index-time boost on a value of a multiValued field applies
          // to all values for that field.
          // see http://wiki.apache.org/solr/SolrRelevancyFAQ#index-time_boosts
          $document->setMultiValue($multi_field, $value['value']);
        }
      }
    }
  }

  /**
   * strip all html from text an avoid word gluing, also do br2nl
   *
   * @param string $text
   *
   * @return string
   */
  public function cleanText($text) {
    $text = strtr($text, array("\r" => ''));
    $text = strtr($text, array("<br />\n" => "\n", "<br\>\n" => "\n", "<br \>\n" => "\n"));
    // br2nl
    $text = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $text);

    // add space for and after each html tag to avoid glued words
    $text = str_replace(array('<', '>'), array(' <', '> '), $text);
    $text = strip_tags($text);

    return trim($text);
  }

  /**
   * process a vocabular,
   * save it to lom document and store it also to  vocabular_text
   *
   * @param ApacheSolrDocument $document
   * @param sring $variable
   *   variable of solr docuemtn to store in
   * @param array $vocabular
   *   list of ArchibaldLomDataVocabulary
   * @param string $vocabular_name
   *   name of vocabular fomr ontology::read()
   *   see: ArchibaldLomDataVocabulary::get_human_string()
   * @param string $lang
   */
  protected function processVocabular(ApacheSolrDocument &$document, $variable, $vocabular, $vocabular_name, $lang) {

    $vocabular_text = '';
    foreach ($vocabular as $vocab_entry) {
      $value = $vocab_entry->getValue();
      $human_value = archibald_get_term_by_key($value, $vocabular_name, FALSE, $lang);

      if (!empty($human_value)) {
        // with this if, cause we only like to add option
        // the we realy have in our ontology
        $document->setMultiValue($variable, $value);

        $vocabular_text .= ' ' . $human_value;
      }
    }
    if (!empty($vocabular_text)) {
      $document->setMultiValue('tm_vocabular_text', trim($vocabular_text));
    }
  }
}

abstract class ArchibaldLomSolrDrupal {

  /**
   * load a lom object by id
   *
   * @param string $lom_id
   * @param string $version_id
   *
   * @return ArchibaldLom
   */
  protected function loadLom($lom_id, $version_id = '') {
    $lom = archibald_load($lom_id, $version_id);
    if (!($lom instanceof ArchibaldLom)) {
      return FALSE;
    }

    $lom = ArchibaldLomPublish::completeLomObject($lom);
    $version_id = $lom->getVersion();

    // if it was deleted set lifeCycle status as deleted
    $deleted = db_query(
      'SELECT deleted
        FROM {archibald_lom}
        WHERE lom_id=:lom_id
        AND version=:version
        LIMIT 1',
      array(
        'lom_id' => $lom_id,
        ':version' => $version_id,
      )
    )
    ->fetchColumn(0);

    if (!empty($deleted)) {
      $lom_life_cycle = $lom->getLifeCycle();
      $lom_life_cycle->setStatus(new ArchibaldLomDataVocabulary('deleted'));
      $lom->setLifeCycle($lom_life_cycle);
    }

    return $lom;
  }

  /**
   * log message processor
   *
   * @param string $messages
   *   english message string with vars within
   * @param array $variables
   * @param string $level
   *  ok / notice / error
   */
  protected function log($messages, $variables, $level) {
    if (function_exists('drush_log')) {
      drush_log(t($messages, $variables), $level);
    }

    $watchdog_levels = array(
      'ok' => WATCHDOG_NOTICE,
      'notice' => WATCHDOG_DEBUG,
      'error' => WATCHDOG_ERROR,
    );
    watchdog('solrIndex', $messages, $variables, $watchdog_levels[$level]);
  }
}

