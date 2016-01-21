<?php

/**
 * @file
 * Contains PMPAPIDrupalPull.
 */

/**
 * Turns PMP hypermedia docs in Drupal entities.
 */

class PMPAPIDrupalPull extends PMPAPIDrupal {

  /**
   * Initializes a PMPAPIDrupalPull object.
   */
  function __construct() {
    parent::__construct();
    $this->uid = variable_get('pmpapi_pull_pull_user', 1);
  }

  /**
   * Pulls a single doc from the PMP
   *
   * @param array $guid
   *   A PMP GUID
   *
   * @return object
   *   The full PMP object.
   */
  function pullDoc($guid) {
    $this->pull(array('guid' => $guid));
    if (empty($this->errors) && !empty($this->query->results->docs[0])) {
      $doc = $this->query->results->docs[0];
      $docs = array($doc);
      drupal_alter('pmpapi_pull_docs_prepare', $docs);
      return $this->saveEntity($doc);
    }
  }


  /**
   * Pulls docs from the PMP
   *
   * @param array $options
   *   PMP query parameters
   *
   * @return object
   *   The full PMP object.
   */
  function pullDocs($options, $context = NULL) {
    $this->pull($options);
    $docs = array();
    if (empty($this->errors) && !empty($this->query->results->docs)) {
      foreach($this->query->results->docs as $doc) {
        // Only save doc if it hasn't already been pulled (or pushed).
        // pmpapi_updates module will take care of updates
        if (!pmpapi_get_eid_from_guid($doc->attributes->guid)) {
          $doc->context = $context;
          $docs[] = $doc;
        }
      }
      if (!empty($docs)) {
        drupal_alter('pmpapi_pull_docs_prepare', $docs);
        foreach ($docs as $doc) {
          $this->saveEntity($doc);
        }
      }
    }
    else {
      drupal_set_message(t('No collection could be found in the PMP with this guid.'), 'warning');
    }
  }

 /**
  * Saves a PMP doc as an entity.
  *
  * @param $doc object
  *   A PMPAPIDrupal object
  */
  function saveEntity($doc) {
    $pieces = explode('/', $doc->links->profile[0]->href);
    $doc->profile = end($pieces);
    $mapped_entity = pmpapi_pull_find_mapped_entity($doc->profile);
    if ($mapped_entity) {
      $entity_type = $mapped_entity['entity_type'];
      $bundle_name = $mapped_entity['bundle_name'];
      $uname = $entity_type . '__' . $bundle_name;
      
      // is an update?
      $entity_id = pmpapi_get_eid_from_guid($doc->attributes->guid);
      if ($entity_id) {
        $entity = entity_load_single($entity_type, $entity_id);
      }
      
      // or a newly pulled doc?
      else {
        $init = array(
          'type' => $bundle_name,
          'uid' => $this->uid,
          'name' => format_username(user_load($this->uid)),
          'pmpapi_guid' => $doc->attributes->guid,
          'pmpapi_pull' => TRUE,
        );
        $entity = entity_create($entity_type, $init);
      }

      // Add created -- or timestamp, if a file (we presume)
      $published = strtotime($doc->attributes->published);
      if ($entity_type == 'node') {
        $entity->created = $published;
      }
      else {
        $entity->timestamp = $published;
      }

      // Add valid->from and valid->to
      $entity->pmpapi_valid_from = $doc->attributes->valid->from;
      $entity->pmpapi_valid_to = $doc->attributes->valid->to;

      if (!empty($doc->links->enclosure[0])) {
        $file = $this->createEnclosureFile($doc->links->enclosure[0], $doc->profile, $doc->attributes->guid);
        $entity = (object) array_merge((array) $file, (array) $entity);
      }
      if ($entity_type != 'file' && !pmpapi_doc_is_valid($doc)) {
        $entity->status = 0;
      }

      // This needs attention
      $entity->language = LANGUAGE_NONE;
      
      $map = variable_get('pmpapi_pull_mapping_' . $uname . '_' . $doc->profile);
      $entity_info = entity_get_info($entity_type);
      $label = !empty($entity_info['entity keys']['label']) ? $entity_info['entity keys']['label'] : NULL;
      $this->mapFields($entity, $doc, $map, $label);
      $this->addItems($entity, $doc, $map);

      // Fill up any unfilled fields
      foreach(field_info_instances($entity_type, $bundle_name) as $field_name => $field) {
        if (empty($entity->$field_name)) {
          $values = array();
          $raw_values = $this->getDefaultFieldValue($entity, $entity_type, $bundle_name,$field_name);
          foreach($raw_values as $raw_value) {
            $values[] = $raw_value;
          }
          drupal_alter('pmpapi_pull_default_values', $values, $field_name);
          $entity->{$field_name}[$entity->language] = $values;
        }
      }

      if (!empty($doc->context)) {
        $entity->pmpapi_pull_context = $doc->context;
      }

      entity_save($entity_type, $entity);
      if ($label) {
        $uri = entity_uri($entity_type, $entity);
        $link = l($entity->$label, $uri['path']);
        drupal_set_message(t('Go to') . ' ' . $link);
      }
      return $entity;
    }
    else {
      drupal_set_message(t('Unable to pull the doc with GUID = @guid, as the profile %profile has not been mapped to any entity.', array('@guid' => $doc->attributes->guid, '%profile' => $doc->profile)), 'warning');
    }
  }

