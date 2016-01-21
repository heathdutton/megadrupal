<?php

/**
* Render the Block form.  Convert the Roles into a table.
*/
function theme_wowguild_recruitment_block_form($variables) {
  $form = $variables['form'];
  $header = array('Role', array('data' => 'Need', 'colspan' => 7), array('data' => 'Specs', 'colspan' => 8));
  $rows = array();
  // Loop over form elements.
  foreach ($form['recruitment'] as $element) {
    // Only take form elements that are arrays and have a title (some mysteriously don't).
    if (isset($element['#title']) && is_array($element)) {
      $saferole = wowguild_get_safe_role($element['#title']);
      $saferole_need = $saferole . '_need';
      $saferole_spec = $saferole . '_spec';
      $row = array();
      // Process specs checkboxes.
      if (array_key_exists($saferole_spec, $element)) {
        $checkbox_count = 0;
        foreach ($element[$saferole_spec] as $k => $spec) {
          if (isset($spec['#type']) && $spec['#type'] == 'checkbox') {
            array_push($row, drupal_render($spec)); // Add checkboxes in order.
            unset($element[$saferole_spec][$k]);
            $checkbox_count++;
          }
        }
        // Pad the rest of the row with empty cells (max number of specs is 6).
        for ($i=0; $i<7-$checkbox_count; $i++) {
          array_push($row, array());
        }
        array_unshift($row, drupal_render($element[$saferole_spec]));
      }
      // Process needs radio buttons.
      if (array_key_exists($saferole_need, $element)) {
        foreach ($element[$saferole_need] as $k => $need) {
          if (isset($need['#type']) && $need['#type'] == 'radio') {
            array_unshift($row, drupal_render($need)); // Add radio buttons in reverse order.
            unset($element[$saferole_need][$k]);
          }
        }
        array_unshift($row, drupal_render($element[$saferole_need]));
      }
      // Prepend anything that hasn't been rendered yet.
      array_unshift($row, drupal_render($element));
      $rows[] = $row;
    }
  }
  // Replace recruitment form array with markup.
  $form['recruitment'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));
  // Unset #theme element to avoid recursion.
  unset($form['#theme']);
  return drupal_render($form);
}




/**
* Render the "manual" part of the Progression Block form. Convert encounters into a table, one table per raid.
*/
function theme_wowguild_progression_block_manual_form($variables) {
  $form = $variables['form'];
  // Loop over form elements (here: raids).
  foreach ($form as $kr => $raid) {
    // Only consider elements that have a title.
    if (is_array($raid) && isset($raid['#title'])) {
      $raid_table=array();
      $header = array('Encounter', 'Show', array('data' => 'Modes', 'colspan' => 2));
      // Loop over sub-elements (here: encounters).
      foreach ($raid as $ke => $encounter) {
        if (is_array($encounter) && isset($encounter['#title'])) {
          $row=array();
          // Encounters are defined as items in function wowguild_block_configure().
          if (isset($encounter['#type']) && $encounter['#type'] == 'item') {
            array_push($row,$encounter['#title']);
            array_push($row, drupal_render($encounter['enable'])); // Add enabled checkbox, in order.
            // Add an empty cell if there is no flex mode.
            if (count(($encounter['modes']['#options']) < 2) && !array_key_exists('F',$encounter['modes']['#options'])) {
              array_push($row,array());
            }
            // Add an empty cell if there is no normal mode.
            if (count(($encounter['modes']['#options']) < 2) && !array_key_exists('N',$encounter['modes']['#options'])) {
              array_push($row,array());
            }
            foreach ($encounter['modes'] as $modes) {
              if (is_array($modes) && isset($modes['#type']) && ($modes['#type'] == 'checkbox')) {
                array_push($row, drupal_render($modes)); // Add modes checkboxes, in order.
              }
            }
            // Add an empty cell if there is no heroic mode.
            if (count(($encounter['modes']['#options']) < 2) && !array_key_exists('H',$encounter['modes']['#options'])) {
              array_push($row,array());
            }
            // Delete encounter now that it is processed.
            unset($form[$kr][$ke]);
          }
          $raid_table[]=$row;
        }
      }
      // Render raid table.
      $form[$kr][]=array('#markup' => theme('table', array('header' => $header, 'rows' => $raid_table)));
    } else {
      // Delete non-raid form elements.
      unset($form[$kr]);
    }
  }
  return drupal_render($form);
}

/**
 * Sets the recruitment block table.
 *
 * @see wowguild-recruitment-block.tpl.php
 *
 */
