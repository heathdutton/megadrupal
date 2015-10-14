<?php
/**
 * @file
 * Contains the Calendar row style plugin.
 *
 * This plugin takes the view results, finds the date value for each,
 * then compares that date to the date range for the current view.
 * Items that started before or ended after the current date range
 * are shortened to the current range. Items that extend over more
 * than one day are cloned to create a calendar item for each day.
 * The resulting array of results (which may have a different number
 * of items than the original view result) are then passed back
 * to the style plugin so they can be displayed in a calendar.
 *
 */

/**
 * Plugin which creates a view on the resulting object
 * and formats it as a Calendar node.
 */
class agres_views_plugin_row extends calendar_plugin_row {

  // Stores the nodes loaded with pre_render.
  var $entities = array();

  function pre_render($values) {
    // @TODO When the date is coming in through a relationship, the nid
    // of the view is not the right node to use, then we need the related node.
    // Need to sort out how that should be handled.

    // Preload each entity used in this view from the cache.
    // Provides all the entity values relatively cheaply, and we don't
    // need to do it repeatedly for the same entity if there are
    // multiple results for one entity.
    $ids = array();
    foreach ($values as $row) {
      // Use the $id as the key so we don't create more than one value per entity.
      $id = $row->{$this->field_alias};
      $ids[$id] = $id;
    }
    if (!empty($ids)) {
      $this->entities = entity_load($this->view->base_table, $ids);
    }

    // Let the style know if a link to create a new date is required.
    $this->view->date_info->calendar_date_link = $this->options['calendar_date_link'];

    // Identify the date argument and fields that apply to this view.
    // Preload the Date Views field info for each field, keyed by the
    // field name, so we know how to retrieve field values from the cached node.
    $data = date_views_fields($this->view->base_table);
    $data = $data['name'];
    $date_fields = array();
    foreach ($this->view->argument as $handler) {
      if (date_views_handler_is_date($handler, 'argument')) {
        // If this is the complex Date argument, the date fields are stored in the handler options,
        // otherwise we are using the simple date field argument handler.
        if ($handler->definition['handler'] != 'date_views_argument_handler') {
          $alias = $handler->table_alias . '.' . $handler->field;
          $info = $data[$alias];
          $field_name  = str_replace(array('_value2', '_value'), '', $info['real_field_name']);
          $date_fields[$field_name] = $info;
        }
        else {
          foreach ($handler->options['date_fields'] as $alias) {
            $info = $data[$alias];
            $field_name  = str_replace(array('_value2', '_value'), '', $info['real_field_name']);
            $date_fields[$field_name] = $info;
          }
        }
        $this->date_argument = $handler;
        $this->date_fields = $date_fields;
      }
    }

    // Get the language for this view.
    $this->language = $this->display->handler->get_option('field_language');
    $substitutions = views_views_query_substitutions($this->view);
    if (array_key_exists($this->language, $substitutions)) {
      $this->language = $substitutions[$this->language];
    }
  }

