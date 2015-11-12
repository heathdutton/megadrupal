<?php

/**
 * @file
 * Base class for CollectiveAccess parser helper objects
 * This is used by collectiveaccess_feeds module to provide easier mapping
 */

abstract class CollectiveAccessBaseParser {
  protected $config;
  protected $ca_item;
  protected $item_type;

  abstract public function parse();

  /**
   * Constructor for CollectiveAccessObjectParser
   */
  public function __construct(CollectiveAccessBaseItem $ca_item, $config = array(), $item_type = '') {
    $this->config = $config;
    $this->ca_item = $ca_item;
    $this->item_type = $item_type;
    $this->parse();
  }

  /**
   * Retrieve a CollectiveAccessObject, parsed for easier comsumption
   */
  public function getParsedItem() {
    return $this->ca_item;
  }

    /**
   * Retrieve a data array, parsed for easier comsumption
   */
  public function getParsedObjectData($strings_only = FALSE) {
    $data = $this->ca_item->getData();
    if ($strings_only) {
      $return = array();
      if ($data) {
        foreach ($data as $key => $value) {
          if (is_string($value)) {
            $return[$key] = $value;
          }
        }
      }
      return $return;
    }

    return $data;
  }

  /**
   * Helper method for the parsing of data.
   * This method parses standard attributes that are available on the object.
   */
  protected function parse_attributes() {
    $ca_instance = $this->ca_item->getCAInstance();
    $locales = $ca_instance->getCataloguingLocalesList();
    $attributes = $ca_instance->getAvailableAttributes($this->item_type);

    if ($attributes) {
      foreach ($attributes as $attribute_name => $attribute_type) {
        if ($attribute_data = $this->ca_item->{$attribute_name}) {
          switch ($attribute_type) {
            case 'Container':
              // parse Container element
              $this->parseContainerAttribute($attribute_name, $attribute_data, $locales);
              break;
            case 'List':
              // parse List element
              $this->parseListAttribute($attribute_name, $attribute_data, $locales);
              break;
            default:
              $this->parseStandardAttribute($attribute_name, $attribute_data, $locales);
              break;
          }
        }
      }
    }
  }

  /**
   * Parse a standard user-defined attribute
   * Supports multiple locales & single + multivalues.
   */
  protected function parseStandardAttribute($attribute_name, $attribute, $locales, $value_alias = NULL) {
    // process $value_alias, as used by List attributes
    if (!$value_alias) {
      $value_alias = $attribute_name;
    }
    $locale_aware_attribute = TRUE;
    if (is_array($attribute)) {
      foreach ($attribute as $langcode => $attribute_value) {
        // check whether it's a multivalue element
        $num = count($attribute_value);
        // define the attribute key & locale awareness
        $locale_key = ($langcode != 'NONE') ? $attribute_name . '-' . $langcode : $attribute_name;
        $locale_aware_attribute = ($langcode != 'NONE') ? TRUE : FALSE;

        if ($num == 1) { // single value
          $this->ca_item->$locale_key = $attribute_value['value_0'][$value_alias];
        }
        else { // multi value
          $multi_value = array();
          foreach ($attribute_value as $val) {
            $multi_value[] = $val[$value_alias];
          }
          $this->ca_item->$locale_key = $multi_value;
        }
      }
      // remove data from the original attribute, if it was localised
      if ($locale_aware_attribute) {
        $this->ca_item->$attribute_name = NULL;
      }
    }
    else {
      return $attribute;
    }
  }

  protected function parseListAttribute($attribute_name, $attribute, $locales) {
    $list_labels = array();
    // Get a list of the list item labels
    if (is_array($attribute)) {
      foreach ($attribute as $attrib_langcode => $attrib) {
        foreach ($attrib as $valuekey => $item) {
          if (isset($item['labels'])) {
            foreach ($item['labels'] as $label_locale => $label_value) {
              $list_labels[$label_locale][] = $label_value;
            }
          }
        }
      }
    }

    // Process the gathered list item labels
    if ($list_labels) {
      foreach ($list_labels as $locale => $label_arr) {
        $localekey = ($locale != 'NONE') ? $attribute_name . '-' . $locale : $attribute_name;
        if (count($label_arr) == 1) { // single value
          $this->ca_item->$localekey = array_shift($label_arr);
        }
        else { // multivalue
          $this->ca_item->$localekey = $label_arr;
        }
      }
    }
  }

  /**
   * Parse a user-defined Container attribute
   * Adds both container value & single container items as object attributes
   * Supports multiple locales & single + multivalues.
   */
  protected function parseContainerAttribute($attribute_name, $attribute, $locales) {
    $locale_aware_attribute = TRUE;
    if (is_array($attribute)) {
      foreach ($attribute as $langcode => $attribute_value) {
        // check whether it's a multivalue element
        $num = count($attribute_value);
        // define the attribute key & locale awareness
        $locale_key = ($langcode != 'NONE') ? $attribute_name . '-' . $langcode : $attribute_name;
        $locale_aware_attribute = ($langcode != 'NONE') ? TRUE : FALSE;

        if ($num == 1) { // single value
          // Add single container attribute
          $this->ca_item->$locale_key = $attribute_value['value_0'];
          // Add attribute for each container element
          if (is_array($attribute_value['value_0'])) {
            foreach ($attribute_value['value_0'] as $elementcode => $element_value) {
              $element_key = ($langcode != 'NONE') ? $elementcode . '-' . $langcode : $elementcode;
              $this->ca_item->$element_key = $element_value;
            }
          }
        }
        else { // multi value
          $multi_value = array();
          $elements_values = array();
          foreach ($attribute_value as $key => $val) {
            $multi_value[] = $val;
            foreach ($val as $element_code => $element_value) {
              $elements_values[$element_code][] = $element_value;
            }

          }
          // Add container attributes
          $this->ca_item->$locale_key = $multi_value;
          // Add attributes for each container element
          if ($elements_values) {
            foreach ($elements_values as $elementcode => $val) {
              $element_key = ($langcode != 'NONE') ? $elementcode . '-' . $langcode : $elementcode;
              $this->ca_item->$element_key = $val;
            }
          }
        }
      }
      // remove data from the original attribute, if it was localised
      if ($locale_aware_attribute) {
        $this->ca_item->$attribute_name = NULL;
      }
    }
    else {
      return $attribute;
    }
  }

  /**
   * Helper method for the parsing of preferred labels
   */
  protected function parse_labels() {
    if ($this->ca_item->preferred_labels) {
      foreach ($this->ca_item->preferred_labels as $locale => $value) {
        $locale_key = ($locale != 'NONE') ? 'title-' . $locale : 'title';
        $this->ca_item->{$locale_key} = $value['name'];
      }
    }
  }

}