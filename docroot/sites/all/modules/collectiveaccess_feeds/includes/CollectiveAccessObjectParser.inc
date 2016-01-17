<?php

/**
 * @file
 * Helper class to parse CollectiveAccessObject attributes for easier access.
 * This is used by collectiveaccess_feeds module to provide a Feeds-ready object to map
 */

class CollectiveAccessObjectParser extends CollectiveAccessBaseParser {
  protected $object_builder;

  public function __construct(CollectiveAccessObject $ca_item, $config = array()) {
    parent::__construct($ca_item, $config, 'ca_objects');
  }

  /**
   * Parse the CollectiveAccessObject to make each attribute more easily accessible
   * This is useful for the collectiveaccess_feeds module.
   */
  public function parse() {
    $ca_instance = $this->ca_item->getCAInstance();
    // Add a reference to an ObjectBuilder object, to fetch attribute & locale data
    $this->object_builder = new CollectiveAccessObjectBuilder($ca_instance);
    //$listdata = $this->parse_getListdata($obj_builder);

    $this->parse_attributes();
    $this->parse_labels();
    $this->parse_images();
    $this->parse_relations();
    $this->parse_sets();
  }

  /**
   * Helper method for the parsing of data.
   * Make all image representation versions available as attributes
   */
  protected function parse_images() {
    $all_images_url = $all_images_path = $all_images_full = $all_images_md5 = array();

    if ($this->ca_item->primary_image) {
      foreach ($this->ca_item->primary_image as $image_version => $image_data) {
        if (!empty($image_data)) {
          // Assign primary image attributes
          $version_key = 'primary_image-' . $image_version;
          $this->ca_item->$version_key = $image_data['url'];
          $version_key_path = 'primary_image-' . $image_version . '-path';
          $this->ca_item->$version_key_path = $image_data['path'];
          $hash_key_path = 'primary_image-' . $image_version . '-md5';
          $this->ca_item->$hash_key_path = $image_data['info']['MD5'];
          $full_key_path = 'primary_image-' . $image_version . '-full';
          $this->ca_item->$full_key_path = $image_data;

          // Fill the "all images" arrays as well
          $all_images_url['all_images-' . $image_version][] = $image_data['url'];
          $all_images_path['all_images-' . $image_version . '-path'][] = $image_data['path'];
          $all_images_md5['all_images-' . $image_version . '-md5'][] = $image_data['info']['MD5'];
          $all_images_full['all_images-' . $image_version . '-full'][] = $image_data;
        }
      }

      $more_images = $this->ca_item->more_images;
      if ($more_images) {
        foreach ($more_images as $image) {
          foreach ($image as $image_version => $image_data) {
            if (!empty($image_data)) {
              $all_images_url['all_images-' . $image_version][] = $image_data['url'];
              $all_images_path['all_images-' . $image_version . '-path'][] = $image_data['path'];
              $all_images_md5['all_images-' . $image_version . '-md5'][] = $image_data['info']['MD5'];
              $all_images_full['all_images-' . $image_version . '-full'][] = $image_data;
            }
          }
        }
      }

      // Now assign the "all images" arrays as an attribute
      if ($all_images_url) {
        foreach ($all_images_url as $attribute => $attribute_values) {
          $this->ca_item->{$attribute} = $attribute_values;
        }
      }
      if ($all_images_path) {
        foreach ($all_images_path as $attribute => $attribute_values) {
          $this->ca_item->{$attribute} = $attribute_values;
        }
      }
      if ($all_images_md5) {
        foreach ($all_images_md5 as $attribute => $attribute_values) {
          $this->ca_item->{$attribute} = $attribute_values;
        }
      }
      if ($all_images_full) {
        foreach ($all_images_full as $attribute => $attribute_values) {
          $this->ca_item->{$attribute} = $attribute_values;
        }
      }
    }
  }

