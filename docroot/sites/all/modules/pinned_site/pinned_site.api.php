<?php

/**
 * @file
 * Hooks provided by the pinned_site module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implementing hook_pinned_site_browserconfig_alter allows a module to change
 * the browserconfig SimpleXMLElement.
 *
 * The following example will add a new badge configuration to the
 * <browserconfig><msapplication> node:
 * 
 * <badge>
 *   <polling-uri src="badge.xml"/>
 *   <frequency>30</frequency>
 * </badge>
 */
function hook_pinned_site_browserconfig_alter(&$browserconfig) {
  $badge = $browserconfig->msapplication->addChild('badge');
  $polling = $badge->addChild('polling-uri');
  $polling->addAttribute('src', 'badge.xml');
  $badge->addChild('frequency', 30);
}

/**
 * @} End of "addtogroup hooks".
 */
