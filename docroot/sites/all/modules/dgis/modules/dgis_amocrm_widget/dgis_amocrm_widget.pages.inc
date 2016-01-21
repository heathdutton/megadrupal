<?php
/**
 * @file
 * Page callbacks for module AmoCRM widget 2gis.
 */

/**
 * Page callback for 2gis-api/amocrm-widget.
 */
function dgis_amocrm_widget_callback() {
  $phones = dgis_amocrm_widget_get_phones_from_context();
  $data = array();
  if (!empty($phones)) {
    if (!empty($_SESSION['amocrm_context']['dgis_city'])) {
      $city = $_SESSION['amocrm_context']['dgis_city'];
    }
    else {
      $city = variable_get('dgis_city', DGIS_DEFAULT_CITY);
    }
    foreach ($phones as $phone) {
      $result = dgis_get_firms(check_plain($phone), 'relevance', 1, 20, FALSE, FALSE, $city);
      if (!empty($result)) {
        foreach ($result as $firm) {
          $data[$firm['id']] = $firm;
        }
      }
    }
  }
  $return = '';
  $return_count = count($data);
  if (!empty($data)) {
    foreach ($data as $item) {
      $return .= theme('dgis_amocrm_widget_company_teaser', array('data' => $item));
    }
  }
  $output = theme(
    'dgis_amocrm_widget_company_teaser_wrapper',
    array(
      'data' => array('results' => $return, 'total' => $return_count)
    )
  );

  return theme('dgis_amocrm_widget_wrapper', array('content' => $output));
}

/**
 * Callback for 2gis-api/amocrm-widget/remove.
 */
function dgis_amocrm_widget_remove_callback() {
  $res = t('Error with removing of connection');
  $entity = dgis_amocrm_widget_get_amocrm_entities_from_context();
  if (empty($entity)) {
    return $res;
  }
  $current_entity = $entity['data'];
  $card_entity = $entity['card_entity'];
  $client = amocrm_api_get_default_client();

  switch($card_entity) {
    case AMOCRM_TYPE_CONTACT:
      if (dgis_amocrm_widget_remove_prepare($current_entity)) {
        amocrm_api_contacts_update($client, array($current_entity));
        $res = t('Connection was successfully removed');
      }
      if (!empty($current_entity['linked_company'])) {
        $linked_company = $current_entity['linked_company'];
        if (!empty($linked_company)) {
          if (dgis_amocrm_widget_remove_prepare($linked_company)) {
            amocrm_api_companies_update($client, array($linked_company));
            $res = t('Connection was successfully removed');
          }
        }
      }
      break;

    case AMOCRM_TYPE_COMPANY:
      if (dgis_amocrm_widget_remove_prepare($current_entity)) {
        amocrm_api_companies_update($client, array($current_entity));
        $res = t('Connection was successfully removed');
      }
      break;
  }

  return $res;
}

/**
 * Callback for 2gis-api/amocrm-widget/connected_entity.
 */
function dgis_amocrm_widget_connected_entity_callback() {
  $cached_dgis_data = dgis_amocrm_widget_get_cached_data();
  if (!empty($cached_dgis_data)) {
    return theme('dgis_amocrm_widget_company_full', array('data' => $cached_dgis_data['data']));
  }

  return t('Error with getting connected company');
}

/**
 * Callback for 2gis-api/amocrm-widget/synchronise.
 */
function dgis_amocrm_widget_synchronise_callback() {
  $error = t('Error with synchronisation');
  $cached_dgis_data = dgis_amocrm_widget_get_cached_data();
  $entity = dgis_amocrm_widget_get_amocrm_entities_from_context();
  if (empty($entity)) {
    return $error;
  }
  $current_entity = $entity['data'];
  $card_entity = $entity['card_entity'];

  if ($card_entity != AMOCRM_TYPE_CONTACT) {
    return t('You should do this operation from the contact page.');
  }
  $dgis_company = $cached_dgis_data['data'];
  $dgis_company_id = $cached_dgis_data['id'];
  $hash = $cached_dgis_data['hash'];
  if ($data = dgis_get_firm_profile($dgis_company_id, $hash)) {
    if (dgis_amocrm_widget_save_connection($current_entity, array('id' => $dgis_company_id, 'hash' => $hash, 'data' => $data))) {
      return t('Company was successfully connected. Please reload the page to see the changes.');
    }
  }
  else {
    // Try to find the company by name again.
    $result = dgis_get_firms($dgis_company['name']);
    if (!empty($result)) {
      $return = '';
      foreach ($result as $item) {
        $return .= theme('dgis_amocrm_widget_company_teaser', array('data' => $item));
      }
      return $return;
    }
    return t('No results were found.');
  }

  return $error;
}

