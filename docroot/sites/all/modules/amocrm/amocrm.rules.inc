<?php

/**
 * @file
 * Rules integration for amoCRM.
 */

/**
 * Implements hook_rules_event_info().
 */
function amocrm_rules_event_info() {
  $items = array();
  $types = array(AMOCRM_TYPE_LEAD, AMOCRM_TYPE_CONTACT, AMOCRM_TYPE_COMPANY);

  $vars[AMOCRM_TYPE_LEAD] = array(
    'amocrm' => array(
      'type' => 'amocrm_' . AMOCRM_TYPE_LEAD,
      'label' => t('Input leads data from amoCRM webhook.'),
      'description' => t('When lead is deleted, there is no available tokens, except lead ID.'),
    ),
  );
  $vars[AMOCRM_TYPE_CONTACT] = array(
    'amocrm' => array(
      'type' => 'amocrm_' . AMOCRM_TYPE_CONTACT,
      'label' => t('Input contacts data from amoCRM webhook.'),
      'description' => t('When contact is deleted, there is no available tokens, except contact ID.'),
    ),
  );
  $vars[AMOCRM_TYPE_COMPANY] = array(
    'amocrm' => array(
      'type' => 'amocrm_' . AMOCRM_TYPE_COMPANY,
      'label' => t('Input companies data from amoCRM webhook.'),
      'description' => t('When company is deleted, there is no available tokens, except company ID.'),
    ),
  );

  foreach ($types as $type) {
    $items['amocrm_' . $type . '_add'] = array(
      'label' => t('After saving new !type', array('!type' => $type)),
      'group' => t('amoCRM'),
      'variables' => $vars[$type],
    );
    $items['amocrm_' . $type . '_update'] = array(
      'label' => t('After updating existing !type', array('!type' => $type)),
      'group' => t('amoCRM'),
      'variables' => $vars[$type],
    );
    $items['amocrm_' . $type . '_delete'] = array(
      'label' => t('After deleting !type', array('!type' => $type)),
      'group' => t('amoCRM'),
      'variables' => $vars[$type],
    );

    if ($type == AMOCRM_TYPE_LEAD) {
      $items['amocrm_' . $type . '_status'] = array(
        'label' => t('After changing status of existing !type', array('!type' => $type)),
        'group' => t('amoCRM'),
        'variables' => $vars[$type],
      );
      $items['amocrm_' . $type . '_restore'] = array(
        'label' => t('After restoring deleted !type', array('!type' => $type)),
        'group' => t('amoCRM'),
        'variables' => $vars[$type],
      );
    }
  }

  return $items;
}
