<?php

/**
 * @file
 * Handles the saving for a lom object
 */

include_once dirname(__FILE__) . '/LomContributor.class.php';

/**
 * Description of ArchibaldLomSaveHandler
 *
 * @author prdatur
 */
class ArchibaldLomSaveHandler {

  /**
   * Holds the last created lom version.
   *
   * @var string
   */
  public static $lastLomVersion = "";

  /**
   *
   * @param ArchibaldLom $lom
   *
   * @return string
   *   json formated lom object
   */
  public static function jsonExport(ArchibaldLom $lom, $beautify = FALSE) {
    // This is ugly, but we have had occurences of stacked archibald###archibald###
    $lom->lomId = str_replace('archibald###', '', $lom->lomId);
    $lom->lomId = 'archibald###' . $lom->lomId;
    // if( strpos($lom->lomId, 'archibald###') === false ) {
    //   $lom->lomId = 'archibald###' . $lom->lomId;
    // }
    return ($beautify && defined('JSON_PRETTY_PRINT') ? json_encode($lom, JSON_PRETTY_PRINT) : json_encode($lom));
  }

  /**
   *
   * @param ArchibaldLom $obj
   *
   * @return array
   *   the object as an array
   */
  private static function obj2arr($obj) {
    $arr = array();
    $_arr = is_object($obj) ? get_object_vars($obj) : $obj;
    foreach ($_arr as $key => $val) {
      if ((is_array($val) || is_object($val))) {
        $val = ArchibaldLomSaveHandler::obj2arr($val);
      }

      $arr[$key] = $val;
    }
    return $arr;
  }

  /**
   * converts json formated string into lom class object
   *
   * @param string $json_lom
   *   json formated string
   *
   * @return ArchibaldLom
   *   Object
   */
  public static function jsonImport($json_lom) {
    if (!is_object($json_lom) && !is_array($json_lom)) {
      $json_lom = json_decode($json_lom);
    }

    $json_lom = ArchibaldLomSaveHandler::obj2arr($json_lom);

    $tmp_lom = new ArchibaldLom();
    $tmp_lom->setFields($json_lom);
    $tmp_lom->relation = array();

    $rights = new ArchibaldLomDataRights();
    $rights->setCost(ArchibaldLomSaveHandler::parseVocabular($json_lom['rights']['cost']));
    $rights->setDescription(ArchibaldLomSaveHandler::parseLangString($json_lom['rights']['description']));
    $tmp_lom->rights = $rights;

    $general = new ArchibaldLomDataGeneral();
    $general->setFields($json_lom['general']);
    $general->structure = ArchibaldLomSaveHandler::parseVocabular($json_lom['general']['structure']);
    $general->aggregationLevel = ArchibaldLomSaveHandler::parseVocabular($json_lom['general']['aggregationLevel']);
    $general->title = ArchibaldLomSaveHandler::parseLangString($json_lom['general']['title']);
    $general->description = ArchibaldLomSaveHandler::parseLangString($json_lom['general']['description']);
    $general->identifier = array();
    $general->keyword    = array();
    $general->coverage   = array();
    foreach ($json_lom['general']['identifier'] as $identifier) {
      if (!empty($identifier['title'])) {
        $identifier['title'] = ArchibaldLomSaveHandler::parseLangString($identifier['title']);
      }

      $identi = new ArchibaldLomDataIdentifier();
      $identi->setFields($identifier);
      $general->addIdentifier($identi);
    }


    foreach ($json_lom['general']['keyword'] as $keyword) {
      $general->addKeyword(ArchibaldLomSaveHandler::parseLangString($keyword));
    }
    foreach ($json_lom['general']['coverage'] as $coverage) {
      $general->addCoverage(ArchibaldLomSaveHandler::parseLangString($coverage));
    }

    $tmp_lom->general = $general;

    $lifecycle = new ArchibaldLomDataLifeCycle();
    $lifecycle->setFields($json_lom['lifeCycle']);
    $lifecycle->status = ArchibaldLomSaveHandler::parseVocabular($json_lom['lifeCycle']['status']);
    $lifecycle->version = ArchibaldLomSaveHandler::parseLangString($json_lom['lifeCycle']['version']);
    $lifecycle->contribute = array();

    foreach ($json_lom['lifeCycle']['contribute'] as $val) {
      $lifecycle->addContribute(ArchibaldLomSaveHandler::parseContribute($val));
    }
    $tmp_lom->lifeCycle = $lifecycle;

    $meta_metadata = new ArchibaldLomDataMetaMetadata();
    $meta_metadata->setFields($json_lom['metaMetadata']);
    $meta_metadata->identifier = array();
    $meta_metadata->contribute = array();
    foreach ($json_lom['metaMetadata']['identifier'] as $identifier) {
      $identi = new ArchibaldLomDataIdentifier();
      $identi->setFields($identifier);
      $meta_metadata->addIdentifier($identi);
    }

    foreach ($json_lom['metaMetadata']['contribute'] as $val) {
      $meta_metadata->addContribute(ArchibaldLomSaveHandler::parseContribute($val));
    }
    $tmp_lom->metaMetadata = $meta_metadata;

    $technical = new ArchibaldLomDataTechnical();
    $technical->setFields($json_lom['technical']);

    $technical->format = $json_lom['technical']['format'];
    $technical->size = $json_lom['technical']['size'];

    /*
      .########..########.##..........###....########.####..#######..##....##..######.
      .##.....##.##.......##.........##.##......##.....##..##.....##.###...##.##....##
      .##.....##.##.......##........##...##.....##.....##..##.....##.####..##.##......
      .########..######...##.......##.....##....##.....##..##.....##.##.##.##..######.
      .##...##...##.......##.......#########....##.....##..##.....##.##..####.......##
      .##....##..##.......##.......##.....##....##.....##..##.....##.##...###.##....##
      .##.....##.########.########.##.....##....##....####..#######..##....##..######.
    */

    $res = db_query('
      SELECT *
        FROM {archibald_relations}
        WHERE lom_id = :lom_id
        AND lom_version = :lom_version',
      array(
        ':lom_id' => $lom_db->lom_id,
        ':lom_version' => $lom_db->version,
      )
    );

    while ($relation_db = $res->fetchAssoc()) {
      // print_r( $relation_db['kind'] );
      // print_r( ArchibaldLomSaveHandler::loadVocabular('lom:rel_catalog', $relation_db['relation_id']) );
      // die();
      $tmp_obj = new ArchibaldLomDataRelation();
      $tmp_obj->setKind(ArchibaldLomSaveHandler::loadVocabular('lom:rel_kind', $relation_db['relation_id']));
      $tmp_obj->setCatalog(ArchibaldLomSaveHandler::loadVocabular('lom:rel_catalog', $relation_db['relation_id']));
      $tmp_obj->setValue($relation_db['value']);
      $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($relation_db['description']));
      $lom->addRelation($tmp_obj);
    }


    $technical->requirement = array();
    $technical->installationRemarks = new ArchibaldLomDataLangString();
    $technical->otherPlattformRequirements = ArchibaldLomSaveHandler::parseLangString(
      $json_lom['technical']['otherPlattformRequirements']
    );

    $duration = new ArchibaldLomDataDuration();
    $duration->duration = $json_lom['technical']['duration']['duration'];
    $duration->description = ArchibaldLomSaveHandler::parseLangString($json_lom['technical']['duration']['description']);
    $technical->duration = $duration;

    $tmp_lom->technical = $technical;

    $education = new ArchibaldLomDataEducation();
    $education->setFields($json_lom['education']);

    $education->interactiveType = ArchibaldLomSaveHandler::parseVocabular($json_lom['education']['interactiveType']);
    $education->interactivityLevel = ArchibaldLomSaveHandler::parseVocabular($json_lom['education']['interactivityLevel']);
    $education->semanticDensity = ArchibaldLomSaveHandler::parseVocabular($json_lom['education']['semanticDensity']);
    $education->difficult = ArchibaldLomSaveHandler::parseVocabular($json_lom['education']['difficult']);
    $education->typicalLearningTime = ArchibaldLomSaveHandler::parseVocabular($json_lom['education']['typicalLearningTime']);

    $education->description = ArchibaldLomSaveHandler::parseLangString($json_lom['education']['description']);
    $education->typicalAgeRange = array();
    foreach ($json_lom['education']['typicalAgeRange'] as $typical_age_range) {
      $education->addTypicalAgeRange(ArchibaldLomSaveHandler::parseLangString($typical_age_range));
    }

    $education->language = $json_lom['education']['language'];

