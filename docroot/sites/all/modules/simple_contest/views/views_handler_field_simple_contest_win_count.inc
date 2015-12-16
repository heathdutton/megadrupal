<?php

/**
 * Field handler for views_handler_field_simple_contest_win_count
 */
class views_handler_field_simple_contest_win_count extends views_handler_field_numeric {
  function init(&$view, &$options) {
    parent::init($view, $options);

    // translate an older setting:
    if (!empty($options['no_empty'])) {
      $this->options['hide_empty'] = TRUE;
      unset($this->options['no_empty']);
    }
  }

  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['contest_win_rank'] = array('default' => 0);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['contest_win_rank'] = array(
      '#title' => t('Winner rank'),
      '#description' => t("Calcuate count by win rank. Leave it as zero to ignore this setting."),
      '#type' => 'textfield',
      '#default_value' => $this->options['contest_win_rank'],
    );

    parent::options_form($form, $form_state);
  }

  function options_validate(&$form, &$form_state) {
    // number validation
    if ( !is_numeric($form_state['values']['options']['contest_win_rank']) ) {
      form_error($form['contest_win_rank'], t('Rank must be 0 or any integer.'));
    }
  }
  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    $this->field_alias = $this->table . '_' . $this->field;
  }

  function pre_render(&$values) {
    global $user;
    if (!$user->uid || empty($values)) {
      return;
    }

    $uids = array();
    $ids = array();
    foreach ($values as $id => $result) {
      $uids[] = $result->{$this->aliases['uid']};
      $values[$id]->{$this->field_alias} = 0;
      // Create a reference so we can find this record in the values again.
      if (empty($ids[$result->{$this->aliases['uid']}])) {
        $ids[$result->{$this->aliases['uid']}] = array();
      }
      $ids[$result->{$this->aliases['uid']}][] = $id;
    }

    if ($uids) {
      
      $args = array(
        ':uids' => $uids,
      );
      
      $sql = "SELECT u.uid, COUNT(sc.uid) as win_count FROM {users} u 
        INNER JOIN {simple_contest_results} sc ON u.uid = sc.uid
        WHERE u.uid IN (:uids) ";
      
      if ( isset($this->options['contest_win_rank']) && $this->options['contest_win_rank'] > 0 ) {
        $sql .= "AND sc.weight = :rank";
        $args[':rank'] = $this->options['contest_win_rank'];
      }
      
      $sql .= "GROUP BY u.uid";
      $result = db_query($sql, $args);
      
      foreach ($result as $users) {
        foreach ($ids[$users->uid] as $id) {
          $values[$id]->{$this->field_alias} = $users->win_count;
        }
      }
    }
  }

}
