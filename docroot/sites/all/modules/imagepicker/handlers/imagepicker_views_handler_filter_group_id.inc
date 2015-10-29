<?php

/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Group name filter handler.
 */
class imagepicker_views_handler_filter_group_id extends views_handler_filter_string {

  function option_definition() {
    $options = parent::option_definition();
    $options['value'] = array('default' => 0);
    return $options;
  }

  function operators() {
    $operators = parent::operators();
    unset($operators['regular_expression']);
    unset($operators['not empty']);
    unset($operators['empty']);
    unset($operators['longerthan']);
    unset($operators['shorterthan']);
    unset($operators['not']);
    unset($operators['not_ends']);
    unset($operators['ends']);
    unset($operators['not_starts']);
    unset($operators['starts']);
    unset($operators['allwords']);
    unset($operators['word']);
    unset($operators['contains']);
    unset($operators['!=']);

    return $operators;
  }

  function value_form(&$form, &$form_state) {
    $opts = array();
    $public_enabled = imagepicker_variable_get('imagepicker_public_enabled', 1);
    if ($public_enabled) {
      module_load_include('inc', 'imagepicker', 'imagepicker.group');
      $grouplist = array('0' => 'All');
      $query = db_select('imagepicker_user_groups', 'g');
      $query->fields('g', array('gid', 'group_name'));
      $query->distinct();
      $query->join('imagepicker_group_images', 'i', 'g.gid = i.gid');
      $query->condition('g.public', 1);
      $rows = $query->execute();
      foreach ($rows AS $row) {
        $ct = imagepicker_get_group_images_count($row->gid);
        $group_name = $row->group_name;
        $grouplist[$row->gid] = $row->gid . ' - ' . t('!ct images', array('!ct' => $ct)) . ' - ' . t('Name: !n', array('!n' => $group_name));
      }
      if (count($grouplist) > 1) {
        $form['value'] = array(
          '#type' => 'select',
          '#title' => t('Group name'),
          '#options' => $grouplist,
          '#default_value' => $this->value,
        );
      }
      else {
        $form['value'] = array('#markup' => t('There are no Public groups available.'));
      }
    }
    else {
      $form['value'] = array('#markup' => t('Public groups has not been enabled.'));
    }
  }
}
