<?php
/**
 * Argument handler to accept a realm.
 */
class openid_sso_provider_handler_argument_rps_realm extends views_handler_argument {
  function construct() {
    parent::construct('realm');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly label
   * of the realm.
   */
  function summary_name($data) {
    return $this->openid_sso_provider_realm_info($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly label for the
   * realm.
   */
  function title() {
    return $this->openid_sso_provider_realm_info($this->argument);
  }

  function openid_sso_provider_realm_info($realm_url) {
    $realms = openid_sso_provider_get_relying_parties();

    foreach ($realms as $realm => $info) {
      $cleaned_realm = rtrim(preg_replace('/^(http)s?:\/+/i', '', $info->realm), "/");
      if ($realm_url == $cleaned_realm) {
        $output = $info->label;
      }
    }

    if (empty($output)) {
      $output = t('Unknown realm');
    }
    return check_plain($output);
  }


  /**
   * Override setting up the query for this argument.
   */
  function query($group_by = FALSE) {
    $this->ensure_my_table();

    // Figure out the real realm url by the limited argument (missing http and https)
    $realms = openid_sso_provider_get_relying_parties();
    foreach ($realms as $realm => $info) {
      $cleaned_realm = rtrim(preg_replace('/^(http)s?:\/+/i', '', $info->realm), "/");
      if ($this->argument == $cleaned_realm) {
        $this->argument = $info->realm;
      }
    }

    $this->query->add_where(0, "$this->table_alias.$this->real_field", $this->argument);
  }
}
