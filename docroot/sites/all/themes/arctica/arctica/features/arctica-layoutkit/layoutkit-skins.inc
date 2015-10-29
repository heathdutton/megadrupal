<?php

/**
 * Arctica LayoutKit skins
 *
 * @code
 * Due to the order of steps that build the skinr form it's not possible
 * to get the current theme for every theme fieldset in the form so we
 * assume user is theming the currently enabled theme.
 */

$current_theme = skinr_current_theme(TRUE);
if (theme_get_setting('responsive_enable', $current_theme)) {
  $media_total = theme_get_setting('media_queries', $current_theme);
  $media = array();
  if ($media_total && is_numeric($media_total)) {
    for ($i = 1; $i <= $media_total; $i++) {
      $media[] = 'medium' . $i;
    }
  }
}
else {
  $media = array('Default');
  $media_total = 1;
}

$skins['equal_heights'] = array(
  'title' => t('Equal heights'),
  'theme hooks' => array('block'),
  'type' => 'checkboxes',
  'group' => 'layout',
  'default status' => TRUE,
  'skin callback' => '_arctica_layoutkit_settings',
  'attached' => array(
     'js' => array('jQuery.equalHeights.js'),
  ),
  'options' => array(
    'equal-heights' => array(
      'title' => 'Check this to equalize the heights of blocks in the same region',
      'class' => array('equal-heights'),
    ),
  ),
);

for ($media_count = 1; $media_count <= $media_total; $media_count++) {
  $medium = $media[$media_count-1];
  $media_query = theme_get_setting("layout_query{$media_count}", $current_theme);

  $skins["grid_width{$media_count}"] = array(
    'title' => t('@medium width', array('@medium' => $medium)),
    'description' => t("Change the width of this block.") . '<div class="media-query">' . $media_query . '</div>',
    'type' => 'select',
    'options' => _arctica_skinr_grid_width_options($media_count),
    'group' => 'layout',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
    // @todo maak functie die active grid system kent en juiste opties geeft
    // 'options' => arctica_skins_grid_options(),
  );

  $skins["grid_clear-{$media_count}"] = array(
    'title' => t('@medium width', array('@medium' => $medium)),
    'title' => t('Float Clearing'),
    'description' => t("Can be used to start new grid row."),
    'type' => 'select',
  'options' => array(
    'clear-both' => array(
      'title' => 'Clear both',
      'class' => array("media-{$media_count}-clear-both"),
    ),
    'clear-left' => array(
      'title' => 'Clear left',
      'class' => array("media-{$media_count}-clear-left"),
    ),
    'clear-right' => array(
      'title' => 'Clear right',
      'class' => array("media-{$media_count}-clear-right"),
    ),
    'clear-none' => array(
      'title' => 'Clear none',
      'class' => array("media-{$media_count}-clear-none"),
    ),
  ),
    'group' => 'layout',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
  );

  $skins["grid_prefill-{$media_count}"] = array(
    'title' => t('@medium postfill', array('@medium' => $medium)),
    'title' => t('prefill'),
    'type' => 'select',
    'options' => _arctica_skinr_grid_prefill_options($media_count),
    'group' => 'layout',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
  );

  $skins["grid_postfill-{$media_count}"] = array(
    'title' => t('@medium width', array('@medium' => $medium)),
    'title' => t('postfill'),
    'type' => 'select',
    'options' => _arctica_skinr_grid_postfill_options($media_count),
    'group' => 'layout',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'default status' => TRUE,
  );
}



/**
 * Assists in hook_skinr_skin_PLUGIN_info() implementation.
 * @return array
 */
