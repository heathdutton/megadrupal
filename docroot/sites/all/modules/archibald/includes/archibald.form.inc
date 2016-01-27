<?php

/**
 * @file
 * handle the ArchibaldLom ressource add/edit form
 * it contains them ain and some smaller sub handler
 * all larger sub handler`s
 * are placed in "../form/" folder
 */
require_once ARCHIBALD_PATH . 'form/form_welcome.inc';
require_once ARCHIBALD_PATH . 'form/form_classification.inc';
require_once ARCHIBALD_PATH . 'form/form_keywords.inc';
require_once ARCHIBALD_PATH . 'form/form_education.inc';
require_once ARCHIBALD_PATH . 'form/form_technical.inc';
require_once ARCHIBALD_PATH . 'form/form_proposal_contact_info.inc';
require_once ARCHIBALD_PATH . 'form/form_general.inc';
require_once ARCHIBALD_PATH . 'form/form_rights.inc';
require_once ARCHIBALD_PATH . 'form/form_life_cycle.inc';
require_once ARCHIBALD_PATH . 'form/form_relation.inc';

// the update is all 3 mins this means
// 3 lost connections for be shure (10 minutes)
define('ARCHIBALD_PROHIBIT_DOUBLE_EDIT_FREE_AFTER', (60 * 10));

/**
 * Returns if the current user has the permission for the given section field
 * or not
 *
 * @param string $field
 *   the field
 *
 * @return boolean
 *   true if access, else FALSE
 */
function archibald_check_field_permission($field) {
  $perm = &archibald_static(__FUNCTION__, NULL);
  if (empty($perms)) {
    $perms = archibald_permission();
  }
  if (!isset($perms['archibald content field ' . $field])) {
    return TRUE;
  }
  $result = user_access('archibald content field ' . $field);
  return $result;
}

/**
 * generate the lom select content partner form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_edit_select_content_partner($form, $form_state) {
  $form = array();

  $form['content_partner'] = array(
    '#type' => 'select',
    '#title' => t('Select the content partner'),
    '#options' => archibald_can_publish_for_content_partner(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Select content partner'),
  );

  return $form;
}

/**
 * Set the temporaly content partner id within the session.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_edit_select_content_partner_submit($form, &$form_state) {
  $_SESSION['archibald_content_partner_id'] = $form_state['values']['content_partner'];
}

/**
 * generate the lom data add / edit formular
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 *
 * @return array
 */
