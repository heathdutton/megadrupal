<?php

/**
 * @file
 * Contains the agres_views row style plugin.
 *
 * This plugin takes the view results, finds the date value for each,
 * then compares that date to the date range for the current view.
 * Items that started before or ended after the current date range
 * are shortened to the current range. Items that extend over more
 * than one day are cloned to create a agres_views item for each day.
 * The resulting array of results (which may have a different number
 * of items than the original view result) are then passed back
 * to the style plugin so they can be displayed in a agres_views.
 *
 * Row plugins are specific to entity types. To create a row plugin
 * for other types of entities, this class can be extended or copied,
 * adjusting the parts that are specific to nodes.
 */

/**
 * Plugin which creates a view on the resulting object
 * and formats it as a agres_views node.
 */
class agres_views_plugin_row_node extends calendar_plugin_row {

  function pre_render($values) {

    // @TODO When the date is coming in through a relationship, the nid
    // of the view is not the right node to use, then we need the related node.
    // Need to sort out how that should be handled.
    // Preload each node used in this view from the cache.
    // Provides all the node values relatively cheaply, and we don't
    // need to do it repeatedly for the same node if there are
    // multiple results for one node.
    $nids = array();
    foreach ($values as $row) {
      // Use the $nid as the key so we don't create more than one value per node.
      $nid = $row->{$this->field_alias};
      $nids[$nid] = $nid;
    }
    if (!empty($nids)) {
      $this->nodes = node_load_multiple($nids);
    }

    // Identify the date argument and fields that apply to this view.
    // Preload the Date Views field info for each field, keyed by the
    // field name, so we know how to retrieve field values from the cached node.
    $data = date_views_fields('node');
    $data = $data['name'];
    $date_fields = array();
    foreach ($this->view->argument as $handler) {
      if (date_views_handler_is_date($handler, 'argument')) {
        // If this is the complex Date argument, the date fields are stored in the handler options,
        // otherwise we are using the simple date field argument handler.
        if ($handler->definition['handler'] != 'date_views_argument_handler') {
          $alias = $handler->table_alias . '.' . $handler->field;
          $info = $data[$alias];
          $field_name = str_replace(array('_value2', '_value'), '', $info['real_field_name']);
          $date_fields[$field_name] = $info;
        }
        else {
          foreach ($handler->options['date_fields'] as $alias) {
            $info = $data[$alias];
            $field_name = str_replace(array('_value2', '_value'), '', $info['real_field_name']);
            $date_fields[$field_name] = $info;
          }
        }
        $this->date_argument = $handler;
        $this->date_fields = $date_fields;
      }
    }
  }

  function render($row) {
    global $base_url;
    $date_info = $this->date_argument->view->date_info;

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    // Load the specified node:
    // We have to clone this or nodes on other views on this page,
    // like an Upcoming block on the same page as a agres_views view,
    // will end up acquiring the values we set here.
    $node = clone($this->nodes[$nid]);
    if (empty($node)) {
      return;
    }

    // There could be more than one date field in a view
    // so iterate through all of them to find the right values
    // for this view result.
    $rows = array();
    foreach ($this->date_fields as $field_name => $info) {

      $table_name = $info['table_name'];
      $delta_field = $info['delta_field'];
      $tz_handling = $info['tz_handling'];
      $tz_field = $info['timezone_field'];
      $rrule_field = $info['rrule_field'];
      $is_field = drupal_substr($info['real_field_name'], 0, 6) == 'field_';

      // Retrieve the field value that matched our query from the cached node.
      // Find the date and set it to the right timezone.

      $item = $node->$field_name;
      $node->date_id = array();
      $item_start_date = NULL;
      $item_end_date = NULL;

      $db_tz = date_get_timezone_db($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);
      $to_zone = date_get_timezone($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);
      $granularity = 'second';
      $increment = 1;

      if ($is_field) {

        // @TODO Not sure if this is the correct way to retrieve the right language value, but I think so.
//        $lang = i18n_langcode(); //$node->language;
        $lang = 'und';
        $lang = field_language('node',$node,$field_name);
        $delta = isset($row->$delta_field) ? $row->$delta_field : 0;
        $item = array_key_exists($lang, $item) ? $item[$lang][$delta] : $item['und'][$delta];

        // Set the date_id for the node, used to identify which field value to display for
        // fields that have multiple values. The theme expects it to be an array.
        $node->date_id = array('agres_views.' . $node->nid . '.' . $field_name . '.' . $delta);

        if (!empty($item['value'])) {
          $item_start_date = new dateObject($item['value'], $db_tz);
          $item_end_date = array_key_exists('value2', $item) ? new dateObject($item['value2'], $db_tz) : $item_start_date;
        }

        $cck_field = field_info_field($field_name);
        $instance = field_info_instance($this->view->base_table, $field_name, $node->type);
        $granularity = date_granularity_precision($cck_field['settings']['granularity']);
        $increment = $instance['widget']['settings']['increment'];
      }
      elseif (!$is_field && !empty($item)) {
        $item_start_date = new dateObject($item, $db_tz);
        $item_end_date = $item_start_date;
        $node->date_id = array('agres_views.' . $node->nid . '.' . $field_name . '.0');
      }

      // If we don't have date value, go no further.
      if (empty($item_start_date)) {
        return;
      }

      // Set the item date to the proper display timezone;
      $item_start_date->setTimezone(new dateTimezone($to_zone));
      $item_end_date->setTimezone(new dateTimezone($to_zone));

      $event = new stdClass();
      $event->nid = $node->nid;
      $event->title = $node->title;
      $event->type = $node->type;
      $event->date_start = $item_start_date;
      $event->date_end = $item_end_date;
      $event->db_tz = $db_tz;
      $event->to_zone = $to_zone;
      $event->granularity = $granularity;
      $event->increment = $increment;
      $event->field = $is_field ? $item : NULL;
      $event->row = $row;
      $event->node = $node;

      // All agres_views row plugins should provide a date_id that the theme can use.
      $event->date_id = $node->date_id[0];

      $nodes = $this->explode_values($event);
      foreach ($nodes as $node) {
        switch ($this->options['colors']['legend']) {
          case 'type':
            $this->agres_views_node_type_stripe($node);
            break;
          case 'taxonomy':
            $this->agres_views_node_taxonomy_stripe($node);
            break;
          case 'group':
            $this->agres_views_node_group_stripe($node);
            break;
        }
        $rows[] = $node;
      }
    }
//    drupal_set_message('<pre>$rows$rows$rows '.print_r( $rows,true).'</pre>');
    return $rows;
  }

