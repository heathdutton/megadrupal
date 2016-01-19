<?php

/**
 * @file
 * Hooks provided by the GPT module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow manipulation of page-level options for the GPT.
 *
 * @param array $options
 *   Page option associative array containing:
 *   - network_code: The network for serving ads from, string.
 *   - targeted_ad_unit: The ad unit hiearchy, string.
 *   - async: Use asynchrounous mode, boolean.
 *   - sra: Use single request architecture, boolean.
 *   - collapse: Collapse divs without creatives, boolean.
 *   - targeting: Page level targeting, stdClass() object containing properties
 *     named by targeting key:
 *     - (dynamic): The key name for key value pairs, contains an indexed
 *       array.
 *       - (indexed)
 *         - value: String for the value.
 *         - eval: Boolean, whether to treat the value key as Javascript, if
 *           true.
 */
function hook_gpt_load_page_options_alter(&$options) {
  // Attempt to fetch current page node, if not node page then NULL.
  $node = menu_get_object();
  // If on a node page.
  if ($node) {
    // Append targeting key value pair of: type to $node->type.
    $options['targeting']->type[] = array(
      'value' => $node->type,
      'eval' => FALSE,
    );
  }
}

/**
 * Modify settings for a specific ad slot.
 *
 * @param array $ad
 *   Associate array containing:
 *   - container: The containing element ID of the ad unit, string.
 *   - size: (conditionally optional) stdClass() object listing breakpoints as
 *     object properties, contains array for sizes, see below for details.
 *     Optional only when "outofpage" is true.
 *   - targeting, see hook_gpt_ad_page_options_alter()'s $options['targeting']
 *     definition.
 *   - refresh: Whether the ad should be able to refresh, boolean.
 *   - outofpage: Whether the ad is an interstitial or not, boolean.
 * @param array $options
 *   See hook_gpt_ad_page_options_alter()'s $options param. Note that modifying
 *   the $options param will only affect the specific ad slot being altered. Any
 *   alterations which should affect $options page wide should be performed in
 *   a hook_gpt_ad_page_options_alter() implementation.
 */
function hook_gpt_ad_solt_settings_alter(&$ad, &$options) {
  // If top ad unit on the homepage alter the ad's size.
  if (drupal_is_front_page() && $ad['container'] == 'ad-manager-ad-top-0') {
    // Take over ad size.
    $ad['size'] = new stdClass();
    // From browser width 0 through 727 do not display an ad.
    $ad['size']->{0} = NULL;
    // From browser width 0 through 969 display a 728x90 ad.
    $ad['size']->{728} = array(array(728,90));
    // From browser width 970 and beyond display a 970x90 or 970x250 ad.
    $ad['size']->{970} = array(array(970,90),array(970,250));
  }
}

/**
 * @} End of "addtogroup hooks".
 */
