<?php

/**
 * @file
 * Provides a CollectiveAccess specific FeedsParser.
 */

/**
 * A parser for incoming CollectiveAccess objects
 */
class FeedsCollectiveAccessParser extends FeedsParser {

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array(
      'collectiveaccess_representation_image_versions' => "thumbnail\r\nsmall\r\npreview\r\npreview170\r\nwidepreview\r\nmedium\r\nmediumlarge\r\nlarge",
      'collectiveaccess_feeds_parser_get_set_info' => TRUE,
      'collectiveaccess_feeds_parser_get_related' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form['collectiveaccess_representation_image_versions'] = array(
      '#type' => 'textarea',
      '#title' => t('Image representation versions'),
      '#description' => t('Specify which image representation versions should be fetched to be made available for imagefield mapping. See app/conf/media_processing.conf in CollectiveAccess for version names. <strong>Versions must be separated by comma, space or newline.</strong>'),
      '#required' => TRUE,
      '#default_value' => isset($this->config['collectiveaccess_representation_image_versions']) ? $this->config['collectiveaccess_representation_image_versions'] : NULL,
      '#rows' => 10,
    );

    $form['collectiveaccess_feeds_parser_get_set_info'] = array(
      '#type' => 'checkbox',
      '#title' => t('Retrieve information about Sets'),
      '#default_value' => isset($this->config['collectiveaccess_feeds_parser_get_set_info']) ? $this->config['collectiveaccess_feeds_parser_get_set_info'] : TRUE,
    );

    $form['collectiveaccess_feeds_parser_get_related'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Retrieve related content'),
      '#description' => t('Specify for which related types of content you also want to retrieve all fields. This can be resource intensive in case of many relations.'),
      '#required' => FALSE,
      '#multiple' => TRUE,
      '#options' => collectiveaccess_get_primary_types_relations_list(),
      '#default_value' => isset($this->config['collectiveaccess_feeds_parser_get_related']) ? array_filter($this->config['collectiveaccess_feeds_parser_get_related']) : array(),
    );
    return $form;
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $config = $this->getConfig();
    $form = array();
    $image_versions = isset($this->config['collectiveaccess_representation_image_versions']) ? $this->config['collectiveaccess_representation_image_versions'] : NULL;
    $set_info = isset($this->config['collectiveaccess_feeds_parser_get_set_info']) ? $this->config['collectiveaccess_feeds_parser_get_set_info'] : TRUE;
    $related = isset($this->config['collectiveaccess_feeds_parser_get_related']) ? $this->config['collectiveaccess_feeds_parser_get_related'] : array();

    $form['collectiveaccess_representation_image_versions'] = array(
      '#type' => 'value',
      '#value' => $image_versions,
    );

    $form['collectiveaccess_feeds_parser_get_set_info'] = array(
      '#type' => 'value',
      '#value' => $set_info,
    );

    $form['collectiveaccess_feeds_parser_get_related'] = array(
      '#type' => 'value',
      '#value' => $related,
    );

    return $form;
  }

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $source_config = $source->getConfigFor($this);
    $result = new FeedsParserResult();
    $ca_instance = collectiveaccess_instance_load($fetcher_result->get_ca_instance_name());
    $data = $fetcher_result->getRaw();
    $source->queued_items = $data;
    $objects = array();
    $options = array();

    $image_versions_data = $source_config['collectiveaccess_representation_image_versions'];
    $image_versions = preg_split("/[\s,]+/", $image_versions_data);
    if ($image_versions) {
      $options['object_representation_image_versions'] = $image_versions;
    }

    // Retrieve the raw CollectiveAccessObject items
    foreach ($data as $object_id) {
      $object = new CollectiveAccessObject($ca_instance, $object_id, $options);
      $this->getAdditionalData($object, $source_config, $ca_instance);
      $objects[] = $object;
    }

    // Provide ability for other modules to alter the raw objects that are going to be parsed
    drupal_alter('collectiveaccess_feeds_raw_objects', $objects);

