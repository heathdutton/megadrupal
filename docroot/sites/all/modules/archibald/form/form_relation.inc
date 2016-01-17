<?php

/**
 * @file
 * lom resource add/edit form part: for relation tab
 */

/*
  ..######..########..########....###....########.########
  .##....##.##.....##.##.........##.##......##....##......
  .##.......##.....##.##........##...##.....##....##......
  .##.......########..######...##.....##....##....######..
  .##.......##...##...##.......#########....##....##......
  .##....##.##....##..##.......##.....##....##....##......
  ..######..##.....##.########.##.....##....##....########
*/

/**
 * part of lom resource add/edit form
 *  tab: relation
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_relation(&$form, &$form_state, ArchibaldLom &$lom, $res_language) {

  if (!archibald_check_field_permission('relations relation')) {
    return;
  }


  // Include the CTools tools that we need.
  ctools_include('ajax');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  $lom_relations = $lom->getRelation();

  $form['relations'] = array(
    '#type' => 'fieldset',
    '#title' => archibald_get_field_label('relation'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'vt_main',
    '#tree' => TRUE,
  );

  $form['relations']['relation'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes' => array('archibald_help_ident' => 'relation.relation'),
    '#prefix' => '<div class="clearfix" id="archibald_relations_relation-wrapper">',
    '#suffix' => '</div>',
  );


  $form['relations']['relation']['#theme'] = 'archibald_relations_relation';

  // Load identifiers from lom object if nothing was commited.
  $rindex = 0;
  if (!isset($form_state['rindex'])) {
    $form_state['rindex'] = 0;
  }

  $k_ddm_options = archibald_get_relation_kind_options();
  $r_ddm_options = archibald_get_relation_catalog_options();

  // echo count( $lom_relations );
  // die();

  if (!isset($form_state['rindex']) || empty($form_state['rindex'])) {
    $form_state['rindex'] = 0;
  }

  if (!empty($lom_relations)) {
    foreach ($lom_relations as $relation) {

      if (!($relation instanceof ArchibaldLomDataRelation)) {
        continue;
      }

      $form['relations']['relation']['relation_' . $rindex] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#tree' => TRUE,
        '#weight' => $rindex,
      );

      // echo '<PRE>';
      // print_r( $k_ddm_options );
      // print_r( $relation->getKind() );
      // print_r( archibald_lom_vocab_default_val($relation->getKind()) );
      // die();
      $form['relations']['relation']['relation_' . $rindex]['kind'] = array(
        '#type' => 'select',
        '#title' => archibald_get_field_label('relation.kind'),
        '#default_value' => archibald_lom_vocab_default_val($relation->getKind()),
        '#options' => $k_ddm_options,
        '#attributes' => array('class' => array('relation_kind')),
      );

      $form['relations']['relation']['relation_' . $rindex]['catalog'] = array(
        '#type' => 'select',
        '#title' => archibald_get_field_label('relation.catalog'),
        '#default_value' => archibald_lom_vocab_default_val($relation->getCatalog()),
        '#options' => $r_ddm_options,
        '#attributes' => array('class' => array('relation_catalog')),
      );

      $default_value = $relation->getValue();
      $form['relations']['relation']['relation_' . $rindex]['value'] = array(
        '#type' => 'textfield',
        '#title' => archibald_get_field_label('relation.value'),
        '#default_value' => $default_value,
        '#size' => 80,
        '#attributes' => array('class' => array('relation_value')),
      );

      $lang_string = $relation->getDescription();
      $default_value = '';
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $default_value = $lang_string->getString($res_language);
      }
      $form['relations']['relation']['relation_' . $rindex]['description'] = array(
        '#type' => 'textarea',
        '#title' => archibald_get_field_label('relation.description'),
        '#default_value' => $default_value,
        '#attributes' => array('class' => array('relation_description')),
      );

      if (!empty($form_state['values']['relations']['relation']['add_more_relation'])) {
        $form_state['rindex']++;
      }
      $rindex++;
    }
  }

  if ($rindex == 0) archibald_content_form_relations_add_empty_relation($form, $form_state);
  if ($form_state['rindex'] > 0) {
    for ($i=0; $i < ($form_state['rindex'] - $rindex) ; $i++) {
      archibald_content_form_relations_add_empty_relation($form, $form_state);
    }
  }

  $form['relations']['relation']['add_more_relation'] = array(
    '#type' => 'submit',
    '#value' => t('Add a relation'),
    '#weight' => 9999,
    '#limit_validation_errors' => array(),
    // #submit is required to use #limit_validation_errors
    '#submit' => array(
      'archibald_content_form_add_more_relations_relation_submit',
    ),
    '#ajax' => array(
      'callback' => 'archibald_ajax_content_form_relations_add_empty_relation',
      'wrapper' => 'archibald_relations_relation-wrapper',
      'method' => 'replace',
    ),
  );
}


/**
 * Get the options for the relation's kind DDM
 *
 */
function archibald_get_relation_kind_options() {
  return archibald_get_taxonomy_options('rel_kind');
}

/**
 * Get the options for the relation's catalog DDM
 *
 */
function archibald_get_relation_catalog_options() {
  return archibald_get_taxonomy_options('rel_catalog');
}


