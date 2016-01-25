<?php

/**
 * @file
 *
 * Defines a class for NPRML creation/transmission and retreival/parsing
 * Unlike NPRAPI class, NPRAPIDrupal is drupal-specific
 */

class NPRAPIDrupal extends NPRAPI {

  /**
   * Makes HTTP request to NPR API.
   *
   * @param array $params
   *   Key/value pairs to be sent (within the request's query string).
   *
   * @param string $method
   *   The HTTP method (e.g., GET, POST) to be used.
   *
   * @param string $data
   *   A string containing the request body.
   *
   * @param string $path
   *   The path part of the request URL (i.e., http://example.com/PATH).
   *
   * @param string $base
   *   The base URL of the request (i.e., HTTP://EXAMPLE.COM/path) with no trailing slash.
   */
  function request($params = array(), $method = 'GET', $data = NULL, $path = 'query') {
    $base = variable_get('npr_pull_api_url', self::NPRAPI_PULL_URL);

    $this->request->method = $method;
    $this->request->params = $params;
    $this->request->data = $data;
    $this->request->path = $path;
    $this->request->base = $base;

    $queries = array();
    foreach ($this->request->params as $k => $v) {
      $queries[] = "$k=$v";
    }
    $request_url = $this->request->base . '/' . $this->request->path . '?' . implode('&', $queries);
    $this->request->request_url = $request_url;

    $response = drupal_http_request($request_url, array('method' => $this->request->method, 'data' => $this->request->data));
    $this->response = $response;

    if ($response->code == self::NPRAPI_STATUS_OK) {
      if ($response->data) {
        $this->xml = $response->data;
      }
      else {
        $this->notice[] = t('No data available.');
      }
    }
  }

  function flatten() {
    foreach($this->stories as $i => $story) {
      foreach($story->parent as $parent) {
        if ($parent->type == 'tag') {
          $this->stories[$i]->tags[] = $parent->title->value;
        }
      }
    }
  }

