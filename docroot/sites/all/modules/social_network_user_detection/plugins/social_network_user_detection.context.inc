<?php
/**
 * @file
 * File with classes that extends context condition.
 */

class SocialNetworkUserDetectionSocialActionFb extends context_condition {

  /**
   * Build a configuration form. Each condition needs this. Required.
   */
  function condition_form($context) {
    return array(
      'social_network_user_detection_context_action_fb' => array(
        '#type' => 'value',
        '#value' => TRUE,
      ),
    );
  }

  /**
   * Configuration form submit function, also required by Context.
   */
  function condition_form_submit($values) {
    return array('social_network_user_detection_context_action_fb' => 1);
  }

  /**
   * Check if Context condition is active,Not called auto by context.
   */
  function execute() {
    $cond = FALSE;
    if ($this->condition_used()) {
      // Add the code for your condition below.
      if (isset($_SESSION['Facebook']) && $_SESSION['Facebook'] == TRUE) {
        $cond = TRUE;
      }
      foreach ($this->get_contexts() as $context) {
        $if_theme = $this->fetch_from_context($context, 'values');
        if ($cond) {
          $this->condition_met($context);
        }
      }
    }
  }
}

class SocialNetworkUserDetectionSocialActionTwitter extends context_condition {

  /**
   * Build a configuration form. Each condition needs this.
   */
  function condition_form($context) {
    return array(
      'social_network_user_detection_context_action_twitter' => array(
        '#type' => 'value',
        '#value' => TRUE,
      ),
    );
  }

  /**
   * Configuration form submit function, also required by Context.
   */
  function condition_form_submit($values) {
    return array('social_network_user_detection_context_action_twitter' => 1);
  }

  /**
   * Check if Context condition is active,Not called auto by context.
   */
  function execute() {
    $cond = FALSE;
    if ($this->condition_used()) {
      // Add the code for your condition below.
      if (isset($_SESSION['Twitter']) && $_SESSION['Twitter'] == TRUE) {
        $cond = TRUE;
      }
      foreach ($this->get_contexts() as $context) {
        $if_theme = $this->fetch_from_context($context, 'values');
        if ($cond) {
          $this->condition_met($context);
        }
      }
    }
  }
}

class SocialNetworkUserDetectionSocialActionGplus extends context_condition {

  /**
   * Build a configuration form. Each condition needs this.
   */
  function condition_form($context) {
    return array(
      'social_network_user_detection_context_action_gplus' => array(
        '#type' => 'value',
        '#value' => TRUE,
      ),
    );
  }

  /**
   * Configuration form submit function, also required by Context.
   */
  function condition_form_submit($values) {
    return array('social_network_user_detection_context_action_gplus' => 1);
  }

  /**
   * Check if Context condition is active,Not called auto by context.
   */
  function execute() {
    $cond = FALSE;
    if ($this->condition_used()) {
      // Add the code for your condition below.
      if (isset($_SESSION['GooglePlus']) && $_SESSION['GooglePlus'] == TRUE) {
        $cond = TRUE;
      }
      foreach ($this->get_contexts() as $context) {
        $if_theme = $this->fetch_from_context($context, 'values');
        if ($cond) {
          $this->condition_met($context);
        }
      }
    }
  }
}

class SocialNetworkUserDetectionSocialActionGmail extends context_condition {

  /**
   * Build a configuration form. Each condition needs this.
   */
  function condition_form($context) {
    return array(
      'social_network_user_detection_context_action_gmail' => array(
        '#type' => 'value',
        '#value' => TRUE,
      ),
    );
  }

  /**
   * Configuration form submit function, also required by Context.
   */
  function condition_form_submit($values) {
    return array('social_network_user_detection_context_action_gmail' => 1);
  }

  /**
   * Check if Context condition is active,Not called auto by context.
   */
  function execute() {
    $cond = FALSE;
    if ($this->condition_used()) {
      // Add the code for your condition below.
      if (isset($_SESSION['Google']) && $_SESSION['Google'] == TRUE) {
        $cond = TRUE;
      }
      foreach ($this->get_contexts() as $context) {
        $if_theme = $this->fetch_from_context($context, 'values');
        if ($cond) {
          $this->condition_met($context);
        }
      }
    }
  }
}

class SocialNetworkUserDetectionSocialActionHyves extends context_condition {

  /**
   * Build a configuration form. Each condition needs this.
   */
  function condition_form($context) {
    return array(
      'social_network_user_detection_context_action_hyves' => array(
        '#type' => 'value',
        '#value' => TRUE,
      ),
    );
  }

  /**
   * Configuration form submit function, also required by Context.
   */
  function condition_form_submit($values) {
    return array('social_network_user_detection_context_action_hyves' => 1);
  }

  /**
   * Check if Context condition is active,Not called auto by context.
   */
  function execute() {
    $cond = FALSE;
    if ($this->condition_used()) {
      // Add the code for your condition below.
      if (isset($_SESSION['Hyves']) && $_SESSION['Hyves'] == TRUE) {
        $cond = TRUE;
      }
      foreach ($this->get_contexts() as $context) {
        $if_theme = $this->fetch_from_context($context, 'values');
        if ($cond) {
          $this->condition_met($context);
        }
      }
    }
  }
}
