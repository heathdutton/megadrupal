<?php

/**
 * @file
 * Rules integration for the regcode module.
 */

/**
 * Implementation of module_rules_event_info().
 *
 * @ingroup rules
 */
function regcode_rules_event_info() {
  return array(
    'regcode_used' => array(
      'label' => t('User has used a registration code'),
      'group' => 'Regcode',
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('The user using the code'),
        ),
        'regcode' => array(
          'type' => 'regcode',
          'label' => t('The regcode which was used'),
        ),
      ),
    ),
  );
}