  /**
   * Create NPRML from drupal node.
   *
   * @param object $node
   *   A drupal node.
   *
   * @return string
   *   An NPRML string.
   */
  function create_NPRML($node) {
    $language = $node->language;
    $xml = new DOMDocument();
    $xml->version = '1.0';
    $xml->encoding = 'UTF-8';

    //$xml->add_element($xml, 'nprml', array('version' => self::NPRML_VERSION), NULL,);
    $nprml_element = $xml->createElement('nprml');
    $nprml_version = $xml->createAttribute('version');
    $nprml_version-> value = self::NPRML_VERSION;
    $nprml_element->appendChild($nprml_version);

    $nprml = $xml->appendChild($nprml_element);
    $list = $nprml->appendChild($xml->createElement('list'));

    $story = $xml->createElement('story');

    // if node has been flagged to send to NPR One, add this specific parent
    // element
    if (!empty($node->npr_push_npr_one_send)) {
      $npr_one = $xml->createElement('parent');
      $npr_one->setAttribute('id', '319418027');
      $npr_one->setAttribute('type', 'collection');
      $story->appendChild($npr_one);
    }

    //if the nprID field is set, (probably because this is an update) send that along too
    if (!empty($node->field_npr_id[$node->language][0]['value'])) {
      $id_attribute = $xml->createAttribute('id');
      $id_attribute->value = $node->field_npr_id[$node->language][0]['value'];
      $story->appendChild($id_attribute);
    }

    $title = substr(($node->title), 0, 100);
    $title_cdata = $xml->createCDATASection($title);
    $title_element = $xml->createElement('title');
    $title_element->appendChild($title_cdata);
    $story->appendChild($title_element);

    if (!empty($node->body[$language][0]['value'])) {
      $body = $node->body[$language][0]['value'];

      $body_cdata = $xml->createCDATASection($body);
      $text = $xml->createElement('text');
      $text->appendChild($body_cdata);
      $story->appendChild($text);

      $teaser_cdata = $xml->createCDATASection(text_summary($body));
      $teaser = $xml->createElement('teaser');
      $teaser->appendChild($teaser_cdata);
      $story->appendChild($teaser);
    }

    $now = format_date(REQUEST_TIME, 'custom', "D, d M Y G:i:s O ");
    $story_date = ($node->changed == $node->created) ? $now : format_date($node->created, 'custom', "D, d M Y G:i:s O ") ;
    $story->appendChild($xml->createElement('storyDate', $story_date));
    $story->appendChild($xml->createElement('pubDate', $now));

    $url = url(drupal_get_path_alias('node/' . $node->nid), array('absolute' => TRUE));
    $url_cdata = $xml->createCDATASection($url);
    $url_type = $xml->createAttribute('type');
    $url_type->value = 'html';
    $url_element = $xml->createElement('link');
    $url_element->appendChild($url_cdata);
    $url_element->appendChild($url_type);
    $story->appendChild($url_element);

    //add the station's org ID
    $org_element = $xml->createElement('organization');
    $org_id = $xml->createAttribute('orgId');
    $org_id->value = variable_get('npr_push_org_id');
    $org_element->appendChild($org_id);
    $story->appendChild($org_element);

    //partner id
    $partner_id = $xml->createElement('partnerId', $node->nid);
    $story->appendChild($partner_id);

    $type = $node->type;
    $nprml_fields = npr_api_get_nprml_fields();
    $map = variable_get('npr_push_field_map_' . $type, array());
    foreach ($map as $custom_field => $npr_field) {
      if ($npr_field) {
        $field = field_get_items('node', $node, $custom_field);
        foreach ($field as $k => $v) {
          $element = NULL;
          $value = !empty($field[$k]['value']) ? $field[$k]['value'] : NULL;

          if ($nprml_fields[$npr_field]['type'] == 'text') {
            //cdata this
            $element = $xml->createElement($npr_field, $value);
          }
          if ($nprml_fields[$npr_field]['type'] == 'attribute' && $value) {
            $element = $xml->createAttribute($npr_field);
            $element->value = $value;
          }
          if ($nprml_fields[$npr_field]['type'] == 'image') {
            $element = $xml->createElement($npr_field);
            if (!empty($image_primary_set)) {
              $image_type = $xml->createAttribute('type');
              $image_type->value = 'primary';
              $element->appendChild($image_type);
              $image_primary_set = TRUE;
            }
            $image_file = file_load($field[$k]['fid']);
            $image_url = file_create_url($image_file->uri);
            $src = $xml->createAttribute('src');
            $src->value = $image_url;
            $element->appendChild($src);
          }
          if ($nprml_fields[$npr_field]['type'] == 'audio') {
            $element = $xml->createElement($npr_field);
            if (!empty($audio_primary_set)) {
              $audio_type = $xml->createAttribute('type');
              $audio_type->value = 'primary';
              $element->appendChild($audio_type);
              $audio_primary_set = TRUE;
            }
            $title = $xml->createElement('title', $v['title']);
            $element->appendChild($title);

            $duration = $xml->createElement('duration', $v['duration']);
            $element->appendChild($duration);

            $description = $xml->createElement('description', $v['description']);
            $element->appendChild($description);

            $format = $xml->createElement('format');
            $mp3 = $xml->createElement('mp3', $v['mp3']);
            $mp3type = $xml->createAttribute('type');
            $mp3type->value = 'm3u';
            $mp3->appendChild($mp3type);
            $format->appendChild($mp3);

            $mediastream = $xml->createElement('mediastream', $v['mediastream']);
            $format->appendChild($mediastream);

            $wm = $xml->createElement('wm', $v['wm']);
            $format->appendChild($wm);



            $element->appendChild($format);

            $permissions = $xml->createElement('permissions');

            $download = $xml->createElement('download');
            $download_allow = $xml->createAttribute('allow');
            $download_allow->value = $v['download'] ? 'true' : 'false';
            $download->appendChild($download_allow);
            $permissions->appendChild($download);

            $stream = $xml->createElement('stream');
            $stream_allow = $xml->createAttribute('stream');
            $stream_allow->value = $v['stream'] ? 'true' : 'false';
            $stream->appendChild($stream_allow);
            $permissions->appendChild($stream);

            $embed = $xml->createElement('embed');
            $embed_allow = $xml->createAttribute('allow');
            $embed_allow->value = $v['embed'] ? 'true' : 'false';
            $embed->appendChild($embed_allow);
            $permissions->appendChild($embed);

            $element->appendChild($permissions);

          }
          if (is_object($element)) {
            $story->appendChild($element);
          }
        }
      }
    }

    $list->appendChild($story);
    return $xml->saveXML();
  }

  /**
   * Takes node, creates NPRML, sends it to NPR API.
   *
   * @param object $node
   *   A drupal node.
   */
  function push_NPRML($node) {
    $org_id = variable_get('npr_push_org_id');
    $api_key = variable_get('npr_api_api_key');
    $params = array(
      'orgId' => $org_id,
      'apiKey' => $api_key,   
    );
    $method = 'PUT';
    $base = variable_get('npr_push_api_url');
    $path = 'story';

    $data = $this->create_NPRML($node);
    $this->request($params, $method, $data, $path, $base);
  }

  /**
   * Delete a node from NPR API.
   *
   * @param string $npr_id
   *   An NPR API id.
   */
  function delete_node($npr_id) {
    $org_id = variable_get('npr_push_org_id');
    $api_key = variable_get('npr_api_api_key');
    $params = array(
      'orgId' => $org_id,
      'apiKey' => $api_key,
      'id' => $npr_id,
    );
    $method = 'DELETE';
    $base = variable_get('npr_push_api_url');
    $path = 'story';

    $this->request($params, $method, NULL, $path, $base);
  }
}
