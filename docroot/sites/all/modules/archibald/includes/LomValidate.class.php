<?php

/**
 * @file
 * ArchibaldLom Validator checks a lom resource if it is valid
 * and ready to be published into the central catalog
 */

/**
 * Check if a lom object is validate
 * do the same checks as archibald_content_form_validate()
 * and some more
 *
 * @author Heiko Henning <h.henning@educa.ch>
 * @author Christian Ackermann <c.ackermann@educa.ch>
 */
class ArchibaldLomValidate {

  /**
   * lom object to check for validity
   * @var ArchibaldLom
   */
  protected $lom = NULL;

  /**
   * List of error messages
   * @var array
   */
  protected $errors = array();

  const LOM_VALIDATE_MISSING = 1;
  const LOM_VALIDATE_INVALID = 2;
  const LOM_VALIDATE_WRONG_DATATYPE = 3;
  const LOM_VALIDATE_DUPLICATE = 4;
  const LOM_VALIDATE_UNDEFINED = 5;

  public function __construct(ArchibaldLom $lom) {
    $this->lom = $lom;
  }


  /**
   * return an array with all previous added errors
   *
   * @return array
   */
  public function getErrors() {
    sort($this->errors);
    return $this->errors;
  }

  /**
   * Returns wether the provided $value is empty or not
   *
   * @param mixed $value
   *  can be ArchibaldLom_* objects, an array or normal strings
   *
   * @return boolean
   *   TRUE if empty, else FALSE
   */
  private function stringIsEmpty($value) {

    if ($value instanceof ArchibaldLomDataLangString) {
      $value = ArchibaldLom::getLangstringText($value);
    }
    elseif ($value instanceof ArchibaldLomDataDuration) {
      $value = $value->getDuration();
    }
    elseif ($value instanceof ArchibaldLomDataContribute) {
      $value = $value->getEntity();
    }
    elseif ($value instanceof ArchibaldLomDataVocabulary) {
      $value = $value->getValue();
    }

    if (is_array($value)) {

      foreach ($value as $val) {
        if ($val instanceof ArchibaldLomDataVocabulary) {
          $val = $val->getValue();
        }

        if (!empty($val)) {
          return FALSE;
        }
      }
      return TRUE;
    }
    return empty($value);
  }

  /**
   * check lom object if it is ready for publishing
   *
   * @return boolean
   *   TRUE of ready, else FALSE
   */
  public function readyForPublish() {

    $this->checkRelations();

    $this->checkGeneral(FALSE);
    $this->checkLifecycle();
    $this->checkEducation();
    $this->checkTechnical();
    $this->checkRights();
    $this->checkClassification();

    return (empty($this->errors)) ? TRUE : FALSE;
  }