function _arctica_skinr_grid_width_options($media_idx) {
  $options = array(
    ".grid-1-8-media-{$media_idx}" => array(
      'title' => t('1/8'),
      'class' => array(".grid-1-8-media-{$media_idx}"),
    ),
    ".grid-1-7-media-{$media_idx}" => array(
      'title' => t('1/7'),
      'class' => array(".grid-1-7-media-{$media_idx}"),
    ),
    ".grid-1-6-media-{$media_idx}" => array(
      'title' => t('1/6'),
      'class' => array(".grid-1-6-media-{$media_idx}"),
    ),
    ".grid-1-5-media-{$media_idx}" => array(
      'title' => t('1/5'),
      'class' => array(".grid-1-5-media-{$media_idx}"),
    ),
    ".grid-1-4-media-{$media_idx}" => array(
      'title' => t('1/4'),
      'class' => array(".grid-1-4-media-{$media_idx}"),
    ),
    ".grid-1-3-media-{$media_idx}" => array(
      'title' => t('1/3'),
      'class' => array(".grid-1-3-media-{$media_idx}"),
    ),
    ".grid-1-2-media-{$media_idx}" => array(
      'title' => t('1/2'),
      'class' => array(".grid-1-2-media-{$media_idx}"),
    ),
    ".grid-3-4-media-{$media_idx}" => array(
      'title' => t('3/4'),
      'class' => array(".grid-3-4-media-{$media_idx}"),
    ),
    ".grid-2-3-media-{$media_idx}" => array(
      'title' => t('2/3'),
      'class' => array(".grid-2-3-media-{$media_idx}"),
    ),
    ".grid-100-media-{$media_idx}" => array(
      'title' => t('100%'),
      'class' => array(".grid-100-media-{$media_idx}"),
    ),
    ".grid-phi-media-{$media_idx}" => array(
      'title' => t('1/phi (61.8%)'),
      'class' => array(".grid-phi-media-{$media_idx}"),
    ),
    ".grid-phi2-media-{$media_idx}" => array(
      'title' => t('1/phi^2 (38.2%)'),
      'class' => array(".grid-phi2-media-{$media_idx}"),
    ),
    ".grid-phi3-media-{$media_idx}" => array(
      'title' => t('1/phi^3 (23.6%)'),
      'class' => array(".grid-phi3-media-{$media_idx}"),
    ),
    ".grid-auto-media-{$media_idx}" => array(
      'title' => t('auto'),
      'class' => array(".grid-auto-media-{$media_idx}"),
    ),
  );

  return $options;
}

/**
 * Assists in hook_skinr_skin_PLUGIN_info() implementation.
 * @return array
 */
function _arctica_skinr_grid_prefill_options($media_idx) {
  $options = array(
    ".grid-prefill-1-8-media-{$media_idx}" => array(
      'title' => t('1/8'),
      'class' => array(".grid-prefill-1-8-media-{$media_idx}"),
    ),
    ".grid-prefill-1-7-media-{$media_idx}" => array(
      'title' => t('1/7'),
      'class' => array(".grid-prefill-1-7-media-{$media_idx}"),
    ),
    ".grid-prefill-1-6-media-{$media_idx}" => array(
      'title' => t('1/6'),
      'class' => array(".grid-prefill-1-6-media-{$media_idx}"),
    ),
    ".grid-prefill-1-5-media-{$media_idx}" => array(
      'title' => t('1/5'),
      'class' => array(".grid-prefill-1-5-media-{$media_idx}"),
    ),
    ".grid-prefill-1-4-media-{$media_idx}" => array(
      'title' => t('1/4'),
      'class' => array(".grid-prefill-1-4-media-{$media_idx}"),
    ),
    ".grid-prefill-1-3-media-{$media_idx}" => array(
      'title' => t('1/3'),
      'class' => array(".grid-prefill-1-3-media-{$media_idx}"),
    ),
    ".grid-prefill-1-2-media-{$media_idx}" => array(
      'title' => t('1/2'),
      'class' => array(".grid-prefill-1-2-media-{$media_idx}"),
    ),
    ".grid-prefill-3-4-media-{$media_idx}" => array(
      'title' => t('3/4'),
      'class' => array(".grid-prefill-3-4-media-{$media_idx}"),
    ),
    ".grid-prefill-2-3-media-{$media_idx}" => array(
      'title' => t('2/3'),
      'class' => array(".grid-prefill-2-3-media-{$media_idx}"),
    ),
    ".grid-prefill-100-media-{$media_idx}" => array(
      'title' => t('100%'),
      'class' => array(".grid-prefill-100-media-{$media_idx}"),
    ),
    ".grid-prefill-phi-media-{$media_idx}" => array(
      'title' => t('1/phi (61.8%)'),
      'class' => array(".grid-prefill-phi-media-{$media_idx}"),
    ),
    ".grid-prefill-phi2-media-{$media_idx}" => array(
      'title' => t('1/phi^2 (38.2%)'),
      'class' => array(".grid-prefill-phi2-media-{$media_idx}"),
    ),
    ".grid-prefill-phi3-media-{$media_idx}" => array(
      'title' => t('1/phi^3 (23.6%)'),
      'class' => array(".grid-prefill-phi3-media-{$media_idx}"),
    ),
  );

  return $options;
}

