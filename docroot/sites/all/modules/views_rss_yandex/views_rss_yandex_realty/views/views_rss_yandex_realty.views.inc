<?php

/**
 * @file
 * Views plugins, handlers and hooks definition for Views RSS Yandex module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_rss_yandex_realty_views_plugins() {
  return array(
    'style' => array(
      'views_rss_yandex_realty_fields' => array(
        'title' => t('RSS Feed - Yandex.Realty'),
        'handler' => 'views_rss_yandex_realty_plugin_style_fields',
        // 'path' => drupal_get_path('module', 'views_rss_yandex_realty') . '/views',
        'parent' => 'views_rss_plugin_style_fields',
        'uses options' => TRUE,
        'help' => t('Outputs RSS formatted for Yandex.Realty'),
        'theme' => 'views_view_views_rss_yandex_realty',
        'theme path' => drupal_get_path('module', 'views_rss_yandex_realty') . '/views',
        'type' => 'feed',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
      ),
    ),
  );
}

/**
 * Template preprocessor for views-view-views-rss-yandex-realty.tpl.php.
 */
function template_preprocess_views_view_views_rss_yandex_realty(&$variables) {
  $view = $variables['view'];

  // Start building the feed array compatible with format_xml_elements().
  $rss_feed = array(
    'key' => 'realty-feed',
    'attributes' => array(
      'xmlns' => 'http://webmaster.yandex.ru/schemas/feed/realty/2010-06',
    ),
    'value' => array(),
  );

  // Add generation date.
  if (module_exists('date_api')) {
    $generation_date = format_date(REQUEST_TIME, 'custom', 'c', date_default_timezone(FALSE));
  }
  else {
    $generation_date = format_date(REQUEST_TIME, 'custom', 'c');
  }
  $rss_feed['value'][] = array(
    'key' => 'generation-date',
    'value' => $generation_date,
  );

  // Prepare <offer> elements and add them to the channel array.
  $offer_elements = views_rss_get('yandex_realty_offer_elements');
  $items = $view->style_plugin->map_rows($variables['rows']);
  foreach ($items as $item_key => $item) {

    // Start building XML offer element array compatible with format_xml_elements().
    $rss_item = array(
      'key' => 'offer',
      'value' => array(),
    );

    // Preprocess whole item array before preprocessing separate elements.
    foreach (module_implements('views_rss_preprocess_item') as $module) {
      $preprocess_function = $module . '_views_rss_preprocess_item';
      $item_variables = array(
        'item' => &$item,
        'view' => $view,
      );
      // Add raw row if generated based on raw item values provided by field formatter.
      if (!empty($view->views_rss['raw_items'][$item_key])) {
        $item_variables['raw'] = $view->views_rss['raw_items'][$item_key];
      }
      $preprocess_function($item_variables);
    }

    // Process each element separately.
    foreach ($item as $module => $module_item_elements) {
      foreach ($module_item_elements as $element => $value) {
        // Special handling for internal-id element.
        if ($element == 'internal-id') {
          $rss_item['attributes'] = array(
            $element => $value,
          );
          continue;
        }

        // Avoid double encoding: the $value might be already encoded here,
        // depending on the field configuration/processing, and because we know
        // it will be encoded again when the whole feed array will be passed to
        // format_xml_elements(), let's make sure we decode it here first.
        if (is_string($value)) {
          $value = decode_entities($value);
        }

        // Start building XML element array compatible with format_xml_elements().
        $rss_elements = array(
          array(
            'key' => isset($offer_elements[$module][$element]['render tag']) ? $offer_elements[$module][$element]['render tag'] : $element,
            'value' => $value,
          ),
        );

        // Preprocess element initial value if required.
        if (isset($offer_elements[$module][$element]['preprocess functions']) && is_array($offer_elements[$module][$element]['preprocess functions'])) {
          foreach ($offer_elements[$module][$element]['preprocess functions'] as $preprocess_function) {
            if (function_exists($preprocess_function)) {
              $item_variables = array(
                'elements' => &$rss_elements,
                'item' => $item,
                'view' => $view,
              );
              // Add raw item if provided by field formatter.
              if (!empty($view->views_rss['raw_items'][$item_key][$module][$element])) {
                $item_variables['raw'] = $view->views_rss['raw_items'][$item_key][$module][$element];
              }
              $preprocess_function($item_variables);
            }
          }
        }
        // If no preprocess function was defined, and we have received
        // #rss_element value (XML element array) from the formatter, it should
        // be added to the feed array without any further modifications.
        elseif (
          !empty($view->views_rss['raw_items'][$item_key][$module][$element])
          && is_array($view->views_rss['raw_items'][$item_key][$module][$element])
        ) {
          // At this point we don't know yet if we got #rss_elements in raw
          // values, so do not overwrite and empty main $rss_elements yet, just
          // start working with new $formatter_rss_elements - it could be
          // overwritten once we are sure we have all required values.
          $formatter_rss_elements = array();
          foreach ($view->views_rss['raw_items'][$item_key][$module][$element] as $raw_item) {
            if (!empty($raw_item['rendered']['#rss_element'])) {
              $formatter_rss_elements[] = $raw_item['rendered']['#rss_element'];
            }
          }
          // Now we can overwrite main $rss_elements.
          if (!empty($formatter_rss_elements)) {
            $rss_elements = $formatter_rss_elements;
          }
        }

        // If there is no value and no attributes (in case of self-closing elements)
        // already set for the element at this stage, it is not going to be set
        // at any point further, so the element should not be added to the feed.
        foreach ($rss_elements as $key => $rss_element) {
          if (empty($rss_element['value']) && empty($rss_element['attributes'])) {
            unset($rss_elements[$key]);
          }
        }
        if (empty($rss_elements)) continue;

        // Add XML element(s) to the item array.
        $target = &$rss_item['value'];
        if (!empty($offer_elements[$module][$element]['parents'])) {
          foreach ($offer_elements[$module][$element]['parents'] as $parent) {
            $parent_found = FALSE;
            foreach ($target as $key => &$candidate) {
              if ($candidate['key'] == $parent) {
                $parent_found = TRUE;
                $target = &$target[$key]['value'];
                break;
              }
            }
            if (!$parent_found) {
              $target[] = array(
                'key' => $parent,
                'value' => array(),
              );
              end($target);
              $target = &$target[key($target)]['value'];
            }
          }
        }
        $target = array_merge($target, $rss_elements);
      }
    }

    // Add XML offer element to the feed array.
    $rss_feed['value'][] = $rss_item;
  }

  // Allow for altering it before final render and passing it to the template.
  drupal_alter('views_rss_feed', $rss_feed);

  // Assign rendered feed to the template variables.
  $variables['rss_feed'] = format_xml_elements(array($rss_feed));

  // Set XML header.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
}

