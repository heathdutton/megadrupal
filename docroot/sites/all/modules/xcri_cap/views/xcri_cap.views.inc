<?php

/**
 * Implements hook_views_plugins().
 */
function xcri_cap_views_plugins() {
  $plugins = array(
    // display, style, row, argument default, argument validator and access.
    'display' => array(
      'xcri-cap' => array(
        'title' => t('XCRI-CAP'),
        'help' => t('Display the view as an xcri-cap feed.'),
        'handler' => 'xcri_cap_plugin_display_xcri_cap',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('XCRI-CAP'),
      ),
    ),
    'style' => array(
      'xcri-cap' => array(
        'title' => t('XCRI-CAP Feed'),
        'help' => t('Generates an XCRI-CAP feed from a view.'),
        'handler' => 'xcri_cap_plugin_style_xcri_cap',
        'theme' => 'xcri_cap_feed',
        'theme path' => drupal_get_path('module', 'xcri_cap') . '/views',
        'uses row plugin' => FALSE,
        'uses row class' => FALSE,
        'uses options' => TRUE,
        'uses fields' => FALSE,
        'type' => 'xcri-cap',
      ),
    ),
  );

  return $plugins;
}

function xcri_cap_feed_url($url) {
  $url = trim($url, " \t\n\r\0\x0B\/");
  $parsed = parse_url($url);
  $built = '';

  if (!isset($parsed['scheme']) || $parsed['scheme'] == 'http' || $parsed['scheme'] == 'https') {

    if (isset($parsed['scheme'])) {
      $built .= $parsed['scheme'] . '://';
    }

    if (isset($parsed['user'])) {
      $built .= $parsed['user'];
      if (isset($parsed['pass'])) {
        $built .= ':' . $parsed['pass'];
      }
      $built .= '@';
    }

    if (isset($parsed['host'])) {
      $built .= $parsed['host'];
    }

    if (isset($parsed['port'])) {
      $built .= ':' . $parsed['port'];
    }

    if (isset($parsed['path'])) {
      $built .= $parsed['path'];
    }

    $built = url($built, array('absolute' => TRUE, 'query' => isset($parsed['query']) ? $parsed['query'] : array(), 'fragment' => isset($parsed['fragment']) ? $parsed['fragment'] : ''));
  }
  else {
    $built = $url;
  }

  return $built;
}