  function render($row) {
    global $base_url;
    $rows = array();
    $date_info = $this->date_argument->view->date_info;
    $id = $row->{$this->field_alias};
    if (!is_numeric($id)) {
      return $rows;
    }

    // There could be more than one date field in a view
    // so iterate through all of them to find the right values
    // for this view result.
    foreach ($this->date_fields as $field_name => $info) {

      // Load the specified node:
      // We have to clone this or nodes on other views on this page,
      // like an Upcoming block on the same page as a calendar view,
      // will end up acquiring the values we set here.
      $entity = clone($this->entities[$id]);
      if (empty($entity)) {
        return $rows;
      }

      $table_name  = $info['table_name'];
      $delta_field = $info['delta_field'];
      $tz_handling = $info['tz_handling'];
      $tz_field    = $info['timezone_field'];
      $rrule_field = $info['rrule_field'];
      $is_field    = $info['is_field'];

      $info = entity_get_info($this->view->base_table);
      $this->id_field = $info['entity keys']['id'];
      $this->bundle = $info['entity keys']['bundle'];
      $callback = $info['uri callback'];
      $path = $callback($entity);
      $entity->url = $path['path'];

      // Retrieve the field value(s) that matched our query from the cached node.
      // Find the date and set it to the right timezone.

      $entity->date_id = array();
      $item_start_date = NULL;
      $item_end_date   = NULL;
      $granularity = 'second';
      $increment = 1;

      if ($is_field) {
//        $lang = 'und';
//        $lang = field_language($entity->type,$entity,$field_name);
        $delta = isset($row->$delta_field) ? $row->$delta_field : 0;
        $items = field_get_items($this->view->base_table, $entity, $field_name, $this->language);
        $item  = $items[$delta];
        $db_tz   = date_get_timezone_db($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);
        $to_zone = date_get_timezone($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);

        // Set the date_id for the node, used to identify which field value to display for
        // fields that have multiple values. The theme expects it to be an array.
        $entity->date_id = array('calendar.' . $id . '.' . $field_name . '.' . $delta);

        if (!empty($item['value'])) {
          $item_start_date = new dateObject($item['value'], $db_tz);
          $item_end_date   = array_key_exists('value2', $item) ? new dateObject($item['value2'], $db_tz) : $item_start_date;
        }

        $cck_field = field_info_field($field_name);
        $instance = field_info_instance($this->view->base_table, $field_name, $entity->type);
        $granularity = date_granularity_precision($cck_field['settings']['granularity']);
        $increment = $instance['widget']['settings']['increment'];

      }
      elseif (!empty($entity->$field_name)) {
        $item = $entity->$field_name;
        $db_tz   = date_get_timezone_db($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);
        $to_zone = date_get_timezone($tz_handling, isset($item->$tz_field) ? $item->$tz_field : $date_info->display_timezone_name);
        $item_start_date = new dateObject($item, $db_tz);
        $item_end_date   = $item_start_date;
        $entity->date_id = array('calendar.' . $id . '.' . $field_name . '.0');
      }

      // If we don't have a date value, go no further.
      if (empty($item_start_date)) {
        continue;
      }

      // Set the item date to the proper display timezone;
      $item_start_date->setTimezone(new dateTimezone($to_zone));
      $item_end_date->setTimezone(new dateTimezone($to_zone));

      $event = new stdClass();
      $event->id = $entity->{$this->id_field};
      $event->title = $entity->title;
      $event->type = $entity->{$this->bundle};
      $event->date_start = $item_start_date;
      $event->date_end = $item_end_date;
      $event->db_tz = $db_tz;
      $event->to_zone = $to_zone;
      $event->granularity = $granularity;
      $event->increment = $increment;
      $event->field = $is_field ? $item : NULL;
      $event->url = $entity->url;
      $event->row = $row;
      $event->entity = $entity;

      // All calendar row plugins should provide a date_id that the theme can use.
      $event->date_id = $entity->date_id[0];

      $entities = $this->explode_values($event);
      foreach ($entities as $entity) {
        switch ($this->options['colors']['legend']) {
          case 'type':
            $this->calendar_node_type_stripe($entity);
            break;
          case 'taxonomy':
            $this->calendar_taxonomy_stripe($entity);
            break;
          case 'group':
            $this->calendar_group_stripe($entity);
            break;
        }
        $rows[] = $entity;
      }

    }
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

    // Now that we have an 'entity' for each view result, we need
    // to remove anything outside the view date range,
    // and possibly create additional nodes so that we have a 'node'
    // for each day that this item occupies in this view.
    $now = max($date_info->min_zone_string, $item_start_date->format(DATE_FORMAT_DATE));
    $to  = min($date_info->max_zone_string, $item_end_date->format(DATE_FORMAT_DATE));
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
      $entity = clone($event);

      // Get start and end of current day.
      $start = $next->format(DATE_FORMAT_DATETIME);
      date_modify($next, '+1 day');
      date_modify($next, '-1 second');
      $end = $next->format(DATE_FORMAT_DATETIME);

      // Get start and end of item, formatted the same way.
      $item_start = $item_start_date->format(DATE_FORMAT_DATETIME);
      $item_end = $item_end_date->format(DATE_FORMAT_DATETIME);

      // Get intersection of current day and the node value's duration (as strings in $to_zone timezone).
      $entity->calendar_start = $item_start < $start ? $start : $item_start;
      $entity->calendar_end = !empty($item_end) ? ($item_end > $end ? $end : $item_end) : $node->calendar_start;

      // Make date objects
      $entity->calendar_start_date = date_create($entity->calendar_start, timezone_open($to_zone));
      $entity->calendar_end_date = date_create($entity->calendar_end, timezone_open($to_zone));

      // Change string timezones into
      // calendar_start and calendar_end are UTC dates as formatted strings
      $entity->calendar_start = date_format($entity->calendar_start_date, DATE_FORMAT_DATETIME);
      $entity->calendar_end = date_format($entity->calendar_end_date, DATE_FORMAT_DATETIME);
      $entity->calendar_all_day = date_is_all_day($entity->calendar_start, $entity->calendar_end, $granularity, $increment);

      unset($entity->calendar_fields);
      if (isset($entity) && (empty($entity->calendar_start))) {
        // if no date for the node and no date in the item
        // there is no way to display it on the calendar
        unset($entity);
      }
      else {
        $entity->date_id .= '.' . $pos;

        $rows[] = $entity;
        unset($entity);
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
  function agres_views_node_type_stripe(&$entity) {
    $colors = isset($this->options['colors']['calendar_colors_type']) ? $this->options['colors']['calendar_colors_type'] : array();
    if (empty($colors)) {
      return;
    }
    if (empty($entity->type)) {
      return;
    }

    $type_names = node_type_get_names();
    $type = $entity->type;
    if (!(isset($entity->stripe))) {
      $entity->stripe = array();
      $entity->stripe_label = array();
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

    $entity->stripe[] = $stripe;
    $entity->stripe_label[] = $label;
    $GLOBALS['calendar_stripe_labels'][][$type] = array('stripe' => $stripe, 'label' => $label);
    return $stripe;
  }

   /**
   * Create a stripe based on a taxonomy term.
   */

  function agres_views_taxonomy_stripe(&$entity) {
    $term_colors = isset($this->options['colors']['calendar_colors_taxonomy']) ? $this->options['colors']['calendar_colors_taxonomy'] : array();
    if (empty($term_colors)) {
      return;
    }

    $entity = $event->entity;
    $terms = array();
    if ($this->options['colors']['legend'] == 'taxonomy') {
      $term_field_name = $this->options['colors']['taxonomy_field'];
      if ($term_field = field_get_items($this->view->base_table, $entity, $term_field_name)) {
        foreach ($term_field as $delta => $items) {
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
        $term_for_entity = taxonomy_term_load($tid);
        if (!array_key_exists($term_for_entity->tid, $term_colors)) {
          continue;
        }
        $stripe = $term_colors[$term_for_entity->tid];
        $stripe_label = $term_for_entity->name;
        $event->stripe[] = $stripe;
        $event->stripe_label[] = $stripe_label;
        $GLOBALS['calendar_stripe_labels'][][$term_for_entity->tid] = array('stripe' => $stripe, 'label' => $stripe_label);
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
  function agres_views_group_stripe(&$entity) {
    $colors_group = isset($this->options['colors']['calendar_colors_group']) ? $this->options['colors']['calendar_colors_group'] : array();

    if (empty($colors_group)) {
      return;
    }
    if (!function_exists('og_get_entity_groups')) {
      return;
    }
    $groups_for_entity = og_get_entity_groups($this->view->base_table, $entity);
    if (!(isset($entity->stripe))) {
      $entity->stripe = array();
      $entity->stripe_label = array();
    }
    if (count($groups_for_entity)) {
      foreach ($groups_for_entity as $gid => $group_name) {
        if (!array_key_exists($gid, $colors_group)) {
          continue;
        }
        $stripe = $colors_group[$gid];
        $stripe_label = $group_name;
        $entity->stripe[] = $stripe;
        $entity->stripe_label[] = $stripe_label;
        $GLOBALS['calendar_stripe_labels'][][$gid] = array('stripe' => $stripe, 'label' => $stripe_label);
      }
    }
    else {
      $entity->stripe[] = '';
      $entity->stripe_label[] = '';
    }
    return;
  }
}