/**
 * Assists in hook_skinr_skin_PLUGIN_info() implementation.
 * @return array
 */
function _arctica_skinr_grid_postfill_options($media_idx) {
  $options = array(
    ".grid-postfill-1-8-media-{$media_idx}" => array(
      'title' => t('1/8'),
      'class' => array(".grid-postfill-1-8-media-{$media_idx}"),
    ),
    ".grid-postfill-1-7-media-{$media_idx}" => array(
      'title' => t('1/7'),
      'class' => array(".grid-postfill-1-7-media-{$media_idx}"),
    ),
    ".grid-postfill-1-6-media-{$media_idx}" => array(
      'title' => t('1/6'),
      'class' => array(".grid-postfill-1-6-media-{$media_idx}"),
    ),
    ".grid-postfill-1-5-media-{$media_idx}" => array(
      'title' => t('1/5'),
      'class' => array(".grid-postfill-1-5-media-{$media_idx}"),
    ),
    ".grid-postfill-1-4-media-{$media_idx}" => array(
      'title' => t('1/4'),
      'class' => array(".grid-postfill-1-4-media-{$media_idx}"),
    ),
    ".grid-postfill-1-3-media-{$media_idx}" => array(
      'title' => t('1/3'),
      'class' => array(".grid-postfill-1-3-media-{$media_idx}"),
    ),
    ".grid-postfill-1-2-media-{$media_idx}" => array(
      'title' => t('1/2'),
      'class' => array(".grid-postfill-1-2-media-{$media_idx}"),
    ),
    ".grid-postfill-3-4-media-{$media_idx}" => array(
      'title' => t('3/4'),
      'class' => array(".grid-postfill-3-4-media-{$media_idx}"),
    ),
    ".grid-postfill-2-3-media-{$media_idx}" => array(
      'title' => t('2/3'),
      'class' => array(".grid-postfill-2-3-media-{$media_idx}"),
    ),
    ".grid-postfill-100-media-{$media_idx}" => array(
      'title' => t('100%'),
      'class' => array(".grid-postfill-100-media-{$media_idx}"),
    ),
    ".grid-postfill-phi-media-{$media_idx}" => array(
      'title' => t('1/phi (61.8%)'),
      'class' => array(".grid-postfill-phi-media-{$media_idx}"),
    ),
    ".grid-postfill-phi2-media-{$media_idx}" => array(
      'title' => t('1/phi^2 (38.2%)'),
      'class' => array(".grid-postfill-phi2-media-{$media_idx}"),
    ),
    ".grid-postfill-phi3-media-{$media_idx}" => array(
      'title' => t('1/phi^3 (23.6%)'),
      'class' => array(".grid-postfill-phi3-media-{$media_idx}"),
    ),
  );

  return $options;
}