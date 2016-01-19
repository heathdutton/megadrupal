<?php

/**
 * @file Internationalization (i18n) class.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * CAT item i18n object.
 */
class i18n_cat_item_object extends i18n_string_object_wrapper {

  /**
   * Construct
   *
   * @param string $type
   * @param object $object
   */
  public function __construct($type, $object) {
    $this->type = $type;
    $this->object = is_scalar($object) ? $this->load_object($object) : $object;
  }

  /**
   * Translation mode for object.
   *
   * @return int Constant showing translate mode.
   */
  public function get_translate_mode() {
    if (!empty($this->object->translatable)) {
  	  return I18N_MODE_TRANSLATE;
    }
    
    return I18N_MODE_NONE;
  }

  /**
   * Access to object translation.
   * This should check object properties and permissions.
   *
   * @return bool Param showing whether CAT item is translatable or not.
   */
  protected function translate_access() {
    return user_access('translate interface')
      && user_access('administer cat items')
      && !empty($this->object->translatable);
  }

  /**
   * Get object strings for translation.
   *
   * @return mixed Translated properties.
   */
  protected function build_properties() {
    $properties = parent::build_properties();
    list (, $object_id) = $this->get_string_context();

    $cat_types = array('cat_tabs', 'cat_actions');
    foreach ($cat_types as $cat_type) {
      if (!empty($this->object->{$cat_type})) {
        foreach ($this->object->{$cat_type} as $key => $tab) {
          $key_name = $key;
          if (!empty($tab['machine_name'])) {
            $key_name = $tab['machine_name'];
          }
          elseif (!empty($tab['reusabled_machine_name'])) {
            $key_name = $tab['reusabled_machine_name'];
          }

          $properties[$this->get_textgroup()][$cat_type][$object_id][$key_name] = array(
            'string' => $tab['title'],
            'title' => t('Title: @title (path: @path)', array(
              '@title' => $tab['title'],
              '@path' => $tab['path_original']
            ))
          );
        }
      }
    }

    return $properties;
  }

}