function template_preprocess_wowguild_recruitment_block(&$variables) {
  global $user;
  $variables['header'] = filter_xss(variable_get('wowguild_recruitment_header', ''));
  $roles = wowguild_get_recruitment_roles();
  $options = array(5 => t('Open'), 4 => t('High'), 3 => t('Medium'), 2 => t('Low'), 1 => t('Closed'), 0 => t('Hidden'));
  $variables['recruitment'] = array();

  $header = array();
  $rows = array();

  foreach ($roles as $key => $role) {
    $saferole = $role['name'];
    $value = variable_get('wowguild_recruitment_' . $saferole, 1);
    if ($value != 0) {
      $variables['recruitment'][$saferole] = array(
        '#title' => $role['role'],
        '#value' => $value,
        '#text' => $options[$value]
      );
      $cell = array('data' => t($role['role']), 'class' => 'role-name ' . $saferole);
      // If is a class
      if ($key < 1000) {
        $cell['class'] .= ' shadow color-c' . $key;
        $variables['recruitment'][$saferole]['#class'] = 'role-name shadow color-c' . $key;
        $variables['recruitment'][$saferole]['#classId'] = $key;
      }
      // Build icons list
      $specs_list = array();
      $default_spec_needs = array();
      foreach (array_keys($role['specs']) as $s) {
        $default_spec_needs[$s] = 0;
      }
      $saved_spec_needs = variable_get('wowguild_recruitment_' . $saferole . '_spec', $default_spec_needs);
      if (!empty($saved_spec_needs)) {
        foreach ($saved_spec_needs as $k => $v) {
          if ($v !== 0) {
            $specs_list[] = '<img src="http://us.media.blizzard.com' . $role['specs_icons'][$k] . '" title="' . t($role['specs'][$k]) . ': ' . t($options[$value]) . '" class="spec-open"/>';
          }
          else {
            $specs_list[] = '<img src="http://us.media.blizzard.com' . $role['specs_icons'][$k] . '" title="' . t($role['specs'][$k]) . ': ' . t($options[1]) . '" class="spec-closed"/>';
          }
        }
      }
      $rows[] = array($cell, array('data' => t($options[$value]) . '<br /><nobr>' . implode(' ', $specs_list) . '</nobr>', 'class' => 'role-need ' . strtolower($options[$value])));
    }
  }

  $variables['rendered_roles'] = theme('table', array('header' => $header, 'rows' => $rows));

  $variables['footer'] = filter_xss(variable_get('wowguild_recruitment_footer', ''));
  if ($user->uid) {
    $variables['application_link'] = wowguild_can_apply_to_guild()?l(t('Apply to Guild'), 'node/add/guild-application'):'';
  }
  else {
    $variables['application_login_link'] = l(t('Log in'), 'user/login', array('query' => array('destination' => 'node/add/guild-application')));
    $variables['application_register_link'] = l(t('Register'), 'user/register', array('query' => array('destination' => 'node/add/guild-application')));
    $variables['application_link'] = wowguild_can_apply_to_guild()?
    t('!login or !register or !apply', array(
        '!login' => $variables['application_login_link'],
        '!register' => $variables['application_register_link'],
        '!apply' => (variable_get('wowguild_hide_apps_for_role_rid' . DRUPAL_ANONYMOUS_RID, 0)?'apply':l('apply', 'node/add/guild-application'))
    ))
    :'';
  }
  $variables['application_url'] = url('node/add/guild-application');
}

/**
 * Process variables for progression block.
 *
 * @see wowguild_block_view()
 */
