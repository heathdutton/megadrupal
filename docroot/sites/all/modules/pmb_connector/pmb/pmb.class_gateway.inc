<?php

/**
 * @file
 * PMB gateway class.
 */

abstract class pmb_gateway {
  protected $session_id = '';
  public function set_session_id($session_id) {
    $this->session_id = $session_id;
  }
  public function unset_session_id() {
    $this->session_id = '';
  }

  abstract public function get_notice($notice_id);
  abstract public function get_notices($notices_ids);
  abstract public function get_notice_external($notice_id);
  abstract public function get_notices_external($notices_ids);
  abstract public function get_serials();
  abstract public function get_bulletin($bulletin_id);
  abstract public function get_bulletins($bulletins_ids);
  abstract public function find_notice_bulletin($notice_id);
  abstract public function get_author($author_id);
  abstract public function get_publisher($publisher_id);
  abstract public function get_serie($serie_id);
  abstract public function get_subserie($subserie_id);

  abstract public function get_locations_and_sections();
  abstract public function get_section_notices_ids($section_id);
  abstract public function get_shelves();
  abstract public function get_shelf_notices_ids($shelf_id);
  abstract public function get_thesauri();
  abstract public function get_category($category_id);

  abstract public function get_search_fields();
  abstract public function get_search_notices_ids($search_terms, $search_fields);
  abstract public function get_search_notice_external_ids($search_terms, $search_fields, $source_ids);
  abstract public function get_search_external_sources();
  abstract public function get_search_advanced_fields($fetch_values);

  abstract public function reader_login($login, $password);
  abstract public function reader_login_md5($login, $password);
  abstract public function reader_login_crypt($login, $password, $hmode);
  abstract public function reader_login_authenticated($login, $mode);
  abstract public function reader_logout();
  abstract public function reader_check_session();
  abstract public function reader_change_password($old_password, $new_password);
  abstract public function reader_get_account_info();
  abstract public function reader_get_cart();
  abstract public function reader_add_notices_to_cart($notices_ids);
  abstract public function reader_delete_notices_from_cart($notices_ids);
  abstract public function reader_empty_cart();
  abstract public function reader_can_reserve_notice($notice_id, $bulletin_id);
  abstract public function reader_get_reservations();
  abstract public function reader_get_reservation_locations($notice_id, $bulletin_id);
  abstract public function reader_add_reservation($notice_id, $bulletin_id, $location_id);
  abstract public function reader_delete_reservation($reservation_id);
  abstract public function reader_get_loans();
  abstract public function reader_get_reading_lists();
  abstract public function reader_get_reading_lists_public();
  abstract public function reader_add_notices_to_reading_list($list_id, $notices_ids);
  abstract public function reader_remove_notices_to_reading_list($list_id, $notices_ids);
  abstract public function reader_empty_reading_list($list_id);
  abstract public function reader_get_suggestions();
  abstract public function reader_get_suggestion_sources_and_categories();
  abstract public function reader_add_suggestion($suggestion);
  abstract public function reader_edit_suggestion($suggestion);
  abstract public function reader_delete_suggestion($suggestion_id);
}

class pmb_gateway_factory {
  protected static $gateway = NULL;

  function __construct() {
    if (empty($this->gateway)) {
      $this->gateway = NULL;
      $link_type = pmb_variable_get('pmb_link_type');
      switch ($link_type) {
        case 'json-rpc':
          $server = pmb_variable_get('pmb_link_serverurl');
          $login = pmb_variable_get('pmb_link_serveruser');
          $password = pmb_variable_get('pmb_link_serverpassword');
          require_once(drupal_get_path('module', 'pmb') . '/pmb.class_gateway_jsonrpc.inc');
          $this->gateway = new pmb_gateway_json_rpc($server, $login, $password);
          break;

        default:
          break;
      }
    }
  }

  function get_gateway() {
    return $this->gateway;
  }
}