<?php
/**
 * @file
 * this file contains a LOM metadata field names
 */

/**
 * This function returns an array with all LOM metadata-field names (machine-readable) and labels
 *
 * The LOM metadata fields have following structure:
 * To form a field name use points to separate the levels, e.g. "education.learningResourceType"
 *
 * @return array
 *   A list of all lom fields including titles and requrements.
 *
 * general
 *    title
 *    proposalMessage
 *    description
 *    language
 *    identifier
 *        entry
 *        catalog
 *    keyword
 *    coverage
 *
 * technical
 *    location
 *    otherPlattformRequirements
 *
 * education
 *    description
 *    intendedEndUserRole
 *    typicalLearningTime
 *    difficulty
 *    learningResourceTypeDocumentary
 *    learningResourceTypePedagogical
 *    typicalAgeRange
 *    aggregationLevel
 *
 * lifecycle
 *    version
 *    contributor
 *        role
 *        entity
 *
 * right
 *     description
 *     cost
 *     copyright
 *     licence
 *
 * classification
 *     context
 *     curriculums
 *
 * relation
 *     resource
 *     relation
 *
 * contactInfo
 *     proposalContactName
 *     proposalContactEmail
 *     proposalContactPhone
 */
function archibald_get_field_info() {
  $general_fields = array(
    'title' => array(
      'label' => t('Title'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
      'help' => t('online help Title')
    ),
    'description' => array(
      'label' => t('General description'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help General description')
    ),
    'language' => array(
      'label' => t('Language of the resource'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Language of the resource')
    ),
    'identifier' => array(
      'label' => t('Identifier'),
      'help' => t('online help Identifier'),
      'subfields' => array(
        'entry' => array(
          'label' => t('Entry'),
        ),
        'catalog' => array(
          'label' => t('Catalogue'),
        ),
        'title' => array(
          'label' => t('Title'),
        ),
      ),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
    ),
    'keyword' => array(
      'label' => t('Keywords'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Keywords')
    ),
    'coverage' => array(
      'label' => t('Coverages'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Coverages')
    ),
    'proposalMessage' => array(
      'label' => t('Your message'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
      'help' => t('online help Your message'),
    ),
  );

  $technical_fields = array(
    'size' => array(
      'label' => t('Size'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Size')
    ),
    'format' => array(
      'label' => t('Format'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Format')
    ),
    'location' => array(
      'label' => t('Location'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Location')
    ),
    'otherPlattformRequirements' => array(
      'label' => t('Technical requirements'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Technical requirements')
    ),
    'previewImage' => array(
      'label' => t('Preview image'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Preview image')
    ),
    'previewImageCopyrightDescription' => array(
      'label' => t('Copyright description'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Copyright description')
    ),
    'duration' => array(
      'label' => t('Duration'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Duration')
    ),
  );

  $lifecycle_fields = array(
    'version' => array(
      'label' => t('Version'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Version'),
    ),
    'contributor' => array(
      'label' => t('Contributors'),
      'subfields' => array(
        'role' => array(
          'label' => t('Role'),
        ),
        'entity' => array(
          'label' => t('Entity'),
        ),
      ),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Contributors')
    ),
  );

  $right_fields = array(
    'description' => array(
      'label' => t('Terms of use'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Terms of use'),
    ),
    'cost' => array(
      'label' => t('Cost'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Cost'),
    ),
  );

  $education_fields = array(
    'intendedEndUserRole' => array(
      'label' => t('Intended end user role'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Intentend end use'),
    ),
    'typicalLearningTime' => array(
      'label' => t('Typical learning time'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Typical learning time'),
    ),
    'difficulty' => array(
      'label' => t('Difficulty'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Difficulty'),
    ),
    'description' => array(
      'label' => t('Educational description'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Educational desciption')
    ),
    'learningResourceTypeDocumentary' => array(
      'label' => archibald_get_term_by_key('documentary', 'learning_resourcetype'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
      'help' => t('online help Learning resource type')
    ),
    'learningResourceTypePedagogical' => array(
      'label' => archibald_get_term_by_key('pedagogical', 'learning_resourcetype'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'typicalAgeRange' => array(
      'label' => t('Typical age range'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Typical age range'),
    ),
    'aggregationLevel' => array(
      'label' => t('Aggregation level'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Aggegation level'),
    ),

  );

  $classification_fields = array(
    'context' => array(
      'label' => t('Education context'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_REQUIRED,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Education context'),
    ),
    'curriculums' => array(
      'label' => t('Specific Curricula'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Specific Curricula'),
    ),
    'defaultCurriculum' => array(
      'label' => t('Default Curriculum'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_RECOMMEND,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'help' => t('online help Default Curriculum'),
    )
  );


  $relation_fields = array(
    'relation' => array(
      'label' => t('Relation'),
      'help' => t('online help Relations'),
      'subfields' => array(
        'description' => array(
          'label' => t('Relation description'),
        )
      ),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'kind' => array(
      'label' => t('Type'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'catalog' => array(
      'label' => t('Catalog'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'value' => array(
      'label' => t('Value'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'description' => array(
      'label' => t('Relation description'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
  );

  $contact_info_fields = array(
    'proposalContactName' => array(
      'label' => t('Name'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
    ),
    'proposalContactEmail' => array(
      'label' => t('E-Mail'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_REQUIRED,
      // 'help' => t('An explanation of what this field is, and what the user should do'),
    ),
    'proposalContactPhone' => array(
      'label' => t('Phone number'),
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
      'minimum_requirements_proposal' => ARCHIBALD_RES_FIELD_OPTIONAL,
      // 'help' => t('An explanation of what this field is, and what the user should do'),
    ),
  );

  $lom_sections = array(
    'general' => array(
      'label' => t('General information'),
      'fields' => $general_fields,
    ),
    'technical' => array(
      'label' => t('Technical information'),
      'fields' => $technical_fields,
    ),
    'lifecycle' => array(
      'label' => t('Lifecycle information'),
      'fields' => $lifecycle_fields,
    ),
    'education' => array(
      'label' => t('Educational information'),
      'fields' => $education_fields,
    ),
    'classification' => array(
      'label' => t('Curricula'),
      'fields' => $classification_fields,
    ),
    'relation' => array(
      'label' => t('Relations'),
      'fields' => $relation_fields,
      'minimum_requirements' => ARCHIBALD_RES_FIELD_OPTIONAL,
    ),
    'right' => array(
      'label' => t('Rights'),
      'fields' => $right_fields,
    ),
    'contactInfo' => array(
      'label' => t('Contact Information'),
      'fields' => $contact_info_fields,
    ),
  );

  return $lom_sections;
}

/**
 * Get a list of minimum requirements for all lom fields.
 *
 * @param string $field_name
 *   Id of field to get minimum requirements from.
 *   If empty the function will return the full array.
 *
 * @return array
 *   A list of minumum requirements for all lom fields.
 *   The key is concatinated from section_id dot field_id
 *   The value is  ARCHIBALD_RES_FIELD_OPTIONAL, ARCHIBALD_RES_FIELD_RECOMMEND or ARCHIBALD_RES_FIELD_REQUIRED
 */
function archibald_get_field_minimum_requirements($field_name = NULL) {
  $minimum_requirements = &archibald_static(__FUNCTION__, NULL);
  $proposal = archibald_fields_is_proposal() ? '_proposal' : '';
  if (is_null($minimum_requirements)) {
    $minimum_requirements = array();
    foreach (archibald_get_field_info() as $section_id => $section) {
      foreach ($section['fields'] AS $field_id => $field) {
        $key = drupal_strtolower($section_id . '.' . $field_id);
        $minimum_requirements[$key] = $field['minimum_requirements' . $proposal];
      }

      if (isset($section['minimum_requirements' . $proposal])) {
        $minimum_requirements[$section_id] = $section['minimum_requirements' . $proposal];
      }
    }
  }

  if (!empty($field_name)) {
    $field_name = drupal_strtolower($field_name);
    $field_id = preg_replace("/\_/", ".", $field_name);

    if (!empty($minimum_requirements[$field_id])) {
      return $minimum_requirements[$field_id];
    }
  }

  return $minimum_requirements;
}

/**
 * Get configured minimum requirements of a field.
 *
 * @param string $field_name
 *   The name  or id of field to check.
 *
 * @return integer
 *   The configured requrements level of this field.
 *   ARCHIBALD_RES_FIELD_OPTIONAL, ARCHIBALD_RES_FIELD_RECOMMEND or ARCHIBALD_RES_FIELD_REQUIRED
 */
function archibald_get_field_requirements($field_name) {
  $field_name = drupal_strtolower($field_name);
  $field_id = str_replace('_', '.', $field_name);
  $field_name = str_replace('.', '_', $field_name);

  $minimum_requirements = archibald_get_field_minimum_requirements($field_id);

  $requirements = variable_get('archibald_field_require_type_' . ((archibald_fields_is_proposal()) ? 'proposal_' : '') . $field_name, $minimum_requirements);

  if ($minimum_requirements > $requirements && $minimum_requirements >= ARCHIBALD_RES_FIELD_REQUIRED) {
    // If minimum value is REQUIRED the current value can not below
    // But if it only is RECOMMEND the current value can be below.
    return $minimum_requirements;
  }

  return $requirements;
}

/**
 * This function returns the translated label for a given field name.
 *
 * @param string $field_name
 *   The LOM metadata field name
 *
 * @param bollean $full_path
 *   Set this value to TRUE to get a full name path.
 *   Default: FALSE
 *
 * @return string
 *   The Label of the Field
 */
function archibald_get_field_label($field_name, $full_path = FALSE) {

  $field_info = archibald_get_field_info();

  $field_name = preg_replace("/\_/", ".", $field_name);
  $parts = explode('.', $field_name);
  $count = count($parts);
  $label = '';

  if (isset($parts[2])) {
    if ($parts[2] == 'value') {
      unset($parts[2]);
    }
  }

  switch ($count) {
    case 1:
      $label = $field_info[$parts[0]]['label'];
      break;
    case 2:
      if ($full_path == TRUE) {
        $label = $field_info[$parts[0]]['label'];
        $label .= ' > ' . $field_info[$parts[0]]['fields'][$parts[1]]['label'];
      }
      else {
        $label .= $field_info[$parts[0]]['fields'][$parts[1]]['label'];
      }
      break;
    case 3:
      if ($full_path == TRUE) {
        $label = $field_info[$parts[0]]['label'];
        $label .= ' > ' . $field_info[$parts[0]]['fields'][$parts[1]]['label'];
        $label .= ' > ' . $field_info[$parts[0]]['fields'][$parts[1]]['subfields'][$parts[2]]['label'];
      }
      else {
        $label = $field_info[$parts[0]]['fields'][$parts[1]]['subfields'][$parts[2]]['label'];
      }

      break;
  }

  if (empty($label)) {
    $label = preg_replace("/\./", " ", $field_name);
  }

  return $label;
}


/**
 * This function returns the optional translated help text for a given field name.
 *
 * @param string $field_name
 *   The LOM metadata field name
 *
 * @return string
 *   The Help string for the Field
 */
function archibald_get_field_help($field_name) {

  $field_info = archibald_get_field_info();

  $field_name = preg_replace("/\_/", ".", $field_name);
  $parts = explode('.', $field_name);
  $count = count($parts);
  $str = '';

  if (isset($parts[2])) {
    if ($parts[2] == 'value') {
      unset($parts[2]);
    }
  }

  switch ($count) {
    case 1:
      if (!empty($field_info[$parts[0]]['help'])) $str .= $field_info[$parts[0]]['help'];
      break;
    case 2:
      if (!empty($field_info[$parts[0]]['fields'][$parts[1]]['help'])) $str .= $field_info[$parts[0]]['fields'][$parts[1]]['help'];
      break;
    case 3:
      if (!empty($field_info[$parts[0]]['fields'][$parts[1]]['subfields'][$parts[2]]['help'])) $str .= $field_info[$parts[0]]['fields'][$parts[1]]['subfields'][$parts[2]]['help'];
      break;
  }


  if (drupal_strlen($str)) {
    $str = addslashes(str_replace(':', ' - ', $str));
    $str = '<span class="form_help" title="' . $str . '">&nbsp;</span>';
  }

  return $str;
}