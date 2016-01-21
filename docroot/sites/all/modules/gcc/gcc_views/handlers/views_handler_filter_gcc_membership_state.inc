<?php

/**
 * @file
 * Definition of views_handler_filter_gcc_membership_state.
 */

/**
 * TODO.
 */
class views_handler_filter_gcc_membership_state extends views_handler_filter_in_operator {

  /**
   * TODO.
   */
  public function construct() {

    parent::construct();

    $this->value_title = t('States');
    $this->value_options = array(

      GCC_PENDING => t('Pending'),
      GCC_ACTIVE => t('Active'),
      GCC_BLOCKED => t('Blocked'),
    );
  }
}
