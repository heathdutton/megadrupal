<?php


class vr_campaign_filter_campaign_id extends views_handler_filter_many_to_one {
  var $value_form_type = 'select';

  function construct() {
    parent::construct();
    $this->value_title = t('Campaigns');
    $this->value_options = NULL;
  }

  /**
   * Child classes should be used to override this function and set the
   * 'value options', unless 'options callback' is defined as a valid function
   * or static public method to generate these values.
   *
   * This can use a guard to be used to reduce database hits as much as
   * possible.
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $campaigns = vr_api('enumerateEmailCampaigns');
    foreach ($campaigns as $campaign) {
      $this->value_options[$campaign->id] = $campaign->name;
    }
  }
}