    $education->learningResourceType = array();
    $opts = array('documentary', 'pedagogical');
    foreach ($opts as $key => $type) {
      $education->learningResourceType[$type] = array();
      foreach ($json_lom['education']['learningResourceType'][$type] as $val) {
        $education->addLearningResourceType($type, ArchibaldLomSaveHandler::parseVocabular($val));
      }
    }

    $education->intendedEndUserRole = array();
    foreach ($json_lom['education']['intendedEndUserRole'] as $val) {
      $education->addIntendedEndUserRole(ArchibaldLomSaveHandler::parseVocabular($val));
    }

    $education->context = array();
    foreach ($json_lom['education']['context'] as $val) {
      $education->addContext(ArchibaldLomSaveHandler::parseVocabular($val));
    }

    $tmp_lom->education = $education;
    #print_r($tmp_lom->education); die();

    /*
      .########..########.##..........###....########.####..#######..##....##..######.
      .##.....##.##.......##.........##.##......##.....##..##.....##.###...##.##....##
      .##.....##.##.......##........##...##.....##.....##..##.....##.####..##.##......
      .########..######...##.......##.....##....##.....##..##.....##.##.##.##..######.
      .##...##...##.......##.......#########....##.....##..##.....##.##..####.......##
      .##....##..##.......##.......##.....##....##.....##..##.....##.##...###.##....##
      .##.....##.########.########.##.....##....##....####..#######..##....##..######.
    */

    // With LOM-CH 1.1 implementatrion, this will not work
    /*
      $tmp_lom->relation = array();
      foreach ($json_lom['relation'] as $relation) {

        $rel = new ArchibaldLomDataRelation();
        $rel->setFields($relation);
        $rel->kind = ArchibaldLomSaveHandler::parseVocabular($relation['kind']);

        $res = new ArchibaldLomDataResource();
        $res->setFields($relation['resource']);
        $res->description = ArchibaldLomSaveHandler::parseLangString($relation['resource']['description']);
        $identifers = array();
        foreach ($relation['resource']['identifier'] as $identifier) {
          $ident = new ArchibaldLomDataIdentifier();
          $ident->setFields($identifier);
          $identifers[] = $ident;
        }
        $res->identifier    = $identifers;
        $rel->resource      = $res;
        $tmp_lom->relation[] = $rel;
      }
    */

