<?php

/**
 * @file
 * lom resource add/edit form part: for rights tab
 */
function archibald_content_form_rights(&$form, &$form_state, &$lom, $language) {

  $cost_access = archibald_check_field_permission('rights cost');
  $description_access = archibald_check_field_permission('rights description');

  if ($cost_access || $description_access) {
    $lom_rights = $lom->getRights();
    if (!($lom_rights instanceof ArchibaldLomDataRights)) {
      $lom_rights = new ArchibaldLomDataRights();
    }

    $form['rights'] = array(
      '#type' => 'fieldset',
      '#title' => '&nbsp;&nbsp;&nbsp;&nbsp;' . archibald_get_field_label('right'),
      '#fieldset_title' => archibald_get_field_label('right'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
      '#theme' => 'archibald_rights',
    );
  }


  if ($cost_access) {
    $default_value = archibald_lom_vocab_default_val($lom_rights->getCost());
    $form['rights']['cost'] = array(
      '#type' => 'select',
      '#title' => archibald_get_field_label('right.cost') .
        archibald_get_require_type_value('right.cost') .
        archibald_get_field_help('right.cost'),
      '#attributes' => array('archibald_help_ident' => 'rights.cost'),
      '#default_value' => $default_value,
      '#options' => archibald_get_right_costs_options($default_value)
    );
  }

  if ($description_access) {
    $default_value = '';
    $lang_string = $lom_rights->getDescription();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $default_value = $lang_string->getString('en');

      if (empty($default_value)) {
        $lang_strings = $lang_string->getStrings();
        $default_value = reset($lang_strings);
      }
    }
    $form['rights']['description'] = array(
      '#type' => 'select',
      '#title' => archibald_get_field_label('right.description') .
        archibald_get_require_type_value('right.description') .
        archibald_get_field_help('right.description'),
      '#attributes' => array('archibald_help_ident' => 'rights.license'),
      '#options' => archibald_get_licences_options($default_value),
      '#default_value' => $default_value,
    );
  }
}

function archibald_get_licences_options(&$default_value) {
  $options = archibald_get_taxonomy_options('rights_licenses');
  archibald_select_options_default_value_filter($options, $default_value);

  if (!isset($options[''])) {
    array_unshift($options, "");
  }

  // ksort($options);

  return $options;
}

function archibald_get_right_costs_options(&$default_value) {
  $options = archibald_get_taxonomy_options('rights_cost');
  return $options;
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
 * save handler for lom Rights form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_rights_submit($form, &$form_state, &$lom) {
  $lom_rights = $lom->getRights();
  if (!($lom_rights instanceof ArchibaldLomDataRights)) {
    $lom_rights = new ArchibaldLomDataRights();
  }

  ##### handle cost
  if (archibald_check_field_permission('rights cost')) {
    if (preg_match('/(.+)\|(.+)/', $form_state['values']['rights']['cost'], $parts)) {
      $vocabulary = new ArchibaldLomDataVocabulary();
      $vocabulary->setSource($parts[1]);
      $vocabulary->setValue($parts[2]);
      $lom_rights->setCost($vocabulary);
    }
    else {
      $lom_rights->setCost(NULL);
    }
  }

  ##### handle description / Licence
  if (archibald_check_field_permission('rights description')) {
    if (!empty($form_state['values']['rights']['description'])) {
      // if key was not able to pass through a form, get it from session store
      if (!empty($_SESSION['archibald_default_value_keys'][$form_state['values']['rights']['description']])) {
        $form_state['values']['rights']['description'] = $_SESSION['archibald_default_value_keys'][$form_state['values']['rights']['description']];
      }

      $lang_string = new ArchibaldLomDataLangString();
      foreach (language_list() as $code => $language) {
        if ($language->enabled == TRUE) {
          $rights_description = $form_state['values']['rights']['description'];
          if (!empty($rights_description)) {
            $lang_string->setString($rights_description, $code);
          }
        }
      }
      $lom_rights->setDescription($lang_string);
    }
    else {
      $lom_rights->setDescription(NULL);
    }
  }

  $lom->setRights($lom_rights);
}

/**
 * theme a archibald form block rights
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_rights($form) {
  $form = reset($form);
  $output = '<label for="edit-rights-title" class="htab_title">' . $form['#fieldset_title'] . '</label>';
  $output .= '<table id="archibald_rights" border="0">';

  $html = array(
    'top' => array(),
    'middle' => array(),
    'bottom' => array(),
  );
  foreach (element_children($form, TRUE) as $key) {
    switch ($key) {
      case 'cost':
        $html['top'][] = drupal_render($form[$key]);
        break;

      case 'description':
        $html['bottom'][] = drupal_render($form[$key]);
        break;

      default:
        $html['top'][] = drupal_render($form[$key]);
        break;
    }
  }

  $top = implode("\n", $html['top']);
  if (!empty($top)) {
    $output .= '<tr>';
    $output .= '<td class="archibald_rights_td archibald_rights_td_top">';
    $output .= $top;
    $output .= '</td>';
    $output .= '</tr>';
  }
  $middle = implode("\n", $html['middle']);
  if (!empty($middle)) {
    $output .= '<tr>';
    $output .= '<td class="archibald_rights_td archibald_rights_td_middle">';
    $output .= $middle;
    $output .= '</td>';
    $output .= '</tr>';
  }

  $bottom = implode("\n", $html['bottom']);
  if (!empty($bottom)) {
    $output .= '<tr>';
    $output .= '<td class="archibald_rights_td archibald_rights_td_bottom">';
    $output .= $bottom;
    $output .= '</td>';
    $output .= '</tr>';
  }
  $output .= '</table>';

  return $output;
}

