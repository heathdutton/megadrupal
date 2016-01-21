<?php
// $Id: userpoints_nodeaccess_handler_filter_access.inc,v 1.2 2010/11/23 14:24:30 berdir Exp $

/**
 * @file
 * Views handler: Allow to filter out nodes the user has not access to.
 */

/**
 * Allow to filter out nodes the user has not access to.
 */
class userpoints_nodeaccess_handler_filter_access extends views_handler_filter {

  /**
   * This filter has no operators, only options.
   */
  protected $no_operator = TRUE;

  /**
   * No admin summary supported.
   */
  function admin_summary() {
  }

  /**
   * Do not add the prefix/suffix to the value form..
   */
  public function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Options'),
      '#default_value' => $this->value,
      '#options' => array(
        'can-buy' => t('Show nodes the current user can buy immediately.'),
        'could-buy' => t('Show nodes the current user could buy if he had more !points.', userpoints_translation()),
        'no-points' => t("Show nodes that don't require !points.", userpoints_translation()),
      ),
    );
  }

  /**
   * Enforces that either one of the options is selected.
   */
  public function value_validate($form, &$form_state) {
    $options = array_filter($form_state['values']['options']['value']);

    // Either can-buy or could-buy must be selected (or both).
    if (!isset($options['can-buy']) && !isset($options['could-buy'])) {
      form_set_error('options][value', t('Select either nodes the user can buy, nodes the user could buy or both.'));
    }
  }

  /**
   * Add filters to the query.
   */
  public function query() {
    global $user;
    $this->ensure_my_table();

    // Always exclude nodes the user has already bought.
    $this->query->add_where($this->options['group'], 'NOT EXISTS (SELECT upna.nid FROM {userpoints_nodeaccess} upna WHERE upna.nid = ' . $this->table_alias . '.nid AND upna.uid = :upnauid)', array(':upnauid' => $user->uid), 'formula');

    // Get the enabled options.
    $options = array_filter($this->value);

    $access_conditions = db_or();
    // If can-buy option is checked and could buy not, limit nodes to those to
    // which the user has enough points.
    // If both options are checked, no conditions need to be added because no
    // nodes are limited.
    if (isset($options['can-buy']) && !isset($options['could-buy'])) {
      // Add a condition for every category. Don't include categories in which
      // the user has no points.
      foreach (array_keys(userpoints_get_categories()) as $tid) {
        if ($points_in_category = userpoints_get_current_points($user->uid, $tid)) {
          $access_conditions->condition(db_and()
            ->condition($this->table_alias . '.tid', $tid)
            ->condition($this->table_alias . '.points', $points_in_category, '<=')
          );
        }
      }
    }
    elseif (!isset($options['can-buy']) && isset($options['could-buy'])) {
      // Add a condition for every category. Also include categories in which
      // the user has no points.
      foreach (array_keys(userpoints_get_categories()) as $tid) {
        $access_conditions->condition(db_and()
          ->condition($this->table_alias . '.tid', $tid)
          ->condition($this->table_alias . '.points', userpoints_get_current_points($user->uid, $tid), '>')
        );
      }
    }

    // If no points option is set and only could-buy is, explicitly allow
    // nodes with 0 points as they would be excluded otherwise.
    if (isset($options['no-points']) && isset($options['could-buy'])) {
      $access_conditions->isNull($this->table_alias . '.points');
      $access_conditions->condition($this->table_alias . '.points', 0);
    }

    // If either can-buy or could-buy is set and no point requirements are defined,
    // then there are no matching nodes. Hide everything by adding a condition
    // that can never be TRUE.
    if ((isset($options['can-buy']) xor isset($options['could-buy'])) && !count($access_conditions)) {
      $this->query->add_where($this->options['group'], 1, 2);
    }
    elseif (count($access_conditions)) {
      $this->query->add_where($this->options['group'], $access_conditions);
    }

    // If the no-points option is not set but can buy is, exlude nodes that have
    // 0 or NULL points.
    if (!isset($options['no-points']) && isset($options['can-buy'])) {
      $this->query->add_where($this->options['group'], $this->table_alias . '.points IS NOT NULL AND ' . $this->table_alias . '.points <> 0', array(), 'formula');
    }
  }
}