    $tmp_lom->annotation = array();
    foreach ($json_lom['annotation'] as $annotation) {
      $anno = new ArchibaldLomDataAnnotation();
      $anno->setFields($annotation);

      $date = new ArchibaldLomDataDateTime();
      $date->setFields($annotation['date']);
      $date->description = ArchibaldLomSaveHandler::parseLangString($annotation['date']['description']);
      $anno->date = $date;

      $anno->description = ArchibaldLomSaveHandler::parseLangString($annotation['description']);

      $tmp_lom->addAnnotation($anno);
    }
    $tmp_lom->classification = array();
    foreach ($json_lom['classification'] as $classification) {

      $classi = new ArchibaldLomDataClassification();
      $classi->setFields($classification);
      $classi->description = ArchibaldLomSaveHandler::parseLangString($classification['description']);
      $classi->purpose = ArchibaldLomSaveHandler::parseVocabular($classification['purpose']);
      $classi->taxonPath = array();
      $classi->keyword = array();
      foreach ($classification['taxonPath'] as $taxon_path) {

        $taxonp = new ArchibaldLomDataTaxonPath();
        $taxonp->setFields($taxon_path);
        $taxonp->source = ArchibaldLomSaveHandler::parseLangString($taxon_path['source']);
        $taxonp->taxon = array();
        foreach ($taxon_path['taxon'] as $taxon) {
          $tax = new ArchibaldLomDataTaxon();
          $tax->setFields($taxon);
          $tax->entry = ArchibaldLomSaveHandler::parseLangString($taxon['entry']);
          $taxonp->addTaxon($tax);
        }

        $classi->addTaxonPath($taxonp);
      }

      foreach ($classification['keyword'] as $keyword) {
        $classi->addKeyword(ArchibaldLomSaveHandler::parseLangString($keyword));
      }
      $tmp_lom->addClassification($classi);
    }
    $tmp_lom->curriculum = array();
    foreach ($json_lom['curriculum'] as $curriculum) {
      $curri = new ArchibaldLomDataCurriculum();
      $curri->setFields($curriculum);
      $tmp_lom->addCurriculum($curri);
    }
    return $tmp_lom;
  }

  /**
   * parse json array element to ArchibaldLomDataContribute object
   *
   * @param array $array
   *
   * @return ArchibaldLomDataContribute
   */
  private static function parseContribute($array) {

    $con = new ArchibaldLomDataContribute();
    if (is_null($array)) {
      return $con;
    }
    $con->setFields($array);

    $date = new ArchibaldLomDataDateTime();
    $date->setFields($array['date']);
    $date->description = ArchibaldLomSaveHandler::parseLangString($array['date']['description']);
    $con->date = $date;
    $con->role = ArchibaldLomSaveHandler::parseVocabular($array['role']);

    return $con;
  }

  /**
   * parse json array element to ArchibaldLomDataVocabulary object
   *
   * @param array $array
   *
   * @return ArchibaldLomDataVocabulary
   */
  private static function parseVocabular($array) {
    $voc = new ArchibaldLomDataVocabulary();
    if (is_null($array)) {
      return $voc;
    }
    $voc->setFields($array);
    return $voc;
  }

  /**
   * parse json array element to ArchibaldLomDataLangString object
   *
   * @param array $array
   *
   * @return ArchibaldLomDataLangString
   */
  private static function parseLangString($array) {
    $l = new ArchibaldLomDataLangString();
    if (is_null($array)) {
      return $l;
    }
    if (empty($array['strings'])) {
      $array['strings'] = array();
    }
    $l->setFields($array);
    return $l;
  }

  /**
   * delete the given lom from DB
   *
   * @param mixed $lom lomid
   *   as a string or
   *   the complete lom class object
   * @param string $version
   *   the lom version
   * @param boolean $delete_status
   *   Wether to delete the archibald_lom_stats entry too
   *   (optional, default = true)
   */
  public static function purge($lom, $version = 0, $delete_status = TRUE) {

    if (($lom instanceof ArchibaldLom) == FALSE) {
      $lom = ArchibaldLomSaveHandler::load($lom, $version);
    }
    foreach ($lom->getClassification() as $classifications) {
      ArchibaldLomSaveHandler::deleteVocabular('lom:classification', $classifications->classification_id);

      foreach ($classifications->getTaxonPath() as $taxonpaths) {

        $db = db_delete("archibald_taxon")
          ->condition('taxon_path_id', $taxonpaths->taxon_path_id)
          ->execute();

        $db = db_delete("archibald_taxon_path")
          ->condition('taxon_path_id', $taxonpaths->taxon_path_id)
          ->execute();
      }

      $db = db_delete("archibald_classification_taxon")
        ->condition('classification_id', $classifications->classification_id)
        ->execute();
      $db = db_delete("archibald_classification_keywords")
        ->condition('classification_id', $classifications->classification_id)
        ->execute();
      $db = db_delete("archibald_classification")
        ->condition('classification_id', $classifications->classification_id)
        ->execute();
    }

    $lom_id = $lom->getLomId();

    $db = db_delete("archibald_curriculum")
      ->condition('lom_id', $lom_id)
      ->execute();

    $db = db_delete("archibald_annotations")
      ->condition('lom_id', $lom_id)
      ->execute();

    $mmid = $lom->getMetaMetadata()->meta_metadata_id;
    $db = db_delete("archibald_meta_metadata_identifier")
      ->condition('meta_metadata_id', $mmid)
      ->execute();

    foreach ($lom->getMetaMetadata()->getContribute() as $contribute) {
      ArchibaldLomSaveHandler::deleteContribute($contribute, 'meta_metadata');
    }

    $db = db_delete("archibald_meta_metadata_contributes")
      ->condition('meta_metadata_id', $mmid)
      ->execute();
    $db = db_delete("archibald_meta_metadata")
      ->condition('meta_metadata_id', $mmid)
      ->execute();

    ArchibaldLomSaveHandler::deleteVocabular('lom:rights_cost', $lom_id . '_' . $lom->getVersion());

    $eduid = $lom->getEducation()->education_id;
    $db = db_delete('archibald_education_language')
      ->condition('education_id', $eduid)
      ->execute();
    $db = db_delete('archibald_education_typicalagerange')
      ->condition('education_id', $eduid)
      ->execute();

    $db = db_delete('archibald_education')
      ->condition('education_id', $eduid)
      ->execute();
    $db = db_delete('archibald_education_context')
      ->condition('education_id', $eduid)
      ->execute();
    $db = db_delete('archibald_education_intendedenduserrole')
      ->condition('education_id', $eduid)
      ->execute();
    $db = db_delete('archibald_education_learningresourcetype')
      ->condition('education_id', $eduid)
      ->execute();

    ArchibaldLomSaveHandler::deleteVocabular('education:interactiveType', $eduid);
    ArchibaldLomSaveHandler::deleteVocabular('education:interactivityLevel', $eduid);
    ArchibaldLomSaveHandler::deleteVocabular('education:semanticDensity', $eduid);
    ArchibaldLomSaveHandler::deleteVocabular('education:difficult', $eduid);

    ArchibaldLomSaveHandler::deleteVocabular('education:learningResourceType_' . $eduid);
    ArchibaldLomSaveHandler::deleteVocabular('education:intendedEndUserRole_' . $eduid);
    ArchibaldLomSaveHandler::deleteVocabular('education:context_' . $eduid);

    $techid = $lom->getTechnical()->technical_id;
    $db = db_delete("archibald_technical")
      ->condition('technical_id', $techid)
      ->execute();
    $db = db_delete("archibald_technical_locations")
      ->condition('technical_id', $techid)
      ->execute();

    $lifeid = $lom->getLifeCycle()->lifecycle_id;

    $db = db_delete("archibald_lifecycle")
      ->condition('lifecycle_id', $lifeid)
      ->execute();

    $db = db_delete("archibald_lifecycle_contributes")
      ->condition('lifecycle_id', $lifeid)
      ->execute();

    foreach ($lom->getLifeCycle()->getContribute() as $contribute) {
      ArchibaldLomSaveHandler::deleteContribute($contribute, 'lifecycle');
    }

    $genid = $lom->getGeneral()->general_id;


    $db = db_delete("archibald_general")
      ->condition('general_id', $genid)
      ->execute();
    $db = db_delete("archibald_general_identifier")
      ->condition('general_id', $genid)
      ->execute();
    $db = db_delete("archibald_general_languages")
      ->condition('general_id', $genid)
      ->execute();
    $db = db_delete("archibald_general_keywords")
      ->condition('general_id', $genid)
      ->execute();
    $db = db_delete("archibald_general_coverage")
      ->condition('general_id', $genid)
      ->execute();

    $relations = $lom->getRelation();
    if( count($relations) ) {
      foreach ($relations as $relation) {
        $rid = $relation->getRelationId();
        ArchibaldLomSaveHandler::deleteVocabular('lom:rel_kind', $rid);
        ArchibaldLomSaveHandler::deleteVocabular('lom:rel_catalog', $rid);
      }
    }
    $db = db_delete("archibald_relations")->condition('lom_id', $lom_id)
      ->condition('lom_version', $lom->getVersion())
      ->execute();

    ArchibaldLomSaveHandler::deleteVocabular('general:structure', $genid);
    ArchibaldLomSaveHandler::deleteVocabular('general:aggregationLevel', $genid);

    if ($delete_status === TRUE) {
      $db = db_delete("archibald_lom_stats")
        ->condition('lom_id', $lom_id)
        ->execute();
    }

    db_delete("archibald_lom")
      ->condition('lom_id', $lom_id)
      ->condition('version', $lom->getVersion())
      ->execute();

    $lom_revision_exist = db_select('archibald_lom', 'l')
      ->condition('lom_id', $lom_id)
      ->fields('l', array('lom_id'))
      ->execute()
      ->fetchAssoc();

    if (!empty($lom_revision_exist) && isset($lom_revision_exist['lom_id']) && !empty($lom_revision_exist['lom_id'])) {
      $lom_revision_exist = TRUE;
    }
    else {
      $lom_revision_exist = FALSE;
    }

    // Delete comments, but only if the comment module is enabled, the table exist and we have no revision left.
    if (!$lom_revision_exist && module_exists('archibald_comments') && db_table_exists("archibald_comments")) {
      $db = db_delete("archibald_comments")
        ->condition('lom_id', $lom_id)
        ->execute();
    }

    // Delete ratings, but only if the rating module is enabled, the table exist and we have no revision left.
    if (!$lom_revision_exist && module_exists('archibald_ratings') &&db_table_exists("archibald_ratings")) {
      $db = db_delete("archibald_ratings")
        ->condition('lom_id', $lom_id)
        ->execute();
    }
  }

  /**
   * insert ArchibaldLom object into database
   *
   * @param ArchibaldLom $lom
   *   reference
   * @param string $version
   *   called by reference
   *   the new version id after saving
   *
   * @return string
   *   Returns the inserted lomid as a string
   */
  public static function insert(ArchibaldLom &$lom, &$version = '') {
    global $user;

    $userid = $user->uid;
    if (empty($userid)) {
      $userid = 1;
    }

    if (empty($version)) {
      $lom->version = ArchibaldLomSaveHandler::generateUuid();
      $version = $lom->version;
    }
    else {
      self::purge($lom, $version, FALSE);
    }

    ArchibaldLomSaveHandler::$lastLomVersion = $version;

    /**
     * @var ArchibaldLomDataGeneral
     */
    $general = $lom->getGeneral();
    $fields = array(
      'title' => new ArchibaldLomDataLangString(),
      'description' => "",
      'structure' => NULL,
      'aggregationLevel' => NULL,
    );

    if ($general->getTitle() instanceof ArchibaldLomDataLangString) {
      $tmp_title = $general->getTitle();
      $fields['title'] = ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_title);
    }

    if ($general->getDescription() instanceof ArchibaldLomDataLangString) {
      $tmp_description = $general->getDescription();
      $fields['description'] = ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_description);
    }

    if ($general->getStructure() instanceof ArchibaldLomDataVocabulary) {
      $fields['structure'] = $general->getStructure()->getValue();
    }

    if ($general->getAggregationLevel() instanceof ArchibaldLomDataVocabulary) {
      $fields['aggregationLevel'] = $general->getAggregationLevel()->getValue();
    }

    $general_inserted_id = db_insert('archibald_general')
      ->fields($fields)
      ->execute();


    ArchibaldLomSaveHandler::insertVocabular($general->getStructure(), 'general:structure', $general_inserted_id);
    ArchibaldLomSaveHandler::insertVocabular($general->getAggregationLevel(), 'general:aggregationLevel', $general_inserted_id);

    foreach ($general->getIdentifier() as $value) {
      $fields = array(
        'catalog' => $value->getCatalog(),
        'entry' => $value->getEntry(),
        'general_id' => $general_inserted_id,
      );
      if ($value->getTitle() instanceof ArchibaldLomDataLangString) {
        $tmp_title = $value->getTitle();
        $fields['title'] = ArchibaldLomSaveHandler::saveOrInsertLangString(
          $tmp_title
        );
      }

      db_insert("archibald_general_identifier")
        ->fields($fields)
        ->execute();
    }

    foreach ($general->getLanguage() as $value) {
      $fields = array(
        'language' => $value,
        'general_id' => $general_inserted_id,
      );
      db_insert("archibald_general_languages")
        ->fields($fields)
        ->execute();
    }

    $check_duplicate_keywords = array();
    foreach ($general->getKeyword() as $value) {
      $fields = array(
        'keyword' => ArchibaldLomSaveHandler::saveOrInsertLangString($value),
        'general_id' => $general_inserted_id,
      );

      if (!isset($check_duplicate_keywords[$fields['keyword'] . $fields['general_id']])) {
        $check_duplicate_keywords[$fields['keyword'] . $fields['general_id']] = TRUE;
        db_insert("archibald_general_keywords")
          ->fields($fields)
          ->execute();
      }
    }



    foreach ($general->getCoverage() as $value) {
      $fields = array(
        'coverage' => ArchibaldLomSaveHandler::saveOrInsertLangString($value),
        'general_id' => $general_inserted_id,
      );
      db_insert("archibald_general_coverage")
        ->fields($fields)
        ->execute();
    }
    unset($general);

    /**
     * @var ArchibaldLomDataLifeCycle
     */
    $lifecycle = $lom->getLifeCycle();
    $tmp_version = $lifecycle->getVersion();
    $fields = array(
      'version' => ArchibaldLomSaveHandler::saveOrInsertLangString(
        $tmp_version
      ),
      'status' => $lifecycle->getStatus(),
    );

    $lifecycle_inserted_id = db_insert('archibald_lifecycle')
      ->fields($fields)
      ->execute();

    foreach ($lifecycle->getContribute() as $k => $value) {
      $fields = array(
        'contribute_id' => ArchibaldLomSaveHandler::insertContribute(
          $value,
          'lifecycle'
        ),
        'lifecycle_id' => $lifecycle_inserted_id,
      );
      db_insert("archibald_lifecycle_contributes")
        ->fields($fields)
        ->execute();
      $lifecycle->setContribute($value, $k);
    }
    $lifecycle->lifecycleId = $lifecycle_inserted_id;
    $lom->setLifeCycle($lifecycle);
    unset($lifecycle);

    /**
     * @var ArchibaldLomDataTechnical
     */
    $technical_inserted_id = 0;
    $technical = $lom->getTechnical();

    $fields = array(
      'size' => 0,
      'format' => '',
      'other_plattform_requirements' => new ArchibaldLomDataLangString(),
      'duration_duration' => '',
      'duration_description' => new ArchibaldLomDataLangString(),
    );

    if ($technical instanceof ArchibaldLomDataTechnical) {
      $tmp_other_platoform_req = $technical->getOtherPlattformRequirements();
      $tmp_description = $technical->getDuration()->getDescription();
      $fields = array(
        'format' => $technical->getFormat(),
        'size' => $technical->getSize(),
        'other_plattform_requirements' => ArchibaldLomSaveHandler::saveOrInsertLangString(
          $tmp_other_platoform_req
        ),
        'duration_duration' => $technical->getDuration()->getDuration(),
        'duration_description' => ArchibaldLomSaveHandler::saveOrInsertLangString(
          $tmp_description
        ),
        'preview_image' => $technical->getPreviewImage()->getImage(),
        'preview_image_description' => $technical->getPreviewImage()->getCopyrightDescription()
      );

      $technical_inserted_id = db_insert('archibald_technical')
        ->fields($fields)
        ->execute();

      foreach (@$technical->getLocation() as $location) {
        $fields = array(
          'location' => $location->getValue(),
          'technical_id' => $technical_inserted_id,
        );
        db_insert('archibald_technical_locations')
          ->fields($fields)
          ->execute();
      }
      unset($technical);
    }

    /**
     * @var $education ArchibaldLomDataEducation
     */
    $education = $lom->getEducation();

    $tmp_description = $education->getDescription();
    $fields = array(
      'interactiveType' => $education->getInteractiveType()->getValue(),
      'interactivityLevel' => $education->getInteractivityLevel()->getValue(),
      'semanticDensity' => $education->getSemanticDensity()->getValue(),
      'difficult' => $education->getDifficult()->getValue(),
      'description' => ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_description),
      'typicalLearningTime' => $education->getTypicalLearningTime()->getValue(),
    );
    $education_inserted_id = db_insert('archibald_education')
      ->fields($fields)
      ->execute();

    ArchibaldLomSaveHandler::insertVocabular($education->getInteractiveType(), 'education:interactiveType', $education_inserted_id);
    ArchibaldLomSaveHandler::insertVocabular($education->getInteractivityLevel(), 'education:interactivityLevel', $education_inserted_id);
    ArchibaldLomSaveHandler::insertVocabular($education->getSemanticDensity(), 'education:semanticDensity', $education_inserted_id);
    ArchibaldLomSaveHandler::insertVocabular($education->getDifficult(), 'education:difficult', $education_inserted_id);
    ArchibaldLomSaveHandler::insertVocabular($education->getTypicalLearningTime(), 'education:typicalLearningTime', $education_inserted_id);

    $opts = array('documentary', 'pedagogical');
    foreach ($opts as $key => $type) {
      foreach ($education->getLearningResourceType($type) as $value) {
        $fields = array(
          'learningResourceType_id' => $value->getValue(),
          'education_id' => $education_inserted_id,
        );

        db_insert('archibald_education_learningresourcetype')
          ->fields($fields)
          ->execute();

        ArchibaldLomSaveHandler::insertVocabular(
          $value, 'education:learningResourceType_' . $education_inserted_id,
          $value->getValue()
        );
      }
    }

    foreach ($education->getIntendedEndUserRole() as $value) {
      $fields = array(
        'intendedEndUserRole_id' => $value->getValue(),
        'education_id' => $education_inserted_id,
      );
      db_insert('archibald_education_intendedenduserrole')
        ->fields($fields)
        ->execute();

      ArchibaldLomSaveHandler::insertVocabular(
        $value,
        'education:intendedEndUserRole_' . $education_inserted_id, $value->getValue()
      );
    }

    foreach ($education->getContext() as $value) {
      $fields = array(
        'context_id' => $value->getValue(),
        'education_id' => $education_inserted_id,
      );
      db_insert("archibald_education_context")
        ->fields($fields)
        ->execute();
      ArchibaldLomSaveHandler::insertVocabular($value, 'education:context_' . $education_inserted_id, $value->getValue());
    }

    foreach ($education->getTypicalAgeRange() as $value) {
      $fields = array(
        'typicalAgeRange' => ArchibaldLomSaveHandler::saveOrInsertLangString($value),
        'education_id' => $education_inserted_id,
      );
      db_insert("archibald_education_typicalagerange")
        ->fields($fields)
        ->execute();
    }

    foreach ($education->getLanguage() as $value) {
      $fields = array(
        'language' => $value,
        'education_id' => $education_inserted_id,
      );
      db_insert("archibald_education_language")
        ->fields($fields)
        ->execute();
    }
    unset($education);

    /**
     * @var ArchibaldLomDataMetaMetadata
     */
    $meta_metadata = $lom->getMetaMetadata();
    $fields = array(
      'metadataschema' => $meta_metadata->getMetadataSchema(),
      'language' => $meta_metadata->getLanguage(),
    );
    $meta_metadata_inserted_id = db_insert('archibald_meta_metadata')
      ->fields($fields)
      ->execute();

    foreach ($meta_metadata->getContribute() as $value) {
      $fields = array(
        'contribute_id' => ArchibaldLomSaveHandler::insertContribute($value, 'meta_metadata'),
        'meta_metadata_id' => $meta_metadata_inserted_id,
      );
      db_insert('archibald_meta_metadata_contributes')
        ->fields($fields)
        ->execute();
    }
    $lom_id = ArchibaldLomSaveHandler::requestLomId($lom->getLomId());

    if (empty($lom->version)) {
      $lom->version = 0;
    }

    ArchibaldLomSaveHandler::insertVocabular($lom->getRights()->getCost(), 'lom:rights_cost', $lom_id . '_' . $lom->version);

    $fields = array(
      'lom_id' => $lom_id,
      'version' => $lom->getVersion(),
      'uid' => $userid,
      'general_id' => $general_inserted_id,
      'lifecycle_id' => $lifecycle_inserted_id,
      'technical_id' => $technical_inserted_id,
      'education_id' => $education_inserted_id,
      'meta_metadata_id' => $meta_metadata_inserted_id,
      'save_time' => time(),
    );

    foreach ($lom->getRelation() as $relation) {
      $relation_fields = array(
        'lom_id' => $lom_id,
        'lom_version' => $lom->getVersion(),
        'kind' => $relation->kind->getValue(),
        'value' => $relation->getValue(),
        'catalog' => $relation->catalog->getValue(),
        'description' => ArchibaldLomSaveHandler::saveOrInsertLangString($relation->description)
      );

      $new_relation_id = db_insert('archibald_relations')
        ->fields($relation_fields)
        ->execute();
      ArchibaldLomSaveHandler::insertVocabular($relation->getKind(), 'lom:rel_kind', $new_relation_id);
      ArchibaldLomSaveHandler::insertVocabular($relation->getCatalog(), 'lom:rel_catalog', $new_relation_id);
    }
    // die();

    if ($lom->getRights()->getCost() instanceof ArchibaldLomDataVocabulary) {
      $fields['rights_cost'] = $lom->getRights()->getCost()->getValue();
    }

    if ($lom->getRights()->getDescription() instanceof ArchibaldLomDataLangString) {
      $tmp_description = $lom->getRights()->getDescription();
      $fields['rights_description'] = ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_description);
    }

    db_insert("archibald_lom")
      ->fields($fields)
      ->execute();


    $archibald_revision_cleaner = variable_get('archibald_revision_cleaner', 1);

    if ($archibald_revision_cleaner) {
      $max_revisions_holded = variable_get('archibald_max_revisions_holded', 50);

      $results = db_select('archibald_lom', 'l')
        ->fields('l', array('version'))
        ->condition('lom_id', $lom_id)
        ->orderBy('save_time', 'DESC')
        ->execute()
        ->fetchAll();

      if (count($results) > $max_revisions_holded) {
        $delete_all_after_holded = TRUE;
        $i = 0;
        $to_delete = array();
        foreach ($results AS $row) {
          if ($i++ < $max_revisions_holded) {
            continue;
          }

          $to_delete[] = $row->version;

          if (archibald_lom_is_published($lom_id, $row->version) != ARCHIBALD_PUBLISH_STATUS_NONE) {
            $to_delete = array();
          }
        }

        foreach ($to_delete AS $version_to_delete) {
          ArchibaldLomSaveHandler::purge($lom_id, $version_to_delete, FALSE);
        }
      }


    }

    $tmp = $lom->getEducation();
    $tmp->educationId = $education_inserted_id;
    $lom->setEducation($tmp);

    $tmp = $lom->getTechnical();
    $tmp->technicalId = $technical_inserted_id;
    $lom->setTechnical($tmp);

    $tmp = $lom->getGeneral();
    $tmp->generalId = $general_inserted_id;
    $lom->setGeneral($tmp);

    $tmp = $lom->getLifeCycle();
    $tmp->lifecycleId = $lifecycle_inserted_id;
    $lom->setLifeCycle($tmp);

    $tmp = $lom->getMetaMetadata();
    $tmp->metaMetadataId = $meta_metadata_inserted_id;
    $lom->setMetaMetadata($tmp);

    $meta_metadata->addIdentifier(new ArchibaldLomDataIdentifier('archibald', $lom_id));

    foreach ($meta_metadata->getIdentifier() as $value) {
      $fields = array(
        'catalog' => $value->getCatalog(),
        'entry' => $value->getEntry(),
        'meta_metadata_id' => $meta_metadata_inserted_id,
      );

      db_insert("archibald_meta_metadata_identifier")
        ->fields($fields)
        ->execute();
    }


    foreach ($lom->getAnnotation() as $k => $annotation) {

      $fields = array(
        'lom_id' => $lom_id,
        'lom_version' => $lom->getVersion(),
        #@TODO vcard parsing
        'entity' => $annotation->getEntity(),
        'date_date' => $annotation->getDate()->getDatetime(),
        'date_description' => ArchibaldLomSaveHandler::saveOrInsertLangString($annotation->getDate()->getDescription()),
        'description' => ArchibaldLomSaveHandler::saveOrInsertLangString($annotation->getDescription()),
      );
      $annotation->annotationId = db_insert('archibald_annotations')
        ->fields($fields)
        ->execute();
      $annotation->lomId = $lom_id;
      $lom->setAnnotation($annotation, $k);
    }

    foreach ($lom->getClassification() as $k => $classification) {
      $tmp_description = $classification->getDescription();
      $fields = array(
        'lom_id' => $lom_id,
        'lom_version' => $lom->getVersion(),
        'purpose' => $classification->getPurpose()->getValue(),
        'description' => ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_description),
      );

      $classification_inserted_id = db_insert('archibald_classification')
        ->fields($fields)
        ->execute();
      $classification->classificationId = $classification_inserted_id;
      $classification->lomId = $lom_id;
      ArchibaldLomSaveHandler::insertVocabular($classification->getPurpose(), 'lom:classification', $classification_inserted_id);

      foreach ($classification->getTaxonPath() as $taxon_path) {
        $tmp_source = $taxon_path->getSource();
        $fields = array(
          'source' => ArchibaldLomSaveHandler::saveOrInsertLangString($tmp_source),
        );
        $taxon_path_inserted_id = db_insert('archibald_taxon_path')
          ->fields($fields)
          ->execute();

        $taxon_path->taxonPathId = $taxon_path_inserted_id;

        $weight = 0;
        foreach ($taxon_path->getTaxon() as $taxon) {
          $weight++;
          $entry = $taxon->getEntry();
          $entry = ArchibaldLomSaveHandler::saveOrInsertLangString($entry);
          $fields = array(
            'taxon_path_id' => $taxon_path_inserted_id,
            'id' => $taxon->getId(),
            'entry' => $entry,
            'weight' => $weight,
          );
          db_insert('archibald_taxon')
            ->fields($fields)
            ->execute();
          $taxon->taxonPathId = $taxon_path_inserted_id;
        }

        $fields = array(
          'taxon_path_id' => $taxon_path_inserted_id,
          'classification_id' => $classification_inserted_id,
        );
        db_insert('archibald_classification_taxon')
          ->fields($fields)
          ->execute();
      }

      foreach ($classification->getKeyword() as $value) {
        $fields = array(
          'keyword' => ArchibaldLomSaveHandler::saveOrInsertLangString($value),
          'classification_id' => $classification_inserted_id,
        );
        db_insert('archibald_classification_keywords')
          ->fields($fields)
          ->execute();
      }
    }

    foreach ($lom->getCurriculum() as $k => $curriculum) {
      $fields = array(
        'lom_id' => $lom_id,
        'lom_version' => $lom->getVersion(),
        'source' => $curriculum->getSource(),
        'entity' => $curriculum->getEntity(),
      );

      $curriculum_inserted_id = db_insert('archibald_curriculum')
        ->fields($fields)
        ->execute();
      $curriculum->curriculumId = $curriculum_inserted_id;
      $curriculum->lomId = $lom_id;
    }

    // check if a entry in stats table exists and create it if nessesary
    $entry_exists = db_query(
      'SELECT COUNT(lom_id) FROM {archibald_lom_stats} WHERE lom_id=:lom_id',
      array(
        ':lom_id' => $lom_id,
      )
    )->fetchField(0);

    if ($entry_exists == 0) {

      // determine content partner
      $content_partners = archibald_can_publish_for_content_partner();
      $content_partner_id = 0;

      if (count($content_partners) == 1) {
        $content_partner_id = key($content_partners);
        if (empty($content_partner_id)) {
          $content_partner_id = 0;
        }
      }

      $fields = array(
        'lom_id' => $lom_id,
        'content_partner_id' => $content_partner_id,
        'republication_required' => 0,
        'indexed_version' => '',
        'local_published' => '',
      );
      db_insert('archibald_lom_stats')
        ->fields($fields)
        ->execute();

      db_delete('archibald_change_log')
        ->condition('lom_id', $lom_id)
        ->execute();
    }

    return $lom_id;
  }

  /**
   * updates the lom object and also increment the version id
   *
   * @param ArchibaldLom $lom reference
   * @param string $version reference
   *
   * @return String the updated lomid as a string
   */
  public static function update(ArchibaldLom $lom, &$version) {
    return ArchibaldLomSaveHandler::insert($lom, $version);
  }

  /**
   * Delete lom object permanently
   *
   * @param ArchibaldLom $lom
   */
  public static function delete(ArchibaldLom $lom) {
    $lom_id = $lom->getLomId();

    if (variable_get('archibald_solr_search_activ', 0) == 1 && module_exists('apachesolr')) {
      // Delete from Solr Index
      $solr_search = new ArchibaldLomSolrSearch();
      $solr_search->deleteFromIndex($lom_id);
    }

    $results = db_select('archibald_lom', 'l')
      ->fields('l', array('version'))
      ->condition('lom_id', $lom_id)
      ->orderBy('save_time', 'DESC')
      ->execute()
      ->fetchAll();

    $to_delete = array();
    foreach ($results AS $row) {
      $to_delete[] = $row->version;

      if (archibald_lom_is_published($lom_id, $row->version) != ARCHIBALD_PUBLISH_STATUS_NONE) {
        $to_delete = array();
        break;
      }
    }

    foreach ($to_delete AS $version_to_delete) {
      ArchibaldLomSaveHandler::purge($lom_id, $version_to_delete, TRUE);
    }
  }

  /**
   * mark lom object as deleted
   *
   * @param ArchibaldLom $lom
   */
  public static function mark_deleted(ArchibaldLom $lom) {
    // mark as deleted
    db_update('archibald_lom')
      ->fields(array('deleted' => 1))
      ->condition('lom_id', $lom->getLomId())
      ->condition('version', $lom->getVersion())
      ->execute();

    // delete all relations
    db_delete('archibald_relations')
      ->condition('lom_id', $lom->getLomId())
      ->execute();

    // reset lom_stats to let solr renindex it
    db_update('archibald_lom_stats')
      ->fields(array('indexed_version' => ''))
      ->condition('lom_id', $lom->getLomId())
      ->execute();

    cache_set('lom:' . $lom->getLomId() . '__version:' . $lom->getVersion(), NULL, 'cache', CACHE_TEMPORARY);
  }

  /**
   * get newest version key for a lom object
   *
   * @param string $lom_id
   *
   * @return string
   */
  public static function getActualVersion($lom_id) {
    return db_query('SELECT version
      FROM {archibald_lom}
      WHERE lom_id=:lom_id
      ORDER BY save_time DESC
      LIMIT 1',
      array(
        'lom_id' => $lom_id,
      )
    )->fetchColumn(0);
  }

  /**
   * Loads a lom class object from given lomid and version,
   * if version is '' it will load the recent version
   *
   * @param string $lom_id
   * @param string $version
   * @param boolean $use_cache
   *   default TRUE
   *
   * @return ArchibaldLom
   */
  public static function load($lom_id, $version = '', $use_cache = TRUE) {

    $where = '';
    $args = array(
      ':lom_id' => $lom_id,
    );

    // get newest if no version given
    if (empty($version)) {
      $version = self::getActualVersion($lom_id);
    }

    $cid = 'lom:' . $lom_id . '__version:' . $version;
    if ($use_cache == TRUE && $cache = cache_get($cid)) {
      if (!empty($cache->data)) {
        #return $cache->data;
      }
    }

    $where = 'AND version = :version ';
    $args[':version'] = $version;

    $lom = new ArchibaldLom();

    $lom_db = db_query("SELECT * FROM {archibald_lom}
      WHERE lom_id = :lom_id " . $where . "
      ORDER BY save_time DESC
      LIMIT 1",
      $args
    );
    $lom_db = $lom_db->fetch();

    if (empty($lom_db)) {
      // if lom id is invalid
      return NULL;
    }

    $lom->setFields($lom_db);
    $version      = $lom_db->version;
    $lom->version = $version;
    $rights       = new ArchibaldLomDataRights();
    $rights->setCost(ArchibaldLomSaveHandler::loadVocabular('lom:rights_cost', $lom->getLomId() . '_' . $lom->version));

    $rights->setDescription(
      ArchibaldLomSaveHandler::requestLangStringObj($lom_db->rights_description)
    );

    $lom->setRights($rights);
    unset($rights);

    $general_db = db_query(
      'SELECT * FROM {archibald_general} WHERE general_id = :general_id',
      array(
        ':general_id' => $lom_db->general_id,
      )
    );

    $row = $general_db->fetchAssoc();
    $tmp_obj = new ArchibaldLomDataGeneral();
    $tmp_obj->setFields($row);

    $tmp_obj->setStructure(ArchibaldLomSaveHandler::loadVocabular('general:structure', $lom_db->general_id));
    $tmp_obj->setAggregationLevel(ArchibaldLomSaveHandler::loadVocabular('general:aggregationLevel', $lom_db->general_id));


    $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($row['description']));
    #print_r(ArchibaldLomSaveHandler::requestLangStringObj($row['title']));die();
    $tmp_obj->setTitle(ArchibaldLomSaveHandler::requestLangStringObj($row['title']));

    $sub_db = db_query(
      'SELECT * FROM {archibald_general_identifier}
        WHERE general_id = :general_id',
      array(
        ':general_id' => $lom_db->general_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $title = '';
      if (!empty($row['title'])) {
        $title = ArchibaldLomSaveHandler::requestLangStringObj($row['title']);
      }
      $tmp_obj->addIdentifier(new ArchibaldLomDataIdentifier($row['catalog'], $row['entry'], $title));
    }
    unset($sub_db);

    $sub_db = db_query('SELECT * FROM {archibald_general_keywords} WHERE general_id = :general_id', array(
      ':general_id' => $lom_db->general_id,
    ));

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addKeyword(ArchibaldLomSaveHandler::requestLangStringObj($row['keyword']));
    }
    unset($sub_db);

    $sub_db = db_query(
      'SELECT *
        FROM {archibald_general_languages}
        WHERE general_id = :general_id
        ORDER BY language',
      array(
        ':general_id' => $lom_db->general_id,
      )
    );

    // print_r( $sub_db->queryString );
    // print_r( $lom_db->general_id );
    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addLanguage($row['language']);
      // print_r( $row['language'] );
    }
    // die();
    unset($sub_db);

    $sub_db = db_query(
      'SELECT *
        FROM {archibald_general_coverage}
        WHERE general_id = :general_id
        ORDER BY coverage',
      array(
        ':general_id' => $lom_db->general_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addCoverage(ArchibaldLomSaveHandler::requestLangStringObj($row['coverage']));
    }
    unset($sub_db);

    $lom->setGeneral($tmp_obj);

    $lifecycle_db = db_query(
      'SELECT *
        FROM {archibald_lifecycle}
        WHERE lifecycle_id = :lifecycle_id',
      array(
        ':lifecycle_id' => $lom_db->lifecycle_id,
      )
    );

    $tmp_obj = new ArchibaldLomDataLifeCycle();
    $tmp_obj->setFields($lifecycle_db->fetchAssoc());
    $tmp_obj->setVersion(ArchibaldLomSaveHandler::requestLangStringObj($tmp_obj->version));
    $tmp_obj->setStatus( $tmp_obj->status );

    $sub_db = db_query('
      SELECT *
        FROM {archibald_lifecycle_contributes}
        WHERE lifecycle_id = :lifecycle_id
        ORDER BY contribute_id',
      array(
        ':lifecycle_id' => $lom_db->lifecycle_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addContribute(ArchibaldLomSaveHandler::loadContribute($row['contribute_id'], 'lifecycle'));
    }
    unset($sub_db);
    $lom->setLifeCycle($tmp_obj);



    $meta_metadata_db = db_query('
      SELECT *
        FROM {archibald_meta_metadata}
        WHERE meta_metadata_id = :meta_metadata_id',
      array(
        ':meta_metadata_id' => $lom_db->meta_metadata_id,
      )
    );

    $tmp_obj = new ArchibaldLomDataMetaMetadata();
    $tmp_obj->setFields($meta_metadata_db->fetchAssoc());

    $sub_db = db_query('
      SELECT *
      FROM {archibald_meta_metadata_identifier}
      WHERE meta_metadata_id = :meta_metadata_id',
      array(
        ':meta_metadata_id' => $lom_db->meta_metadata_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addIdentifier(new ArchibaldLomDataIdentifier($row['catalog'], $row['entry']));
    }
    unset($sub_db);

    $sub_db = db_query('
      SELECT *
        FROM {archibald_meta_metadata_contributes}
        WHERE meta_metadata_id = :meta_metadata_id',
      array(
        ':meta_metadata_id' => $lom_db->meta_metadata_id,
      )
    );
    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addContribute(ArchibaldLomSaveHandler::loadContribute($row['contribute_id'], 'meta_metadata'));
    }
    unset($sub_db);

    $lom->setMetaMetadata($tmp_obj);

    $technical_db = db_query('
      SELECT *
        FROM {archibald_technical}
        WHERE technical_id = :technical_id',
      array(
        ':technical_id' => $lom_db->technical_id,
      )
    );

    $tmp_obj = new ArchibaldLomDataTechnical();
    $technical_db = $technical_db->fetchAssoc();
    $tmp_obj->setFields($technical_db);
    $tmp_obj->setOtherPlattformRequirements(
      ArchibaldLomSaveHandler::requestLangStringObj($technical_db['other_plattform_requirements'])
    );
    $tmp_obj->setSize($tmp_obj->getSize());
    $tmp_obj->setFormat($technical_db['format']);

    $tmp_obj->setPreviewImage(
      new ArchibaldLomDataPreviewImage(
        $technical_db['preview_image'],
        $technical_db['preview_image_description']
      )
    );

    $sub_db = db_query('
      SELECT *
        FROM {archibald_technical_locations}
        WHERE technical_id = :technical_id',
      array(
        ':technical_id' => $lom_db->technical_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addLocation(
        new ArchibaldLomDataLocation($row['location'])
      );
    }
    unset($sub_db);

    $tmp_obj->setDuration(
      new ArchibaldLomDataDuration(
        $technical_db['duration_duration'],
        ArchibaldLomSaveHandler::requestLangStringObj($technical_db['duration_description']))
    );
    $lom->setTechnical($tmp_obj);

    $education_db = db_query('
      SELECT *
        FROM {archibald_education}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );

    $row = $education_db->fetchAssoc();

    $tmp_obj = new ArchibaldLomDataEducation();
    $tmp_obj->setFields($row);

    $tmp_obj->setInteractiveType(ArchibaldLomSaveHandler::loadVocabular('education:interactiveType', $lom_db->education_id));
    $tmp_obj->setInteractivityLevel(ArchibaldLomSaveHandler::loadVocabular('education:interactivityLevel', $lom_db->education_id));
    $tmp_obj->setSemanticDensity(ArchibaldLomSaveHandler::loadVocabular('education:semanticDensity', $lom_db->education_id));
    $tmp_obj->setDifficult(ArchibaldLomSaveHandler::loadVocabular('education:difficult', $lom_db->education_id));
    $tmp_obj->setTypicalLearningTime(ArchibaldLomSaveHandler::loadVocabular('education:typicalLearningTime', $lom_db->education_id));
    $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($row['description']));

    $options = array();
    $options['pedagogical'] = archibald_get_taxonomy_options(
      'learning_resourcetype', 'pedagogical', TRUE
    );
    $options['documentary'] = archibald_get_taxonomy_options(
      'learning_resourcetype', 'documentary', TRUE
    );

    $sub_db = db_query('
      SELECT *
        FROM {archibald_education_learningresourcetype}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addLearningResourceType(
        ( array_key_exists($row['learningresourcetype_id'], $options['documentary']) ) ? 'documentary' : 'pedagogical',
        ArchibaldLomSaveHandler::loadVocabular(
          'education:learningResourceType_' . $lom_db->education_id,
          $row['learningresourcetype_id']
        )
      );
    }
    unset($sub_db);

    $sub_db = db_query('
      SELECT *
        FROM {archibald_education_intendedenduserrole}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );
    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addIntendedEndUserRole(
        ArchibaldLomSaveHandler::loadVocabular(
          'education:intendedEndUserRole_' . $lom_db->education_id,
          $row['intendedenduserrole_id']
        )
      );
    }
    unset($sub_db);

    $sub_db = db_query('
      SELECT *
        FROM {archibald_education_context}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addContext(
        ArchibaldLomSaveHandler::loadVocabular('education:context_' . $lom_db->education_id, $row['context_id'])
      );
    }
    unset($sub_db);

    $sub_db = db_query('
      SELECT *
        FROM {archibald_education_typicalagerange}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addTypicalAgeRange(ArchibaldLomSaveHandler::requestLangStringObj($row['typicalagerange']));
    }
    unset($sub_db);

    $sub_db = db_query('
      SELECT *
        FROM {archibald_education_language}
        WHERE education_id = :education_id',
      array(
        ':education_id' => $lom_db->education_id,
      )
    );

    while ($row = $sub_db->fetchAssoc()) {
      $tmp_obj->addLanguage($row['language']);
    }
    unset($sub_db);


    $lom->setEducation($tmp_obj);

    $res = db_query('
      SELECT *
      FROM {archibald_annotations}
      WHERE lom_id = :lom_id
      AND lom_version = :lom_version',
      array(
        ':lom_id' => $lom_db->lom_id,
        ':lom_version' => $lom_db->version,
      )
    );
    while ($annotation_db = $res->fetchAssoc()) {
      $tmp_obj = new ArchibaldLomDataAnnotation();
      $tmp_obj->setFields($annotation_db);

      $datetime = new ArchibaldLomDataDateTime($annotation_db['date_date']);
      $datetime->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($annotation_db['date_description']));
      $tmp_obj->setDate($datetime);
      unset($datetime);

      $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($annotation_db['description']));
      $lom->addAnnotation($tmp_obj);
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

    $res = db_query('
      SELECT *
        FROM {archibald_relations}
        WHERE lom_id = :lom_id
        AND lom_version = :lom_version',
      array(
        ':lom_id' => $lom_db->lom_id,
        ':lom_version' => $lom_db->version,
      )
    );

    while ($relation_db = $res->fetchAssoc()) {
      // print_r( $relation_db['kind'] );
      // print_r( ArchibaldLomSaveHandler::loadVocabular('lom:rel_catalog', $relation_db['relation_id']) );
      // die();
      $tmp_obj = new ArchibaldLomDataRelation();
      $tmp_obj->setRelationId($relation_db['relation_id']);
      $tmp_obj->setKind(ArchibaldLomSaveHandler::loadVocabular('lom:rel_kind', $relation_db['relation_id']));
      $tmp_obj->setCatalog(ArchibaldLomSaveHandler::loadVocabular('lom:rel_catalog', $relation_db['relation_id']));
      $tmp_obj->setValue($relation_db['value']);
      $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($relation_db['description']));
      $lom->addRelation($tmp_obj);
    }

    $res = db_query('
      SELECT *
        FROM {archibald_classification}
        WHERE lom_id = :lom_id
        AND lom_version = :lom_version
        ORDER BY classification_id',
      array(
        ':lom_id' => $lom_db->lom_id,
        ':lom_version' => $lom_db->version,
      )
    );
    while ($classification_db = $res->fetchAssoc()) {
      $tmp_obj = new ArchibaldLomDataClassification();
      $tmp_obj->setFields($classification_db);
      $tmp_obj->setPurpose(ArchibaldLomSaveHandler::loadVocabular('lom:classification', $tmp_obj->classification_id));
      $tmp_obj->setDescription(ArchibaldLomSaveHandler::requestLangStringObj($classification_db['description']));

      $sub_db = db_query('
        SELECT *
          FROM {archibald_classification_keywords}
          WHERE classification_id = :classification_id',
        array(
          ':classification_id' => $tmp_obj->classification_id,
        )
      );
      while ($row = $sub_db->fetchAssoc()) {
        $tmp_obj->addKeyword(ArchibaldLomSaveHandler::requestLangStringObj($row['keyword']));
      }
      unset($sub_db);

      $tp_res = db_query('
        SELECT tp.*
        FROM {archibald_classification_taxon} ctp
        JOIN {archibald_taxon_path} tp
          ON (ctp.taxon_path_id = tp.taxon_path_id)
        WHERE classification_id = :classification_id
        ORDER BY ctp.taxon_path_id',
        array(
          ':classification_id' => $classification_db['classification_id'],
        )
      );
      while ($taxonpath_db = $tp_res->fetchAssoc()) {

        $tmp_taxonpath_obj = new ArchibaldLomDataTaxonPath();
        $tmp_taxonpath_obj->setFields($taxonpath_db);
        $tmp_taxonpath_obj->setSource(ArchibaldLomSaveHandler::requestLangStringObj($taxonpath_db['source']));

        $t_res = db_query('
          SELECT *
            FROM {archibald_taxon}
            WHERE taxon_path_id = :taxon_path_id
            ORDER BY weight ASC',
          array(
            ':taxon_path_id' => $taxonpath_db['taxon_path_id'],
          )
        );
        while ($taxon_db = $t_res->fetchAssoc()) {
          $taxon = new ArchibaldLomDataTaxon();
          $taxon->setFields($taxon_db);
          $taxon->setEntry(ArchibaldLomSaveHandler::requestLangStringObj($taxon_db['entry']));
          $tmp_taxonpath_obj->addTaxon($taxon);
        }


        $tmp_obj->addTaxonPath($tmp_taxonpath_obj);
      }
      $lom->addClassification($tmp_obj);
    }

    // echo '<pre>';
    $res = db_query('
      SELECT *
        FROM {archibald_curriculum}
        WHERE lom_id = :lom_id
        AND lom_version = :lom_version
        ORDER BY curriculum_id',
      array(
        ':lom_id' => $lom_db->lom_id,
        ':lom_version' => $lom_db->version,
      )
    );
    // print_r( $res );
    while ($curriculum_db = $res->fetchAssoc()) {
      $tmp_obj = new ArchibaldLomDataCurriculum();
      $tmp_obj->setFields($curriculum_db);
      // print_r( $tmp_obj );
      $lom->addCurriculum($tmp_obj);
    }
    // var_dump( $res );
    // var_dump( $lom_db );
    // print_r($lom->getCurriculum());

    cache_set($cid, $lom, 'cache', CACHE_TEMPORARY);

    // print_r($lom->getCurriculum());
    // die();

    return $lom;
  }

  /**
   * insert the given contribute for given type into database
   * and returns the inserted id
   *
   * @param ArchibaldLomDataContribute $contribute
   * @param string $type
   *
   * @return integer
   *   the inserted contribute id
   */
  private static function insertContribute(ArchibaldLomDataContribute &$contribute, $type) {

    $desc = $contribute->getDate()->getDescription();

    $fields = array(
      'type' => $type,
      'role' => $contribute->getRole()->getValue(),
      'date_date' => $contribute->getDate()->getDatetime(),
      'date_description' => ArchibaldLomSaveHandler::saveOrInsertLangString(
        $desc
      ),
    );
    $contribute_inserted_id = db_insert('archibald_contribute')
      ->fields($fields)
      ->execute();

    ArchibaldLomSaveHandler::insertVocabular($contribute->getRole(), 'contribute:' . $type, $contribute_inserted_id);

    $contribute->contribute_id = $contribute_inserted_id;
    foreach ($contribute->getEntity() as $entity) {
      $entity_id = '';
      if (class_exists('ArchibaldLomContributor')) {
        $archibald_lom_contributor = new ArchibaldLomContributor($entity);
        $entity_id = $archibald_lom_contributor->getId();
      }
      $fields = array(
        'entity' => $entity,
        'entity_id' => $entity_id,
        'contribute_id' => $contribute_inserted_id,
      );
      db_insert('archibald_contribute_entity')
        ->fields($fields)
        ->execute();
    }
    return $contribute_inserted_id;
  }

  /**
   * load contribute from database
   *
   * @param int $contribute_id
   *
   * @return ArchibaldLomDataContribute
   */
  private static function loadContribute($contribute_id, $type) {
    $contribute = new ArchibaldLomDataContribute();
    $contribute_db = db_query('
      SELECT *
        FROM {archibald_contribute}
        WHERE contribute_id = :contribute_id',
      array(
        ':contribute_id' => $contribute_id,
      )
    )->fetchAssoc();
    $contribute->setFields($contribute_db);
    $contribute->setDate(
      new ArchibaldLomDataDateTime(
        $contribute_db['date_date'],
        ArchibaldLomSaveHandler::requestLangStringObj($contribute_db['date_description'])
      )
    );

    $contribute->setRole(ArchibaldLomSaveHandler::loadVocabular('contribute:' . $type, $contribute->contribute_id));

    $res = db_query('
      SELECT *
        FROM {archibald_contribute_entity}
        WHERE contribute_id = :contribute_id
        ORDER BY contribute_id',
      array(
        ':contribute_id' => $contribute_id,
      )
    );

    while ($row = $res->fetchAssoc()) {
      $contribute->addEntity($row['entity']);
    }
    return $contribute;
  }

  /**
   * delte contribute from database
   *
   * @param integer $contribute_id
   * @param string $type
   */
  private static function deleteContribute($contribute_id, $type) {
    if ($contribute_id instanceof ArchibaldLomDataContribute) {
      $id = $contribute_id->contributeId;
    }
    else {
      $id = $contribute_id;
      $contribute_id = ArchibaldLomSaveHandler::loadContribute($contribute_id, $type);
    }

    ArchibaldLomSaveHandler::deleteVocabular('contribute:' . $type, $contribute_id->contributeId);

    $db = db_delete('archibald_contribute_entity')
      ->condition('contribute_id', $contribute_id->contributeId, '=')
      ->execute();

    $db = db_delete('archibald_contribute')
      ->condition('contribute_id', $contribute_id->contributeId, '=')
      ->execute();
  }

  /**
   *
   * @param ArchibaldLomDataVocabulary $vocabular
   * @param string $type
   * @param integer $id
   *   uuid
   *
   * @return string
   *   the inserted contribute value
   */
  private static function insertVocabular($vocabular, $type, $id) {

    if (!($vocabular instanceof ArchibaldLomDataVocabulary)) {
      return '';
    }

    $value = $vocabular->getValue();
    if (empty($value)) {
      return '';
    }
    $fields = array(
      'id' => $id,
      'type' => $type,
      'source' => $vocabular->getSource(),
      'value' => $value,
    );

    try {
      db_insert('archibald_vocabulars')
        ->fields($fields)
        ->execute();
    }
    catch (Exception $e) {
      # do nothing if we insert it twice
      # $e->getMessage()
    }
    return $vocabular->getValue();
  }

  /**
   *
   * @param String $type
   * @param Int $id
   */
  private static function deleteVocabular($type, $id = 0) {
    $db = db_delete("archibald_vocabulars")->condition('type', $type, '=');
    if ($id !== 0) {
      $db = $db->condition('id', $id, '=');
    }
    $db->execute();
  }

  /**
   *
   * @param int $lom_id
   * @param string $type
   *
   * @return ArchibaldLomDataVocabulary
   */
  private static function loadVocabular($type, $id) {

    $vocabular = new ArchibaldLomDataVocabulary();
    $vocabular_db = db_select("archibald_vocabulars", 'archibaldv')
      ->fields('archibaldv')
      ->condition('id', $id, '=')
      ->condition('type', $type, '=')
      ->execute();

    $vocabular->setFields($vocabular_db->fetch());
    return $vocabular;
  }

  /**
   * generate new uuid
   *
   * @return string
   *   uuid
   */
  private static function generateUuid() {
    return md5(uniqid(mt_rand(), TRUE));
  }

  /**
   * Load from given id the langstring object and return it
   *
   * @param string $id
   *
   * @return ArchibaldLomDataLangString
   */
  public static function requestLangStringObj($id) {
    if (is_object($id)) {
      return $id;
    }
    $return_obj = new ArchibaldLomDataLangString();
    $return_obj->strings = array();
    $return_obj->setId($id);
    $res = db_select('archibald_langstring_terms', 'archibaldls')
      ->fields('archibaldls')
      ->condition('langstring_terms_id', $id)
      ->execute();
    while ($row = $res->fetchAssoc()) {
      $return_obj->setString($row['term'], $row['language']);
    }
    if (is_null($return_obj->strings)) {
      $return_obj->strings = array();
    }
    return $return_obj;
  }

  /**
   * save a ArchibaldLomDataLangString to database and return id
   *
   * @param string $langstring
   *
   * @return string
   *    the langstring id
   */
  public static function saveOrInsertLangString(&$langstring) {
    if (empty($langstring)) {
      return FALSE;
    }

    if (is_array($langstring)) {
      $tlangstring = new ArchibaldLomDataLangString();
      $tlangstring->setFields($langstring);
      $langstring = $tlangstring;
      unset($tlangstring);
    }

    if (($langstring instanceof ArchibaldLomDataLangString) === FALSE) {
      return '';
    }

    $strings = $langstring->getStrings();

    if ($langstring->getId() == '') {
      // Is a new string.
      $langstring->setId(ArchibaldLomSaveHandler::generateUuid());
    }

    foreach ($strings as $language => $term) {
      $res_test = db_select('archibald_langstring_terms', 'n')
        ->fields('n')->condition('langstring_terms_id', $langstring->getId())
        ->condition('language', $language)
        ->execute()
        ->fetchAssoc();

      if (empty($res_test)) {
        $fields = array(
          'langstring_terms_id' => $langstring->getId(),
          'language' => $language,
          'term' => $term,
        );

        try {
          db_insert('archibald_langstring_terms')
            ->fields($fields)
            ->execute();
        }
        catch (Exception $e) {
          // its only for debuging, it should never trown
          echo "\n\n### {$e->getMessage()}\n";
          foreach (debug_backtrace() as $e) {
            echo $e['file'] . ':' . $e['line'] . "\t" . $e['function'] . "\n";
          }
          die('archibald_langstring_terms error');
        }
      }
      else {
        if (empty($term)) {
          db_delete('archibald_langstring_terms')
            ->condition('langstring_terms_id', $langstring->getId())
            ->condition('language', $language)
            ->execute();
        }
        else {
          db_update('archibald_langstring_terms')
            ->fields(array('term' => $term))
            ->condition('langstring_terms_id', $langstring->getId())
            ->condition('language', $language)
            ->execute();
        }
      }
    }
    return $langstring->getId();
  }

  /**
   * Get a lom id, if 0 given it will generate a uuid
   *
   * @param string $lom_id
   *
   * @return string
   */
  private static function requestLomId($lom_id = 0) {
    if (empty($lom_id)) {
      $lom_id = ArchibaldLomSaveHandler::generateUuid();
    }
    return $lom_id;
  }

  /**
   * get responsible uid
   *
   * @param string $lom_id
   *
   * @return integer $uid
   */
  public static function getResponsibleUid($lom_id) {
    return db_query('SELECT responsible_uid FROM {archibald_lom_stats} WHERE lom_id = :lom_id', array(
      ':lom_id' => $lom_id,
    ))->fetchField(0);
  }

  /**
   * set responsible uid
   *
   * @param string $lom_id
   * @param integer $uid
   */
  public static function setResponsibleUid($lom_id, $uid) {
    db_update('archibald_lom_stats')
      ->fields(array('responsible_uid' => $uid))
      ->condition('lom_id', $lom_id)
      ->execute();
  }

  /**
   * get if local published or not
   *
   * @param string $lom_id
   *
   * @return string version_id
   */
  public static function getLocalPublished($lom_id) {
    $result = db_query('SELECT local_published FROM {archibald_lom_stats} WHERE lom_id = :lom_id', array(
      ':lom_id' => $lom_id,
    ))->fetchField(0);

    return (empty($result)) ? FALSE : $result;
  }

  /**
   * set local publish status
   *
   * @param string $lom_id
   * @param string $published
   *   lom_version string or
   *   NULL for unpublish
   */
  public static function setLocalPublished($lom_id, $published) {

    $change = array(
      'local_published' => $published,
    );
    if (empty($published)) {
      $change['batch_op_key'] = '';
      $change['batch_op_time'] = 0;
    }

    db_update('archibald_lom_stats')
      ->fields($change)
      ->condition('lom_id', $lom_id)
      ->execute();
  }
}

