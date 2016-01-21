<?php
// $Id$
/**
 * @file
 * Contains the basic 'wowtoon' field handler.
 */

/**
 * Field handler to provide image / text display of wowtoon_cached_data items.
 */
class avatar_handler_wowtoon_field extends views_handler_field {

  function construct() {
    parent::construct();
    if ($this->definition['field'] == 'class') {
      $this->additional_fields['classId'] = 'classId';
      $this->additional_fields['class'] = 'class';
    }
    elseif ($this->definition['field'] == 'race') {
      $this->additional_fields['raceId'] = 'raceId';
      $this->additional_fields['race'] = 'race';
      $this->additional_fields['genderId'] = 'genderId';
      $this->additional_fields['gender'] = 'gender';
    }
    elseif ($this->definition['field'] == 'guild_rank') {
      //$this->additional_fields['gid'] = 'gid';
      $this->additional_fields['guild_rank'] = 'guild_rank';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    if ($this->definition['field'] == 'class' || $this->definition['field'] == 'race') {
      $options['display_type'] = array('default' => 1);
    }
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if ($this->definition['field'] == 'class' || $this->definition['field'] == 'race') {
      $form['display_type'] = array(
        '#title' => t('Display as'),
        '#type' => 'radios',
        '#options' => array(0 => 'text', 1 => 'image'),
        '#description' => t('Display as plain text, or an 18x18 icon.'),
        '#default_value' => isset($this->options['display_type'])?$this->options['display_type']:1,
      );
    }
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  
  function click_sort($order) {
    $this->query->add_orderby($this->table, $this->field, $order, $this->field_alias);
  }
  
  function render($values) {
    if ($this->definition['field'] == 'class' || $this->definition['field'] == 'race') {
      if ($this->definition['field'] == 'class') {
        $txt = $values->wowtoon_cached_data_class;
        $img = sprintf('<img src="http://us.media.blizzard.com/wow/icons/18/class_%d.jpg" title="%s" width="18" height="18" />', $values->wowtoon_cached_data_classid, check_plain($values->wowtoon_cached_data_class));
      }
      elseif ($this->definition['field'] == 'race') {
        $txt = $values->wowtoon_cached_data_race;
        $img = sprintf('<img src="http://us.media.blizzard.com/wow/icons/18/race_%d_%d.jpg" title="%s" width="18" height="18" />', $values->wowtoon_cached_data_raceid, $values->wowtoon_cached_data_genderid, check_plain($values->wowtoon_cached_data_race));
      }
      //kpr($this->aliases, true)
      $display = isset($this->options['display_type'])?$this->options['display_type']:1;
      switch ($display) {
        case 0:
          return $txt;
          break;
        case 1:
        default:
          return $img;
      }
    }
    elseif ($this->definition['field'] == 'guild_rank') {
      $rank_names = variable_get('wowguild_guild_rank_names', array('Guild Master', 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Rank 7', 'Rank 8', 'Rank 9'));
      return empty($rank_names[$values->wowtoon_cached_data_guild_rank])?t('Rank') . ' ' . $values->wowtoon_cached_data_guild_rank:$rank_names[$values->wowtoon_cached_data_guild_rank];
    }
  }
}
