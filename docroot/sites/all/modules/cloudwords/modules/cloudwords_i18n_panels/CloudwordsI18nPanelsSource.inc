<?php

/**
 * @file
 * Contains CloudwordsI18nStringSource.
 */

class CloudwordsI18nPanelsSource implements CloudwordsSourceControllerInterface {

  protected $objectInfo = array();
  protected $stringInfo = array();
  protected $type;

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function __construct($type) {
    $this->type = $type;
    $this->objectInfo = i18n_object_info($type);
    $this->stringInfo = i18n_string_group_info($this->objectInfo['string translation']['textgroup']);
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function typeLabel() {
    return $this->objectInfo['title'];
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function textGroup() {
    return $this->objectInfo['string translation']['textgroup'];
  }

  /**
   * Implements CloudwordsSourceControllerInterface::__construct().
   */
  public function textGroupLabel() {
    return $this->stringInfo['title'];
  }

  /**
   * Returns the i18n object wrapper for a translatable.
   *
   * @param CloudwordsTranslatable $translatable
   *   A Cloudwords translatable.
   *
   * @return i18n_object_wrapper
   *   An i18n_object_wrapper object.
   *
   * @see i18n_object
   */
  protected function getWrapper(CloudwordsTranslatable $translatable) {
    //$object = $this->loadObject($translatable->objectid);
    return i18n_get_object('pane_configuration', $translatable->objectid);
  }

  /**
   * Implements CloudwordsSourceControllerInterface::targetLabel().
   */
  public function targetLabel(CloudwordsTranslatable $translatable) {
    $label = $translatable->label;
    
    //TODO can we load these via uuid?
    $query = db_select('panels_pane');
    $query->fields('panels_pane')
      ->condition('uuid', $translatable->objectid);
    $pane_row = $query->execute()->fetchAssoc();
    if($pane_row['configuration']){
      $pane_row_configuration = unserialize($pane_row['configuration']);
      
      if(strlen($pane_row_configuration['admin_title']) > 0) {
        $label = $pane_row_configuration['admin_title'];
      }elseif(strlen($pane_row_configuration['title']) > 0){
        $label = $pane_row_configuration['title'];      
      }else{
        $label = substr(strip_tags($pane_row_configuration['body']), 0, 60);       
      }
      
    }

    return $label;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::uri().
   */
  public function uri(CloudwordsTranslatable $translatable) {
    $wrapper = $this->getWrapper($translatable);
    $strings = $wrapper->get_strings();
    
    if (method_exists($wrapper, 'get_translate_path') && ($edit_url = $wrapper->get_translate_path($translatable->language))) {
      return array(
        'path' => $edit_url,
      );
    }
  }

  /**
   * Implements CloudwordsSourceControllerInterface::data().
   */
  public function data(CloudwordsTranslatable $translatable) {
    //TODO improve upon fetching strings
    $locations = array('panels:pane_configuration:'.$translatable->objectid.':title', 
                       'panels:pane_configuration:'.$translatable->objectid.':body');
    
    $query = db_select('i18n_string');
    $query->join('locales_source', 'l', 'i18n_string.lid = l.lid');
    $query->fields('i18n_string')
      ->fields('l',array('source', 'location'))
      ->condition('type', $translatable->type)
      ->condition('l.location', $locations, 'IN');   
    $i18n_strings = $query->execute();

    $structure = array('#label' => 'i18n Strings: ' . $translatable->typeLabel());
    foreach($i18n_strings as $i18n_string){
      $structure[$i18n_string->location] = array(
        '#label' => $i18n_string->property,
        '#text' => $i18n_string->source,
        '#translate' => TRUE,
      );
    }
    return $structure;
  }

  /**
   * Implements CloudwordsSourceControllerInterface::save().
   */
  public function save(CloudwordsTranslatable $translatable) {
    foreach ($translatable->getData() as $i18n_string => $item) {
      if (isset($item['#translation']['#text'])) {
        i18n_string_translation_update($i18n_string, $item['#translation']['#text'], $translatable->language);
      }
    }
    return TRUE;
  }
}
