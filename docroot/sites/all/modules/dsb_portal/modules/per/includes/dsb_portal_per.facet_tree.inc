<?php

/**
 * @file
 * Logic for rendering the facets like a tree.
 */

/**
 * Render the facets as a tree.
 *
 * @see dsb_portal_per_form_dsb_portal_rest_api_search_form_alter()
 */
function dsb_portal_per_render_facet_tree(&$form) {
  $tree = dsb_portal_per_get_curriculum_tree();

  // Prepare a new form structure. We need to recursively treat items in the
  // tree, and as soon as we reach the leaf, we need to check if the facet is
  // actually there. If not, we discard the path, and pass on to the next.
  $facet_tree = array();
  foreach ($tree as $cycle => $cycle_values) {
    foreach ($cycle_values as $domain => $domain_values) {
      foreach ($domain_values as $discipline => $discipline_values) {
        foreach ($discipline_values as $theme => $theme_data) {
          if (isset($form[$theme_data->code])) {
            // We have it. Put it in the facet tree.
            if (!isset($facet_tree[$cycle])) {
              $facet_tree[$cycle] = array();
            }
            if (!isset($facet_tree[$cycle][$domain])) {
              $facet_tree[$cycle][$domain] = array();
            }
            if (!isset($facet_tree[$cycle][$domain][$discipline])) {
              $facet_tree[$cycle][$domain][$discipline] = array();
            }
            $facet_tree[$cycle][$domain][$discipline][$theme] = $theme_data;
          }
        }
      }
    }
  }

  // Now, re-iterate through this new tree, and create the form structure.
  $new_form = array();
  foreach ($form as $key => $value) {
    // Retrieve all Form API setting keys.
    if (preg_match('/^#/', $key)) {
      $new_form[$key] = $value;
    }
  }

  foreach ($facet_tree as $cycle => $cycle_values) {
    // See _dsb_portal_rest_api_get_facet_checkboxes() for more information on
    // the facet checkbox structure.
    $new_form[$cycle] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($cycle),
      '#default_value' => isset($_GET['filter']['perCycle']) && in_array($cycle, $_GET['filter']['perCycle']) ? $cycle : 0,
      '#parents' => array('filter', 'perCycle', $cycle),
      '#return_value' => $cycle,
    );

    foreach ($cycle_values as $domain => $domain_values) {
      $new_form["$cycle:$domain"] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($domain),
        // Currently, there's no way to filter by domain AND cycle in the same
        // filter. Because domains are shared among cycles, using it as a filter
        // does not make much sense. That is why we disable it (for now).
        '#disabled' => TRUE,
        '#prefix' => theme('indentation', array('size' => 1)),
      );

      foreach ($domain_values as $discipline => $discipline_values) {
        $new_form["$cycle:$domain:$discipline"] = array(
          '#type' => 'checkbox',
          '#title' => check_plain($discipline),
          // Currently, there's no way to filter by discipline AND domain AND
          // cycle in the same filter. Because disciplines are shared among
          // cycles and domains, using it as a filter does not make much sense.
          // That is why we disable it (for now).
          '#disabled' => TRUE,
          '#prefix' => theme('indentation', array('size' => 2)),
        );

        foreach ($discipline_values as $theme => $theme_data) {
          $new_form[$theme_data->code] = $form[$theme_data->code];
          $new_form[$theme_data->code]['#prefix'] = theme('indentation', array('size' => 3));
        }
      }
    }
  }
  $form = $new_form;
}

/**
 * Refresh the official PER objective code tree.
 *
 * Download the PER curriculum XML definition and construct the tree structure.
 * This data is cached.
 *
 * @return array|null
 *    The PER curriculum tree.
 */