function template_preprocess_wowguild_progression_block_manual(&$variables) {
  $raids = wowguild_get_progression_raids();
  $progression = array();
  $progression_variables = variable_get('wowguild_progression_manual_block',array());

  $guild = wowguild_get_guild();

  foreach ($raids as $raid) {
    $raid_key = wowguild_get_safe_role($raid['name']);
    if (!array_key_exists($raid_key,$progression_variables) && !isset($progression_variables[$raid_key])) {
      $progression_variables[$raid_key] = array();
      $progression_variables[$raid_key]['display_options'] = array('e', 0);
    }
    $progression[$raid_key] = new stdClass();
    $progression[$raid_key]->name = $raid['name'];
    // Use guild's zone?  us/eu.battle.net?
    $progression[$raid_key]->icon = wowguild_get_icon_path($guild->zone, $raid['icon'], 18);
    $raid_display_options = $progression_variables[$raid_key]['display_options'];
    if (array_key_exists('e', $raid_display_options) && (strcmp($raid_display_options['e'], 'e') == 0)) {
      $progression[$raid_key]->enable = TRUE;
    }
    else {
      $progression[$raid_key]->enable = FALSE;
    }
    if (array_key_exists('s', $raid_display_options) && (strcmp($raid_display_options['s'], 's') == 0)) {
      $progression[$raid_key]->show_numeric = TRUE;
    }
    else {
      $progression[$raid_key]->show_numeric = FALSE;
    }
    $progression[$raid_key]->numeric_summary = '';
    $progression[$raid_key]->encounters = array();
    $progression[$raid_key]->total_encounters_normal = 0;
    $progression[$raid_key]->total_encounters_heroic = 0;
    $progression[$raid_key]->total_encounters_mythic = 0;
    $progression[$raid_key]->defeated_encounters_normal = 0;
    $progression[$raid_key]->defeated_encounters_heroic = 0;
    $progression[$raid_key]->defeated_encounters_mythic = 0;
    foreach ($raid['encounters'] as $encounter) {
      $encounter_key = wowguild_get_safe_role($encounter['name']);
      if (!array_key_exists($encounter_key,$progression_variables[$raid_key]) && !isset($progression_variables[$raid_key][$encounter_key])) {
        $progression_variables[$raid_key][$encounter_key] = array();
        $progression_variables[$raid_key][$encounter_key]['enable'] = 1;
        $progression_variables[$raid_key][$encounter_key]['modes'] = array(0,0);
      }
      $progression[$raid_key]->encounters[$encounter_key] = new stdClass();
      $progression[$raid_key]->encounters[$encounter_key]->name = $encounter['name'];
      $progression[$raid_key]->encounters[$encounter_key]->icon = wowguild_get_icon_path($guild->zone, $encounter['icon'], 18);
      $progression[$raid_key]->encounters[$encounter_key]->enable = $progression_variables[$raid_key][$encounter_key]['enable'] ? TRUE : FALSE;
      $modes = $progression_variables[$raid_key][$encounter_key]['modes'];
      if (array_key_exists('F', $modes)) {
        if (strcmp($modes['F'], 'F') == 0) {
          $progression[$raid_key]->encounters[$encounter_key]->flex = TRUE;
          $progression[$raid_key]->total_encounters_flex++;
        }
        else {
          $progression[$raid_key]->encounters[$encounter_key]->flex = FALSE;
        }
        $progression[$raid_key]->total_encounters_flex++;
      }
      if (array_key_exists('N', $modes)) {
        if (strcmp($modes['N'], 'N') == 0) {
          $progression[$raid_key]->encounters[$encounter_key]->normal = TRUE;
          $progression[$raid_key]->defeated_encounters_normal++;
        }
        else {
          $progression[$raid_key]->encounters[$encounter_key]->normal = FALSE;
        }
        $progression[$raid_key]->total_encounters_normal++;
      }
      if (array_key_exists('H', $modes)) {
        if (strcmp($modes['H'], 'H') == 0) {
          $progression[$raid_key]->encounters[$encounter_key]->heroic = TRUE;
          $progression[$raid_key]->defeated_encounters_heroic++;
        }
        else {
          $progression[$raid_key]->encounters[$encounter_key]->heroic = FALSE;
        }
        $progression[$raid_key]->total_encounters_heroic++;
      }
      if (array_key_exists('M', $modes)) {
        if (strcmp($modes['M'], 'M') == 0) {
          $progression[$raid_key]->encounters[$encounter_key]->mythic = TRUE;
          $progression[$raid_key]->defeated_encounters_mythic++;
        }
        else {
          $progression[$raid_key]->encounters[$encounter_key]->mythic = FALSE;
        }
        $progression[$raid_key]->total_encounters_mythic++;
      }
    }
    if ($progression[$raid_key]->defeated_encounters_mythic > 0) {
      $progression[$raid_key]->numeric_summary = $progression[$raid_key]->defeated_encounters_mythic . '/' . $progression[$raid_key]->total_encounters_mythic . 'M';
    }
    elseif ($progression[$raid_key]->defeated_encounters_heroic > 0) {
      $progression[$raid_key]->numeric_summary = $progression[$raid_key]->defeated_encounters_heroic . '/' . $progression[$raid_key]->total_encounters_heroic . 'H';
    }
    else {
      $progression[$raid_key]->numeric_summary = $progression[$raid_key]->defeated_encounters_normal . '/' . $progression[$raid_key]->total_encounters_normal . 'N';
    }
  }
  $variables['progression'] = $progression;
}

/**
 * Render the progression block block body.
 *
 * @param unknown_type $raids
 * @param unknown_type $progression
 */