  /**
   * Creates a file object from a PMP object.
   *
   * @param object $enclosure
   *   A PMP object enclosure
   *
   * @param string $profile
   *   The name of a PMP profile
   *
   * @param string $guid
   *   A PMP GUID
   *
   * @return
   *   A file object
   */
  function createEnclosureFile($enclosure, $profile, $guid) {
    $href = pmpapi_pull_get_enclosure_url($enclosure);
    $file = new stdClass;
    if (pmpapi_pull_make_local_files($profile)) {
      $file_mimetype = $enclosure->type;
      // NPR enclosures are M3U's. The call to pmpapi_pull_get_enclosure_url()
      // above should extract an MP3 URL, but we also need to also adjust the
      // enclosure->type (if it exists)
      if ($file_mimetype == 'audio/m3u') {
        $file_mimetype = 'audio/mpeg';
      }
      $ext = pmpapi_pull_get_ext_from_mimetype($file_mimetype);

      $filename = $guid . '.' . $ext;
      // Each (media) profile can have its own dedicated stream wrapper (no GUI
      // for this yet)
      $scheme = variable_get('pmpapi_stream_wrapper_' . $profile, file_default_scheme());
      $wrapper = file_stream_wrapper_get_instance_by_scheme($scheme);
      $path = $wrapper->getDirectoryPath();
      $subdir = method_exists($wrapper, 'getSubdirectory') ? $wrapper->getSubdirectory() : NULL;
      if ($subdir) {
        $dest = $path . '/' . $subdir . '/' . $filename;
      }
      else {
        $dest = $path . '/' . $filename;
      }

      $local_uri = system_retrieve_file($href, $dest, FALSE, FILE_EXISTS_REPLACE);
      $filesize = filesize($local_uri);
      if ($subdir) {
        $uri = $scheme . '://' . $subdir . '/' . $filename;
      }
      else {
        $uri = $scheme . '://' . $filename;
      }

      $file->filename = $filename;
      $file->filemime = $file_mimetype;
      $file->filesize = $filesize;
    }
    else {
      $uri = $href;
    }

    $file->uid = $this->uid;
    $file->uri = $uri;
    $file->status = 1;
    $file->pmpapi_guid = $guid;
    // Give valid->from and valid->to NULL values; the second entity save will
    // map any applicable values from the doc.
    $file->pmpapi_valid_from = NULL;
    $file->pmpapi_valid_to = NULL;
    $file->pmpapi_pull = TRUE; // to ensure it doesn't get pushed

    // Does file already exist?
    $files = entity_load('file', FALSE, array('uri' => $uri));
    if (!empty($files)) {
      $existing_file = array_shift($files);
      $file->fid = $existing_file->fid;
    }
    file_save($file);
    return $file;
  }
  
  /**
   * Maps PMP doc attributes to entity fields.
   *
   * @param object $entity
   *   Any drupal entity object
   * @param object $doc
   *   A PMP doc
   * @param array $map
   *   A mapping of PMP profile attributes (etc.) to entity fields
   */
  function mapFields($entity, $doc, $map, $label) {
    $info = pmpapi_get_profile_info($doc->profile);
    $pmp_label_field = ($label) ? array_search($label, $map) : NULL;

    foreach($map as $pmp_field => $local_field) {
      $pmp_info = $info[$pmp_field];
      $pmp_type = $pmp_info['type'];
      if (!empty($doc->attributes->$pmp_field) && $local_field !== '0' && $pmp_type != 'item') {
        if ($pmp_field == $pmp_label_field) {
          $this->mapLabel($entity, $doc, $label, $pmp_label_field);
        }
        else {
          $this->mapAttribute($entity, $doc, $local_field, $pmp_field, $pmp_type);
        }
      }
    }
  }