  /**
   * Helper method
   */
  protected function parse_relations() {
    $ca_instance = $this->ca_item->getCAInstance();
    $all_attributes = $this->ca_item->getAttributes();
    $locales = $ca_instance->getCataloguingLocalesList(TRUE);

    if ($all_attributes) {
      foreach ($all_attributes as $attribute) {
        if (substr($attribute, 0, 11) == "related_ca_") {
          $data = $this->ca_item->{$attribute};
          $relations = array();
          if ($data) {
            $num = count($data);
            if ($num == 1) { // single value
              $relation = array_shift($data);
              $relation_type = $relation['item_type_id'];
              $labels = $relation['labels'];

              // Add localized fields: related_ca_<type>-<locale>, related_ca_<type>_<typeid>-<locale>
              foreach ($locales as $key => $locale) {
                $locale_key = $attribute . '-' . $key;
                $locale_type_key = $attribute . '_' . $relation_type . '-' . $key;

                $localeid = $locale['locale_id'];
                $label_value = (isset($labels[$localeid])) ? $labels[$localeid] : $relation['label'];
                $this->ca_item->{$locale_key} = $label_value;
                $this->ca_item->{$locale_type_key} = $label_value;
              }

              // Add fields: related_ca_<type>, related_ca_<type>_<typeid>
              $type_key = $attribute . '_' . $relation_type;
              $this->ca_item->{$type_key} = $relation['label'];
              $this->ca_item->{$attribute} = $relation['label'];

              $this->parse_relation_objects($relation, $locales);
            }
            else { // multi value
              foreach ($data as $relation) {
                $relation_type = $relation['item_type_id'];
                $labels = $relation['labels'];

                // Add localized fields: related_ca_<type>-<locale>, related_ca_<type>_<typeid>-<locale>
                foreach ($locales as $key => $locale) {
                  $locale_key = $attribute . '-' . $key;
                  $locale_type_key = $attribute . '_' . $relation_type . '-' . $key;

                  $localeid = $locale['locale_id'];
                  $label_value = (isset($labels[$localeid])) ? $labels[$localeid] : $relation['label'];
                  $relations[$locale_key][] = $label_value;
                  $relations[$locale_type_key][] = $label_value;
                }
                // Add fields: related_ca_<type>, related_ca_<type>_<typeid>
                $type_key = $attribute . '_' . $relation_type;
                $relations[$type_key][] = $relation['label'];
                $relations[$attribute][] = $relation['label'];

                $this->parse_relation_objects($relation, $locales);
              }
              if ($relations) {
                foreach ($relations as $attribute_key => $attribute_value) {
                  $this->ca_item->$attribute_key = $attribute_value;
                }
              }
            }
          }
        }
      }
    }
  }

  /**
   * Parse full relation objects
   */
  protected function parse_relation_objects($relation, $locales) {
    if (isset($this->config['collectiveaccess_feeds_parser_get_related']) && !empty($this->config['collectiveaccess_feeds_parser_get_related'])) {
      $types_to_fetch = array_filter($this->config['collectiveaccess_feeds_parser_get_related']);
      $type_ids = collectiveaccess_get_type_ids();
      $ca_instance = $this->ca_item->getCAInstance();
      $locales = $ca_instance->getCataloguingLocalesList(TRUE);
      $parsers = collectiveaccess_feeds_get_item_parsers();
      $related_data = array();

      // A bit of a hack, but we check the existence of a specific ID, to find out
      // which type of item we are dealing with
      foreach ($type_ids as $type_id => $type) {
        if (array_key_exists($type_id, $relation)) {
          // check if we are supposed to fully fetch objects of this type (e.g. ca_entities)
          if (in_array($type['key'], $types_to_fetch)) {
            // Create new item of the related type, e.g. CollectiveAccessEntity
            $class = $type['class'];
            $item = new $class($ca_instance, $relation[$type_id]);
            // Create a new parser for the item, e.g. CollectiveAccessEntityParser
            $item_parser_class = $parsers[$type['key']];
            if (class_exists($item_parser_class)) {
              $item_parser = new $item_parser_class($item);
              // get the parsed item fields
              $item_data = $item_parser->getParsedObjectData();
              if ($item_data) {
                foreach ($item_data as $rel_key => $rel_attrib) {
                  $related_attrib_key = 'related_' . $type['key'] . ':' . $rel_key;
                  if (is_array($rel_attrib) && count($rel_attrib) == 1) {
                    $related_data[$related_attrib_key] = array_shift($rel_attrib);
                  }
                  else {
                    $related_data[$related_attrib_key] = $rel_attrib;
                  }
                }
              }
            }
          }
        }
      }

      // now assign the gathered related data to this object
      if ($related_data) {
        foreach ($related_data as $reldata_key => $reldata_val) {
          $this->ca_item->{$reldata_key} = $reldata_val;
        }
      }
    }
  }

  /**
   * Parse set information
   * @TODO: add access control
   */
  protected function parse_sets() {
    $ca_instance = $this->ca_item->getCAInstance();
    $locales = $ca_instance->getCataloguingLocalesList(TRUE);
    $sets = $this->ca_item->sets;
    if ($sets) {
      $locale_set_names = array();
      $set_codes = array();

      foreach ($sets as $set_id => $set_locales) {
        // 1. Try to get set information for each locale that's available
        foreach ($locales as $key => $locale) {
          $localeid = $locale['locale_id'];
          if (isset($set_locales[$localeid])) {
            $setinfo = $set_locales[$localeid];
            $locale_set_names[$key][] = $setinfo['name'];
          }
        }

        // 2. get set codes
        foreach ($set_locales as $entry) {
          if (!in_array($entry['set_code'], $set_codes)) {
            $set_codes[] = $entry['set_code'];
          }
        }
      }

      // Now add the info to the CollectiveAccessObject
      // 1. as sets-<locale>
      foreach ($locale_set_names as $language => $values) {
        $locale_set_key = 'sets-' . $language;
        if (count($values) == 1) {
          $this->ca_item->{$locale_set_key} = array_shift($values);
        }
        else {
          $this->ca_item->{$locale_set_key} = $values;
        }
      }
      // 2. as sets-code
      $k = 'sets-code';
      if (count($set_codes) == 1) {
        $this->ca_item->{$k} = array_shift($set_codes);
      }
      else {
        $this->ca_item->{$k} = $set_codes;
      }
    }
  }
}