/**
 * add new empty relation, this could called by default form or ajax callback
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_relations_add_empty_relation(&$form, &$form_state) {

  $i = 0;
  foreach (array_keys($form['relations']['relation']) as $k) {
    if (preg_match('/^relation_/', $k)) {
      $i++;
    }
  }

  $k_ddm_options = archibald_get_relation_kind_options();
  $r_ddm_options = archibald_get_relation_catalog_options();

  $form['relations']['relation']['relation_' . $i] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#weight' => $i,
  );

  $form['relations']['relation']['relation_' . $i]['kind'] = array(
    '#type' => 'select',
    '#title' => archibald_get_field_label('relation.kind'),
    '#options' => $k_ddm_options,
    '#attributes' => array('class' => array('relation_kind')),
  );

  $form['relations']['relation']['relation_' . $i]['catalog'] = array(
    '#type' => 'select',
    '#title' => archibald_get_field_label('relation.catalog'),
    '#options' => $r_ddm_options,
    '#attributes' => array('class' => array('relation_catalog')),
  );

  $form['relations']['relation']['relation_' . $i]['value'] = array(
    '#type' => 'textfield',
    '#title' => archibald_get_field_label('relation.value'),
    '#size' => 80,
    '#attributes' => array('class' => array('relation_value')),
  );

  $form['relations']['relation']['relation_' . $i]['description'] = array(
    '#type' => 'textarea',
    '#title' => archibald_get_field_label('relation.description'),
    '#attributes' => array('class' => array('relation_description')),
  );

}

/**
 * ajax menu callback
 * place new empty relation fields on bottom of relation list
 *
 * @return string
 */
function archibald_ajax_content_form_relations_add_empty_relation($form, $form_state) {
  return $form['relations']['relation'];
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_more_relations_relation_submit($form, &$form_state) {
  if (!isset($form_state['rindex'])) {
    $form_state['rindex'] = 0;
  }
  $form_state['rindex']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * theme a archibald form element relation
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_relations_relation($form) {
  $form = reset($form);
  $output = '<label for="edit-relations-title" class="htab_title">' . archibald_get_field_label('relation') .
    archibald_get_require_type_value('relation.relation') . archibald_get_field_help('relation.relation');
  $output .= '</label>';

  $output .= '<div id="archibald_relations_relation">';

  $index = 0;
  foreach ($form as $k => $element) {
    if (!preg_match('/^relation_\d+/', $k)) {
      continue;
    }
    unset($element['catalog']['#title']);
    unset($element['value']['#title']);

    $output .= '<div class="archibald_relation">';
      $output .= '<div class="archibald_remove_relation" rel="' . $index . '"></div>';
      $output .= '<table class="archibald_relations_relation_item">';
        $output .= '<tr><td colspan="2">';
          $output .= drupal_render($element['kind']);
        $output .= '</td></tr><tr><td class="archibald_relation_catalog_td">';
          $output .= drupal_render($element['catalog']);
        $output .= '</td><td>';
          $output .= drupal_render($element['value']);
        $output .= '</td></tr><tr><td colspan="2">';
          $output .= drupal_render($element['description']);
        $output .= '</td></tr>';
      $output .= '</table>';
    $output .= '</div>';
    $index++;
  }


  $output .= '</div>';
  $output .= drupal_render($form['add_more_relation']);

  return $output;
}


/*
  ..######..##.....##.########..##.....##.####.########
  .##....##.##.....##.##.....##.###...###..##.....##...
  .##.......##.....##.##.....##.####.####..##.....##...
  ..######..##.....##.########..##.###.##..##.....##...
  .......##.##.....##.##.....##.##.....##..##.....##...
  .##....##.##.....##.##.....##.##.....##..##.....##...
  ..######...#######..########..##.....##.####....##...
*/

/**
 * save handler for lom relation form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_relation_submit($form, &$form_state, &$lom) {
  if (empty($form_state['values']['relations']['relation'])) {
    return FALSE;
  }

  $relations = array();
  foreach ($form_state['values']['relations']['relation'] as $i => $relation) {

    if (!is_array($relation) || empty($relation['value'])) {
      continue;
    }


    $new_relation = new ArchibaldLomDataRelation();
    if (preg_match('/(.+)\|(.+)/', $relation['kind'], $parts)) {
      $vocabulary = new ArchibaldLomDataVocabulary();
      $vocabulary->setSource($parts[1]);
      $vocabulary->setValue($parts[2]);
      $new_relation->setKind($vocabulary);
    }

    if (preg_match('/(.+)\|(.+)/', $relation['catalog'], $parts)) {
      $vocabulary = new ArchibaldLomDataVocabulary();
      $vocabulary->setSource($parts[1]);
      $vocabulary->setValue($parts[2]);
      $new_relation->setCatalog($vocabulary);
    }

    $new_relation->setValue($relation['value']);

    $description = trim($relation['description']);
    if (strlen($description))
    {
      $lang_string = new ArchibaldLomDataLangString($description,$form_state['values']['language']);
      $lang_string->setString(trim($relation['description']), $form_state['values']['language']);
      $new_relation->setDescription($lang_string);
    }

    $relations[] = $new_relation;
  }

  $lom->setRelation($relations);
}