/**
 * Page callback for 2gis-api/amocrm-widget/%.
 */
function dgis_amocrm_widget_company_callback($dgis_company_id, $hash) {
  $content = t('Company was not found, please choose another company');
  if (empty($dgis_company_id) || empty($hash)) {
    return $content;
  }
  if ($data = dgis_get_firm_profile($dgis_company_id, $hash)) {
    $content = theme('dgis_amocrm_widget_company_full', array('data' => $data, 'hash' => $hash, 'show_choose_link' => TRUE));
  }

  return theme('dgis_amocrm_widget_wrapper', array('content' => $content));
}

/**
 * Callback for 2gis-api/amocrm-widget/connect/%/%.
 */
function dgis_amocrm_widget_company_connect($dgis_company_id, $hash) {
  if (!variable_get('dgis_amocrm_widget_linking_field_id', FALSE)) {
    return t('Internal problems with module installation are faced. Please contact the administrator of your Drupal site.');
  }
  $error = t('Error with connection');
  if (empty($dgis_company_id) || empty($hash)) {
    return $error;
  }
  if ($data = dgis_get_firm_profile($dgis_company_id, $hash)) {
    $entity = dgis_amocrm_widget_get_amocrm_entities_from_context();
    if (empty($entity)) {
      return $error;
    }
    $current_entity = $entity['data'];
    $card_entity = $entity['card_entity'];

    if ($card_entity != AMOCRM_TYPE_CONTACT) {
      return t('You should do this operation from the contact page.');
    }

    if (dgis_amocrm_widget_save_connection($current_entity, array('id' => $dgis_company_id, 'hash' => $hash, 'data' => $data))) {
      $content = '<div class="widget-2gis-ico-status widget-2gis-ico-status-success"></div>
          <div class="widget-2gis-ico-status-text">' . t('Connection with 2GIS was successful') . '</div>';
      return theme('dgis_amocrm_widget_wrapper', array('content' => $content));
    }
  }

  return $error;
}

/**
 * Callback for 2gis-api/amocrm-widget/city-change.
 */
function dgis_amocrm_widget_change_city_callback() {
  $cities = dgis_get_cities();
  if (empty($cities)) {
    return t('No cities were found');
  }
  $items = array();
  foreach ($cities as $city) {
    $link = l(
      $city['name']. ' (' . $city['code'] . ')',
      '2gis-api/amocrm-widget/city-change/' . $city['name'],
      array(
        'attributes' => array(
          'id' => 'drupal-2gis-link-city-choose',
          'class' => array('drupal-2gis-link')
        ),
      )
    );
    $items[] = array('data' => $link, 'class' => array('widget-2gis-list-item'));
  }
  $return = theme('item_list', array(
    'items' => $items,
    'type' => 'ul',
    'attributes' => array('class' => array('widget-2gis-list-wrap'))
  ));
  $return = theme('dgis_amocrm_widget_city_change_wrapper', array('cities' => $return));

  return theme('dgis_amocrm_widget_wrapper', array('content' => $return));
}

/**
 * Callback for 2gis-api/amocrm-widget/city-change/%.
 */
function dgis_amocrm_widget_set_city_callback($city) {
  if (!empty($city)) {
    $_SESSION['amocrm_context']['dgis_city'] = $city;
  }
  drupal_goto(
    'amocrm_widget/embed',
    array(
      'query' => array('url' => '2gis-api/amocrm-widget')
    )
  );
}