  /**
   * checks if classification section is valid
   */
  protected function checkClassification() {

    $has_default = FALSE;
    $has_specific = FALSE;

    $curriculums = $this->lom->getCurriculum();
    if (!empty($curriculums)) {
      foreach ($curriculums AS $curriculum) {
        if (!empty($curriculum->entity)) {
          switch( $curriculum->source ){
            default:
              $has_specific = TRUE;
            break;
            case 'educa':
              $has_default = TRUE;
            break;
          }
        }
      }
    }

    $classification = $this->lom->getClassification();
    if (!empty($classification)) {
      foreach ($classification as $c) {
        if ($c instanceof ArchibaldLomDataClassification) {
          $t = $c->getTaxonPath();
          foreach ($t as $taxon_path_id => $taxon_path) {
            if ($taxon_path instanceof ArchibaldLomDataTaxonPath) {
              $source = $taxon_path->getSource()->getStrings();
              // get first element
              $source = reset($source);
              switch( $source ) {
                default:
                  $has_specific = TRUE;
                break;
                case 'educa':
                  $has_default = TRUE;
                break;
              }
            }
          }
        }
      }
    }

    if( archibald_get_field_requirements('classification.curriculums') == ARCHIBALD_RES_FIELD_REQUIRED && !$has_specific ) {
      $this->addError('classification.curriculums', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }
    if( archibald_get_field_requirements('classification.defaultCurriculum') == ARCHIBALD_RES_FIELD_REQUIRED && !$has_default ) {
      $this->addError('classification.defaultCurriculum', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

  }

  /**
   * checks if general section is valid
   *
   * @param boolean $identifier_can_be_empty
   *   default:FALSE
   */
  protected function checkGeneral($identifier_can_be_empty = FALSE) {

    //Need this because we have a differenct check on identifiers
    $this->checkGeneralIdentifier($identifier_can_be_empty);

    $general = $this->lom->getGeneral();

    $description_required = archibald_get_field_requirements('general.description') == ARCHIBALD_RES_FIELD_REQUIRED;
    $aggregation_level_required = archibald_get_field_requirements('education.aggregationLevel') == ARCHIBALD_RES_FIELD_REQUIRED;
    $keyword_required = archibald_get_field_requirements('general.keyword') == ARCHIBALD_RES_FIELD_REQUIRED;
    $coverage_required = archibald_get_field_requirements('general.coverage') == ARCHIBALD_RES_FIELD_REQUIRED;

    $section_is_required = (
      $description_required &&
      $aggregation_level_required &&
      $keyword_required &&
      $coverage_required
    );

    if ($section_is_required && (empty($general) || !($general instanceof ArchibaldLomDataGeneral))) {
      $this->addError('general', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return;
    }

    ######## Check: general title
    if ($this->stringIsEmpty($general->getTitle())) {
      $this->addError('general.title', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: general description
    if ($description_required && $this->stringIsEmpty($general->getDescription())) {
      $this->addError('general.description', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: general language
    if ($this->stringIsEmpty($general->getLanguage())) {
      $this->addError('general.language', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: general aggregationLevel
    if ($aggregation_level_required && $this->stringIsEmpty($general->getAggregationLevel())) {
      $this->addError('education.aggregationLevel', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: general keyword
    $this->checkGeneralKeyword();

    ######## Check: general coverage
    $this->checkGeneralCoverage();
  }

  /**
   * checks if technical section is valid
   */
  protected function checkTechnical() {

    $technical = $this->lom->getTechnical();

    $location_required = archibald_get_field_requirements('technical.location') == ARCHIBALD_RES_FIELD_REQUIRED;
    $other_plattform_requirements_required = archibald_get_field_requirements('technical.otherPlattformRequirements') == ARCHIBALD_RES_FIELD_REQUIRED;

    $section_is_required = (
      $location_required &&
      $other_plattform_requirements_required
    );

    if ($section_is_required && (empty($technical) || !($technical instanceof ArchibaldLomDataTechnical))) {
      if ($field_required) {
        $this->addError('technical', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      }
      return FALSE;
    }

    ######## Check: technical size
    // $size = $technical->getSizeHuman();
    // $size = $size[0];
    // if (!$this->stringIsEmpty($size)) {
    //   // If not numerical
    //   if( !is_numeric($size) ) {
    //     $this->addError('technical.size', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $size);
    //   } else {
    //     if (strlen($size) > 5) {
    //       // If integer > 3 digits
    //       $this->addError('technical.size', t('Maximum allowed is three digits and one decimal'), $size);
    //     } else if (floor($size) == $size && strlen($size) > 3) {
    //       // If integer > 3 digits
    //       $this->addError('technical.size', t('Maximum allowed is three digits'), $size);
    //     } else if (strlen(substr(strrchr($size, "."), 1)) > 1) {
    //       // If more than 1 decimal
    //       $this->addError('technical.size', t('Maximum allowed is one decimal'), $size);
    //     }
    //   }
    // }


    ######## Check: technical location
    if ($location_required && $this->stringIsEmptyempty($technical->getLocation())) {
      $this->addError('technical.location', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: technical otherPlattformRequirements
    if ($other_plattform_requirements_required && $this->stringIsEmpty($technical->getOtherPlattformRequirements())) {
      $this->addError('technical.otherPlattformRequirements', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: technical previewImage
    $img = $technical->getPreviewImage()->getImage();
    if( strlen($img) ) {
      ######## Preview Image Copyright Description
      $desc = $technical->getPreviewImage()->getCopyrightDescription();
      if ( strlen($desc) > 200 ) {
        $this->addError('technical.previewImageCopyrightDescription', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $desc);
      }
    }
  }

  /**
   * checks if rights section is valid
   */
  protected function checkRights() {

    $rights = $this->lom->getRights();

    $description_required = archibald_get_field_requirements('right.description') == ARCHIBALD_RES_FIELD_REQUIRED;

    $section_is_required = (
      $description_required
    );

    if ($section_is_required && (empty($rights) || !($rights instanceof ArchibaldLomDataRights))) {
      if ($field_required) {
        $this->addError('right', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      }
      return FALSE;
    }

    ######## Check: rights description
    if ($description_required && $this->stringIsEmpty($rights->getDescription())) {
      $this->addError('right.description', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }
  }

  /**
   * checks if lifecycle section is valid
   */
  protected function checkLifecycle() {

    $lifecycle = $this->lom->getLifeCycle();

    $version_required = archibald_get_field_requirements('lifecycle.version') == ARCHIBALD_RES_FIELD_REQUIRED;
    $contribute_required = archibald_get_field_requirements('lifecycle.contributor') == ARCHIBALD_RES_FIELD_REQUIRED;

    $section_is_required = (
      $version_required &&
      $contribute_required
    );

    if ($section_is_required && (empty($lifecycle) || !($lifecycle instanceof ArchibaldLomDataLifeCycle))) {
      if ($field_required) {
        $this->addError('lifecycle', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      }
      return FALSE;
    }

    ######## Check: lifecycle version
    if ($version_required && $this->stringIsEmpty($lifecycle->getVersion())) {
      $this->addError('lifecycle.version', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    $this->checkLifecycleContributor();
  }

  /**
   * checks if education section is valid
   */
  protected function checkEducation() {

    $education = $this->lom->getEducation();

    $intended_end_user_role_required = archibald_get_field_requirements('education.intendedEndUserRole') == ARCHIBALD_RES_FIELD_REQUIRED;
    $typical_learning_time_required = archibald_get_field_requirements('education.typicalLearningTime') == ARCHIBALD_RES_FIELD_REQUIRED;
    $difficulty_required = archibald_get_field_requirements('education.difficulty') == ARCHIBALD_RES_FIELD_REQUIRED;
    $description_required = archibald_get_field_requirements('education.description') == ARCHIBALD_RES_FIELD_REQUIRED;

    $learning_resource_type_required = archibald_get_field_requirements('education.learningResourceType') == ARCHIBALD_RES_FIELD_REQUIRED;
    $context_required = archibald_get_field_requirements('classification.context') == ARCHIBALD_RES_FIELD_REQUIRED;

    $section_is_required = (
      $intended_end_user_role_required &&
      $typical_learning_time_required &&
      $difficulty_required &&
      $description_required &&
      $learning_resource_type_required &&
      $context_required
    );

    if ($section_is_required && (empty($education) || !($education instanceof ArchibaldLomDataEducation))) {
      $this->addError('education', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return FALSE;
    }

    ######## Check: education intendedEndUserRole
    if ($intended_end_user_role_required && $this->stringIsEmpty($education->getIntendedEndUserRole())) {
      $this->addError('education.intendedEndUserRole', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: education typicalLearningTime
    if ($typical_learning_time_required && $this->stringIsEmpty($education->getTypicalLearningTime())) {
      $this->addError('education.typicalLearningTime', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: education difficulty
    if ($difficulty_required && $this->stringIsEmpty($education->getDifficult())) {
      $this->addError('education.difficulty', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: education description
    if ($description_required && $this->stringIsEmpty($education->getDescription())) {
      $this->addError('education.description', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }

    ######## Check: education learningResourceType
    $this->checkEducationLearningResourceType();

    ######## Check: education context
    $this->checkEducationContext();
  }

  /**
   * check if general identifier is valid
   *
   * @param boolean $can_be_empty
   *   default:FALSE
   */
  protected function checkGeneralIdentifier($can_be_empty = FALSE) {

    $general = $this->lom->getGeneral();
    if (empty($general) && $can_be_empty == FALSE) {
      $this->addError('general', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return;
    }

    $identifiers = $general->getIdentifier();
    if (empty($identifiers) && $can_be_empty == FALSE) {
      $this->addError('general.identifier', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return;
    }

    foreach ($identifiers as $identifier) {
      if (!($identifier instanceof ArchibaldLomDataIdentifier)) {
        $this->addError('general.identifier', ArchibaldLomValidate::LOM_VALIDATE_WRONG_DATATYPE);
        continue;
      }

      switch ($identifier->getCatalog()) {
        case 'URL':
          if (strpos($identifier->getEntry(), '://') === FALSE) {
            if (!preg_match('/archibald_file\/[0-9]+\/.+/', $identifier->getEntry())) {
              $value = $identifier->getEntry();
              $this->addError('general.identifier.entry', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
            }
          }
          else {
            $url_parts = parse_url($identifier->getEntry());
            if (empty($url_parts['scheme']) || empty($url_parts['host'])) {
              $value = $identifier->getEntry();
              $this->addError('general.identifier.entry', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
            }
          }
          break;

        case 'ISBN':
          if (!preg_match('/(ISBN)?[ \-]*[0-9\-\ ]{6,}/i', $identifier->getEntry())) {
            $value = $identifier->getEntry();
            $this->addError('general.identifier.entry', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
          }
          break;

        case 'DOI':
          if (!preg_match('/^(10\.\d{4})((?:[.][0-9]+)*)\/(.*)/i', $identifier->getEntry())) {
            $value = $identifier->getEntry();
            $this->addError('general.identifier.entry', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
          }
          break;


        default:
          $value = $identifier->getEntry();
          $this->addError('general.identifier.catalog', ArchibaldLomValidate::LOM_VALIDATE_WRONG_DATATYPE, $value);
          break;
      }
    }
  }

  /**
   * check if general keywords is valid
   */
  protected function checkGeneralKeyword() {
    $existing = array();

    $field_required = archibald_get_field_requirements('general.keyword') == ARCHIBALD_RES_FIELD_REQUIRED;

    $keywords = $this->lom->getGeneral()->getKeyword();
    if (empty($keywords) && $field_required) {
      $this->addError('general.keyword', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return;
    }

    foreach ($keywords as $keyword) {
      if (!($keyword instanceof ArchibaldLomDataLangString)) {
        $value = print_r($keyword, TRUE);
        $this->addError('general.keyword', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      foreach ($keyword->getStrings() as $lang_key => $string) {
        if (isset($existing[$lang_key][md5($string)])) {
          $value = $lang_key . '->' . $string;
          $this->addError('general.keyword', ArchibaldLomValidate::LOM_VALIDATE_DUPLICATE, $value);
        }

        $existing[$lang_key][md5($string)] = $string;
      }
    }
    if (empty($existing) && $field_required) {
      $this->addError('general.keyword', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }
  }

  /**
   * check if general coverages is valid
   */
  protected function checkGeneralCoverage() {
    $existing = array();
    $field_required = archibald_get_field_requirements('general.coverage') == ARCHIBALD_RES_FIELD_REQUIRED;

    $coverages = $this->lom->getGeneral()->getCoverage();
    if (empty($coverages) && $field_required) {
      $this->addError('general.coverage', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      return;
    }

    foreach ($coverages as $coverage) {
      if (!($coverage instanceof ArchibaldLomDataLangString)) {
        $value = print_r($coverage, TRUE);
        $this->addError('general.coverage', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      foreach ($coverage->getStrings() as $lang_key => $string) {
        if (isset($existing[$lang_key][md5($string)])) {
          $value = $lang_key . '->' . $string;
          $this->addError('general.coverage', ArchibaldLomValidate::LOM_VALIDATE_DUPLICATE, $value);
        }

        $existing[$lang_key][md5($string)] = $string;
      }
    }
    if (empty($existing) && $field_required) {
      $this->addError('general.coverage', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
    }
  }

  /**
   * check if life_cycle contributor is valid
   */
  protected function checkLifecycleContributor() {
    $existing = array();
    $field_required = archibald_get_field_requirements('lifecycle.contributor') == ARCHIBALD_RES_FIELD_REQUIRED;
    foreach ($this->lom->getLifeCycle()->getContribute() as $contribute) {
      if (!($contribute instanceof ArchibaldLomDataContribute)) {
        $value = print_r($contribute, TRUE);
        $this->addError('lifecycle.contributor', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      $role = $contribute->getRole();
      $entity = $contribute->getEntity();

      if (!($role instanceof ArchibaldLomDataVocabulary)) {
        $value = print_r($role, TRUE);
        $this->addError('lifecycle.contributor.role', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      $parser      = new ArchibaldAppDataVcard_Parser(reset($entity));
      $vcard       = $parser->parse();
      $vcard       = $vcard[0];
      $vcard_ident = trim(@$vcard->firstname . ' ' . @$vcard->lastname . ' ' . @$vcard->organization);

      if (empty($vcard_ident)) {
        $value = print_r($entity, TRUE);
        $this->addError('lifecycle.contributor.entity', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      if (isset($existing[md5($role->getValue() . '#' . $vcard_ident)])) {
        $this->addError('lifecycle.contributor.entity', ArchibaldLomValidate::LOM_VALIDATE_DUPLICATE, $vcard_ident);
      }

      $existing[md5($role->getValue() . '#' . $vcard_ident)] = $vcard_ident;
    }

    if (empty($existing) && $field_required) {
      $this->addError('lifecycle.contributor', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
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
   * check if relations are valid
   *
   */
  protected function checkRelations() {
    $field_required = archibald_get_field_requirements('relation.relation') == ARCHIBALD_RES_FIELD_REQUIRED;
    $relations      = $this->lom->getRelation();
    if (empty($relations)) {
      if ($field_required) {
        $this->addError('relation', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      }
      return TRUE;
    }

    foreach ($relations as $relation) {
      if (!($relation instanceof ArchibaldLomDataRelation)) {
        $this->addError('relation', ArchibaldLomValidate::LOM_VALIDATE_INVALID, print_r($relation, TRUE));
        continue;
      }

      $kind = $relation->getKind();
      if (!($kind instanceof ArchibaldLomDataVocabulary)) {
        $value = print_r($kind, TRUE);
        $this->addError('relation.resource.kind', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      $catalog = $relation->getCatalog();
      if (!($kind instanceof ArchibaldLomDataVocabulary)) {
        $value = print_r($catalog, TRUE);
        $this->addError('relation.resource.catalog', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
        continue;
      }

      $value = $relation->getValue();
      $relation_catalog = $relation->getCatalog()->getValue();
      $relation_catalog = explode( '|', $relation_catalog);
      $relation_catalog = strtoupper( array_pop($relation_catalog) );
      switch ($relation_catalog) {
        case 'REL_URL':
          $url_parts = parse_url($value);
          if (empty($url_parts['scheme']) || empty($url_parts['host'])) {
            $this->addError('relations.relation.value', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
          }
          break;

        case 'REL_ISBN':
          if (!preg_match('/(ISBN)?[ \-]*[0-9\-\ ]{6,}/i', $value)) {
            $this->addError('relations.relation.value', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
          }
          break;

        case 'REL_DOI':
          if (!preg_match('/^(10\.\d{4})((?:[.][0-9]+)*)\/(.*)/i', $value)) {
            $this->addError('relations.relation.value', ArchibaldLomValidate::LOM_VALIDATE_INVALID, $value);
          }
          break;

        default:
          $this->addError('relations.relation.catalog', ArchibaldLomValidate::LOM_VALIDATE_WRONG_DATATYPE, $relation_catalog);
          break;
      }
    }
  }

  // Check if education learning_ressource_type is valid
  /*
    .......##.......########..########
    .......##.......##.....##....##...
    .......##.......##.....##....##...
    .......##.......########.....##...
    .......##.......##...##......##...
    .......##.......##....##.....##...
    .......########.##.....##....##...
  */
  protected function checkEducationLearningResourceType() {


    // First, check the (mandatory) documentary type
    $field_required = archibald_get_field_requirements('education.learningResourceTypeDocumentary') == ARCHIBALD_RES_FIELD_REQUIRED;
    $learning_resource_types = $this->lom->getEducation()->getLearningResourceType('documentary');

    if (empty($learning_resource_types) || !is_array($learning_resource_types)) {
      if ($field_required) {
        $this->addError('education.learningResourceTypeDocumentary', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
        return FALSE;
      }
    }

    // Add 'pedagogical' type to the check
    $field_required = archibald_get_field_requirements('education.learningResourceTypePedagogical') == ARCHIBALD_RES_FIELD_REQUIRED;
    $learning_resource_types = $this->lom->getEducation()->getLearningResourceType('pedagogical');
    if (empty($learning_resource_types) || !is_array($learning_resource_types)) {
      if ($field_required) {
        $this->addError('education.learningResourceTypePedagogical', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
        return FALSE;
      }
    }

    foreach ($learning_resource_types as $learning_resource_type) {
      if (!($learning_resource_type instanceof ArchibaldLomDataVocabulary)) {
        $this->addError('education.learningResourceType', ArchibaldLomValidate::LOM_VALIDATE_INVALID);
        continue;
      }
    }
    return TRUE;
  }

  /**
   * check if education context is valid
   */
  protected function checkEducationContext() {
    $field_required = archibald_get_field_requirements('classification.context') == ARCHIBALD_RES_FIELD_REQUIRED;

    $contextes = $this->lom->getEducation()->getContext();
    if (empty($contextes) || !is_array($contextes)) {
      if ($field_required) {
        $this->addError('classification.context', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
      }
      return FALSE;
    }

    foreach ($contextes as $context) {
      if (!($context instanceof ArchibaldLomDataVocabulary)) {
        $this->addError('classification.context', ArchibaldLomValidate::LOM_VALIDATE_INVALID);
        continue;
      }

      if ($context->getValue() == '') {
        if ($field_required) {
          $this->addError('classification.context.value', ArchibaldLomValidate::LOM_VALIDATE_MISSING);
        }
        return FALSE;
      }
    }
  }

  /**
   * Set error for a special lom field.
   *
   * @param string $field
   *   the field
   * @param string $type
   *   the type
   * @param string $value
   *   the value
   */
  protected function addError($field, $type = NULL, $value = '') {

    $message = '';
    $placeholder_values = array('@field' => archibald_get_field_label($field, TRUE));

    if (!isset($type)) {
      $type = ArchibaldLomValidate::LOM_VALIDATE_UNDEFINED;
    }

    switch ($type) {
      case ArchibaldLomValidate::LOM_VALIDATE_MISSING:
        $message = t('The value of field «@field» is missing.', $placeholder_values);
        break;
      case ArchibaldLomValidate::LOM_VALIDATE_INVALID:
        $message = t('The value of field «@field» is invalid.', $placeholder_values);
        break;
      case ArchibaldLomValidate::LOM_VALIDATE_WRONG_DATATYPE:
        $message = t('The value of field «@field» has a wrong data type.', $placeholder_values) . print_r($value, 1);
        break;
      case ArchibaldLomValidate::LOM_VALIDATE_DUPLICATE:
        $message = t('The value of field «@field» is redundant.', $placeholder_values);
        break;
      case ArchibaldLomValidate::LOM_VALIDATE_UNDEFINED:
        $message = t('In field «@field» occured an undefined error.', $placeholder_values);
        break;
    }

    $this->errors[md5($message)] = $message;
  }
}

