<?php

/**
 * @file
 * Contains PMPAPIDrupalPush.
 */

/**
 * Creates hypermedia docs and pushes entities.
 */

class PMPAPIDrupalPush extends PMPAPIDrupal {

  /**
   * Pushes entity to PMP
   *
   * @param object $entity
   *   Any drupal entity object
   * @param string $type
   *   The type of entity (node, file, etc.)
   * @param $profile string
   *   The type of PMP profile that will be created
   * @param array $mapping
   *   A mapping of entity fields to PMP profile attributes (etc.)
   *
   * @return object
   *   The full PMP object (after ->push()).
   */

  function pushEntity($entity, $type, $profile, $mapping) {
    $data = $this->createHypermediaDoc($entity, $type, $profile, $mapping);
    $pmp = $this->push($data);
    if ($pmp) {
      $pmp->pmpapi_guid = $entity->pmpapi_guid;
      return $pmp;
    }
  }

  /**
   * Creates a hypermedia doc to be sent to the PMP.
   *
   * @param object $entity
   *   Any drupal entity object
   * @param string $type
   *   The type of entity (node, file, etc.)
   * @param $profile string
   *   The type of PMP profile that will be created
   * @param array $mapping
   *   A mapping of entity fields to PMP profile attributes (etc.)
   *
   * @return object
   *   A PMP doc (though NOT a CollectionDocJson object).
   */
  function createHypermediaDoc($entity, $type, $profile, $mapping) {
    global $base_url;
    $lang = !empty($entity->language) ? $entity->language : LANGUAGE_NONE;

    $wrapper = entity_metadata_wrapper($type, $entity);
    $bundle_name = $wrapper->getBundle();
    
    $doc = new stdClass();
    $doc->attributes = new stdClass();
    $doc->version = '1.0';
    $doc->attributes->hreflang = 'en';
    $profile_obj = new stdClass();
    $profile_obj->href = $this->base . '/profiles/' . $profile;
    $doc->links->profile[] = $profile_obj;

    $alt_url = new stdClass();
    $entity_uri = entity_uri($type, $entity);
    $entity_uri['options']['absolute'] = TRUE;
    $alt_url->href = url($entity_uri['path'], $entity_uri['options']);
    $doc->links->alternate[] = $alt_url;

    $media_profiles = array('audio', 'image', 'video');
    if (in_array($profile, $media_profiles)) {
      // crops?
      $enclosure = new stdClass();
      $enclosure->href = file_create_url($entity->uri);
      if ($profile == 'image') {
        $meta = new stdClass();
        $meta->crop = 'primary';
        if (!empty($entity->metadata)) {
          $meta->height = $entity->metadata['height'];
          $meta->width = $entity->metadata['width'];
        }
        elseif (!empty($entity->image_dimensions)) {
          $meta->height = $entity->image_dimensions['height'];
          $meta->width = $entity->image_dimensions['width'];
        }
        $enclosure->meta = $meta;
        $enclosure->type = $entity->filemime;
      }
      drupal_alter('pmpapi_push_enclosure', $enclosure, $entity, $type);
      $doc->links->enclosure = array($enclosure);
    }

    $doc->attributes->guid = $entity->pmpapi_guid;
    
    // get local field that's mapped to title
    $title_field = array_search('title', $mapping);
    $doc->attributes->title = !empty($entity->$title_field) ? $entity->$title_field : $entity->pmpapi_guid;

    // Set published = created OR timestamp OR REQUEST_TIME, but can still be
    // overridden by mapping. This doesn't make sense if, say, a node has
    // status = 0. But if status = 0, it should never get here; instead, it
    // should be deleted from API
    if (!empty($entity->created)) {
      $published = $entity->created;
    }
    elseif (!empty($entity->timestamp)) {
      $published = $entity->timestamp;
    }
    else {
      $published = '';
    }
    $doc->attributes->published = $this->date($published);

    // Set byline to the public-facing name of the (Drupal) user who created the
    // entity. This attribute can always be overridden through push mapping.
    $account = user_load($entity->uid);
    $doc->attributes->byline = format_username($account);

    // Add permissions
    if (!empty($entity->pmpapi_permissions)) {
      $doc->links->permission = $entity->pmpapi_permissions;
    }
    $pmp_info = pmpapi_get_profile_info($profile);
    foreach($mapping as $local_field => $pmp_field) {
      if ($pmp_field && isset($entity->{$local_field}[$lang][0]) && $local_field != $title_field) {
        $info = field_info_field($local_field);
        $field_type = $info['type'];

        // item
        if (stripos($pmp_field, 'item-') === 0) {
          if ($field_type == 'entityreference') {
            // Get target entity type (e.g., node)
            $item_entity_type = $info['settings']['target_type'];
            $item_key = 'target_id';
          }
          // Assume a file
          if ($field_type == 'image' || $field_type == 'file') {
            $item_entity_type = 'file';
            $item_key = 'fid';
          }

          $item_ids = array();
          foreach ($entity->{$local_field}[$lang] as $ref) {
            $item_ids[] = $ref[$item_key];
          }

          foreach (entity_load($item_entity_type, $item_ids) as $item_entity) {
            if (!empty($item_entity->pmpapi_guid)) {
              $item = new stdClass();
              $item->href = variable_get('pmpapi_base_url') . '/docs/' . $item_entity->pmpapi_guid;
              $doc->links->item[] = $item;
            }
            else {
              $item_wrapper = entity_metadata_wrapper($item_entity_type, $item_entity);
              $label = $item_wrapper->label();
              $uri = entity_uri($item_entity_type, $item_entity);
              $link = l($label, $uri['path'], $uri['options']);
              $message = t('The @item_entity_type !link was attached to this @type, but has not been pushed to the PMP API. If you would like this association to appear in the API, you must first re-save !link, and then re-save this @type.', array('@item_entity_type' => $item_entity_type, '@type' => $type, '!link' => $link));
              drupal_set_message($message, 'warning');
            }
          }
        }
        else {
          if ($pmp_field == 'tags') {
            if ($field_type == 'taxonomy_term_reference') {
              foreach($entity->{$local_field}[$lang] as $tag_array) {
                $tid = $tag_array['tid'];
                $term = taxonomy_term_load($tid);
                $doc->attributes->{$pmp_field}[] = $term->name;
              }
            }
            else {
              // A tag, but not taxo reference
              $doc->attributes->{$pmp_field}[] = $entity->{$local_field}[$lang][0]['value'];
            }
          }
          else {
            // Not a doc's item, nor a tag
            $text = $entity->{$local_field}[$lang][0]['value'];
            // dates need to be converted to ISO-8601
            if ($pmp_info[$pmp_field]['type'] == 'datetime') {
              if ($field_type !== 'datestamp') {
                // 'Regular' date field gives you a UTC time (with no internal
                // offset). So we have to add that.
                $dt = new DateTime($text, new DateTimeZone('UTC'));
                // This looks redundant, but it isn't. We convert to ISO_8601
                // not because it's what PMP uses, but because it's a nice
                // format with internal offsets.
                $text = $dt->format(DATE_ISO8601);

                // Convert value to timestamp (unless it already is one)
                $text = strtotime($text);
              }
              $text = $this->date($text);
            }

            if (!empty($entity->{$local_field}[$lang][0]['format'])) {
              // Run through format, if field provides one
              $format_id = $entity->{$local_field}[$lang][0]['format'];
              $doc->attributes->$pmp_field = check_markup($text, $format_id, $lang);
            }
            else {
              $doc->attributes->$pmp_field = $text;
            }
          }
        }
      }
    }
    return $doc;
  }
}