  /**
   * Maps a doc attribute to an entity's label.
   *
   * @param $entity object
   *   A Drupal entity.
   *
   * @param $doc object
   *   A PMP doc.
   *
   * @param $label string
   *   The name of the label of the entity.
   *
   * @param $pmp_label_field string
   *   The doc attribute (e.g. - title) that is being mapped to the entity
   *   label.
   */
  function mapLabel($entity, $doc, $label, $pmp_label_field) {
    // Since node and file titles have a hard DB limit of 255, we truncate here,
    // to prevent all sorts of issues.
    $entity->$label = substr($doc->attributes->$pmp_label_field, 0, 255);
  }

  /**
   * Maps a doc attribute to an entity.
   *
   * @param $entity object
   *   A Drupal entity.
   *
   * @param $doc object
   *   A PMP doc.
   *
   * @param $local_field string
   *   The name of the field to which the attribute is being mapped.
   *
   * @param $pmp_field string
   *   The name of the doc attribute that is being mapped.
   *
   * @param $pmp_type string
   *   The type of the doc attribute (e.g., array)
   */
  function mapAttribute($entity, $doc, $local_field, $pmp_field, $pmp_type) {
    $default_format = filter_default_format(user_load($this->uid));
    $local_field_info = field_info_field($local_field);

    if ($pmp_type == 'array') {
      if ($local_field_info['type'] == 'taxonomy_term_reference') {
        $tids = array();
        $vocab_name = $local_field_info['settings']['allowed_values'][0]['vocabulary'];
        $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
        $vid = $vocab->vid;

        foreach ($doc->attributes->$pmp_field as $element) {
          $terms = taxonomy_get_term_by_name($element, $vocab_name);
          if (!empty($terms)) {
            $term = array_shift($terms);
            $tids[] = array('tid' => $term->tid);
          }
          else {
            $term = new stdClass();
            $term->name = $element;
            $term->vid = $vid;
            $term->vocabulary_machine_name = $vocab->machine_name;
            taxonomy_term_save($term);
            $tids[] = array('tid' => $term->tid);
          }
        }
        $value = array($entity->language => $tids);
      }
      else {
        $value = implode(', ', $doc->$pmp_field);
      }
    }
    else {
      $values = array(
        'value' => $doc->attributes->$pmp_field,
        'format' => $default_format
      );
      $this->alterMappedValues($values, $local_field_info, $entity, $doc->profile);
      $value = array($entity->language => array(0 => $values));
    }
    $entity->$local_field = $value;
  }

  /**
   * Alter values just prior to mapping.
   *
   * @param $values array
   *   The values (values, format, timezone) that constitute the local field
   * @param $local_field_info array
   *   Info about the local field being mapped to.
   * @param $entity object
   *   The entity to which the doc is being mapped.
   * @param $profile string
   *   The kind of PMP doc being mapped.
   */
  function alterMappedValues(&$values, $local_field_info, $entity, $profile) {
    switch ($local_field_info['type']) {
      case 'date':
      case 'datetime':
      case 'datestamp':
        $mapped_entity = pmpapi_pull_find_mapped_entity($profile);
        $entity_type = $mapped_entity['entity_type'];
        $bundle_name = $mapped_entity['bundle_name'];
        $this->modifyDate($values, $local_field_info, $entity, $entity_type, $bundle_name);
      default:
    }
  }