    // Parse the CollectiveAccessObject items for mapping
    foreach ($objects as $id => $record) {
      $record = $this->parseCollectiveAccessRecord($record, $source_config);
      $result->items[] = $record;
    }
    // Provide ability for other modules to alter the feed items that are passed along to the Feeds Processor
    drupal_alter('collectiveaccess_feeds_items', $result->items, $fetcher_result);
    return $result;
  }

  /**
   * Provides additional parsing of CollectiveAccess records, to make them ready
   * for Feeds Processor consumption.
   */
  protected function parseCollectiveAccessRecord($record, $config = array()) {
    $parser = new CollectiveAccessObjectParser($record, $config);
    return $parser->getParsedObjectData();
  }

  /**
   * Attach additional data to the raw object, if required
   */
  protected function getAdditionalData(&$object, $source_config, $ca_instance) {
    // Add sets information
    if ($source_config['collectiveaccess_feeds_parser_get_set_info'] == TRUE) {
      $set_info = new CollectiveAccessSets($ca_instance);
      $object->sets = $set_info->getSetsForItem($object->object_id);
    }

    //// Add related items
    //$typeinfo = collectiveaccess_get_types();
    //if ($related = $source_config['collectiveaccess_feeds_parser_get_related']) {
    //  $value = array();
    //  foreach ($related as $item_type) {
    //    $type = $typeinfo[$item_type];
    //    $class = $type['class'];
    //    if (class_exists($class)) {
    //      $instance = new $class($ca_instance, $id);
    //    }
    //  }
    //}
  }

  /**
   * Implements FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    $attributelist = array();
    $feeds_config = feeds_importer($this->id)->config;
    $instance = NULL;
    if (isset($feeds_config['fetcher']['config']['collectiveaccess_instance'])) {
      $instance = $feeds_config['fetcher']['config']['collectiveaccess_instance'];
    }

    if (isset($feeds_config['parser']['config']['collectiveaccess_representation_image_versions'])) {
      $image_versions_data = $feeds_config['parser']['config']['collectiveaccess_representation_image_versions'];
      $image_versions = preg_split("/[\s,]+/", $image_versions_data);
    }

    $get_sets = FALSE;
    if (isset($feeds_config['parser']['config']['collectiveaccess_feeds_parser_get_set_info']) && $feeds_config['parser']['config']['collectiveaccess_feeds_parser_get_set_info'] == TRUE) {
      $get_sets = TRUE;
    }

    $related_types = array();
    if (isset($feeds_config['parser']['config']['collectiveaccess_feeds_parser_get_related'])) {
      $related_types = array_filter($feeds_config['parser']['config']['collectiveaccess_feeds_parser_get_related']);
    }

    if ($instance) {
      $ca_instance = collectiveaccess_instance_load($instance);
      $ca_object_builder = new CollectiveAccessObjectBuilder($ca_instance);
      $locales = $ca_instance->getCataloguingLocalesList();

      // Basic attributes
      $attributelist['idno'] = array(
        'name' => 'idno',
        'description' => t('Object identifier'),
      );
      $attributelist['idno_sort'] = array(
        'name' => 'idno_sort',
        'description' => t('Object identifier (sort)'),
      );
      $attributelist['object_id'] = array(
        'name' => 'object_id',
        'description' => t('object_id (internal)'),
      );

      $other_basic_attributes = array('parent_id', 'hier_object_id', 'lot_id', 'locale_id', 'source_id', 'type_id', 'item_status_id', 'acquisition_type_id', 'source_info', 'extent', 'extent_units', 'access', 'status', 'deleted', 'rank');
      foreach ($other_basic_attributes as $basic_attrib) {
        $attributelist[$basic_attrib] = array(
          'name' => $basic_attrib,
          'description' => $basic_attrib,
        );
      }

      if (is_array($locales)) {
        foreach ($locales as $localekey => $locale) {
          $attributelist['title-' . $localekey] = array(
            'name' => 'title-' . $localekey,
            'description' => t('Object title in @language', array('@language' => $locale)),
          );
        }
      }
      else {
        $attributelist['title'] = array(
          'name' => 'title',
          'description' => t('Object title'),
        );
      }

      // User defined attributes
      // @TODO: make parameter dependent on selected type of content
      if ($attributes = $ca_instance->getAvailableAttributes('ca_objects')) {
        foreach ($attributes as $attribute_name => $attribute_type) {
          $attributelist[$attribute_name] = array(
            'name' => $attribute_name,
            'description' => '',
          );
          // Add locale-specific attributes
          if (is_array($locales)) {
            foreach ($locales as $localekey => $locale) {
              $locale_attribute_name = $attribute_name . '-' . $localekey;
              $attributelist[$locale_attribute_name] = array(
                'name' => $locale_attribute_name,
                'description' => '',
              );
            }
          }
        }
      }

      // Object representation image versions
      if ($image_versions) {
        foreach ($image_versions as $image_version) {

          // URL value (primary)
          $version_key = 'primary_image-' . $image_version;
          $attributelist[$version_key] = array(
            'name' => $version_key,
            'description' => t('Object representation primary image - image version: @version', array('@version' => $image_version)),
          );
          $attributelist['all_images-' . $image_version] = array(
            'name' => 'all_images-' . $image_version,
            'description' => t('Object representation all images - image version: @version', array('@version' => $image_version)),
          );

          // PATH value (internal) (primary)
          $version_key_path = 'primary_image-' . $image_version . '-path';
          $attributelist[$version_key_path] = array(
            'name' => $version_key_path,
            'description' => t('Object representation primary image path - image version: @version', array('@version' => $image_version)),
          );
          $attributelist['all_images-' . $image_version . '-path'] = array(
            'name' => 'all_images-' . $image_version . '-path',
            'description' => t('Object representation all image paths - image version: @version', array('@version' => $image_version)),
          );

          // MD5 HASH value (primary)
          $version_key_hash = 'primary_image-' . $image_version . '-md5';
          $attributelist[$version_key_hash] = array(
            'name' => $version_key_hash,
            'description' => t('MD5 hash for image - image version: @version', array('@version' => $image_version)),
          );
          $attributelist['all_images-' . $image_version . '-md5'] = array(
            'name' => 'all_images-' . $image_version . '-md5',
            'description' => t('MD5 hashes for all images - image version: @version', array('@version' => $image_version)),
          );


          // Full image array (primary)
          $version_key_full = 'primary_image-' . $image_version . '-full';
          $attributelist[$version_key_full] = array(
            'name' => $version_key_full,
            'description' => t('Full information array for image - image version: @version', array('@version' => $image_version)),
          );
          $attributelist['all_images-' . $image_version . '-full'] = array(
            'name' => 'all_images-' . $image_version . '-full',
            'description' => t('Full information array for all images - image version: @version', array('@version' => $image_version)),
          );

        }
      }

      // Sets
      if ($get_sets) {
        if (is_array($locales)) {
          foreach ($locales as $localekey => $locale) {
            $attributelist['sets-' . $localekey] = array(
              'name' => 'sets-' . $localekey,
              'description' => t('Sets in @language', array('@language' => $locale)),
            );
          }
          $attributelist['sets-code'] = array(
            'name' => 'sets-code',
            'description' => t('Set codes'),
          );
        }
      }

      // Relations (basic - just name)
      $relationtypes = array('related_objects', 'related_ca_entities', 'related_ca_collections', 'related_ca_occurrences', 'related_ca_places', 'related_ca_object_lots', 'related_ca_storage_locations');
      foreach ($relationtypes as $relation) {
        if (is_array($locales)) {
          foreach ($locales as $localekey => $locale) {
            $attributelist[$relation . '-' . $localekey] = array(
              'name' => $relation . '-' . $localekey,
              'description' => t('Relation in @language', array('@language' => $locale)),
            );
          }
        }
        $attributelist[$relation] = array(
          'name' => $relation,
          'description' => t('Relation'),
        );
      }

      // Relations (full - all fields)
      foreach ($related_types as $related_type) {
        $attributes = $ca_instance->getAvailableAttributes($related_type);
        foreach ($attributes as $attribute_name => $attribute_type) {
          $attributelist['related_' . $related_type . ':' . $attribute_name] = array(
            'name' => 'related_' . $related_type . ':' . $attribute_name,
            'description' => '',
          );
          $attributelist['related_' . $related_type . ':title'] = array(
            'name' => 'related_' . $related_type . ':title',
            'description' => '',
          );
          // Add locale-specific attributes
          if (is_array($locales)) {
            foreach ($locales as $localekey => $locale) {
              $locale_attribute_name = 'related_' . $related_type . ':' . $attribute_name . '-' . $localekey;
              $attributelist[$locale_attribute_name] = array(
                'name' => $locale_attribute_name,
                'description' => '',
              );
              $attributelist['related_' . $related_type . ':title' . '-' . $localekey] = array(
                'name' => 'related_' . $related_type . ':title' . '-' . $localekey,
                'description' => '',
              );
            }
          }
        }
      }

      // Allow other modules to add extra mapping fields if neccessary
      drupal_alter('collectiveaccess_feeds_mapping_sources', $attributelist, $ca_instance, $ca_object_builder);

      if (is_array($attributelist)) {
        return $attributelist + parent::getMappingSources();
      }
    }
    return parent::getMappingSources();
  }
}