<?php

/**
 * @file
 * Contains \MpxViewsFilterAvailability.
 */

/**
 * Views filter handler for if a video is unavailable, available, or expired.
 */
class MpxViewsFilterAvailability extends views_handler_filter_in_operator {

  /**
   * {@inheritdoc}
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = array(
        MPX_VIDEO_UNAVAILABLE => t('Unavailable'),
        MPX_VIDEO_AVAILABLE => t('Available'),
        MPX_VIDEO_EXPIRED => t('Expired'),
      );
    }
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    if (empty($this->value)) {
      return;
    }
    elseif ($this->value == array_keys($this->value_options)) {
      // If all options were selected, it does not make sense to filter since
      // all results would be returned anyway.
      return;
    }

    $sub_query = db_select('mpx_video', 'mpxv');
    $sub_query->addField('mpxv', 'video_id');
    $sub_query_condition = db_or();

    foreach ($this->value as $value) {
      if ($value == MPX_VIDEO_UNAVAILABLE) {
        $and = db_and();
        $and->condition("mpxv.available_date", 0, '<>');
        $and->condition("mpxv.available_date", REQUEST_TIME, '>');
        $sub_query_condition->condition($and);
      }
      elseif ($value == MPX_VIDEO_AVAILABLE) {
        $and = db_and();
        $and->condition("mpxv.available_date", 0);
        $and->condition("mpxv.expiration_date", 0);
        $sub_query_condition->condition($and);
        $and = db_and();
        $and->condition("mpxv.available_date", REQUEST_TIME, '<=');
        $and->condition("mpxv.expiration_date", REQUEST_TIME, '>');
        $sub_query_condition->condition($and);
      }
      elseif ($value == MPX_VIDEO_EXPIRED) {
        $and = db_and();
        $and->condition("mpxv.expiration_date", 0, '<>');
        $and->condition("mpxv.expiration_date", REQUEST_TIME, '<=');
        $sub_query_condition->condition($and);
      }
    }

    $sub_query->condition($sub_query_condition);

    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $sub_query, $this->operator);
  }

}
