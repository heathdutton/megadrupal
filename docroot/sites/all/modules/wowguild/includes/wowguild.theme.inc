<?php

/**
 * Return the validated image.  Used as a theme so it can be overridden.
 *
 * @param array $variables
 */
function theme_wowtoon_validated_img($variables) {
  $path = drupal_get_path('module', 'wowguild');
  switch ($variables['status']) {
    case WOWTOON_STATUS_DISABLED:
    case WOWTOON_STATUS_ENABLED:
      return sprintf('<img src="/%s" width="24" height="24" class="validated-img" title="%s" />', $path . '/images/unvalidated.png', t("Unvalidated Character"));
      break;
    case WOWTOON_STATUS_VALIDATED:
      return sprintf('<img src="/%s" width="24" height="24" class="validated-img" title="%s" />', $path . '/images/validated.png', t("Validated Character"));
      break;
  }
}

/**
 * Preprocess the stats section of wowtoons.
 *
 * @param array $variables
 *
 */
function template_preprocess_wowtoon_stats(&$variables) {
  // Define what stats will be displayed PER class.
  $avatar = $variables['avatar'];
  $set = $variables['set'];
  $stats_lookup = array(
      'sta' => t('Stamina'),
      'str' => t('Strength'),
      'agi' => t('Agility'),
      'int' => t('Int'),
      'spr' => t('Spirit'),
  );

  /*
   switch($toon->classId) {
  case 6: // Death Knight
  $stats_lookup['str'] = t('Strength');
  break;
  default:
  $stats_lookup['str'] = t('Strength');
  $stats_lookup['agi'] = t('Agility');
  $stats_lookup['int'] = t('Int');
  $stats_lookup['spr'] = t('Spirit');
  }
  */
  foreach ($stats_lookup as $stat => $value) {
    $variables['all_stats'][$stat]['text'] =  $value;
    $variables['all_stats'][$stat]['value'] =  check_plain($set->stats[$stat]);
  }
}

/**
 * Process variables for wowtoon-item-image.tpl.php
 * @see wowtoon-items.tpl.php
 * @see template_preprocess_wowtoon_right_item()
 */
function template_preprocess_wowtoon_item_image(&$variables) {
  template_preprocess_wowtoon_right_item($variables);
}
/**
 * Process variables for wowtoon-left-item.tpl.php
 * @see wowtoon-items.tpl.php
 * @see template_preprocess_wowtoon_right_item()
 */
function template_preprocess_wowtoon_left_item(&$variables) {
  template_preprocess_wowtoon_right_item($variables);
}
/**
 * Process variables for wowtoon-right-item.tpl.php
 * @see wowtoon-items.tpl.php
 */
function template_preprocess_wowtoon_right_item(&$variables) {
  $prefix = variable_get('wowtoon_country', 'us');

  if (isset($variables['item']['name']) && $variables['item']['name']) {
    $gemimg = '';

    $gemids = array();
    $enchant = '';
    $setpieces = array();
    $extrasocket = '';
    $rand = 0;

    if (!empty($variables['item']['tooltipParams'])) {
      foreach ($variables['item']['tooltipParams'] as $key => $value) {
        if (substr($key, 0, 3) == 'gem') {
          $gemids[] = $value;
        }
        if ($key == 'enchant') {
          $enchant = 'ench=' . $value;
        }
        if ($key == 'reforge') {
          // no wowhead tooltip for reforge?
        }
        if ($key == 'suffix') {
          // Random Enchant
          $rand = $value;
        }
        if ($key == 'set') {
          // This is an item in a set.
          $setpieces = $value;
        }
        if ($key == 'tinker') {
          // no wowhead tooltip for tinker?
        }
        if ($key == 'extraSocket') {
          $extrasocket = 1;
        }
      }
    }

    //$variables['gems'] = $gems;

    $variables['icon'] = sprintf('http://%s.media.blizzard.com/wow/icons/%d/%s.jpg', $prefix, 56, $variables['item']['icon']);
    $variables['url'] = WOWTOON_WOWHEAD_BASE_URL . '/' . WOWTOON_WOWHEAD_SEARCH_ID . $variables['item']['id'];

    $rel = array();
    if ($enchant) {
      $rel[] = $enchant;
    }
    if ($setpieces) {
      $rel[] = 'pcs=' . implode(':', $setpieces);
    }
    if ($gemids) {
      $rel[] = 'gems=' . implode(':', $gemids);
    }
    if ($rand) {
      $rel[] = 'rand=' . $rand;
    }
    $variables['rel'] = implode('&', $rel);
  }
}

/**
 * Process variables for wowtoon-items.tpl.php
 * @see wowtoon_build_content()
 */
function template_preprocess_wowtoon_items(&$variables) {
  //if (variable_get('wowtoon_cache_backgrounds', 0) && !empty($variables['set']->background)) {
    //$variables['toonbackground'] = check_plain($variables['set']->background);
  //}
  //else {
    $variables['toonbackground'] = $variables['avatar']->render['profile'];
  //}
}
