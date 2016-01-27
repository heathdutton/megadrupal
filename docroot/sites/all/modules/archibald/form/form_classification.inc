<?php

/**
 * @file
 * lom resource add/edit form part: for classification tab
 */

/**
 * part of lom resource add/edit form
 *  tab: classification
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_classification(&$form, &$form_state, &$lom, $language) {
  ctools_add_js('auto-submit');

  $lom_education = $lom->getEducation();
  if (!($lom_education instanceof ArchibaldLomDataEducation)) {
    $lom_education = new ArchibaldLomDataEducation();
  }

  if(
    archibald_check_field_permission('classification context') ||
    archibald_check_field_permission('classification curriculums') ||
    archibald_check_field_permission('classification defaultCurriculum')
  ) {
    $form['classification'] = array(
      '#type' => 'fieldset',
      '#title' => '&nbsp;&nbsp;&nbsp;&nbsp;' . archibald_get_field_label('classification'),
      '#fieldset_title' => archibald_get_field_label('classification'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
      '#theme' => 'archibald_classification',
    );

    // Context
    if (archibald_check_field_permission('classification context')) {
      $default_value = archibald_lom_vocab_default_val(
        $lom_education->getContext()
      );
      $form['classification']['context'] = array(
        '#type' => 'checkboxes',
        '#title' => archibald_get_field_label('classification.context') .
          archibald_get_require_type_value('classification.context') .
          archibald_get_field_help('classification.context'),
        '#default_value' => $default_value,
        '#attributes' => array(
          'archibald_help_selector' => 'for|edit-education-context',
          'archibald_help_ident' => 'education.context',
        ),
        '#options' => archibald_get_education_context_options(),
      );
    }

    if (archibald_check_field_permission('classification curriculums') || archibald_check_field_permission('classification defaultCurriculum')) {

      if (!empty($form_state['values']['classification']['session_ident'])) {
        $session_ident = $form_state['values']['classification']['session_ident'];
      }
      else {
        $session_ident = $lom->getLomId();
        if (empty($session_ident)) {
          $session_ident = md5(uniqid(mt_rand(), TRUE));
        }
        $_SESSION['lom_classification'][$session_ident] = serialize($lom->getClassification());
        $_SESSION['lom_curriculum'][$session_ident] = serialize($lom->getCurriculum());

        $lom_classification = $lom->getClassification();
        $lom_curriculum = $lom->getCurriculum();

        // echo '<PRE>';
        // print_r( $lom_curriculum );
        // print_r( $lom_classification );
        // die();

      }

      if (!isset($lom_classification)) {
        $lom_classification = unserialize($_SESSION['lom_classification'][$session_ident]);
      }

      if (!isset($lom_curriculum)) {
        $lom_curriculum = unserialize($_SESSION['lom_curriculum'][$session_ident]);
      }

      $form['classification']['session_ident'] = array(
        '#type' => 'hidden',
        '#default_value' => $session_ident,
        '#attributes' => array('class' => array('classification_session_ident')),
      );

      foreach (archibald_get_active_curriculums() as $curriculum) {
        $curr_name = 'curriculum_' . $curriculum;
        $classname = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);
        module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class');
        drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_classification.js');
        drupal_add_js(drupal_get_path('module', 'archibald') . '/js/' . $curr_name . '.js');
        drupal_add_css(drupal_get_path('module', 'archibald') . '/css/' . $curr_name . '.css');

        $curr_handler = new $classname();
        $curr_handler->sessionIdent = $session_ident;

        if ($curr_handler instanceof ArchibaldCurriculumAdvanced) {
          $curr_handler->setLomCurriculum($lom_curriculum);
        }
        else {
          // $curr_handler instanceof ArchibaldCurriculumBasic
          $curr_handler->setLomClassification($lom_classification);
        }

        $form['classification']['curriculums_list'][$curriculum] = array(
          '#type' => 'curriculum',
          '#name' => $curr_handler->getName(),
          '#machine_name' => $curriculum,
          // this will be overloadet by js cause if the form has a error
          // it will be reloadet but dont come by here
          '#entrys' => $curr_handler->getEntrys(),
          '#valid_for' => $curr_handler->isValidFor(),
        );
      }
    }
  }
}

/**
 * theme a archibald form block general
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_classification($form) {
  $form = reset($form);

  $header = '<label for="edit-classification-title" class="htab_title">' . $form['#fieldset_title'] . '</label>';
  $header .= '<table id="archibald_classification" border="0">';
  $footer = '';
  $hidden = '';

  // echo '<PRE>';
  // print_r( element_children($form, TRUE) );
  // die();

  // echo '<PRE>';
  foreach (element_children($form, TRUE) as $k) {
    if ($k == 'curriculums_list') {
      foreach (element_children($form[$k], TRUE) as $element_k) {
        $element = &$form[$k][$element_k];
        // print_r( $element );
        switch( $element['#machine_name'] ) {
            default:
              $show = archibald_check_field_permission('classification curriculums');
            break;
            case 'educa':
              $show = archibald_check_field_permission('classification defaultCurriculum');
            break;
        }
        if( !$show ) continue;

        switch( $element['#machine_name'] ) {
          default :
            $title = t('Specific Curricula') . archibald_get_require_type_value('classification.curriculums') . archibald_get_field_help('classification.curriculums');
            break;
          case 'per' :
            $title = t('Specific Curricula') . archibald_get_require_type_value('classification.curriculums') . archibald_get_field_help('classification.curriculums');
            break;
          case 'educa' :
            $title = t('Default Curriculum') . archibald_get_require_type_value('classification.defaultCurriculum') . archibald_get_field_help('classification.defaultCurriculum');
            break;
        }
        $footer .= '<label class="curriculums">' . $title . '</label>';

        $footer .= '<div class="archibald_classification" valid_for="' . implode(';', $element['#valid_for']) . '" machine_name="' . $element['#machine_name'] . '">';
        $footer .= ' <ul class="tabs action-links clearfix">';
        $footer .= '  <li class="tab"><span>' . $element['#name'] . '</span></li>';
        $footer .= '  <li><a machine_name="' . $element['#machine_name'] . '" href="#" class="archibald_classification_add">' . t('Add') . '</a></li>';
        $footer .= ' </ul>';
        $footer .= ' <div class="archibald_classification_entrys">';

        if (empty($element['#entrys'])) {
          echo '&nbsp;';
        }
        else {
          $footer .= theme(
            'archibald_content_view_curriculum_' . $element['#machine_name'], array(
              'entrys' => $element['#entrys'],
              'action' => 'edit',
            )
          );
        }

        $footer .= ' </div>';
        $footer .= '</div>';
      }
    }
    elseif ($form[$k]['#type'] != 'hidden') {
      $header .= '<tr>';
      $header .= '<td class="archibald_classification_td">';
      $header .= drupal_render($form[$k]);
      $header .= '</td>';
      $header .= '</tr>';
    }
    else {
      $hidden .= drupal_render($form[$k]);
    }
  }
  // die();
  $header .= '</table>';

  return $header . $footer . $hidden;
}

/**
 * ajax menu callback
 *
 * @param string $op
 *   Operator
 *   possible command:
 *     get_add_form
 *     remove
 *     get
 * @param string $param
 *   optional parameter with extra attributes
 *
 * @return boolean
 */