function xcri_cap_views_default_views() {
  $view = new view();
  $view->name = 'xcri_cap';
  $view->description = 'Display an XCRI-CAP feed for all the ongoing courses presentations.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'XCRI-CAP';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'xcri-cap';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Start/End -  start date (field_start_end) */
  $handler->display->display_options['sorts']['field_start_end_value']['id'] = 'field_start_end_value';
  $handler->display->display_options['sorts']['field_start_end_value']['table'] = 'field_data_field_start_end';
  $handler->display->display_options['sorts']['field_start_end_value']['field'] = 'field_start_end_value';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'course_presentation' => 'course_presentation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Start/End - end date (field_start_end:value2) */
  $handler->display->display_options['filters']['field_start_end_value2']['id'] = 'field_start_end_value2';
  $handler->display->display_options['filters']['field_start_end_value2']['table'] = 'field_data_field_start_end';
  $handler->display->display_options['filters']['field_start_end_value2']['field'] = 'field_start_end_value2';
  $handler->display->display_options['filters']['field_start_end_value2']['operator'] = '>=';
  $handler->display->display_options['filters']['field_start_end_value2']['default_date'] = 'now';

  /* Display: XCRI-CAP */
  $handler = $view->new_display('xcri-cap', 'XCRI-CAP', 'xcri-cap_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['path'] = 'xcri-cap';

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // At the end, return array of default views.
  return $views;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function template_preprocess_xcri_cap_feed(&$variables) {
  $date_format = "Y-m-d\TH:i:sP";
  $variables['generated'] = date_format(date_now(), $date_format);

  $variables['contributor'] = check_plain(isset($variables['options']['contributor']) ? $variables['options']['contributor'] : '');
  $variables['description'] = isset($variables['options']['description']) ? $variables['options']['description'] : '';


  $tree = array();
  // Start building the elements that will be used on the feed
  foreach ($variables['rows'] as $presentation) {
    //
    // Provider's data
    // There should really be one but we handle multiple providers just in case
    //
    $providers = entity_metadata_wrapper('node', $presentation)->field_provider->value();
    if (!is_array($providers)) {
      $providers = array($providers);
    }
    foreach ($providers as $provider) {
      if (!isset($tree[$provider->tid])) {
        $data = entity_metadata_wrapper('taxonomy_term', $provider);

        $url = '';
        if (isset($data->field_url)) {
          $url_data = $data->field_url->value();
          $url = is_array($url_data) ? xcri_cap_feed_url($url_data['url']) : xcri_cap_feed_url($url_data);
        }

        $tree[$provider->tid] = array(
          'identifier' => check_url(empty($url) ? url('taxonomy/term/' . $provider->tid, array('absolute' => TRUE)) : $url),
          'url' => check_url(empty($url) ? url('taxonomy/term/' . $provider->tid, array('absolute' => TRUE)) : $url),
          'title' => check_plain($data->name->value()),
          'description' => $data->description->value(),
        );

        $url_location = '';
        if (isset($data->field_location_url)) {
          $url_data = $data->field_location_url->value();
          $url_location = is_array($url_data) ? $url_data['url'] : $url_data;
        }
        $tree[$provider->tid]['location'] = array(
            'address' => check_plain(isset($data->field_address) ? $data->field_address->value() : ''),
            'email' => check_plain(isset($data->field_email) ? $data->field_email->value() : ''),
            'fax' => check_plain(isset($data->field_fax) ? $data->field_fax->value() : ''),
            'phone' => check_plain(isset($data->field_phone) ? $data->field_phone->value() : ''),
            'postcode' => check_plain(isset($data->field_postcode) ? $data->field_postcode->value() : ''),
            'town' => check_plain(isset($data->field_town) ? $data->field_town->value() : ''),
            'url' => check_url(xcri_cap_feed_url($url_location)),
        );
      }
    }

    //
    // Course's data
    // There should really be one but we handle multiple courses just in case
    //
    $courses = entity_metadata_wrapper('node', $presentation)->field_course->value();
    if (!is_array($courses)) {
      $courses = array($courses);
    }
    foreach ($providers as $provider) {
      foreach ($courses as $course) {
        if (!isset($tree[$provider->tid]['courses'])) {
          $tree[$provider->tid]['courses'] = array();
        }
        if (!isset($tree[$provider->tid]['courses'][$course->tid])) {
          $data = entity_metadata_wrapper('taxonomy_term', $course);

          // formatted textareas
          $formatted_values = array();
          foreach (array('application_procedure', 'assessment', 'learning_outcome', 'objective', 'prerequisite', 'regulations') as $field) {
            $key = 'field_' . $field;
            if (isset($data->$key)) {
              $field_data = $data->$key->value();
              $formatted_values[$field] = is_array($field_data) ? $field_data['safe_value'] : $field_data;
            }
          }

          $credits = array();
          if (isset($data->field_credit)) {
            $credits_objects = $data->field_credit->value();
            foreach ($credits_objects as $credit_object) {
              $credit_data = entity_metadata_wrapper('field_collection_item', $credit_object);
              $credits[] = array(
                'level' => check_plain(isset($credit_data->field_credit_level) ? $credit_data->field_credit_level->value() : ''),
                'scheme' => check_plain(isset($credit_data->field_credit_scheme) ? $credit_data->field_credit_scheme->value() : ''),
                'value' => check_plain(isset($credit_data->field_value) ? $credit_data->field_value->value() : ''),
              );
            }
          }

          $url = '';
          if (isset($data->field_url)) {
            $url_data = $data->field_url->value();
            $url = is_array($url_data) ? xcri_cap_feed_url($url_data['url']) : xcri_cap_feed_url($url_data);
          }

          $subjects = array();
          if (isset($data->field_subject)) {
            $subjetcts_data = $data->field_subject->value();
            if (!is_array($subjetcts_data)) {
              $subjetcts_data = array($subjetcts_data);
            }
            foreach ($subjetcts_data as $subject) {
              $subjects[] = check_plain($subject);
            }
          }

          $tree[$provider->tid]['courses'][$course->tid] = array(
            'identifier' => check_url(url('taxonomy/term/' . $course->tid, array('absolute' => TRUE))),
            'title' => check_plain($data->name->value()),
            'description' => $data->description->value(),
            'abstract' => check_plain(isset($data->field_abstract) ? $data->field_abstract->value() : ''),
            'credits' => $credits,
            'subjects' => $subjects,
            'type' => check_plain(isset($data->field_type) ? $data->field_type->value() : ''),
            'url' => check_url(empty($url) ? url('taxonomy/term/' . $course->tid, array('absolute' => TRUE)) : $url),
          ) + $formatted_values;

        }
      }
    }

    //
    // Qualification's data
    //
    $qualifications = entity_metadata_wrapper('node', $presentation)->field_qualification->value();
    if (!is_array($qualifications)) {
      $qualifications = array($qualifications);
    }
    foreach ($providers as $provider) {
      foreach ($courses as $course) {
        if (!isset($tree[$provider->tid]['courses'][$course->tid]['qualifications'])) {
          $tree[$provider->tid]['courses'][$course->tid]['qualifications'] = array();
        }
        foreach ($qualifications as $qualification) {
          if (!isset($tree[$provider->tid]['courses'][$course->tid]['qualifications'][$qualification->tid])) {
            $data = entity_metadata_wrapper('taxonomy_term', $qualification);
            $tree[$provider->tid]['courses'][$course->tid]['qualifications'][$qualification->tid] = array(
                'identifier' => check_url(url('taxonomy/term/' . $qualification->tid, array('absolute' => TRUE))),
                'title' => check_plain($data->name->value()),
                'description' => $data->description->value(),
                'abbr' => check_plain(isset($data->field_abbr) ? $data->field_abbr->value() : ''),
                'education_level' => check_plain(isset($data->field_education_level) ? $data->field_education_level->value() : ''),
                'type' => check_plain(isset($data->field_type) ? $data->field_type->value() : ''),
                'awarded_by' => check_plain(isset($data->field_awarded_by) ? $data->field_awarded_by->value() : ''),
                'accredited_by' => check_plain(isset($data->field_accredited_by) ? $data->field_accredited_by->value() : ''),
            );
          }
        }
      }
    }

    //
    // Presentation's data
    //
    foreach ($providers as $provider) {
      foreach ($courses as $course) {
        if (!isset($tree[$provider->tid]['courses'][$course->tid]['presentations'])) {
          $tree[$provider->tid]['courses'][$course->tid]['presentations'] = array();
        }
        if (!isset($tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid])) {
          $data = entity_metadata_wrapper('node', $presentation);

          $start = $end = '';
          if (isset($data->field_start_end)) {
            $date_data = $data->field_start_end->value();
            // check for existance and date object
            if (is_array($date_data) && isset($date_data['date_type'])) {
              $date_start = new DateObject($date_data['value'], $date_data['timezone']);
              $start = date_format($date_start, $date_format);
              $date_end = new DateObject($date_data['value2'], $date_data['timezone']);
              $end = date_format($date_end, $date_format);
            }
          }

          $apply_from = $apply_until = '';
          if (isset($data->field_apply)) {
            $date_data = $data->field_apply->value();
            // check for existance and date object
            if (is_array($date_data) && isset($date_data['date_type'])) {
              $date_start = new DateObject($date_data['value'], $date_data['timezone']);
              $apply_from = date_format($date_start, $date_format);
              $date_end = new DateObject($date_data['value2'], $date_data['timezone']);
              $apply_until = date_format($date_end, $date_format);
            }
          }

          $url_apply = '';
          if (isset($data->field_apply_to)) {
            $url_data = $data->field_apply_to->value();
            $url_apply = is_array($url_data) ? xcri_cap_feed_url($url_data['url']) : xcri_cap_feed_url($url_data);
          }

          // formatted textareas
          $formatted_values = array();
          foreach (array('application_procedure', 'assessment', 'learning_outcome', 'objective', 'prerequisite', 'regulations', 'body') as $field) {
            if ($field == 'body') {
              $key = $field;
              $field_key = 'description';
            }
            else {
              $key = 'field_' . $field;
              $field_key = $field;
            }
            if (isset($data->$key)) {
              $field_data = $data->$key->value();
              $formatted_values[$field_key] = is_array($field_data) ? $field_data['safe_value'] : $field_data;
            }
          }

          // allowed values fields
          $allowed_values = array();
          foreach (array('attendance_mode', 'attendance_pattern', 'study_mode', 'language_of_assessment', 'language_of_instruction') as $field) {
            $id = $value = '';
            $field_key = 'field_' . $field;
            if (isset($data->$field_key) && $data->$field_key->value()) {
              $field_info = field_info_field($field_key);
              $values = list_allowed_values($field_info);
              $allowed_values[$field . '_id'] = check_plain($data->$field_key->value());
              $allowed_values[$field] = check_plain($values[$data->$field_key->value()]);
            }
          }

          $subjects = array();
          if (isset($data->field_subject)) {
            $subjetcts_data = $data->field_subject->value();
            if (!is_array($subjetcts_data)) {
              $subjetcts_data = array($subjetcts_data);
            }
            foreach ($subjetcts_data as $subject) {
              $subjects[] = check_plain($subject);
            }
          }

          $tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid] = array(
            'identifier' => check_url(url('node/' . $presentation->nid, array('absolute' => TRUE))),
            'title' => check_plain($data->title->value()),
            'start' => check_plain($start),
            'end' => check_plain($end),
            'apply_from' => check_plain($apply_from),
            'apply_until' => check_plain($apply_until),
            'apply_to' => check_url($url_apply),
            'subjects' => $subjects,
            'abstract' => check_plain(isset($data->field_abstract) ? $data->field_abstract->value() : ''),
            'age' => check_plain(isset($data->field_age) ? $data->field_age->value() : ''),
            'cost' => check_plain(isset($data->field_cost) ? $data->field_cost->value() : ''),
            'duration' => check_plain(isset($data->field_duration) ? $data->field_duration->value() : ''),
            'places' => check_plain(isset($data->field_places) ? $data->field_places->value() : ''),
          ) + $formatted_values + $allowed_values;

          if (!isset($tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid]['venues'])) {
            $tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid]['venues'] = array();
          }
          $venues = entity_metadata_wrapper('node', $presentation)->field_venue->value();
          if (!is_array($venues)) {
            $venues = array($venues);
          }
          foreach ($venues as $venue) {
            if (!isset($tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid]['venues'][$venue->tid])) {
              $data = entity_metadata_wrapper('taxonomy_term', $venue);
              $tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid]['venues'][$venue->tid] = array(
                  'identifier' => check_url(url('taxonomy/term/' . $venue->tid, array('absolute' => TRUE))),
                  'title' => check_plain($data->name->value()),
                  'description' => $data->description->value(),
              );

              $url_location = '';
              if (isset($data->field_location_url)) {
                $url_data = $data->field_location_url->value();
                $url_location = is_array($url_data) ? $url_data['url'] : $url_data;
              }
              $tree[$provider->tid]['courses'][$course->tid]['presentations'][$presentation->nid]['venues'][$venue->tid]['location'] = array(
                'address' => check_plain(isset($data->field_address) ? $data->field_address->value() : ''),
                'email' => check_plain(isset($data->field_email) ? $data->field_email->value() : ''),
                'fax' => check_plain(isset($data->field_fax) ? $data->field_fax->value() : ''),
                'phone' => check_plain(isset($data->field_phone) ? $data->field_phone->value() : ''),
                'postcode' => check_plain(isset($data->field_postcode) ? $data->field_postcode->value() : ''),
                'town' => check_plain(isset($data->field_town) ? $data->field_town->value() : ''),
                'url' => check_url(xcri_cap_feed_url($url_location)),
              );

            }
          }
        }
      }
    }
  }

  $variables['providers'] = $tree;

  // During live preview we don't want to output the header since the contents
  // of the feed are being displayed inside a normal HTML page.
  if (empty($vars['view']->live_preview)) {
    drupal_add_http_header('Content-Type', 'application/xml; charset=utf-8');
  }
}

