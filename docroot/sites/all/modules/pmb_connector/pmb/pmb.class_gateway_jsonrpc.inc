<?php

/**
 * @file
 * PMB JSON-RPC gateway.
 */

require_once(drupal_get_path('module', 'pmb') . '/includes/pmb.jsonrpc_client.inc');
require_once(drupal_get_path('module', 'pmb') . '/pmb.class_gateway.inc');

class pmb_gateway_json_rpc extends pmb_gateway{
  protected $client;
  public $session_id;

  function __construct($server_url, $login = '', $password = '') {
    $this->client = new pmb_jsonrpc_client($server_url);
    if ($login || $password) {
      $this->client->setAuthenticationType('http');
      $this->client->setCredentials($login, $password);
    }
  }

  public function get_notice($notice_id) {
    $notice_id += 0;
    if (!$notice_id)
      return array();

    $notices = $this->get_notices(array($notice_id));

    $result = isset($notices[$notice_id]) ?
      $notices[$notice_id] :
      array();

    return $result;
  }

  public function get_notices($notices_ids) {
    $results = array();

    if (!$notices_ids)
      throw new Exception(t('Missing parameter: notices_ids'));

    // Remove non integer items.
    array_walk($notices_ids, create_function('&$a', '$a += 0;'));
    $notices_ids = array_unique($notices_ids);

    if ($this->session_id) {
      $notices = $this->client->pmbesOPACEmpr_fetchNoticeListFull($this->session_id, $notices_ids, 'serialized_unimarc_assoc', 'utf-8', TRUE);
    }
    else {
      $notices = $this->client->pmbesOPACAnonymous_fetchNoticeListFull($notices_ids, 'serialized_unimarc_assoc', 'utf-8', TRUE);
    }
    if (isset($notices)) {
      foreach ($notices as &$notice) {
        $results[$notice->noticeId] = array(
          'id' => $notice->noticeId,
          'notice' => unserialize($notice->noticeContent),
          'items' => $notice->noticeItems,
          'expl_nums' => $notice->noticeExplNums,
          'bulletins' => $notice->noticeBulletins,
        );
      }
    }

    return $results;
  }

  public function get_notice_external($notice_id) {
    $notice_id += 0;
    if (!$notice_id)
      return array();

    $notices = $this->get_notices_external(array($notice_id));

    $result = isset($notices[$notice_id]) ?
      $notices[$notice_id] :
      array();

    return $result;
  }

  public function get_notices_external($notices_ids) {
    $results = array();

    if (!$notices_ids)
      throw new Exception(t('Missing parameter: notices_ids'));

    // Remove non integer items.
    array_walk($notices_ids, create_function('&$a', '$a += 0;'));
    $notices_ids = array_unique($notices_ids);

    if ($this->session_id) {
      $notices = $this->client->pmbesOPACEmpr_fetchExternalNoticeList($this->session_id, $notices_ids, 'serialized_unimarc_assoc', 'utf-8');
    }
    else {
      $notices = $this->client->pmbesOPACAnonymous_fetchExternalNoticeList($notices_ids, 'serialized_unimarc_assoc', 'utf-8');
    }
    if (isset($notices)) {
      foreach ($notices as &$notice) {
        $results[$notice->noticeId] = array(
          'id' => $notice->noticeId,
          'notice' => unserialize($notice->noticeContent),
        );
      }
    }

    return $results;
  }