function archibald_content_form($form, $form_state, $lom, $language, $content_partner_id = 0) {
  global $user;
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  // drupal_add_library('system', 'ui.sortable');

  $form['lom_id'] = array(
    '#type' => 'hidden',
    '#value' => $lom->getLomId(),
  );


  archibald_fields_is_proposal(archibald_form_is_proposal(empty($form['lom_id']['#value'])));

  $form['content_partner_id'] = array(
    '#type' => 'hidden',
    '#value' => $content_partner_id,
  );

  $form['version'] = array(
    '#type' => 'hidden',
    '#value' => $lom->getVersion(),
  );

  $form['language'] = array(
    '#type' => 'hidden',
    '#value' => $language,
  );

  $form['vt_main'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  archibald_content_form_general($form, $form_state, $lom, $language);
  archibald_content_form_life_cycle($form, $form_state, $lom, $language);
  archibald_content_form_keywords($form, $form_state, $lom, $language);
  archibald_content_form_rights($form, $form_state, $lom, $language);
  archibald_content_form_education($form, $form_state, $lom, $language);
  archibald_content_form_classification($form, $form_state, $lom, $language);
  archibald_content_form_technical($form, $form_state, $lom, $language);
  archibald_content_form_relation($form, $form_state, $lom, $language);

  archibald_content_form_welcome($form, $form_state, $lom, $language);
  archibald_content_form_proposal_contact_info($form, $form_state, $lom, $language);

  archibald_check_field_permission_recursive($form);

  $form['save_top'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 0,
    '#attributes' => array('class' => array('archibald_form_submit_top')),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 9999,
    '#attributes' => array('class' => array('archibald_form_submit')),
  );

  // Prohibit double edit.
  drupal_add_library('system', 'ui.dialog');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/jquery-ui-1.8.23.custom.min.js');


  $data = db_query('
    SELECT editing_uid, editing_last_seen
    FROM {archibald_lom_stats}
    WHERE lom_id=:lom_id',
    array(
      ':lom_id' => $lom->getLomId(),
    )
  )->fetchAssoc();

  if (!empty($data['editing_uid'])) {
    $data['user'] = user_load($data['editing_uid']);
    $data['editing_last_seen'] = (INT) $data['editing_last_seen'];
  }
  else {
    $data['user'] = NULL;
    $data['editing_last_seen'] = 0;
    $data['editing_uid'] = 0;
  }

  // free:
  //   0 = resource is free to edit
  //   1 = resource is currendly in editing by another editor
  $free = 0;
  if (empty($data['editing_last_seen']) ||
      $data['editing_last_seen'] < (time() - ARCHIBALD_PROHIBIT_DOUBLE_EDIT_FREE_AFTER) ||
      (!empty($data['editing_uid']) && $data['editing_uid'] == $user->uid)) {
    $free = 1;
  }

  $form_blur_message = FALSE;
  if (variable_get('archibald_form_blur_message', 0) == 1) {
    $form_blur_message = TRUE;
  }

  $settings = array(
    'user' => (isset($data['user']->name)) ? $data['user']->name : '',
    'last_seen' => $data['editing_last_seen'],
    'url_ping' => url('archibald_prohibit_double_edit/ping/' . $lom->getLomId()),
    'url_checkout' => url(
      'archibald_prohibit_double_edit/checkout/' . $lom->getLomId()
    ),
    'free' => $free,
    'form_blur_message' => $form_blur_message,
  );

  if (empty($form_state['values'])) {
    archibald_show_lock_status_dialog($lom->getLomId(), $lom->getVersion(), $lom->getLifeCycle()->getStatus());
  }

  if (!empty($data['editing_uid'])) {
    drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald_ui_dialog.css');
    drupal_add_js(drupal_get_path('module', 'archibald') . '/js/prohibit_double_edit.js');
    drupal_add_js(array('archibald_prohibit_double_edit' => $settings), 'setting');
  }

  return $form;
}

/**
 * add recomendet or required stars to field name
 *
 * @param string $name
 *   Field name or id to check.
 *
 * @return string
 *   Html appendix for formfield.
 */
function archibald_get_require_type_value($name) {
  switch (archibald_get_field_requirements($name)) {
    case ARCHIBALD_RES_FIELD_REQUIRED:
      return ' <span class="form-required">*</span>';

    case ARCHIBALD_RES_FIELD_RECOMMEND:
      return ' <span class="form-recommended">*</span>';
  }

  return '';
}

/**
 * check can edit permission recursiv
 * and hide fieldsets if user have no permission to any included fields
 * @param array $form
 * @param string $parent
 * @return boolean
 */
function archibald_check_field_permission_recursive(&$form, $parent = "") {
  if (empty($form)) {
    return FALSE;
  }

  if (!is_array($form)) {
    return FALSE;
  }

  if (!empty($parent)) {
    $parent .= " ";
  }

  $remove_self_key_check = array();
  $return_true = TRUE;
  foreach ($form as $k => & $v) {
    if (preg_match("/^\#/", $k)) {
      continue;
    }
    $current_parent = $parent;

    $return_true = FALSE;
    $remove_self_key_check[$k] = $k;
    switch ($k) {
      case 'session_ident':
        $check_k = array('curriculums', 'defaultCurriculum');
        break;

      case 'description_spell_check':
        $check_k = 'description';
        break;

      case 'learningResourceType_pedagogical':
      case 'learningResourceType_documentary':
        $check_k = 'learningResourceType';
        break;

      case 'preview_image':
      case 'location':
      case 'otherPlattformRequirements':
        $current_parent = 'technical ';

      default:
        $check_k = $k;
        break;
    }

    if( is_array($check_k) ) {
      $check = 0;
      foreach ($check_k as $kk => $vv) {
        if(archibald_check_field_permission($current_parent . $vv)) $check++;
      }
      if( $check == 0 ){
        unset($form[$k]);
        unset($remove_self_key_check[$k]);
        continue;
      }
    } else {
      if (archibald_check_field_permission($current_parent . $check_k) . '' != "1") {
        unset($form[$k]);
        unset($remove_self_key_check[$k]);
        continue;
      }
    }

    if (!archibald_check_field_permission_recursive($v, $parent . $k)) {
      unset($form[$k]);
    }
  }
  return $return_true || !empty($remove_self_key_check);
}

/**
 * get by taxonomy vocabulary machine_name a flat select options list
 * @staticvar string $vocabularies
 *
 * @param string $vocab
 * @param strign $parant
 *   taxon_key
 *
 * @return array
 */
function archibald_get_taxonomy_options($vocab, $parent=NULL, $ignore_context=FALSE, $depth=FALSE) {
  $vocabularies = &archibald_static(__FUNCTION__, NULL);

  if (is_null($vocabularies)) {
    $vocabularies = array();
    foreach (taxonomy_get_vocabularies() as $vocabularie) {
      $vocabularies[$vocabularie->machine_name] = $vocabularie->vid;
    }
  }

  if (empty($vocabularies[$vocab])) {
    return array();
  }

  $vals = array();
  $tree = taxonomy_get_tree($vocabularies[$vocab], 0, NULL, TRUE);

  $parent_key = NULL;
  if (!empty($parent)) {
    foreach ($tree as $term) {
      $taxon_key = @$term->field_taxon_key['und'][0]['value'];
      if ($parent == $taxon_key) {
        $parent_key = $term->tid;
        break;
      }
    }
  }

  foreach ($tree as $term) {

    $taxon_key = @$term->field_taxon_key['und'][0]['value'];
    $taxon_context = @$term->field_taxon_context['und'][0]['value'];

    if (!empty($taxon_key)) {
      if (!$ignore_context && !empty($taxon_context)) {
        $key = $taxon_context . '|' . $taxon_key;
      }
      else {
        $key = $taxon_key;
      }
    }
    else {
      $key = $term->name;
    }
    if( !empty($depth) ) $key .= '|' . $term->depth;

    $value = $term->name;

    if (is_null($parent_key) || in_array($parent_key, $term->parents)) {
      if (isset($term->field_taxon_deprecated['und'][0]['value'])) {
        if ($term->field_taxon_deprecated['und'][0]['value'] == 0) {
          $vals[$key] = i18n_taxonomy_term_name($term);
        }
      }
      else {
        $vals[$key] = i18n_taxonomy_term_name($term);
      }
    }
  }

  return $vals;
}

/**
 * Set the form error if we want to not allow empty required values
 * or displays a warning message but pass the validation
 *
 * @param string $field
 *   the form field to be marked as invalid if invalid
 * @param string $field_name
 *   the name of a field
 */
function archibald_form_set_error_or_info($field, $field_name) {
  $msg  = filter_xss(t('!name field is required.', array('!name' => archibald_get_field_label($field_name, TRUE))));

  if (!archibald_can_only_propose() && variable_get('archibald_resource_save_able_with_errors', 1) == TRUE) {
    drupal_set_message($msg, 'warning');
  }
  else {
    form_set_error($field, $msg);
  }
}

/**
 * Check if a given form field is empty or not
 *
 * @param string $require_key
 *   the key for is requriec configuration
 *
 * @param string $value_name
 *   the key of the form value
 *
 * @param array &form_state
 *
 * @param array $form
 *
 * @return boolean
 *   TRUE if empty, else FALSE
 */
function archibald_form_value_is_empty($require_key, $value_name, &$form_state, &$form) {

  if (archibald_get_field_requirements($require_key) != ARCHIBALD_RES_FIELD_REQUIRED) {
    // This field is not required.
    return FALSE;
  }

  $key_arr = explode('-', $value_name);
  $form_state_value = $form_state['values'];
  $form_value = $form;

  foreach ($key_arr as $key) {

    if (!isset($form_value[$key])) {
      // The field does not seem to exist.
      return FALSE;
    }

    $form_value = $form_value[$key];

    if (!is_array($form_state_value) && empty($form_state_value[$key])) {
      $form_state_value = NULL;
    }
    else {
      $form_state_value = $form_state_value[$key];
    }
  }

  if (is_array($form_state_value)) {

    if ($require_key == 'technical.duration') {
      return( (array_sum($form_state_value) == 0 ? TRUE : FALSE) );
    }

    if (!isset($form_value['#title'])) {
      // The field dosend them to exis.
      return FALSE;
    }
    foreach ($form_state_value as $val) {
      if (!empty($val)) {
        return FALSE;
      }
    }
    return TRUE;
  }
  return empty($form_state_value);
}

/**
 * Merge an array recrsuive
 *
 * @param array $source_array
 *   the source array which will be filled
 *   if the key from array2 is not set within $source_array
 * @param array $array2
 */
function archibald_form_merge_recrusive(&$source_array, $array2) {
  foreach ($array2 as $k => $v) {
    if (!isset($source_array[$k])) {
      $source_array[$k] = $v;
    }
    else {
      if (is_array($v)) {
        archibald_form_merge_recrusive($source_array[$k], $v);
      }
    }
  }
}

/*
  .##.....##....###....##.......####.########.....###....########.####..#######..##....##
  .##.....##...##.##...##........##..##.....##...##.##......##.....##..##.....##.###...##
  .##.....##..##...##..##........##..##.....##..##...##.....##.....##..##.....##.####..##
  .##.....##.##.....##.##........##..##.....##.##.....##....##.....##..##.....##.##.##.##
  ..##...##..#########.##........##..##.....##.#########....##.....##..##.....##.##..####
  ...##.##...##.....##.##........##..##.....##.##.....##....##.....##..##.....##.##...###
  ....###....##.....##.########.####.########..##.....##....##....####..#######..##....##
*/

/**
 * validator for archibald_content_form()
 * it is only a basic validator
 * the full validaton will be dont on status change to "finale"
 * by class ArchibaldLomValidate
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_validate($form, &$form_state) {
  ctools_include('modal');
  ctools_modal_add_js();

  // drupal_add_library('system', 'ui.sortable');

  $form_is_proposal = archibald_form_is_proposal(empty($form_state['values']['lom_id']));
  archibald_fields_is_proposal($form_is_proposal);

  // archibald_general_handle_file_upload($form_state);

  if ($form_state['values']['op'] != $form_state['values']['save']) {
    // Prevent validation on "Add keywords" or simular buttons.
    return FALSE;
  }

  //Validation: general -> title
  // if ($form_is_proposal && empty($form_state['values']['general']['title'])) {
  if (empty($form_state['values']['general']['title'])) {
    archibald_form_set_error_or_info('general][title', 'general.title');
  }

  // Validation: general -> Your message (proposal)
  if ($form_is_proposal && archibald_form_value_is_empty('general.proposalMessage', 'general-proposal_message', $form_state, $form)) {
    archibald_form_set_error_or_info('general][proposal_message', 'general.proposalMessage');
  }

  //Validation: general -> description
  if (archibald_form_value_is_empty('general.description', 'general-description', $form_state, $form)) {
    archibald_form_set_error_or_info('general][description', 'general.description');
  }

  //Validation: general -> language
  if (archibald_form_value_is_empty('general.language', 'general-language', $form_state, $form)) {
    archibald_form_set_error_or_info('general][language', 'general.language');
  }

  //Validation: general -> previewImage
  if (archibald_form_value_is_empty('technical.previewImage', 'general-preview_image', $form_state, $form)) {
    archibald_form_set_error_or_info('general][preview_image', 'technical.previewImage');
  }

  //Validation: life_cycle -> version
  if (archibald_form_value_is_empty('lifecycle.version', 'general-life_cycle-version', $form_state, $form)) {
    archibald_form_set_error_or_info('general][life_cycle][version', 'lifecycle.version');
  }

  // Validation: technical -> location
  if (archibald_form_value_is_empty('technical.location', 'general-technical-location', $form_state, $form)) {
    archibald_form_set_error_or_info('general][technical][location', 'technical.location');
  }

  //Validation: technical -> otherPlattformRequirements
  if (archibald_form_value_is_empty('technical.otherPlattformRequirements', 'technical-otherPlattformRequirements', $form_state, $form)) {
    archibald_form_set_error_or_info('technical][otherPlattformRequirements', 'technical.otherPlattformRequirements');
  }

  //Validation: technical -> previewImageCopyrightDescription
  if (archibald_form_value_is_empty('technical.previewImageCopyrightDescription', 'general-previewImageCopyrightDescription', $form_state, $form)) {
    archibald_form_set_error_or_info('general][previewImageCopyrightDescription', 'technical.previewImageCopyrightDescription');
  }

  //Validation: technical -> duration
  if (archibald_form_value_is_empty('technical.duration', 'education-duration', $form_state, $form)) {
    archibald_form_set_error_or_info('education][duration', 'technical.duration');
  }

  //Validation: technical -> Format
  if (archibald_form_value_is_empty('technical.format', 'technical-format', $form_state, $form)) {
    archibald_form_set_error_or_info('technical][format', 'technical.format');
  }

  //Validation: technical -> Size
  if (archibald_form_value_is_empty('technical.size', 'technical-size', $form_state, $form)) {
    archibald_form_set_error_or_info('technical][size', 'technical.size');
  }

  //Validation: education -> intendedEndUserRole
  if (archibald_form_value_is_empty('education.intendedEndUserRole', 'education-intendedEndUserRole', $form_state, $form)) {
    archibald_form_set_error_or_info('education][intendedEndUserRole', 'education.intendedEndUserRole');
  }

  //Validation: education -> typicalAgeRange
  if (archibald_form_value_is_empty('education.typicalAgeRange', 'education-typicalAgeRange-from', $form_state, $form) && archibald_form_value_is_empty('education.typicalAgeRange', 'education-typicalAgeRange-to', $form_state, $form)) {
    archibald_form_set_error_or_info('education[typicalAgeRange', 'education.typicalAgeRange');
  }

  //Validation: education -> typicalLearningTime
  if (archibald_form_value_is_empty('education.typicalLearningTime', 'education-typicalLearningTime', $form_state, $form)) {
    archibald_form_set_error_or_info('education][typicalLearningTime', 'education.typicalLearningTime');
  }

  //Validation: education -> difficulty
  if (archibald_form_value_is_empty('education.difficulty', 'education-difficulty', $form_state, $form)) {
    archibald_form_set_error_or_info('education][difficulty', 'education.difficulty');
  }

  //Validation: education -> aggregationLevel
  if (archibald_form_value_is_empty('education.aggregationLevel', 'general-aggregationLevel', $form_state, $form)) {
    archibald_form_set_error_or_info('general][aggregationLevel', 'education.aggregationLevel');
  }

  //Validation: education -> description
  if (archibald_form_value_is_empty('education.description', 'education-description', $form_state, $form)) {
    archibald_form_set_error_or_info('education][description', 'education.description');
  }

  //Validation: rights -> description
  if (archibald_form_value_is_empty('right.description', 'rights-description', $form_state, $form)) {
    archibald_form_set_error_or_info('rights][description', 'right.description');
  }

  //Validation: classification -> context
  if (archibald_form_value_is_empty('classification.context', 'classification-context', $form_state, $form)) {
    archibald_form_set_error_or_info('classification][context', 'classification.context');
  }

  foreach (archibald_get_active_curriculums() as $curriculum) {
    $curr_name = 'curriculum_' . $curriculum;
    drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_classification.js');
    drupal_add_js(drupal_get_path('module', 'archibald') . '/js/' . $curr_name . '.js');
    drupal_add_css(drupal_get_path('module', 'archibald') . '/css/' . $curr_name . '.css');
  }

  /*
    ..######..##.....##.########..########..####..######..##.....##.##.......##.....##.##.....##..######.
    .##....##.##.....##.##.....##.##.....##..##..##....##.##.....##.##.......##.....##.###...###.##....##
    .##.......##.....##.##.....##.##.....##..##..##.......##.....##.##.......##.....##.####.####.##......
    .##.......##.....##.########..########...##..##.......##.....##.##.......##.....##.##.###.##..######.
    .##.......##.....##.##...##...##...##....##..##.......##.....##.##.......##.....##.##.....##.......##
    .##....##.##.....##.##....##..##....##...##..##....##.##.....##.##.......##.....##.##.....##.##....##
    ..######...#######..##.....##.##.....##.####..######...#######..########..#######..##.....##..######.
  */

  // Validation: classification -> curriculums
  if (archibald_get_field_requirements('classification.curriculums') == ARCHIBALD_RES_FIELD_REQUIRED || archibald_get_field_requirements('classification.defaultCurriculum') ==
    ARCHIBALD_RES_FIELD_REQUIRED) {
    $session_ident = $form_state['values']['classification']['session_ident'];

    $classification = (array) unserialize($_SESSION['lom_classification'][$session_ident]);

    $has_default = FALSE;
    $has_specific = FALSE;

    if (!empty($classification)) {
      $has_default = TRUE;

      // foreach ($classification as $c) {
      //   if ($c instanceof ArchibaldLomDataClassification) {
      //     $t = $c->getTaxonPath();
      //     foreach ($t as $taxon_path_id => $taxon_path) {
      //       if ($taxon_path instanceof ArchibaldLomDataTaxonPath) {
      //         $source = $taxon_path->getSource()->getStrings();
      //         // get first element
      //         $source = reset($source);
      //         switch( $source ) {
      //           default:
      //             $has_specific = TRUE;
      //           break;
      //           case 'educa':
      //             $has_default = TRUE;
      //           break;
      //         }
      //       }
      //     }
      //   }
      // }
    }

    $curriculums = unserialize( $_SESSION['lom_curriculum'][$session_ident]);
    if (!empty($curriculums)) {
      foreach ($curriculums AS $curriculum) {
        if (!empty($curriculum->entity)) {
          $o = json_decode($curriculum->entity);

          // @todo (dp) take care of this bug better: specific curricula do not get properly deleted when removed in the ctools dialog. check here if an empty object is present, in  which case the session value should be empty.
          $has_children = TRUE;
          while( $has_children ) {
            foreach( $o AS $key=>$obj ) {
              $obj = (array)$obj;
              if( !empty($obj) ) {
                $o = $obj;
                if( $key == 'code' )
                {
                  $has_children = FALSE;
                  $has_specific = TRUE;
                  break;
                }
              } else {
                $has_children = FALSE;
              }
            }
          }
        }
        if( !$has_specific ) {
          $_SESSION['lom_curriculum'][$session_ident] = '';
        }
      }
    }

    if( archibald_get_field_requirements('classification.curriculums') == ARCHIBALD_RES_FIELD_REQUIRED && !$has_specific ) {
      archibald_form_set_error_or_info('classification][session_ident', 'classification.curriculums');
    }
    if( archibald_get_field_requirements('classification.defaultCurriculum') == ARCHIBALD_RES_FIELD_REQUIRED && !$has_default ) {
      archibald_form_set_error_or_info('classification][session_ident', 'classification.defaultCurriculum');
    }
  }


  //Get all identifiers
  $existing_identifiers = array();
  $prepend_error = t('Identifiers') . ' :: ';
  foreach ($form_state['values']['general']['identifier'] as $i => $identifier) {

    if (is_array($identifier) && !empty($identifier['entity']) && !empty($identifier['catalog'])) {

      switch (drupal_strtoupper($identifier['catalog'])) {
        case 'ISBN':
          if (!preg_match('/(ISBN)?[ \-]*[0-9\-\ ]{6,}/i', $identifier['entity'])) {
            form_set_error(
              'general][identifier][' . $i . '][entity',
              $prepend_error . t('Please enter a valid ISBN, example: "ISBN 978-3-16-148410-0"')
            );
          }
          break;

        case 'DOI':
          if (!preg_match('/^(10\.\d{4})((?:[.][0-9]+)*)\/(.*)/i', $identifier['entity'])) {
            form_set_error(
              'general][identifier][' . $i . '][entity',
              $prepend_error . t('Please enter a valid DOI, example: "10.1000/123456"')
            );
          }
          break;

        case 'URL':
          $url_parts = parse_url($identifier['entity']);
          if (empty($url_parts['scheme']) || empty($url_parts['host'])) {
            form_set_error('general][identifier][' . $i . '][entity', $prepend_error . t('Please enter a valid URL.'));
          }
          break;
      }
      $key = $identifier['catalog'] . '_____' . $identifier['entity'];

      if (isset($existing_identifiers[$key])) {
        form_set_error('general][identifier][' . $i . '][entity', t('Please don\'t set an identifier twice.'));
      }

      $existing_identifiers[$key] = TRUE;
    }
  }


  // at least one identifier needs to be set
  if (count($existing_identifiers) == 0) {
    archibald_form_set_error_or_info('general][identifier', 'general.identifier');
  }

  //Validation: life cycle -> keywords
  if (archibald_check_field_permission('keywords keyword') && isset($form_state['values']['keywords']['keyword'])) {

    $is_empty = TRUE;
    $existing_keyword = array();

    foreach ($form_state['values']['keywords']['keyword'] as $i => $keyword) {
      $keyword = drupal_strtolower(trim($keyword));

      if (empty($keyword) || $keyword == 'archibald_delete' || !preg_match("/^keyword_[a-z0-9]+$/", $i)) {
        continue;
      }

      $is_empty = FALSE;

      if (isset($existing_keyword[$keyword])) {
        form_set_error('keywords][keyword][' . $i, t("Please don't set keywords twice."));
      }

      $existing_keyword[$keyword] = TRUE;
    }

    if ($is_empty && archibald_get_field_requirements('general.keyword') == ARCHIBALD_RES_FIELD_REQUIRED) {
      archibald_form_set_error_or_info('keywords][keyword', 'general.keyword');
    }
  }

  //Validation: keywords -> coverages
  if (archibald_check_field_permission('keywords coverage') && isset($form_state['values']['keywords']['coverage'])) {

    $is_empty = TRUE;
    $existing_coverage = array();

    foreach ($form_state['values']['keywords']['coverage'] as $i => $coverage) {
      $coverage = drupal_strtolower(trim($coverage));

      if (empty($coverage) || !preg_match("/^coverage_[a-z0-9]+$/", $i)) {
        continue;
      }

      $is_empty = FALSE;

      if (isset($existing_coverage[$coverage])) {
        form_set_error('keywords][coverage][' . $i, t("Please don't set coverages twice."));
      }
      $existing_coverage[$coverage] = TRUE;

      $exist = db_select('archibald_coverage_list', 'cl')
        ->fields('cl')
        ->where('LOWER(coverage) = :cov', array(':cov' => $coverage))
        ->condition('language', $form_state['values']['language'])
        ->execute()
        ->fetchAssoc();

      if (empty($exist)) {
        form_set_error('keywords][coverage][' . $i, t('The coverage "@coverage (@lang)" does not exist within the database.', array(
            '@coverage' => $coverage,
            '@lang' => $form_state['values']['language'],
        )));
      }
    }

    if ($is_empty && archibald_get_field_requirements('general.coverage') == ARCHIBALD_RES_FIELD_REQUIRED) {
      archibald_form_set_error_or_info('keywords][coverage', 'general.coverage');
    }
  }

  //Validation: life cycle -> contributor
  if (archibald_check_field_permission('life_cycle contribute') && isset($form_state['values']['life_cycle']['contribute'])) {

    $is_empty = TRUE;
    $existing_contributes = array();

    foreach ($form_state['values']['life_cycle']['contribute'] as $i => $contribute) {

      if (empty($contribute['entity']) || !preg_match("/^contribute_[a-z0-9]+$/", $i)) {
        continue;
      }

      $is_empty = FALSE;
      $key = $contribute['role'] . $contribute['entity'];
      if (isset($existing_contributes[$key])) {
        form_set_error(
          'life_cycle][contribute][' . $i,
          t('Please don\'t set contributors with same person and same role twice.')
        );
      }
      $existing_contributes[$key] = TRUE;
    }

    if ($is_empty && archibald_get_field_requirements('lifecycle.contributor') == ARCHIBALD_RES_FIELD_REQUIRED) {
      archibald_form_set_error_or_info('life_cycle][contribute', 'lifecycle.contributor');
    }
  }


  //Validation: Learning resource type
  if (archibald_check_field_permission('education learningResourceTypeDocumentary') || archibald_check_field_permission('education learningResourceTypePedagogical')) {

    if (!empty($form_state['values']['education'])) {
      $opts = array(
        'Documentary' => 'documentary',
        'Pedagogical' => 'pedagogical'
      );
      foreach ($opts as $ktype => $type) {
        if (archibald_get_field_requirements('education.learningResourceType'. $ktype) == ARCHIBALD_RES_FIELD_REQUIRED) {
          $selected_options = $form_state['values']['education']['learningResourceType_' . $type];

          $is_empty = 0;

          foreach ($selected_options as $i => $learning_resource_type) {
            if (!empty($learning_resource_type)) {
              $is_empty++;
            }
          }

          if( empty($is_empty) ) {
            archibald_form_set_error_or_info('education][learningResourceType' . $ktype, 'education.learningResourceType' . $ktype);
          }
        }
      }
    }
  }

  /*
    ..######..####.########.########
    .##....##..##.......##..##......
    .##........##......##...##......
    ..######...##.....##....######..
    .......##..##....##.....##......
    .##....##..##...##......##......
    ..######..####.########.########
  */

  //Validation: Size
  if (archibald_check_field_permission('technical size')) {
    if (!empty($form_state['values']['technical']) && !empty($form_state['values']['technical']['size'])) {
      $n = $form_state['values']['technical']['size'];
      $msg = t('Please enter a numerical size');
      // If not numerical
      if( !is_numeric($n) ) {
        form_set_error('technical][size', t('Technical requirements') . ' - ' . t('Size') . ' :: ' . t('Please enter a numerical value'));
      } else {
        // If there is a decimal, and it is equal to zero, truncate it.
        if (($n - floor($n)) == 0) $n = floor($n);
        if (strlen($n) > 5) {
          // If integer > 3 digits
          form_set_error('technical][size', t('Technical requirements') . ' - ' . t('Size') . ' :: ' . t('Maximum allowed is three digits and one decimal'));
        } else {
          // Check if there is a decimal
          $decimal_length = strlen(substr(strrchr($n, "."), 1));
          if ($decimal_length > 1) {
              // If more than 1 decimal
              form_set_error('technical][size', t('Technical requirements') . ' - ' . t('Size') . ' :: ' . t('Maximum allowed is one decimal'));
          } elseif (strlen(floor($n)) > 3) {
            // If integer > 3 digits
            form_set_error('technical][size', t('Technical requirements') . ' - ' . t('Size') . ' :: ' . t('Maximum allowed is three digits'));
          }
        }
      }
    }
  }

  /*
    .##........#######...######.....###....########.####..#######..##....##
    .##.......##.....##.##....##...##.##......##.....##..##.....##.###...##
    .##.......##.....##.##........##...##.....##.....##..##.....##.####..##
    .##.......##.....##.##.......##.....##....##.....##..##.....##.##.##.##
    .##.......##.....##.##.......#########....##.....##..##.....##.##..####
    .##.......##.....##.##....##.##.....##....##.....##..##.....##.##...###
    .########..#######...######..##.....##....##....####..#######..##....##
  */
  //Validation: Location(s)
  if (archibald_check_field_permission('technical location')) {
    $empty = TRUE;
    if (!empty($form_state['values']['technical']) && !empty($form_state['values']['technical']['location'])) {
      foreach ($form_state['values']['technical']['location'] as $i => $location) {
        if( !empty($location['type']) && $location['type'] == 'url' && strlen($location['value']) &&  $location['value'] != t('Add location') )
        {
          $empty = FALSE;
          if ($location['type'] == 'url') {
            $url = $location['value'];
            if (!filter_var($url, FILTER_VALIDATE_URL)) {
              form_set_error('technical][location][' . $i .'][value', t('Location') . ' :: ' . t('Please enter a valid URL'));
            }
          }
        }

        if( !empty($location['type']) && $location['type'] == 'text' && strlen($location['value']) )
        {
          $empty = FALSE;
        }
      }
    }

    if ($empty && archibald_get_field_requirements('technical.location') == ARCHIBALD_RES_FIELD_REQUIRED) {
      archibald_form_set_error_or_info('technical][location', 'technical.location');
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
  // Validation: Relations.
  if (archibald_check_field_permission('relations relation')) {
    $is_empty = TRUE;
    $prepend_error = t('Relations') . ' :: ';
    if (!empty($form_state['values']['relations']['relation'])) {
      foreach ($form_state['values']['relations']['relation'] as $i => $relation) {
        if ($i == 'add_more_relation' || !is_array($relation) || empty($relation['value'])) {
          continue;
        }
        $is_empty = FALSE;

        if (!empty($relation['catalog']) && !empty($relation['value'])) {
          $relation_catalog = $relation['catalog'];
          $relation_catalog = explode( '|', $relation_catalog);
          $relation_catalog = array_pop($relation_catalog);
          switch (drupal_strtoupper($relation_catalog)) {
            case 'REL_ISBN':
              if (!preg_match('/(ISBN)?[ \-]*[0-9\-\ ]{6,}/i', $relation['value'])) {
                form_set_error(
                  'relations][relation][' . $i . '][value',
                  $prepend_error . t('Please enter a valid ISBN, example: "ISBN 978-3-16-148410-0"')
                );
              }
              break;

            case 'REL_DOI':
              if (!preg_match('/^(10\.\d{4})((?:[.][0-9]+)*)\/(.*)/i', $relation['value'])) {
                form_set_error(
                  'relations][relation][' . $i . '][value',
                  $prepend_error . t('Please enter a valid DOI, example: "10.1000/123456"')
                );
              }
              break;

            case 'REL_URL':
              $url_parts = parse_url($relation['value']);
              if (empty($url_parts['scheme']) || empty($url_parts['host'])) {
                $prepend_error . form_set_error('relations][relation][' . $i . '][value', $prepend_error . t('Please enter a valid URL.'));
              }
              break;
          }
        }
      }
      if ($is_empty && archibald_get_field_requirements('relation.relation') == ARCHIBALD_RES_FIELD_REQUIRED) {
        archibald_form_set_error_or_info('relations][relation', 'relation.relation');
      }
    }
  }

  //Validation: proposal elements
  if ($form_is_proposal) {
    //Validation: contactInfo -> proposalContactName
    if (archibald_form_value_is_empty('contactInfo.proposalContactName', 'contactInfo-proposalContactName', $form_state, $form)) {
      archibald_form_set_error_or_info('contactInfo][proposalContactName', 'contactInfo.proposalContactName');
    }

    //Validation: contactInfo -> proposalContactEmail
    if (archibald_form_value_is_empty('contactInfo.proposalContactEmail', 'contactInfo-proposalContactEmail', $form_state, $form)) {
      archibald_form_set_error_or_info('contactInfo][proposalContactEmail', 'contactInfo.proposalContactEmail');
    }
    elseif (!valid_email_address($form_state['values']['contactInfo']['proposalContactEmail'])) {
      form_set_error('contactInfo][proposalContactEmail', t('The field !field is not a valid E-Mail', array('!field' => archibald_get_field_label('contactInfo.proposalContactEmail', TRUE))));
    }

    //Validation: contactInfo -> proposalContactPhone
    if (archibald_form_value_is_empty('contactInfo.proposalContactPhone', 'contactInfo-proposalContactPhone', $form_state, $form)) {
      archibald_form_set_error_or_info('contactInfo][proposalContactPhone', 'contactInfo.proposalContactPhone');
    }
  }
}

/**
 * Form submit handler which saves form values to database.
 *
 * @param array $form
 *  put a description of the param
 *  bla bla.
 * @param array $form_state
 *
 * @return
 *  if there were one.
 */
function archibald_content_form_submit($form, &$form_state) {
  global $user;

  // drupal_add_library('system', 'ui.sortable');

  archibald_fields_is_proposal(archibald_form_is_proposal(empty($form_state['values']['lom_id'])));
  $set_responsible_uid = FALSE;
  if (!empty($form_state['values']['lom_id'])) {
    $lom_version = '';
    if (!empty($form_state['values']['version'])) {
      $lom_version = $form_state['values']['version'];
    }
    $lom = archibald_load($form_state['values']['lom_id'], $lom_version);

    // as current user as editor
    $meta_metadata = $lom->getMetaMetadata();
    $contribute = archibald_add_meta_meta_contributor();
    if ($contribute instanceof ArchibaldLomDataContribute) {
      $meta_metadata->addContribute($contribute);
    }
    $lom->setMetaMetadata($meta_metadata);

    // In case lom resource is already set to 'final', set status back to draft.
    $lom_lifeCycle = $lom->getLifeCycle();
    $status = $lom_lifeCycle->getStatus();

    if ($status == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_FINALE) {
      $status = ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT;
      $lom_lifeCycle->setStatus($status);
      $lom->setLifeCycle($lom_lifeCycle);
    }
  }
  else {
    $lom = new ArchibaldLom();

    $meta_metadata = new ArchibaldLomDataMetaMetadata();
    $meta_metadata->setLanguage($form_state['values']['language']);

    $contribute = archibald_add_meta_meta_contributor();
    if ($contribute instanceof ArchibaldLomDataContribute) {
      $meta_metadata->addContribute($contribute);
    }
    $lom->setMetaMetadata($meta_metadata);

    $lom_lifeCycle = new ArchibaldLomDataLifeCycle();
    $status = (user_access('edit archibald content') ? ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_DRAFT : ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE);
    $lom_lifeCycle->setStatus($status);
    $lom->setLifeCycle($lom_lifeCycle);

    $set_responsible_uid = TRUE;
  }

  archibald_content_form_general_submit($form, $form_state, $lom);
  archibald_content_form_keywords_submit($form, $form_state, $lom);
  archibald_content_form_education_submit($form, $form_state, $lom);
  archibald_content_form_technical_submit($form, $form_state, $lom);
  archibald_content_form_rights_submit($form, $form_state, $lom);
  archibald_content_form_classification_submit($form, $form_state, $lom);
  archibald_content_form_life_cycle_submit($form, $form_state, $lom);
  archibald_content_form_relation_submit($form, $form_state, $lom);

  $version = '';
  $lom_id  = archibald_save($lom, $version);
  $archibald_lom = new ArchibaldLom();
  $title   = $archibald_lom->determinTitle($lom);

  watchdog('Content', 'Description "!title" has been successfully saved', array(
    '!title' => $title,
  ), WATCHDOG_NOTICE, 'archibald/' . $lom_id);

  $proposal_message_viewed = FALSE;

  // is is a new resource
  if ($set_responsible_uid == TRUE) {

    if (!empty($form_state['values']['content_partner_id'])) {
      db_update('archibald_lom_stats')
        ->fields(array(
          'content_partner_id' => $form_state['values']['content_partner_id'],
        ))
        ->condition('lom_id', $lom_id)
        ->execute();
    }

    ArchibaldLomSaveHandler::setResponsibleUid($lom_id, $user->uid);

    // if a new lom object was created and it is a proposal
    if (empty($form_state['values']['lom_id']) && archibald_is_proposal($lom)) {
      drupal_set_message(t('Thank you! Your proposal "!title" has been successfully submitted.', array('!title' => $title)));

      $params = array(
        'lom_id' => $lom_id,
        'subject' => '',
        'body' => '',
      );

      db_update('archibald_lom_stats')
        ->fields(array(
          'proposal_contact_name' => $form_state['values']['contactInfo']['proposalContactName'],
          'proposal_contact_email' => $form_state['values']['contactInfo']['proposalContactEmail'],
          'proposal_contact_phone' => $form_state['values']['contactInfo']['proposalContactPhone'],
          'proposal_message' => $form_state['values']['general']['proposal_message'],
        ))
        ->condition('lom_id', $lom_id)
        ->execute();

      // Send proposal confirmation to the user who has submitted it
      $language = $form_state['values']['language'];
      $mail = drupal_mail('archibald', 'new_proposal', $form_state['values']['contactInfo']['proposalContactEmail'], $language, $params);

      // Send new proposal notice
      archibald_send_mail($lom_id, 'new_unavailable');
      $proposal_message_viewed = TRUE;
    }
  }

  if ($proposal_message_viewed === FALSE) {
    drupal_set_message(t('Description "!title" has been successfully saved.', array('!title' => $title)));
  }



  // clean up key session store
  unset($_SESSION['archibald_default_value_keys']);


  if (user_access('edit archibald content')) {
    $form_state['redirect'] = 'archibald/' . $lom_id;

    if (!empty($version)) {
      // give version id to url, cause postgres is sometimes to slow
      // and cache sql query with determine newsest lom version
      $form_state['redirect'] .= '/revisions/' . $version . '/view';
    }
  }
  else {
    // Redirect users submitting a proposal to the homepage
    $form_state['redirect'] = '';
  }


  // reset editor last seen bit, to release resource look
  archibald_prohibit_double_edit_checkout($lom_id, FALSE);
}

/**
 * generate form for duration element
 *
 * @param array $default_value
 * @param string $title
 * @param boolean $with_desc
 *   FALSE or language code for langstring
 * @param string $show
 * @param array $archibald_help
 *   options for
 *    archibald_help,
 *    possible keys,
 *    archibald_help_selector (value: attr|value),
 *    archibald_help_ident (value: help file identifier)
 *
 * @return array
 */
function archibald_duration_form($default_value, $title, $with_desc = FALSE, $show = 'ymdt', $archibald_help = array()) {

  if (empty($default_value)) {
    $default_value = array();
  }

  $default_value += array('year' => 0,
    'month' => 0,
    'days' => 0,
    'hours' => 0,
    'minutes' => 0,
    'seconds' => 0,
  );

  $form = array(
    '#type' => 'markup',
    '#title' => check_plain($title) . ' <span class="form-recommended">*</span>',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes' => $archibald_help,
    '#theme' => 'archibald_duration_form',
  );

  if (strpos($show, 'y') !== FALSE) {
    $options = range(0, 100);
    $form['year'] = array(
      '#type' => 'select',
      '#title' => t('Year'),
      '#default_value' => $default_value['year'],
      '#options' => array_combine($options, $options),
    );
  }

  if (strpos($show, 'm') !== FALSE) {
    $options = range(0, 12);
    $form['month'] = array(
      '#type' => 'select',
      '#title' => t('Month'),
      '#default_value' => $default_value['month'],
      '#options' => array_combine($options, $options),
    );
  }

  if (strpos($show, 'd') !== FALSE) {
    $options = range(0, (strpos($show, 'm') !== FALSE) ? 31 : 356);
    $form['days'] = array(
      '#type' => 'select',
      '#title' => t('Days'),
      '#default_value' => $default_value['days'],
      '#options' => array_combine($options, $options),
    );
  }

  if (strpos($show, 't') !== FALSE) {
    $options = range(0, 24);
    $form['hours'] = array(
      '#type' => 'select',
      '#title' => t('Hours'),
      '#default_value' => $default_value['hours'],
      '#options' => array_combine($options, $options),
    );

    $options = range(0, 60);
    $form['minutes'] = array(
      '#type' => 'select',
      '#title' => t('Minutes'),
      '#default_value' => $default_value['minutes'],
      '#options' => array_combine($options, $options),
    );

    $options = range(0, 60);
    $form['seconds'] = array(
      '#type' => 'select',
      '#title' => t('Seconds'),
      '#default_value' => $default_value['seconds'],
      '#options' => array_combine($options, $options),
    );
  }

  if (!empty($with_desc)) {
    $default_value = @$default_value['description'];
    if ($default_value instanceof ArchibaldLomDataLangString) {
      $default_value = $default_value->getString($with_desc);
    }
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $default_value,
      '#rows' => 5,
      '#attributes' => array('class' => array('lang_string_autocomplete')),
    );
  }

  return $form;
}

/**
 * theme a archibald form element duration
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_duration_form($form) {
  $form = reset($form);
  $attr = array();
  if (!is_array($form['#attributes'])) {
    $form['#attributes'] = array();
  }
  foreach ($form['#attributes'] as $attribute => $val) {
    $attr[] = $attribute . "=\"" . $val . "\"";
  }
  $output = '<label for="edit-duration" ' . implode(" ", $attr) . '>' . $form['#title'] . '</label>';

  $output .= '<div class="archibald_duration">';
  $output .= '<table border=0 cellpadding=1 cellspacing=0>';

  $elements = array();
  $time_intervals = array(
    t('Year'),
    t('Month'),
    t('Days'),
    t('Hours'),
    t('Minutes'),
    t('Seconds'),
  );
  foreach ($time_intervals as $k) {
    if (isset($form[$k])) {
      unset($form[$k]['#title']);
      $elements[0][] = '<th>' . $k . '</th>';
      $elements[1][] = '<td>' . theme($form[$k]['#type'], $form[$k]) . '</td>';
    }
  }

  $output .= '<tr>' . implode('', $elements[0]) . '</tr>';
  $output .= '<tr>' . implode('', $elements[1]) . '</tr>';
  $output .= '</table>';

  if (!empty($form['description'])) {
    $output .= drupal_render($form['description']);
  }

  $output .= '</div>';

  return $output;
}

/**
 * convert possible ArchibaldLomDataVocabulary to select/checkbox string
 *
 * @param ArchibaldLomDataVocabulary $default_value
 * return string
 */
function archibald_lom_vocab_default_val($default_value, $ignore_context = FALSE) {
  if ($default_value instanceof ArchibaldLomDataVocabulary) {
    $prepend = ($ignore_context) ? '' : $default_value->getSource() . '|';
    $default_value = $prepend . $default_value->getValue();
  }
  elseif (is_array($default_value)) {
    foreach ($default_value as $k => $v) {
      $default_value[$k] = archibald_lom_vocab_default_val($v, $ignore_context);
    }
  }

  return $default_value;
}

/**
 * filter options list for default value
 *
 * @param array $options
 * @param string $default_value
 */
function archibald_select_options_default_value_filter(&$options, &$default_value = array()) {

  $existing_options = array();
  foreach ($options as $k => $v) {
    $k = archibald_select_options_default_value_filter_splitter($k);
    $existing_options[$k] = $v;
  }

  if (!empty($default_value)) {
    if (is_array($default_value)) {
      foreach ($default_value as $k => $v) {
        $v = archibald_select_options_default_value_filter_splitter($v);
        $default_value[$k] = $v;
        if (!isset($existing_options[$v])) {
          $options[$v] = t($v);
        }
      }
    }
    elseif (!isset($options[$default_value])) {
      $default_value = archibald_select_options_default_value_filter_splitter($default_value);

      $default_value_key = $default_value;

      $default_value_human = $default_value;

      // if key is not pass through a form,
      // build md5 hashes and store it in session
      if (strpos($default_value_key, "\n") || drupal_strlen($default_value_key) > 50) {
        $default_value_hash = md5($default_value);
        $_SESSION['archibald_default_value_keys'][$default_value_hash] = $default_value;

        $default_value_human = strtr(
          $default_value_human,
          array(
            "\r" => '',
            "\n" => ' ',
            '  ' => ' ',
          )
        );

        if (drupal_strlen($default_value_human) >= 80) {
          $default_value_human = drupal_substr($default_value_human, 0, 80) . ' ...';
        }

        $default_value = $default_value_hash;
      }

      $options[$default_value] = $default_value_human;
    }
  }
}
