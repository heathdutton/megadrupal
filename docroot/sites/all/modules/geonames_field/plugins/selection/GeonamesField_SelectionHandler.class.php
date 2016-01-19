<?php

/**
 * Entity handler for Geonames.
 */
class GeonamesField_SelectionHandler implements EntityReference_SelectionHandler {

  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new GeonamesField_SelectionHandler($field, $instance, $entity_type, $entity);
  }

  protected function __construct($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $this->field = $field;
    $this->instance = $instance;
    $this->entity_type = $entity_type;
    $this->entity = $entity;
  }
  
  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $settings = GeonamesField_SelectionHandler::getSettings($field);
    $options = array(
      'short' => t('Short'),
      'medium' => t('Medium'),
      'long' => t('Long'),
      'full' => t('Full'),
    );
    $form['style'] = array(
      '#title' => t('Style'),
      '#desctiption' => t('Verbosity of returned xml document.'),
      '#type' => 'select',
      '#default_value' => $settings['style'],
      '#required' => TRUE,
      '#options' => $options,
    );
    $form['display'] = array(
      '#title' => t('Display'),
      '#type' => 'textfield',
      '#default_value' => $settings['display'],
      '#required' => TRUE,
      '#description' => theme('token_tree', array('token_types' => array('geoname'))),
    );
    $options = array(
      'query' => t('Search over all attributes of a place: place name, country name, continent, admin codes, ...'),
      'name' => t('Place name only'),
      'name_equals' => t('Exact place name'),
    );
    $form['search'] = array(
      '#title' => t('Search'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $settings['search'],
      '#required' => TRUE,
    );
    $form['name_startsWith'] = array(
      '#title' => t('Place name starts with given characters.'),
      '#type' => 'checkbox',
      '#default_value' => $settings['name_startsWith'],
    );
    $form['maxRows'] = array(
      '#title' => t('Max rows'),
      '#description' => t('The maximal number of rows in the document returned by the service. Default is 100, the maximal allowed value is 1000.'),
      '#type' => 'textfield',
      '#default_value' => $settings['maxRows'],
    );
    $form['country'] = array(
      '#title' => t('Country'),
      '#description' => t('Default is all countries. The country parameter may occur more then once, example: FR, GP.'),
      '#type' => 'textfield',
      '#default_value' => $settings['country'],
    );
    $form['featureClass'] = array(
      '#title' => t('Feature class'),
      '#description' => t('Featureclass(es) (default= all feature classes)'),
      '#type' => 'checkboxes',
      '#options' => GeonamesField_SelectionHandler::featureclasses(),
      '#default_value' => $settings['featureClass'],
    );
    $form['featureCode'] = array(
      '#title' => t('Feature code'),
      '#description' => t('Featurecode(s) (default= all feature codes); this parameter may occur more then once, example: PPLC, PPLX.'),
      '#type' => 'textfield',
      '#default_value' => $settings['featureCode'],
    );
    $form['continentCode'] = array(
      '#title' => t('Continent'),
      '#description' => t('Restricts the search for toponym of the given continent.'),
      '#type' => 'select',
      '#options' => array('') + GeonamesField_SelectionHandler::continents(),
      '#default_value' => $settings['continentCode'],
    );
    return $form;
  }

  /**
   * Returns settings or default values.
   */
  protected static function getSettings($field) {
    $settings = $field['settings']['handler_settings'];
    $settings += array(
      'style' => 'medium',
      'display' => '[geoname:name]',
      'search' => 'query',
      'maxRows' => '',
      'name_startsWith' => FALSE,
      'country' => '',
      'featureClass' => array('P'),
      'featureCode' => '',
      'continentCode' => '',
    );
    return $settings;
  }

  /**
   * Returns available feature classes.
   */
  protected static function featureclasses() {
    return array(
      'A' => t('Administrative Boundary Features'),
      'H' => t('Hydrographic Features'),
      'L' => t('Area Features'),
      'P' => t('Populated Place Features'),
      'R' => t('Road / Railroad Features'),
      'S' => t('Spot Features'),
      'T' => t('Hypsographic Features'),
      'U' => t('Undersea Features'),
      'V' => t('Vegetation Features'),
    );
  }

  /**
   * Returns available continents.
   */
  protected static function continents() {
    return array(
      'AF' => t('Africa'),
      'AS' => t('Asia'),
      'EU' => t('Europe'),
      'NA' => t('North America'),
      'OC' => t('Oceania'),
      'SA' => t('South America'),
      'AN' => t('Antarctica'),
    );
  }
  
  /**
   * Implements EntityReferenceHandler::getReferencableEntities().
   */
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {
    global $language;
    $settings = GeonamesField_SelectionHandler::getSettings($this->field);
    $q = array(
      'style' => $settings['style'],
      'lang' => $language->language,
    );
    if($match) {
      $q[$settings['search']] = $match;
      if($settings['name_startsWith']) {
        $q['name_startsWith'] = $match;
      }
    }
    foreach(array('maxRows', 'country', 'featureCode', 'continentCode') as $op) {
      if($settings[$op]) {
        switch($op) {
          case 'country':
          case 'featureCode':
            $settings[$op] = explode(',', $settings[$op]);
            break;
        }
        $q[$op] = $settings[$op];
      }
    }
    $arr = array_filter($settings['featureClass']);
    if(count($arr)) {
      $q['featureClass'] = $arr;
    }
    $options = array();
    if($result = geonames_query('search', $q)) {
      foreach($result->results as $geoname) {
        $options[$geoname['geonameid']] = $this->getLabel((object) $geoname);
      }
    }
    return array('geoname' => $options);
  }
  
  /**
   * Implements EntityReferenceHandler::getLabel().
   */
  public function getLabel($entity) {
    return token_replace($this->field['settings']['handler_settings']['display'], array('geoname' => $entity));
  }

  /**
   * Implements EntityReferenceHandler::validateReferencableEntities().
   */
  public function validateReferencableEntities(array $ids) {
    $valid = array();
    foreach($ids as $id) {
      $res = geonames_query('get', array('geonameid' => $id));
      if($res->results && ($geoname = array_shift($res->results))) {
        $settings = GeonamesField_SelectionHandler::getSettings($this->field);
        $settings['featureClass'] = implode(',', array_filter($settings['featureClass']));
        $check = array(
          'country' => 'country',
          'featureClass' => 'fcl',
          'featureCode' => 'fcode',
          'contintentCode' => 'continentcode',
        );
        foreach($check as $key => $value) {
          if(!empty($settings[$key]) && ($possible = explode(',', $settings[$key])) && !in_array($geoname[$value], $possible)) {
            $id = FALSE;
            break;
          }
        }
        if($id) {
          $valid[] = $id;
        }
      }
    }
    return $valid;
  }

  /**
   * Implements EntityReferenceHandler::countReferencableEntities().
   */
  public function countReferencableEntities($match = NULL, $match_operator = 'CONTAINS') {
  }

  /**
   * Implements EntityReferenceHandler::validateAutocompleteInput().
   */
  public function validateAutocompleteInput($input, &$element, &$form_state, $form) {
  }

  /**
   * Implements EntityReferenceHandler::entityFieldQueryAlter().
   */
  public function entityFieldQueryAlter(SelectQueryInterface $query) {
  }

}
