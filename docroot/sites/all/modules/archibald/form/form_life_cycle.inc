<?php

/**
 * @file
 * lom resource add/edit form part: for life_cycle tab
 */

/**
 * part of lom resource add/edit form
 *  tab: life_cycle
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_life_cycle(&$form, &$form_state, &$lom, $language) {
  global $perms;

  if (empty($perms)) {
    $perms = archibald_permission();
  }

  // echo '<pre>';
  // print_r($perms);
  // exit;

  if (!archibald_check_field_permission('life_cycle contribute')){
    return;
  }

  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  $lom_life_cycle = $lom->getLifeCycle();
  if (!($lom_life_cycle instanceof ArchibaldLomDataLifeCycle)) {
    $lom_life_cycle = new ArchibaldLomDataLifeCycle();
  }

  $contributes = $lom_life_cycle->getContribute();

  $form['life_cycle'] = array(
    '#type' => 'fieldset',
    '#title' => '&nbsp;&nbsp;&nbsp;&nbsp;' . archibald_get_field_label('lifecycle.contributor') ,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'vt_main',
    '#tree' => TRUE
  );

  $form['life_cycle']['contribute'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#attributes' => array('archibald_help_ident' => 'life_cycle.contribute'),
    '#theme' => 'archibald_life_cycle_contribute',
    '#prefix' => '<div class="clearfix" ' .
    'id="archibald_life_cycle_contribute-wrapper">',
    '#suffix' => '</div>',
  );

  /** Load identfiers from lom object if nothing was commited * */
  $num_life_cycle_contribute = 0;
  if (!isset($form_state['num_life_cycle_contribute'])) {
    $form_state['num_life_cycle_contribute'] = 0;
  }

  if (!empty($contributes)) {
    foreach ($contributes as $contribute) {
      $date = $contribute->getDate();
      if ($date instanceof ArchibaldLomDataDateTime) {
        $date = $date->getDatetime(TRUE);
      }
      else {
        $date = NULL;
      }

      $form['life_cycle']['contribute']['contribute_' . $num_life_cycle_contribute] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#tree' => TRUE,
        '#weight' => $num_life_cycle_contribute,
      );

      $form['life_cycle']['contribute']['contribute_' . $num_life_cycle_contribute]['role'] = array(
        '#type' => 'select',
        '#title' => t('Type'),
        '#default_value' => archibald_lom_vocab_default_val(
          $contribute->getRole()
        ),
        '#options' => archibald_life_cycle_contribute_role_options(),
        '#attributes' => array(
          'class' => array(
            'life_cycle_contribute_role',
          ),
        ),
      );

      $archibald_lom_contributor = new ArchibaldLomContributor();
      $archibald_lom_contributor->load($contribute->getEntity(0));

      $default_name = $archibald_lom_contributor->getFormName();
      $default_value = $archibald_lom_contributor->getId();

      $form['life_cycle']['contribute']['contribute_' . $num_life_cycle_contribute]['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Person'),
        '#default_value' => $default_name,
        '#size' => 70,
        '#attributes' => array(
          'class' => array(
            'life_cycle_contribute_name',
          ),
        ),
      );

      $form['life_cycle']['contribute']['contribute_' . $num_life_cycle_contribute]['entity'] = array(
        '#type' => 'hidden',
        '#default_value' => $default_value,
      );

      if (!empty($form_state['values']['life_cycle']['contribute']['add_more_life_cycle_contribute'])) {
        $form_state['num_life_cycle_contribute']++;
      }
      $num_life_cycle_contribute++;
    }
  }
  if (empty($form_state['num_life_cycle_contribute'])) {
    $form_state['num_life_cycle_contribute'] = 1;
  }

  for ($i = $num_life_cycle_contribute; $i <= $form_state['num_life_cycle_contribute']; $i++) {
    archibald_content_form_life_cycle_add_empty_contribute($form, $form_state);
  }

  $form['life_cycle']['contribute']['add_more_life_cycle_contribute'] = array(
    '#type' => 'submit',
    '#value' => t('Add a contributor'),
    '#weight' => 9999,
    '#limit_validation_errors' => array(),
    // #submit is required to use #limit_validation_errors
    '#submit' => array(
      'archibald_content_form_add_more_life_cycle_contribute_submit',
    ),
    '#ajax' => array(
      'callback' =>
      'archibald_ajax_content_form_life_cycle_add_empty_contribute',
      'wrapper' => 'archibald_life_cycle_contribute-wrapper',
      'method' => 'replace',
      # 'effect'   => 'fade',
    ),
  );
}