function theme_wowguild_progression_block_manual($variables) {
  $raids = $variables['raids'];
  $progression = $variables['progression'];
  $output = '';
  foreach ($progression as $raid) {
    $rows = array();
    $row = array();
    $head = array();
    if ($raid->enable) {
      if ($raid->show_numeric) {
        $head = array(array('data' => theme('image', array('path' => $raid->icon)), 'class' => array('wowguild-progression-manual-block-icon', 'wowguild-progression-manual-block-raid')), array('data' => t($raid->name), 'colspan' => 1, 'class' => 'wowguild-progression-manual-block-raid'), array('data' => $raid->numeric_summary, 'colspan' => 4, 'class' => array('wowguild-progression-manual-block-icon-numericsummary', 'wowguild-progression-manual-block-raid')));
      }
      else {
        $head = array(array('data' => theme('image', array('path' => $raid->icon)), 'class' => array('wowguild-progression-manual-block-icon', 'wowguild-progression-manual-block-raid')), array('data' => t($raid->name), 'colspan' => 5, 'class' => 'wowguild-progression-manual-block-raid'));
      }
      $rows[] = $row;
      foreach ($raid->encounters as $encounter) {
        if ($encounter->enable) {
          $row = array(array('data' => theme('image', array('path' => $encounter->icon)), 'class'=>'wowguild-progression-manual-block-icon'), array('data' => t($encounter->name)));
          if (isset($encounter->flex)) {
            if ($encounter->flex) {
              $row[] = array('data' => 'F', 'class' => array('wowguild-progression-manual-block-completed', 'wowguild-progression-manual-block-normal'));
            }
            else {
              $row[] = array('data'=>'F', 'class' => array('wowguild-progression-manual-block-notcompleted', 'wowguild-progression-manual-block-normal'));
            }
          }
          else {
            $row[] = array('data'=>'<div>N</div>', 'class' => 'wowguild-progression-manual-block-hidden');
          }
          if (isset($encounter->normal)) {
            if ($encounter->normal) {
              $row[] = array('data' => 'N', 'class' => array('wowguild-progression-manual-block-completed', 'wowguild-progression-manual-block-normal'));
            }
            else {
              $row[] = array('data'=>'N', 'class' => array('wowguild-progression-manual-block-notcompleted', 'wowguild-progression-manual-block-normal'));
            }
          }
          else {
            $row[] = array('data'=>'<div>N</div>', 'class' => 'wowguild-progression-manual-block-hidden');
          }
          if (isset($encounter->heroic)) {
            if ($encounter->heroic) {
              $row[] = array('data' => 'H','class'=>array('wowguild-progression-manual-block-completed','wowguild-progression-manual-block-heroic'));
            }
            else {
              $row[] = array('data' => 'H','class'=>array('wowguild-progression-manual-block-notcompleted','wowguild-progression-manual-block-heroic'));
            }
          }
          else {
            $row[] = array('data'=>'<div>H</div>', 'class' => 'wowguild-progression-manual-block-hidden');
          }
          if (isset($encounter->mythic)) {
            if ($encounter->mythic) {
              $row[] = array('data' => 'M','class'=>array('wowguild-progression-manual-block-completed','wowguild-progression-manual-block-heroic'));
            }
            else {
              $row[] = array('data' => 'M','class'=>array('wowguild-progression-manual-block-notcompleted','wowguild-progression-manual-block-heroic'));
            }
          }
          else {
            $row[] = array('data'=>'<div>M</div>', 'class' => 'wowguild-progression-manual-block-hidden');
          }
          $rows[] = $row;
        }
      }
      $output .= theme('table', array('header'=>$head,'rows' => $rows));
    }
  }

  if (!empty($output)) {
    $output = '<div id="wowguild-progression-manual-block">' . $output . '</div>';
  }
  return $output;
}




function theme_wowguild_progression_block_automatic($variables) {
  $guild = $variables['guild'];

  $output = '';
  if (!empty($guild->raid_progression)) {
    $progression = $guild->raid_progression;
    foreach ($progression as $raid) {
      //$output .= sprintf('<a href="%s?achievement=%d" rel="criteria=18480">%s</a>', WOWTOON_WOWHEAD_BASE_URL, $raid['guild_achid'], $raid['name']);
      // TODO: Lookup ZONE from server.
      $output .= sprintf('<img src="http://%s.media.blizzard.com/wow/icons/18/%s.jpg" /> ', $guild->zone, $raid['icon']);
      $output .= $raid['name'];
      $output .= '<ul>';

      $class = (!empty($raid['normal_completed']))?"completed":"incomplete";

      $output .= '<li class="normal ' . $class . '">';
      $text = t('Normal @complete', array('@complete' => (!empty($raid['normal_completed']))?"Cleared":""));
      $output .= l($text, WOWTOON_WOWHEAD_BASE_URL . '?achievement=' . $raid['guild_achid']);
      $output .= '</li>';

      $class = (!empty($raid['mythic_completed']))?"completed":"incomplete";
      $output .= '<li class="mythic ' . $class . '">';
      $text = t('Mythic @complete', array('@complete' => (!empty($raid['mythic_completed']))?"Cleared":""));
      $output .= l($text, WOWTOON_WOWHEAD_BASE_URL . '?achievement=' . $raid['guild_mythic_achid']);
      $output .= "</li>";

      $output .= '</ul>';
    }
  }
  if (!empty($output)) {
    $output = '<div id="wowguild-progression-automatic-block">' . $output . '</div>';
  }
  return $output;
}
