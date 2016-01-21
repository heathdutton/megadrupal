<?php
/**
 * @file
 * Theme function of AmoCRM 2gis widget.
 */

/**
 * Preprocess for theming dgis_amocrm_widget_company_teaser.
 */
function dgis_amocrm_widget_preprocess_dgis_amocrm_widget_company_teaser(&$vars) {
  if (!empty($vars['data']['name']) && !empty($vars['data']['id']) && !empty($vars['data']['hash'])) {
    $vars['company_title'] = l(
      t($vars['data']['name']),
      'amocrm_widget/embed',
      array(
        'attributes' => array(
          'id' => 'drupal-2gis-link-firm',
          'class' => array('drupal-2gis-link', 'widget-2gis-list-item-title')
        ),
        'query' => array('url' => '2gis-api/amocrm-widget/' . $vars['data']['id'] . '/' . $vars['data']['hash'])
      )
    );
  }
}

/**
 * Preprocess for theming dgis_amocrm_widget_company_teaser_wrapper.
 */
function dgis_amocrm_widget_preprocess_dgis_amocrm_widget_company_teaser_wrapper(&$vars) {
  $change_city = l(
    t('Change city'),
    'amocrm_widget/embed',
    array(
      'attributes' => array('id' => 'drupal-2gis-link-city', 'class' => array('drupal-2gis-link, widget-2gis-change-city')),
      'query' => array('url' => '2gis-api/amocrm-widget/city-change')
    )
  );
  if (!empty($_SESSION['amocrm_context']) && !empty($_SESSION['amocrm_context']['dgis_city'])) {
    $city = $_SESSION['amocrm_context']['dgis_city'];
  }
  else {
    $city = variable_get('dgis_city', DGIS_DEFAULT_CITY);
  }

  $vars['data']['summary'] = t(
    'Found !count organisations in the city !city !change_city',
    array(
      '!count' => $vars['data']['total'],
      '!city' => $city,
      '!change_city' => $change_city
    )
  );
}

/**
 * Preprocess for theming dgis_amocrm_widget_company_teaser_wrapper.
 */
function dgis_amocrm_widget_preprocess_dgis_amocrm_widget_wrapper(&$vars) {
  $logo = theme('image', array('path' => DGIS_AMOCRM_WODGET_MODULE_PATH . '/images/2gis-logo.png'));
  $dgis_site_external = variable_get('dgis_site_external', 'http://2gis.ru');
  if ($dgis_site_external) {
    $logo = l(
      $logo,
      $dgis_site_external,
      array(
        'external' => TRUE,
        'html' => TRUE,
        'attributes' => array('class' => 'widget-2gis-logo')
      )
    );
  }
  $vars['logo'] = $logo;
}

/**
 * Preprocess for theming dgis_amocrm_widget_company_full.
 */
function dgis_amocrm_widget_preprocess_dgis_amocrm_widget_company_full(&$vars) {
  if (!empty($vars['data']['contacts'])) {
    $vars['contacts'] = array();
    foreach ($vars['data']['contacts'] as $group) {
      foreach ($group['contacts'] as $contact) {
        $group['name'] = $group['name'] ? $group['name'] : 'default';
        if ($contact['type'] == 'website') {
          $contact['value'] = l($contact['alias'], $contact['value'], array('extarnal' => TRUE));
        }
        switch($contact['type']) {
          case 'fax':
          case 'phone':
            $contact['type'] = 'phone';
            break;
          case 'website':
            $contact['type'] = 'site';
            break;
          default:
            $contact['type'] = 'address';
        }
        $vars['contacts'][$group['name']][$contact['type']][] = $contact['value'];
      }
    }
  }
  if (!empty($vars['show_choose_link'])) {
    $vars['choose_link'] = l(
      '+ <span>' . t('Connect with 2GIS') . '</span>',
      'amocrm_widget/embed',
      array(
        'attributes' => array('id' => 'drupal-2gis-link-firm', 'class' => array('drupal-2gis-link')),
        'query' => array('url' => '2gis-api/amocrm-widget/connect/' . $vars['data']['id'] . '/' . $vars['hash']),
        'html' => TRUE,
      )
    );
    $vars['back_link'] = l(
      '← <span>' . t('Return') . '</span>',
      'amocrm_widget/embed',
      array(
        'attributes' => array('id' => 'drupal-2gis-link-back', 'class' => array('drupal-2gis-link')),
        'query' => array('url' => '2gis-api/amocrm-widget'),
        'html' => TRUE,
      )
    );
  }
  else {
    $vars['choose_link'] = l(
      '+ <span>' . t('Synchronise with 2GIS') . '</span>',
      'amocrm_widget/embed',
      array(
        'attributes' => array('id' => 'drupal-2gis-link-firm', 'class' => array('drupal-2gis-link')),
        'query' => array('url' => '2gis-api/amocrm-widget/synchronise'),
        'html' => TRUE,
      )
    );
  }
}