function archibald_classification_ajax($op, $param = '') {
  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  if (!isset($_POST['machine_name'])) {
    return FALSE;
  }

  $curriculum = filter_xss($_POST['machine_name']);
  $session_ident = filter_xss($_POST['session_ident']);


  if (empty($curriculum)) {
    die('No curriculum given');
  }

  $lom_classification = unserialize($_SESSION['lom_classification'][$session_ident]);
  $lom_curriculum = unserialize($_SESSION['lom_curriculum'][$session_ident]);

  $curr_name = 'curriculum_' . $curriculum;
  $classname = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);
  module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class');

  $curr_handler = new $classname();
  $curr_handler->sessionIdent = $session_ident;
  if ($curr_handler instanceof ArchibaldCurriculumAdvanced) {
    $curr_handler->setLomCurriculum($lom_curriculum);
  }
  else {
    // $curr_handler instanceof ArchibaldCurriculumBasic
    $curr_handler->setLomClassification($lom_classification);
  }

  switch ($op) {
    case 'get_add_form':
      $form_state = array(
        'title' => t('Add new curriculum entry'),
        'ajax' => TRUE,
        'curr_handler' => $curr_handler,
      );
      $commands = ctools_modal_form_wrapper(
        'archibald_classification_ajax_form', $form_state
      );
      $commands[] = array(
        'command' => 'archibald_ajax_init_classifications',
      );

      // close modal frame
      if (!empty($form_state['was_saved']) || (!empty($form_state['values']['op']) && $form_state['values']['op'] != $form_state['complete form']['save']['#value'])) {
        $commands = array();

        if (!empty($form_state['values']['op']) && $form_state['values']['op'] != $form_state['complete form']['save']['#value']) {
          $commands[] = ctools_modal_command_dismiss();
        }
        else {
          if (!in_array($curriculum, archibald_get_active_curriculums())) {
            $commands[] = ctools_modal_command_dismiss();
          }
        }

        // add new content for the classification curriculum area
        if (!empty($form_state['was_saved'])) {
          $commands[] = array(
            'command' => 'archibald_ajax_update_classifications',
          );

          $html = theme(
            'archibald_content_view_curriculum_' . filter_xss($_POST['machine_name']),
            array(
              'entrys' => $curr_handler->getEntrys(),
              'action' => 'edit',
            )
          );

          $commands[] = ajax_command_html(
            '.archibald_classification[machine_name=' . filter_xss($_POST['machine_name']) . '] .archibald_classification_entrys',
            $html
          );

          // reload additional curriculums
          if (!empty($form_state['reload_other_curriculums'])) {
            $lom_classification = unserialize(
              $_SESSION['lom_classification'][$session_ident]
            );

            $lom_curriculum = unserialize(
              $_SESSION['lom_curriculum'][$session_ident]
            );

            foreach ($form_state['reload_other_curriculums'] as $curriculum_to_reload) {
              $curr_name_to_reload = 'curriculum_' . $curriculum_to_reload;
              $classname_to_reload = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum_to_reload);
              module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name_to_reload . '.class');

              $curr_handler_to_reload = new $classname_to_reload();
              if ($curr_handler_to_reload instanceof ArchibaldCurriculumAdvanced) {
                $curr_handler_to_reload->setLomCurriculum(
                  $lom_curriculum
                );
              }
              else {
                // $curr_handler instanceof ArchibaldCurriculumBasic
                $curr_handler_to_reload->setLomClassification(
                  $lom_classification
                );
              }

              $html = theme(
                'archibald_content_view_curriculum_' . $curriculum_to_reload,
                array(
                  'entrys' => $curr_handler_to_reload->getEntrys(),
                  'action' => 'edit',
                )
              );

              $commands[] = ajax_command_html(
                '.archibald_classification[machine_name=' . $curriculum_to_reload . '] .archibald_classification_entrys',
                $html
              );
            }

            unset($tmp_lom_classification);
          }
        }
      }
      drupal_add_http_header('Content-Type', 'application/json');
      print ajax_render($commands);
      break;

    case 'remove':
      $curr_handler->remove(filter_xss($_POST['key']));
      print theme(
        'archibald_content_view_curriculum_' .
        filter_xss($_POST['machine_name']), array(
          'entrys' => $curr_handler->getEntrys(),
          'action' => 'edit',
        )
      );
      break;

    case 'get':
      print theme(
        'archibald_content_view_curriculum_' .
        filter_xss($_POST['machine_name']), array(
          'entrys' => $curr_handler->getEntrys(),
          'action' => (empty($param)) ? 'edit' : $param,
        )
      );
      break;

    default:
      if (!method_exists($curr_handler, $op)) {
        die('illegal operator!');
      }

      if (isset($_POST['custom_html'])) {
        print $curr_handler->$op();
      }
      elseif (isset($_POST['custom_handler'])) {
        print drupal_json_encode($curr_handler->$op());
      }
      else {
        print ajax_render($curr_handler->$op());
      }
      break;
  }

  if ($curr_handler instanceof ArchibaldCurriculumAdvanced) {
    $_SESSION['lom_curriculum'][$session_ident] = serialize(
      $curr_handler->getLomCurriculum()
    );
  }
  else {
    // $curr_handler instanceof ArchibaldCurriculumBasic
    $_SESSION['lom_classification'][$session_ident] = serialize(
      $curr_handler->getLomClassification()
    );
  }

  exit;
}