function dsb_portal_per_get_curriculum_tree() {
  $cid = 'dsb_portal_per:curriculum_tree';
  $data = cache_get($cid);

  if (!empty($data->data)) {
    return $data->data;
  }
  else {
    $url = variable_get('dsb_portal_per_xml_url', 'http://ontology.biblio.educa.ch/curriculum/latest.xml');
    if ($url) {
      $xml_file = drupal_http_request($url);
      $xml = simplexml_load_string(mb_convert_encoding($xml_file->data, 'UTF-8'));

      if (!$xml) {
        $errors = array();
        foreach (libxml_get_errors() as $error) {
          $errors[] = $error->message;
        }
        watchdog('dsb_portal_per', "Could not load the XML data. Errors:<br /><pre>!errors</pre>", array('!errors' => print_r($errors, 1)), WATCHDOG_ERROR);
      }
      else {
        // Get the subtitle first.
        // Prepare a function to recursively lookup subtitles. Most of this code
        // is a verbatim copy from the class ArchibaldCurriculumPer, shipped
        // with the Archibald module.
        // @todo This code should be added to the dsb Client library.
        $recursive_subtitle_lookup = function(SimpleXMLElement $items, $tree_string) use(&$recursive_subtitle_lookup) {
          $subtitles = array();
          foreach ($items as $item) {
            $subtitle           = new stdClass();
            $subtitle->title    = (string) $item->title;
            $subtitle->url_part = (string) $item->attributes()->url_part;
            $subtitle->uuid     = md5($tree_string . '/' . $item->title);
            $subtitle->childs   = array();
            $subtitle->details  = array();

            if (!empty($item->subtitle)) {
              $subtitle->childs = $recursive_subtitle_lookup($item->subtitle, $tree_string . '|' . $item->title);
            }

            if (!empty($item->details)) {
              foreach ($item->details as $detail_item) {
                $detail              = new stdClass();
                $detail->url_part    = (string) $detail_item->attributes()->url_part;
                $detail->text        = (string) $detail_item->text;
                $detail->school_year = array();
                if (!empty($detail_item->school_year)) {
                  foreach ($detail_item->school_year as $school_year_item) {
                    $detail->school_year[] = (string) $school_year_item;
                  }
                }
                $subtitle->details[] = $detail;
              }
            }

            $subtitles[] = $subtitle;
          }

          return $subtitles;
        };

        $subtitles = array();
        foreach ($xml->subtitles->code as $code) {
          if (!empty($code->attributes()->code)) {
            $code_string = (string) $code->attributes()->code;
          }
          else {
            $code_string = NULL;
          }

          if (!empty($code->subtitle) && !empty($code_string)) {
            $subtitles[$code_string] = $recursive_subtitle_lookup($code->subtitle, $code_string);
          }
        }

        $curriculum = array();
        foreach ($xml->code_matrix->domain as $domain) {
          foreach ($domain->discipline as $discipline) {
            foreach ($discipline->theme as $theme) {
              foreach ($theme as $cycle) {
                $val = new stdClass();
                $val->code = (string) $cycle;
                $val->url_part = '';
                $val->description = '';
                if (!empty($val->code) && drupal_strtolower($val->code) != 'x') {
                  $objectiv = $xml->xpath('//objectiv_codes/code[@code="' . $val->code . '"]');
                  if (!empty($objectiv)) {
                    $val->url_part = (string) $objectiv[0]->attributes()->url_part;
                    $val->description = (string) $objectiv[0]->description;
                  }

                  if (!empty($subtitles[$val->code])) {
                    $val->subtitles = $subtitles[$val->code];
                  }

                  $curriculum[(string) $cycle->attributes()->name][(string) $domain->attributes()->name][(string) $discipline->attributes()->name][(string) $theme->attributes()->name] = $val;
                }
              }
            }
          }
        }

        cache_set($cid, $curriculum, 'cache', CACHE_TEMPORARY);

        return $curriculum;
      }
    }
    else {
      watchdog('dsb_portal_per', "No PER curriculum XML URL set.", array(), WATCHDOG_ERROR, l('configure module', 'admin/config/services/dsb-portal-per'));
      return NULL;
    }
  }
}