  function explode_values($event) {
    $rows = array();

    $date_info = $this->date_argument->view->date_info;
    $item_start_date = $event->date_start;
    $item_end_date = $event->date_end;
    $to_zone = $event->to_zone;
    $db_tz = $event->db_tz;
    $granularity = $event->granularity;
    $increment = $event->increment;

    // Now that we have a 'node' for each view result, we need
    // to remove anything outside the view date range,
    // and possibly create additional nodes so that we have a 'node'
    // for each day that this item occupies in this view.
    $now = max($date_info->min_zone_string, $item_start_date->format(DATE_FORMAT_DATE));
    $to = min($date_info->max_zone_string, $item_end_date->format(DATE_FORMAT_DATE));
    $next = new DateObject($now . ' 00:00:00', $date_info->display_timezone);
    if ($date_info->display_timezone_name != $to_zone) {
      // Make $start and $end (derived from $node) use the timezone $to_zone, just as the original dates do.
      date_timezone_set($next, timezone_open($to_zone));
    }
    if (empty($to) || $now > $to) {
      $to = $now;
    }
    // $now and $next are midnight (in display timezone) on the first day where node will occur.
    // $to is midnight on the last day where node will occur.
    // All three were limited by the min-max date range of the view.
    $pos = 0;
    while (!empty($now) && $now <= $to) {
      $node = clone($event);
      $node->url = url('node/' . $node->nid, array('absolute' => TRUE));

      // Get start and end of current day.
      $start = $next->format(DATE_FORMAT_DATETIME);
      date_modify($next, '+1 day');
      date_modify($next, '-1 second');
      $end = $next->format(DATE_FORMAT_DATETIME);

      // Get start and end of item, formatted the same way.
      $item_start = $item_start_date->format(DATE_FORMAT_DATETIME);
      $item_end = $item_end_date->format(DATE_FORMAT_DATETIME);

      // Get intersection of current day and the node value's duration (as strings in $to_zone timezone).
      $node->agres_views_start = $item_start < $start ? $start : $item_start;
      $node->agres_views_end = !empty($item_end) ? ($item_end > $end ? $end : $item_end) : $node->agres_views_start;

      // Make date objects
      $node->agres_views_start_date = date_create($node->agres_views_start, timezone_open($to_zone));
      $node->agres_views_end_date = date_create($node->agres_views_end, timezone_open($to_zone));

      // Change string timezones into
      // agres_views_start and agres_views_end are UTC dates as formatted strings
      $node->agres_views_start = date_format($node->agres_views_start_date, DATE_FORMAT_DATETIME);
      $node->agres_views_end = date_format($node->agres_views_end_date, DATE_FORMAT_DATETIME);
      $node->agres_views_all_day = date_is_all_day($node->agres_views_start, $node->agres_views_end, $granularity, $increment);

      // Change string timezones into
      // agres_views_start and agres_views_end are UTC dates as formatted strings
      $node->agres_views_start = date_format($node->agres_views_start_date, DATE_FORMAT_DATETIME);
      $node->agres_views_end = date_format($node->agres_views_end_date, DATE_FORMAT_DATETIME);

      unset($node->agres_views_fields);
      if (isset($node) && (empty($node->agres_views_start))) {
        // if no date for the node and no date in the item
        // there is no way to display it on the agres_views
        unset($node);
      }
      else {
        $node->date_id .= '.' . $pos;

        $rows[] = $node;
        unset($node);
      }
      date_modify($next, '+1 second');
      $now = date_format($next, DATE_FORMAT_DATE);
      $pos++;
    }
    return $rows;
  }