/**
 * wrapper for curriculum class
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_classification_ajax_form($form, &$form_state) {
  $form = $form_state['curr_handler']->getAddForm($form_state);

  $form['#attributes'] = array('class' => array('archibald_classification_ajax_form'));

  $form['machine_name'] = array(
    '#type' => 'hidden',
    '#default_value' => $_POST['machine_name'],
  );

  $form['session_ident'] = array(
    '#type' => 'hidden',
    '#default_value' => $_POST['session_ident'],
  );

  if (empty($form['close'])) {
    $form['cancel'] = array(
      '#type' => 'button',
      '#value' => t('Cancel'),
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * wrapper for curriculum class
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_classification_ajax_form_validate($form, &$form_state) {
  if (method_exists($form_state['curr_handler'], 'validate_add_form')) {
    $form_state['curr_handler']->validateAddForm($form_state);
  }
}

/**
 * wrapper for curriculum class
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_classification_ajax_form_submit($form, &$form_state) {
  $form_state['curr_handler']->submitAddForm($form_state);
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
 * save handler for lom classification form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_classification_submit($form, &$form_state, ArchibaldLom &$lom) {

  $lom_education = $lom->getEducation();
  if (!($lom_education instanceof ArchibaldLomDataEducation)) {
    $lom_education = new ArchibaldLomDataEducation();
  }

  ##### handle context
  if (archibald_check_field_permission('classification context')) {
    $existing_contexts = array();
    foreach ($lom_education->getContext() as $context) {
      if ($context instanceof ArchibaldLomDataVocabulary) {
        $key = $context->getSource() . '|' . $context->getValue();
      }
      else {
        // this shoud realy never happen
        continue;
      }

      $existing_contexts[$key] = $context;
    }

    foreach ($form_state['values']['classification']['context'] as $i => $context) {
      $key = drupal_strtolower(trim($context));

      if (empty($key) || !preg_match('/(.+)\|(.+)/', $context, $parts)) {
        continue;
      }

      if (isset($existing_contexts[$key])) {
        unset($existing_contexts[$key]);
      }
      else {
        $vocabulary = new ArchibaldLomDataVocabulary();
        $vocabulary->setSource($parts[1]);
        $vocabulary->setValue($parts[2]);
        $lom_education->addContext($vocabulary);
      }
    }

    if (!empty($existing_contexts)) {
      foreach ($existing_contexts as $key => $context) {
        $lom_education->removeContext($context);
      }
    }

    $lom->setEducation($lom_education);
  }

  // handle the classifcation section
  if (archibald_check_field_permission('classification curriculums') || archibald_check_field_permission('classification defaultCurriculum')) {
    $session_ident = $form_state['values']['classification']['session_ident'];

    $lom_classification = array();
    if (!empty($_SESSION['lom_classification'][$session_ident])) {
      $lom_classification = unserialize(
        $_SESSION['lom_classification'][$session_ident]
      );

      unset($_SESSION['lom_classification'][$session_ident]);
    }
    // save sub object back
    $lom->setClassification($lom_classification);

    $lom_curriculum = array();
    if (!empty($_SESSION['lom_curriculum'][$session_ident])) {
      $lom_curriculum = unserialize($_SESSION['lom_curriculum'][$session_ident]);
      unset($_SESSION['lom_curriculum'][$session_ident]);
    }
    // save sub object back
    $lom->setCurriculum($lom_curriculum);

    // check with curriculums are used
    $used_curriculums = array();
    if (!empty($lom_classification)) {

      foreach ($lom_classification as $classification) {
        if ($classification instanceof ArchibaldLomDataClassification) {

          $purpose = $classification->getPurpose()->getValue();
          $taxon_pathes = $classification->getTaxonPath();

          foreach ($taxon_pathes as $taxon_path_id => $taxon_path) {

            if ($taxon_path instanceof ArchibaldLomDataTaxonPath) {
              $source = $taxon_path->getSource()->getStrings();
              // get first element
              $source = reset($source);
              $used_curriculums[$source] = TRUE;
            }
          }
        }
      }
    }

    // if (empty($used_curriculums['educa']) && (count($used_curriculums) > 0 || !empty($lom_curriculum))) {
    if (empty($used_curriculums['educa']) && (count($used_curriculums) > 0)) {
      drupal_set_message(
        t('You have assigned this description to a curriculum but not yet to the default curriculum. The result is that your description will not be found by the search function.'),
        'warning'
      );
    }
  }
}

/**
 * get option array
 * for a checkboxes / select form item
 * with all possible context taxonomy terms
 *
 * @return array
 */
function archibald_get_education_context_options() {
  $options = archibald_get_taxonomy_options('education_context');

  $ret  = array();
  $keys = array_keys($options);
  $c    = count($options);
  $ct   = ($c % 2 == 1) ? ($c + 1) / 2 : $c / 2;

  // echo '<PRE>';
  // print_r($options);
  // print_r($keys);
  // die();


  for ($i = 0; $i < $ct; $i++) {
    $k = $keys[$i];
    $ret[$k] = $options[$k];

    if (!empty($keys[$i + $ct])) {
      $k = $keys[$i + $ct];
      $ret[$k] = $options[$k];
    }
  }

  return $ret;
}