  public function get_serials() {
    $result = array();

    try {
      if ($this->session_id) {
        $serials = $this->client->pmbesOPACEmpr_fetchSerialList($this->session_id);
      }
      else {
        $serials = $this->client->pmbesOPACAnonymous_fetchSerialList();
      }
      $result = $serials;
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_bulletin($bulletin_id) {
    $bulletin_id += 0;
    if (!$bulletin_id)
      return array();

    $bulletins = $this->get_bulletins(array($bulletin_id));

    $result = isset($bulletins[$bulletin_id]) ?
      $bulletins[$bulletin_id] :
      array();

    return $result;
  }

  public function get_bulletins($bulletins_ids) {
    $results = array();

    if (!$bulletins_ids)
      throw new Exception(t('Missing parameter: notices_ids'));

    // Remove non integer items.
    array_walk($bulletins_ids, create_function('&$a', '$a += 0;'));
    $bulletins_ids = array_unique($bulletins_ids);

    if (!$bulletins_ids)
      return array();

    try {
      if ($this->session_id) {
        $bulletins = $this->client->pmbesOPACEmpr_fetchBulletinListFull($this->session_id, $bulletins_ids, 'serialized_unimarc_assoc', 'utf-8');
      }
      else {
        $bulletins = $this->client->pmbesOPACAnonymous_fetchBulletinListFull($bulletins_ids, 'serialized_unimarc_assoc', 'utf-8');
      }

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    if (isset($bulletins)) {
      foreach ($bulletins as &$bulletin) {
        $results[$bulletin->bulletin_id] = array(
          'id' => $bulletin->bulletin_id,
          'bulletin' => $bulletin->bulletin_bulletin,
          'notice' => ($bulletin->bulletin_notice) ? unserialize($bulletin->bulletin_notice) : array(),
          'items' => $bulletin->bulletin_items,
          'expl_nums' => $bulletin->bulletin_doc_nums,
          'analysis' => $bulletin->bulletin_analysis_notice_ids,
        );
      }
    }

    return $results;
  }

  public function find_notice_bulletin($notice_id) {
    $notice_id += 0;
    if (!$notice_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $bulletin_id = $this->client->pmbesOPACEmpr_findNoticeBulletinId($this->session_id, $notice_id);
      }
      else {
        $bulletin_id = $this->client->pmbesOPACAnonymous_findNoticeBulletinId($notice_id);
      }

      if (!$bulletin_id)
        return FALSE;

      $result = $bulletin_id;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_author($author_id) {
    $author_id += 0;
    if (!$author_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $author = $this->client->pmbesOPACEmpr_get_author_information_and_notices($this->session_id, $author_id);
      }
      else {
        $author = $this->client->pmbesOPACAnonymous_get_author_information_and_notices($author_id);
      }

      if (!$author)
        return FALSE;

      $result = $author;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_publisher($publisher_id) {
    $publisher_id += 0;
    if (!$publisher_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $publisher = $this->client->pmbesOPACEmpr_get_publisher_information_and_notices($this->session_id, $publisher_id);
      }
      else {
        $publisher = $this->client->pmbesOPACAnonymous_get_publisher_information_and_notices($publisher_id);
      }

      if (!$publisher)
        return FALSE;

      $result = $publisher;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_serie($serie_id) {
    $serie_id += 0;
    if (!$serie_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $serie = $this->client->pmbesOPACEmpr_get_collection_information_and_notices($this->session_id, $serie_id);
      }
      else {
        $serie = $this->client->pmbesOPACAnonymous_get_collection_information_and_notices($serie_id);
      }

      if (!$serie)
        return FALSE;

      $result = $serie;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_subserie($subserie_id) {
    $subserie_id += 0;
    if (!$subserie_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $subserie = $this->client->pmbesOPACEmpr_get_subcollection_information_and_notices($this->session_id, $subserie_id);
      }
      else {
        $subserie = $this->client->pmbesOPACAnonymous_get_subcollection_information_and_notices($subserie_id);
      }

      if (!$subserie)
        return FALSE;

      $result = $subserie;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_locations_and_sections() {
    $result = array();
    try {
      $locations_and_sections = $this->client->pmbesOPACGeneric_get_all_locations_and_sections();
      foreach ($locations_and_sections as $alocation) {
        $result[$alocation->location->location_id] = $alocation;
      }
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_section_notices_ids($section_id) {
    $search = array();
    $search[] = array(
      'inter' => 'and',
      'field' => 17,
      'operator' => 'EQ',
      'value' => $section_id,
    );
    if ($this->session_id) {
      $search_res = $this->client->pmbesOPACEmpr_advancedSearch($this->session_id, $search);
    }
    else {
      $search_res = $this->client->pmbesOPACAnonymous_advancedSearch($search);
    }
    $results = array();
    if ($search_res && isset($search_res->nbResults)) {

      if ($this->session_id) {
        $search_res_nids = $this->client->pmbesOPACEmpr_fetchSearchRecordsSorted($this->session_id, $search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', 'c_text_1');
      }
      else {
        $search_res_nids = $this->client->pmbesOPACAnonymous_fetchSearchRecordsSorted($search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', 'c_text_1');
      }
      if ($search_res_nids) {
        foreach ($search_res_nids as $anotice) {
          $results[] = $anotice->noticeId;
        }
      }
    }
    return $results;
  }

  public function get_shelves() {
    $result = array();
    try {
      if ($this->session_id) {
        $shelves = $this->client->pmbesOPACEmpr_list_shelves($this->session_id);
      }
      else {
        $shelves = $this->client->pmbesOPACAnonymous_list_shelves();
      }

      $result = $shelves;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_shelf_notices_ids($shelf_id) {
    $shelf_id += 0;
    if (!$shelf_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $ids = $this->client->pmbesOPACEmpr_retrieve_shelf_content($this->session_id, $shelf_id);
      }
      else {
        $ids = $this->client->pmbesOPACAnonymous_retrieve_shelf_content($shelf_id);
      }

      if (!$ids)
        return FALSE;

      $result = $ids;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_thesauri() {
    $result = array();
    try {
      if ($this->session_id) {
        $thesauri = $this->client->pmbesOPACEmpr_list_thesauri($this->session_id);
      }
      else {
        $thesauri = $this->client->pmbesOPACAnonymous_list_thesauri();
      }

      $result = $thesauri;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_category($category_id) {
    $category_id += 0;
    if (!$category_id)
      return array();
    $result = array();

    try {
      if ($this->session_id) {
        $category = $this->client->pmbesOPACEmpr_fetch_thesaurus_node_full($this->session_id, $category_id);
      }
      else {
        $category = $this->client->pmbesOPACAnonymous_fetch_thesaurus_node_full($category_id);
      }

      if (!$category)
        return FALSE;

      $result = $category;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_search_fields() {
    $result = array();
    //on récupère la langue de drupal
    global $language;
    try {
      $result = $this->client->pmbesOPACAnonymous_getAdvancedSearchFields('fr_FR', TRUE);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_search_notices_ids($search_terms, $search_fields) {
    if (!$search_terms)
      return array();
    if (!$search_fields)
      return array();

    $field_mapping = array(
      'all_fields' => 42,
      'title' => 1,
      'author' => 2,
      'publisher' => 3,
      'serie' => 4,
      'subserie' => 5,
      'category' => 11,
      'indexint' => 12,
    );
    $search = array();
    foreach ($search_fields as $asearch_field) {
      if (!isset($field_mapping[$asearch_field]))
        continue;
      $search[] = array(
        'inter' => 'or',
        'field' => $field_mapping[$asearch_field],
        'operator' => 'BOOLEAN',
        'value' => $search_terms,
      );
    }

    $results = array();
    try {
      if ($this->session_id) {
        $search_res = $this->client->pmbesOPACEmpr_advancedSearch($this->session_id, $search);
      }
      else {
        $search_res = $this->client->pmbesOPACAnonymous_advancedSearch($search);
      }
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    try {
      if ($search_res && isset($search_res->nbResults)) {
        if ($this->session_id) {
          $search_res_nids = $this->client->pmbesOPACEmpr_fetchSearchRecordsSorted($this->session_id, $search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', 'd_num_6');
        }
        else {
          $search_res_nids = $this->client->pmbesOPACAnonymous_fetchSearchRecordsSorted($search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', 'd_num_6');
        }
        if ($search_res_nids) {
          foreach ($search_res_nids as $anotice) {
            $results[] = $anotice->noticeId;
          }
        }
      }
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $results;
  }

  public function get_search_notice_external_ids($search_terms, $search_fields, $source_ids) {
    if (!$search_terms)
      return array();
    if (!$search_fields)
      return array();

    $field_mapping = array(
      'all_fields' => 42,
      'title' => 1,
      'author' => 2,
      'publisher' => 3,
      'serie' => 4,
      'subserie' => 5,
      'isbn_bar_code' => 22,
    );
    $field_mapping_operators = array(
      'all_fields' => 'BOOLEAN',
      'title' => 'BOOLEAN',
      'author' => 'BOOLEAN',
      'publisher' => 'BOOLEAN',
      'serie' => 'BOOLEAN',
      'subserie' => 'BOOLEAN',
      'isbn_bar_code' => 'STARTWITH',
    );
    $search = array();
    foreach ($search_fields as $asearch_field) {
      if (!isset($field_mapping[$asearch_field]))
        continue;
      $search[] = array(
        'inter' => 'or',
        'field' => $field_mapping[$asearch_field],
        'operator' => $field_mapping_operators[$asearch_field],
        'value' => array($search_terms),
      );
    }

    $results = array();
    try {
      if ($this->session_id) {
        $search_res = $this->client->pmbesOPACEmpr_advancedSearchExternal($this->session_id, $search, $source_ids);
      }
      else {
        $search_res = $this->client->pmbesOPACAnonymous_advancedSearchExternal($search, $source_ids);
      }
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    try {
      if ($search_res && isset($search_res->nbResults)) {
        if ($this->session_id) {
          $search_res_nids = $this->client->pmbesOPACEmpr_fetchSearchRecordsSorted($this->session_id, $search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', '');
        }
        else {
          $search_res_nids = $this->client->pmbesOPACAnonymous_fetchSearchRecordsSorted($search_res->searchId, 0, $search_res->nbResults, 'notice_id', 'utf-8', '');
        }
        if ($search_res_nids) {
          foreach ($search_res_nids as $anotice) {
            $results[] = $anotice->noticeId;
          }
        }
      }
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $results;
  }

  public function get_search_external_sources() {
    $result = array();
    try {
      if ($this->session_id) {
        $result = $this->client->pmbesOPACEmpr_listExternalSources($this->session_id);
      }
      else {
        $result = $this->client->pmbesOPACAnonymous_listExternalSources();
      }

      if (!$result)
        return FALSE;

    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function get_search_advanced_fields($fetch_values) {
    $result = array();
    //on récupère la langue de drupal
    global $language;
    try {
      $result = $this->client->pmbesOPACAnonymous_getAdvancedSearchFields('fr_FR', TRUE);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }

    return $result;
  }

  public function reader_login($login, $password) {
    $result = '';
    try {
      $result = $this->client->pmbesOPACEmpr_login($login, $password);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_login_md5($login, $password) {
    $result = '';
    try {
      $result = $this->client->pmbesOPACEmpr_login_md5($login, $password);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_login_crypt($login, $password, $mode) {
    $result = '';
    try {
      $result = $this->client->pmbesOPACEmpr_login_crypt($login, $password, $mode);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_login_authenticated($login, $mode) {
    $result = '';
    try {
      $result = $this->client->pmbesOPACEmpr_login_authenticated($login, $mode);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_logout() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_logout($this->session_id);
      // PMB logout function doesn't provide any information on success/failure.
      $result = TRUE;
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_check_session() {
    if (!$this->session_id)
      return array();
    $result = array();
    try {
      $result = $this->client->pmbesOPACEmpr_retrieve_session_information($this->session_id, FALSE);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_change_password($old_password, $new_password) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_change_password($this->session_id, $old_password, $new_password);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_account_info() {
    if (!$this->session_id)
      return array();
    $result = array();
    try {
      $result = $this->client->pmbesOPACEmpr_get_account_info($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_cart() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_list_cart_content($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_add_notices_to_cart($notices_ids) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_add_notices_to_cart($this->session_id, $notices_ids);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_delete_notices_from_cart($notices_ids) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_delete_notices_from_cart($this->session_id, $notices_ids);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_empty_cart() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_empty_cart($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_can_reserve_notice($notice_id, $bulletin_id) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_can_reserve_notice($this->session_id, $notice_id, $bulletin_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_reservations() {
    if (!$this->session_id)
      return array();
    $result = array();
    try {
      $result = $this->client->pmbesOPACEmpr_list_resas($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_reservation_locations($notice_id, $bulletin_id) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_list_resa_locations($this->session_id, $notice_id, $bulletin_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_add_reservation($notice_id, $bulletin_id, $location_id) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_add_resa($this->session_id, $notice_id, $bulletin_id, $location_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result->status;
  }

  public function reader_delete_reservation($reservation_id) {
    if (!$this->session_id)
      return array();
    $result = array();
    try {
      $result = $this->client->pmbesOPACEmpr_delete_resa($this->session_id, $reservation_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_loans() {
    if (!$this->session_id)
      return array();
    $result = array();
    try {
      $result = $this->client->pmbesOPACEmpr_list_loans($this->session_id, 1);
      //$result['late'] = $this->client->pmbesOPACEmpr_list_loans($this->session_id, 0);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_reading_lists() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_getReadingLists($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_add_notices_to_reading_list($list_id, $notices_ids) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_addNoticesToReadingList($this->session_id, $list_id, $notices_ids);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_remove_notices_to_reading_list($list_id, $notices_ids) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_removeNoticesFromReadingList($this->session_id, $list_id, $notices_ids);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_empty_reading_list($list_id) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_emptyReadingList($this->session_id, $list_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_reading_lists_public() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_getPublicReadingLists($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_suggestions() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_list_suggestions($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_get_suggestion_sources_and_categories() {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_list_suggestion_sources_and_categories($this->session_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_add_suggestion($suggestion) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_add_suggestion2($this->session_id, $suggestion);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_edit_suggestion($suggestion) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_edit_suggestion($this->session_id, $suggestion);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }

  public function reader_delete_suggestion($suggestion_id) {
    if (!$this->session_id)
      return FALSE;
    $result = FALSE;
    try {
      $result = $this->client->pmbesOPACEmpr_delete_suggestion($this->session_id, $suggestion_id);
    } catch (Exception $e) {
      drupal_set_message(t('JSON-RPC error: @message [!function].', array('@message' => $e->getMessage(), '!function' => __FUNCTION__)), 'error');
    }
    return $result;
  }
}