/**
 * add new general keyword, this could called by default form or ajax callback
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_life_cycle_add_empty_contribute(&$form, &$form_state) {

  $i = 0;
  foreach (array_keys($form['life_cycle']['contribute']) as $k) {
    if (drupal_substr($k, 0, 11) == 'contribute_') {
      $i++;
    }
  }

  $form['life_cycle']['contribute']['contribute_' . $i] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#weight' => $i,
  );

  $form['life_cycle']['contribute']['contribute_' . $i]['role'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => archibald_life_cycle_contribute_role_options(),
    '#attributes' => array('class' => array('life_cycle_contribute_role')),
  );

  $form['life_cycle']['contribute']['contribute_' . $i]['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Person'),
    '#size' => 70,
    '#attributes' => array('class' => array('life_cycle_contribute_name')),
  );

  $form['life_cycle']['contribute']['contribute_' . $i]['entity'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );
}

/**
 * ajax menu callback
 * place new empty contribute field on bottom of contribute list
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_ajax_content_form_life_cycle_add_empty_contribute($form, $form_state) {

  return $form['life_cycle']['contribute'];
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_more_life_cycle_contribute_submit($form, &$form_state) {
  if (!isset($form_state['num_life_cycle_contribute'])) {
    $form_state['num_life_cycle_contribute'] = 0;
  }
  $form_state['num_life_cycle_contribute']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * get option array
 * for a checkboxes / select form item
 * with all possible role taxonomy terms
 *
 * @return array
 */
function archibald_life_cycle_contribute_role_options() {
  return archibald_get_taxonomy_options('lc_cont_role');
}

/**
 * theme a archibald form element life_cycle contribute
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_life_cycle_contribute($form) {
  $form = reset($form);

  $output = '<label for="edit-life_cycle-title" class="htab_title">' . t('Contributors') . archibald_get_require_type_value('lifecycle.contributor') .
    archibald_get_field_help('lifecycle.contributor');
  $output .= '</label>';

  $output .= '<div id="archibald_life_cycle_contribute">';

  foreach ($form as $k => $element) {
    if (drupal_substr($k, 0, 11) == 'contribute_') {
      unset($element['role']['#title']);
      unset($element['name']['#title']);

      // at some where here the MetaTMeta
      // Date Object Key to load it in modal frame
      $output .= '<table class="archibald_life_cycle_contribute_item">' . '<tr><td align="left">';
      $output .= drupal_render($element['role']);
      $output .= '</td><td align="left">';
      $output .= drupal_render($element['name']);
      $output .= drupal_render($element['entity']);
      $output .= '</td></tr>';
      $output .= '</table>';
    }
  }

  $output .= '</div>';
  $output .= drupal_render($form['add_more_life_cycle_contribute']);

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
 * save handler for lom education form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_life_cycle_submit($form, &$form_state, &$lom) {
  $lom_life_cycle = $lom->getLifeCycle();
  if (!($lom_life_cycle instanceof ArchibaldLomDataLifeCycle)) {
    $lom_life_cycle = new ArchibaldLomDataLifeCycle();
  }

  if (archibald_check_field_permission('life_cycle contribute')) {
    $contributes = array();
    foreach ($form_state['values']['life_cycle']['contribute'] as $i => $contribute) {

      if (empty($contribute['entity'])) {
        continue;
      }


      $entity = new ArchibaldLomContributor($contribute['entity']);

      $vcard = $entity->getVcard();

      if (empty($vcard)) {
        continue;
      }

      // create new contribute
      $new_contribute = new ArchibaldLomDataContribute();

      // add role
      preg_match('/(.+)\|(.+)/', $contribute['role'], $parts);
      $vocabulary = new ArchibaldLomDataVocabulary();
      $vocabulary->setSource($parts[1]);
      $vocabulary->setValue($parts[2]);
      $new_contribute->setRole($vocabulary);

      // add vcard
      $new_contribute->addEntity($vcard);

      // set date
      $new_contribute->setDate(new ArchibaldLomDataDateTime());

      $contributes[] = $new_contribute;
    }
    $lom_life_cycle->setContribute($contributes);
  }
  $lom->setLifeCycle($lom_life_cycle);
}