  /**
   * Turn PMP date data into acceptable Drupal date field data
   *
   * @param $values
   *   The values (values, format, timezone) that constitute the local field
   * @param $local_field_info array
   *   Info about the local field being mapped to.
   * @param $entity
   *  The entity to which the doc is being mapped.
   * @param $entity_type string
   *   The type (node, file, etc.) of the entity
   * @param $bundle_name string
   *   The bundle (article, audio, etc.) of the entity
   */
  function modifyDate(&$values, $local_field_info, $entity, $entity_type, $bundle_name) {
    $instance_info = field_info_instance($entity_type, $local_field_info['field_name'], $bundle_name);
    $input_format = $instance_info['widget']['settings']['input_format'];
    $tz_handling = $local_field_info['settings']['tz_handling'];

    // Overly complex logic is intentional, as to spell out all five
    // timezone handler options
    switch ($tz_handling) {
      case 'site':
      case 'user':
      case 'utc':
      case 'none':
        $tz = 'UTC';
        break;
      case 'date':
        $tz = 'UTC';
        $values['timezone'] = 'UTC';
        break;
    }
    $ts = strtotime($values['value']);
    switch ($local_field_info['type']) {
      // Public field type = 'Date' (yes, this is confusing)
      // See https://www.drupal.org/node/1455576
      case 'datetime':
        $values['value'] = format_date($ts, 'custom', 'Y-m-d H:i:s', $tz);
        break;
      // Public field type = 'Date (ISO format)'
      case 'date':
        $values['value'] = format_date($ts, 'custom', 'Y-m-d\TH:i:s', $tz);
        break;
      case 'datestamp':
        $values['value'] = $ts;
        break;
    }
  }

  /**
   * Maps PMP doc items to entity fields.
   *
   * @param object $entity
   *   Any drupal entity object
   * @param object $doc
   *   A PMP doc
   * @param array $map
   *   A mapping of PMP profile attributes (etc.) to entity fields
   */
  function addItems($entity, $doc, $map) {
    if (!empty($doc->items) && (empty($doc->recurse) || !isset($doc->recurse))) {
      foreach($doc->items as $item) {
        $pieces = explode('/', $item->links->profile[0]->href);
        $item_profile = end($pieces);
        $local_field = !empty($map['item-' . $item_profile]) ? $map['item-' . $item_profile] : NULL;
        if ($local_field && ($item_profile == 'image' || $item_profile == 'audio' || $item_profile == 'video')) {
          $item_guid = $item->attributes->guid;
          $item_pmp = pmpapi_fetch($item_guid);
          if (is_object($item_pmp) && !empty($item_pmp->query->results->docs) && empty($item_pmp->errors)) {
            $item_doc = $item_pmp->query->results->docs[0];
            $item_doc->recurse = FALSE;
            $item_doc->pmpapi_pull = TRUE;
            $this->saveEntity($item_doc);
            $eid = pmpapi_get_eid_from_guid($item_guid);
            $local_field_info = field_info_field($local_field);
            $mapping = array(
              'image' => array('fid' => $eid, 'display' => 1),
              'file' => array('fid' => $eid, 'display' => 1),
              'entityreference' => array('target_id' => $eid),
            );
            // Attach entity reference
            if (!$this->reference_exists($entity, $local_field_info['type'], $local_field, $eid)) {
              $entity->{$local_field}[$entity->language][] = $mapping[$local_field_info['type']];
            }
          }
          else {
            drupal_set_message(t('No related doc could be found in the PMP with this guid.'), 'warning');
          }
        }
      }
    }
  }

  /**
   * Determines if an entity already references an entity id.
   *
   * @param $entity object
   *   A Drupal entity
   * @param $field_type string
   *   The type of reference field
   * @param $field string
   *   The name of the reference field
   * @param $eid
   *   The id of the entity being referenced
   * @return boolean
   *   TRUE if entity references the entity id, FALSE otherwise
   */
  function reference_exists($entity, $field_type, $field, $eid) {
    if (!empty($entity->{$field}[$entity->language])) {
      $key = ($field_type == 'entityreference') ? 'target_id' : 'fid';
      foreach($entity->{$field}[$entity->language] as $reference) {
        if (isset($reference[$key]) && $reference[$key] == $eid) {
          return TRUE;
        }
      }
    }
  }

  /**
   * Finds the default value of a given field instance.
   *
   * @param $entity object
   *   The Drupal entity to which the field is attached
   *
   * @param $entity_type string
   *   The type of the entity
   *
   * @param $bundle_name
   *   The type of bundle of the entity
   *
   * @param $field string
   *   The name of the field whose default value will be determined
   *
   * @return array
   *   The default value(s) of the field
   */
  function getDefaultFieldValue($entity, $entity_type, $bundle_name, $field) {
    $field_info = field_info_field($field);
    $instance = field_info_instance($entity_type, $field, $bundle_name);
    $value = field_get_default_value($entity_type, $entity, $field, $instance, $entity->language);
    return $value;
  }
}