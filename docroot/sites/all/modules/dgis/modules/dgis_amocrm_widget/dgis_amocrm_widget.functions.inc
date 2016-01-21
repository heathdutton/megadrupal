<?php
/**
 * @file
 * Functions for module AmoCRM widget 2gis.
 */

/**
 * Remove connection for current AmoCRM card.
 */
function dgis_amocrm_widget_remove_prepare(&$crm_data) {
  if ($crm_data && isset($crm_data['custom_fields'])) {
    foreach ($crm_data['custom_fields'] as $k => $field) {
      if ($field['id'] == variable_get('dgis_amocrm_widget_linking_field_id', -1)) {
        $crm_data['custom_fields'][$k]['values'] = array();
      }
    }
    return TRUE;
  }

  return FALSE;
}

/**
 * Get the cached 2gis data for current card.
 */
function dgis_amocrm_widget_get_cached_data() {
  $entity = dgis_amocrm_widget_get_amocrm_entities_from_context();
  if (empty($entity)) {
    return FALSE;
  }
  $current_entity = $entity['data'];
  $card_entity = $entity['card_entity'];
  if ($card_entity == AMOCRM_TYPE_CONTACT) {
    if (!empty($current_entity['linked_company'])) {
      $linked_company = $current_entity['linked_company'];
      if (!empty($linked_company)) {
        $current_entity['custom_fields'] = $linked_company['custom_fields'];
      }
    }
  }

  $cached_dgis_data = FALSE;
  foreach ($current_entity['custom_fields'] as $k => $field) {
    if ($field['id'] == variable_get('dgis_amocrm_widget_linking_field_id', -1)) {
      $cached_dgis_data = $field['values'][0]['value'];
      break;
    }
  }
  if ($cached_dgis_data) {
    $cached_dgis_data = unserialize(htmlspecialchars_decode($cached_dgis_data));
  }

  return $cached_dgis_data;
}

/**
 * Save connection into the AmoCRM.
 */
function dgis_amocrm_widget_save_connection($current_entity, $dgis_data) {
  if (empty($current_entity) || empty($dgis_data)) {
    return FALSE;
  }
  $data = $dgis_data['data'];
  $cached_info = serialize(array('id' => $dgis_data['id'], 'hash' => $dgis_data['hash'], 'data' => $data));
  $client = amocrm_api_get_default_client();

  $linked_company = FALSE;
  if (!empty($current_entity['linked_company'])) {
    $linked_company = $current_entity['linked_company'];
  }

  if ($linked_company) {
    $crm_data = array(
      'id' => $linked_company['id'],
      'name' => $data['name'],
      'custom_fields' => $linked_company['custom_fields'],
    );
  }
  else {
    $crm_data = array(
      'name' => $data['name'],
      'custom_fields' => array(),
    );
  }
  $dgis_contacts = array();
  foreach ($data['contacts'] as $group) {
    foreach ($group['contacts'] as $contact) {
      $dgis_contacts[$contact['type']][] = array('value' => $contact['value']);
    }
  }
  $dgis_contacts['address'][] = array('value' => $data['city_name'] . ', ' . $data['address']);

  $contacts_types = dgis_amocrm_widget_get_corresponding_contact_codes($client);
  foreach (array_keys($contacts_types) as $type) {
    if (isset($dgis_contacts[$type])) {
      $added = FALSE;
      foreach ($crm_data['custom_fields'] as $k => $field) {
        if ($field['id'] == $contacts_types[$type]['id']) {
          if (isset($contacts_types[$type]['enum'])) {
            foreach($dgis_contacts[$type] as $kk => $var) {
              $dgis_contacts[$type][$kk]['enum'] = $contacts_types[$type]['enum'];
            }
          }
          if ($contacts_types[$type]['multiple']) {
            $crm_data['custom_fields'][$k]['values'] = dgis_amocrm_widget_unique_values(
              array_merge($crm_data['custom_fields'][$k]['values'], $dgis_contacts[$type])
            );
          }
          else {
            $crm_data['custom_fields'][$k]['values'] = dgis_amocrm_widget_unique_values($dgis_contacts[$type]);
          }
          $added = TRUE;
        }
      }
      if (!$added) {
        if (isset($contacts_types[$type]['enum'])) {
          foreach($dgis_contacts[$type] as $kk => $var) {
            $dgis_contacts[$type][$kk]['enum'] = $contacts_types[$type]['enum'];
          }
        }
        $crm_data['custom_fields'][] = array(
          'id' => $contacts_types[$type]['id'],
          'name' => $contacts_types[$type]['name'],
          'code' => $contacts_types[$type]['code'],
          'values' => dgis_amocrm_widget_unique_values($dgis_contacts[$type]),
        );
      }
    }
  }

  // Set 2gis organisation id to the card, but take care that it is unique.
  $linked = FALSE;
  foreach ($crm_data['custom_fields'] as $k => $field) {
    if ($field['id'] == variable_get('dgis_amocrm_widget_linking_field_id', -1)) {
      $crm_data['custom_fields'][$k]['values'] = array();
      $crm_data['custom_fields'][$k]['values'][] = array('value' => $cached_info);
      $linked = TRUE;
    }
  }
  if (!$linked) {
    $crm_data['custom_fields'][] = array(
      'id' => variable_get('dgis_amocrm_widget_linking_field_id', -1),
      'name' => 'Linked 2gis company',
      'code' => '2GIS',
      'values' => array(array('value' => $cached_info)),
    );
  }

  if ($linked_company) {
    amocrm_api_companies_update($client, array($crm_data));
    return TRUE;
  }
  else {
    $res = amocrm_api_companies_add($client, array($crm_data));
    $new_company = $res['contacts']['add'][0]['id'];
    if (!$new_company) {
      return FALSE;
    }
    $current_entity['linked_company_id'] = $new_company;
    $current_entity['company_name'] = $crm_data['name'];
    amocrm_api_contacts_update($client, array($current_entity));
    return TRUE;
  }
}

