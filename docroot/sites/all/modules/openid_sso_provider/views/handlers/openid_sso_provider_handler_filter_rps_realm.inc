<?php
/**
 * Filter by node type
 *
 * @ingroup views_filter_handlers
 */
class openid_sso_provider_handler_filter_rps_realm extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('OpenID relying parties');
      $realms = openid_sso_provider_get_relying_parties();
      foreach ($realms as $realm => $info) {
        $options[$info->realm] = t($info->label);
      }
      asort($options);
      $this->value_options = $options;
    }
  }
}