  /**
   * Create a stripe base on node type.
   */
  function agres_views_node_type_stripe(&$node) {
    $colors = isset($this->options['colors']['agres_views_colors_type']) ? $this->options['colors']['agres_views_colors_type'] : array();
    if (empty($colors)) {
      return;
    }
    if (empty($node->type)) {
      return;
    }

    $type_names = node_type_get_names();
    $type = $node->type;
    if (!(isset($node->stripe))) {
      $node->stripe = array();
      $node->stripe_label = array();
    }
    if (array_key_exists($type, $type_names)) {
      $label = $type_names[$type];
    }
    if (array_key_exists($type, $colors)) {
      $stripe = $colors[$type];
    }
    else {
      $stripe = '';
    }

    $node->stripe[] = $stripe;
    $node->stripe_label[] = $label;
    $GLOBALS['agres_views_stripe_labels'][][$type] = array(
      'stripe' => $stripe,
      'label' => $label,
    );
    return $stripe;
  }

  /**
   * Create a stripe based on a taxonomy term.
   */
  function agres_views_node_taxonomy_stripe(&$event) {
    $term_colors = isset($this->options['colors']['term_colors']) ? $this->options['colors']['term_colors'] : array();
    if (empty($term_colors)) {
      return;
    }

    $node = $event->node;
    $terms = array();
    if ($this->options['colors']['legend'] == 'taxonomy') {
      $term_field_name = $this->options['colors']['taxonomy_field'];
      $term_field = $node->$term_field_name;
      foreach ($term_field as $lang => $items) {
        if ($lang == $node->language) {
          foreach ($items as $item) {
            $terms[] = $item['tid'];
          }
        }
      }
    }

    if (empty($terms)) {
      return;
    }

    if (!(isset($event->stripe))) {
      $event->stripe = array();
      $event->stripe_label = array();
    }
    if (count($terms)) {
      foreach ($terms as $tid) {
        $term_for_node = taxonomy_term_load($tid);
        if (!array_key_exists($term_for_node->tid, $term_colors)) {
          continue;
        }
        $stripe = $term_colors[$term_for_node->tid];
        $stripe_label = $term_for_node->name;
        $event->stripe[] = $stripe;
        $event->stripe_label[] = $stripe_label;
        $GLOBALS['agres_views_stripe_labels'][][$term_for_node->tid] = array(
          'stripe' => $stripe,
          'label' => $stripe_label,
        );
      }
    }
    else {
      $event->stripe[] = '';
      $event->stripe_label[] = '';
    }

    return;
  }

  /**
   * Create a stripe based on group.
   */
  function agres_views_node_group_stripe(&$node) {
    $colors_group = isset($this->options['colors']['agres_views_colors_group']) ? $this->options['colors']['agres_views_colors_group'] : array();
    if (empty($colors_group)) {
      return;
    }
    if (!function_exists('og_get_node_groups')) {
      return;
    }

    $groups_for_node = og_get_node_groups($node);
    if (!(isset($node->stripe))) {
      $node->stripe = array();
      $node->stripe_label = array();
    }
    if (count($groups_for_node)) {
      foreach ($groups_for_node as $gid => $group_name) {
        if (!array_key_exists($gid, $colors_group)) {
          continue;
        }
        $stripe = $colors_group[$gid];
        $stripe_label = $group_name;
        $node->stripe[] = $stripe;
        $node->stripe_label[] = $stripe_label;
        $GLOBALS['agres_views_stripe_labels'][][$gid] = array(
          'stripe' => $stripe,
          'label' => $stripe_label,
        );
      }
    }
    else {
      $node->stripe[] = '';
      $node->stripe_label[] = '';
    }
    return $stripe;
  }

}