/**
 * Get current AmoCRM card from context.
 */
function dgis_amocrm_widget_get_amocrm_entities_from_context() {
  if (!empty($_SESSION['amocrm_context'])) {
    $card_id = $_SESSION['amocrm_context']['card_id'];
    $card_entity = isset($_SESSION['amocrm_context']['card_entity']) ? check_plain(trim($_SESSION['amocrm_context']['card_entity'])) : NULL;
    if (!empty($card_id) && !empty($card_entity)) {
      $client = amocrm_api_get_default_client();
      $current_entity = array();
      switch($card_entity) {
        case AMOCRM_TYPE_CONTACT:
          $current_entity = amocrm_api_contact_load($client, $card_id);
          if (!empty($current_entity['linked_company_id'])) {
            $linked_company = amocrm_api_company_load($client, $current_entity['linked_company_id']);
            if (!empty($linked_company)) {
              $current_entity['linked_company'] = $linked_company;
            }
          }
          break;

        case AMOCRM_TYPE_COMPANY:
          $current_entity = amocrm_api_company_load($client, $card_id);
          break;
      }
      return array('card_entity' => $card_entity, 'data' => $current_entity);
    }
  }

  return array();
}

/**
 * Returns the phones of current contact or firm displayed in AmoCRM.
 */
function dgis_amocrm_widget_get_phones_from_context() {
  $entity = dgis_amocrm_widget_get_amocrm_entities_from_context();
  if (empty($entity)) {
    return array();
  }
  $current_entity = $entity['data'];
  $card_entity = $entity['card_entity'];

  if ($card_entity == AMOCRM_TYPE_CONTACT) {
    if (!empty($current_entity['linked_company'])) {
      $linked_company = $current_entity['linked_company'];
      if (!empty($linked_company) && !empty($linked_company['custom_fields'])) {
        $current_entity['custom_fields'] = array_merge($current_entity['custom_fields'], $linked_company['custom_fields']);
      }
    }
  }
  $phones = array();
  if (!empty($current_entity['custom_fields'])) {
    foreach ($current_entity['custom_fields'] as $custom_fields) {
      if ($custom_fields['code'] === 'PHONE') {
        foreach ($custom_fields['values'] as $phone) {
          $phones[] = $phone['value'];
        }
      }
    }
  }

  return $phones;
}

/**
 * Get connections between the 2gis fields and AmoCRM fields.
 */
function dgis_amocrm_widget_get_corresponding_contact_codes($client) {
  $client_data = amocrm_api_account_load($client);
  if (!$client_data) {
    return array();
  }
  $return = array();
  foreach ($client_data['custom_fields']['companies'] as $field) {
    $code = FALSE;
    switch ($field['code']) {
      case 'PHONE':
        $code = array('phone', 'fax');
        break;

      case 'EMAIL':
        $code = array('email');
        break;

      case 'WEB':
        $code = array('website'/*, 'facebook', 'twitter', 'vkontakte'*/);
        break;

      case 'ADDRESS':
        $code = array('address');
        break;
    }
    if ($code) {
      foreach ($code as $item) {
        $return[$item] = array(
          'code' => $field['code'],
          'name' => $field['name'],
          'id' => $field['id'],
          'multiple' => $field['multiple'] == 'Y',
        );
        if (isset($field['enums'])) {
          $enum = FALSE;
          if ($code == 'fax') {
            foreach ($field['enums'] as $i => $e) {
              if ($e == 'FAX') {
                $enum = $i;
              }
            }
          }
          if (!$enum) {
            $enum = array_shift(array_keys($field['enums']));
          }
          $return[$item]['enum'] = $enum;
        }
      }
    }
  }

  return $return;
}

/**
 * Make unique array of values for AmoCRM.
 */
function dgis_amocrm_widget_unique_values(&$values) {
  $unique_values = array();
  foreach ($values as $value) {
    $hash = $value['value'];
    if (isset($value['enum'])) {
      $hash .= $value['enum'];
    }
    $hash = md5($hash);
    $unique_values[$hash] = $value;
  }

  return array_values($unique_values);
}
