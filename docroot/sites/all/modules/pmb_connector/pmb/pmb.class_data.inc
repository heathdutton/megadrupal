<?php

/**
 * @file
 * PMB main class.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_gateway.inc');

class pmb_data {
  protected $gateway;
  protected $reader = NULL;
  protected $cache_profile = 'cache_anonymous_all';

  public function __construct() {
    $factory = new pmb_gateway_factory();
    $this->gateway = $factory->get_gateway();
    $this->cache_profile = pmb_variable_get('pmb_user_cache_profile');

    $this->flag_fields_sine = pmb_variable_get('pmb_notice_sine');
  }

  /**
   * Set user only if he is a reader and not anonymous.
   *
   * @param object $user
   *
   * @return
   *   TRUE in case of success, else FALSE.
   */
  public function set_reader($user) {
    if (isset($user->uid) && isset($user->data['pmb_reader']['credentials'])) {
      $this->reader = $user;
      return TRUE;
    }
    else {
      $this->reader = NULL;
      return FALSE;
    }
  }

  /**
   * Check if a user is a reader or not.
   *
   * @param object $account
   *
   * @return
   *   TRUE in case of success, else FALSE.
   */
  public function check_reader($account) {
    return (isset($account->uid) && isset($account->data['pmb_reader']['credentials'])) ? TRUE : FALSE;
  }

  /**
   * Check if a PMB session is opened for a user.
   *
   * The gateway and the user need to be checked before use of this function.
   *
   * @return
   *   Session id in case of success, else FALSE.
   */
  protected function reader_check_session() {
    if (isset($this->reader->data['pmb_reader']['session']['sessid'])) {
      // A session is set and not expired.
      $this->gateway->set_session_id($this->reader->data['pmb_reader']['session']['sessid']);

      if ((REQUEST_TIME - $this->reader->data['pmb_reader']['session']['sessid_time']) < (pmb_variable_get('pmb_reader_session_duration') - 5)) {
        return $this->reader->data['pmb_reader']['session']['sessid'];
      }

      // @todo Currently, PMB retrieve_session_information() doesn't return
      // anything.
//       // A session is set, but locally expired. Try to extend remote session.
//       $result = $this->gateway->reader_check_session($this->reader->data['pmb_reader']['session']['sessid']);
//       if ($result) {
//         // Update session infos for cache in case of local session expires.
//         if ($this->reader) {
//           $to_save = array('pmb_reader' => $user->data['pmb_reader']);
//           $to_save['pmb_reader']['session']['sessid_time'] = REQUEST_TIME;
//           user_save($this->reader, $to_save);
//         }
//
//         return $this->reader->data['pmb_reader']['session']['sessid'];
//       }
    }

    // No session or expired local session.
    $this->gateway->unset_session_id();

    // Open a new session to get sessid.
    return $this->reader_get_session_id(
      $this->reader->data['pmb_reader']['credentials']['login'],
      $this->reader->data['pmb_reader']['credentials']['password'],
      $this->reader->data['pmb_reader']['credentials']['mode']
    );
  }

  /**
   * Create cache id(s) using the cache profile from one or multiple items.
   *
   * @param string $base
   *   The string to add to cache id.
   * @param $item
   *   Can either be a single string or an array of strings.
   *
   * @return
   *   Cache id(s) of string(s).
   */
  protected function get_cache_id($base, $item = '') {
    // No prefix for anonymous or non-reader.
    if (!$this->reader || $this->cache_profile == 'cache_anonymous_all') {
      $prefix = '';
    }
    else {
      $prefix = ($this->cache_profile == 'cache_anonymous_reader_all') ?
        // Same prefix for all non-anonymous readers.
        'readers:' :
        // One prefix for each reader.
        'reader:' . $this->reader->uid . ':';
    }

    if (is_array($item)) {
      // Foreach is quicker than array_walk.
      foreach ($item as &$value) {
        $value = $prefix . $base . ':' . $value;
      }
      return $item;
    }
    else {
      return $prefix . $base . ':' . $item;
    }
  }

  protected function delete_old_cache_objects() {
    cache_clear_all(NULL, 'cache_pmb_connector');
  }

  public function get_notice($notice_id) {
    if (!$this->gateway)
      return array();

    // Check if $notice_id is an integer.
    $notice_id += 0;
    if (!$notice_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('notice', $notice_id);
    $notice = cache_get($cid, 'cache_pmb_connector');
    if ($notice) {
      return $notice->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $notice = $this->gateway->get_notice($notice_id);
    if ($notice) {
      // Add readable, displayable and checked fields.
      $notice['fields'] = $this->get_notice_fields($notice);

      cache_set($cid, $notice, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_notice'));
    }
    return $notice;
  }

  public function get_notices($notices_ids) {
    if (!$this->gateway)
      return array();

    if (!$notices_ids)
      return array();

    // Remove non integer items.
    array_walk($notices_ids, create_function('&$a', '$a += 0;'));
    $notices_ids = array_unique($notices_ids);
    if (!$notices_ids)
      return array();

    $this->delete_old_cache_objects();

    $results = array();
    $cids = $this->get_cache_id('notice', $notices_ids);
    $notices = cache_get_multiple($cids, 'cache_pmb_connector');
    if ($notices) {
      foreach ($notices as $notice) {
        $results[$notice->data['id']] = $notice->data;
      }
    }

    // Get non cached notices if any.
    $notices_ids = array_diff($notices_ids, array_keys($results));
    if ($notices_ids) {
      if ($this->reader)
        $this->reader_check_session();

      $cache_duration = pmb_variable_get('pmb_cache_duration_notice');
      $notices = $this->gateway->get_notices($notices_ids);
      foreach ($notices as $notice) {
        // Is this check useful?
        if ($notice) {
          // Add readable, displayable and checked fields.
          $notice['fields'] = $this->get_notice_fields($notice);

          $results[$notice['id']] = $notice;
          cache_set($this->get_cache_id('notice', $notice['id']), $notice, 'cache_pmb_connector', REQUEST_TIME + $cache_duration);
        }
      }
    }

    return $results;
  }

  public function get_notice_external($notice_id) {
    if (!$this->gateway)
      return array();

    $notice_id += 0;
    if (!$notice_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('notice_external', $notice_id);
    $notice = cache_get($cid, 'cache_pmb_connector');
    if ($notice) {
      return $notice->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $notice = $this->gateway->get_notice_external($notice_id);
    if ($notice) {
      // Add readable, displayable and checked fields.
      $notice['fields'] = $this->get_notice_fields($notice, FALSE);

      cache_set($cid, $notice, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_notice_external'));
    }

    return $notice;
  }

  public function get_notices_external($notices_ids) {
    if (!$this->gateway)
      return array();

    if (!$notices_ids)
      return array();

    // Remove non integer items.
    array_walk($notices_ids, create_function('&$a', '$a += 0;'));
    $notices_ids = array_unique($notices_ids);
    if (!$notices_ids)
      return array();

    $this->delete_old_cache_objects();

    $results = array();
    $cids = $this->get_cache_id('notice_external', $notices_ids);
    $notices = cache_get_multiple($cids, 'cache_pmb_connector');
    if ($notices) {
      foreach ($notices as $notice) {
        $results[$notice->data['id']] = $notice->data;
      }
    }

    // Get non cached notices if any.
    $notices_ids = array_diff($notices_ids, array_keys($results));
    if ($notices_ids) {
      if ($this->reader)
        $this->reader_check_session();

      $cache_duration = pmb_variable_get('pmb_cache_duration_notice_external');
      $notices = $this->gateway->get_notices_external($notices_ids);
      foreach ($notices as $notice) {
        // Is this check useful?
        if ($notice) {
          // Add readable, displayable and checked fields.
          $notice['fields'] = $this->get_notice_fields($notice, FALSE);

          $results[$notice['id']] = $notice;
          cache_set($this->get_cache_id('notice_external', $notice['id']), $notice, 'cache_pmb_connector', REQUEST_TIME + $cache_duration);
        }
      }
    }

    return $results;
  }

  public function get_serials() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('serials');
    $serials = cache_get($cid, 'cache_pmb_connector');
    if ($serials) {
      return $serials->data;
    }

    $serials = $this->gateway->get_serials();
    if ($serials) {
      cache_set($cid, $serials, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_serials'));
    }
    return $serials;
  }

  public function get_bulletin($bulletin_id) {
    if (!$this->gateway)
      return array();

    $bulletin_id += 0;
    if (!$bulletin_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('bulletin', $bulletin_id);
    $bulletin = cache_get($cid, 'cache_pmb_connector');
    if ($bulletin) {
      return $bulletin->data;
    }

    $bulletin = $this->gateway->get_bulletin($bulletin_id);
    if ($bulletin) {
      // Add readable, displayable and checked fields.
      $bulletin['fields'] = $this->get_notice_fields($bulletin, FALSE);

      cache_set($cid, $bulletin, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_notice'));
    }
    return $bulletin;
  }

  public function get_bulletins($bulletins_ids) {
    if (!$this->gateway)
      return array();

    if (!$bulletins_ids)
      return array();

    // Remove non integer items.
    array_walk($bulletins_ids, create_function('&$a', '$a += 0;'));
    $bulletins_ids = array_unique($bulletins_ids);
    if (!$bulletins_ids)
      return array();

    $this->delete_old_cache_objects();

    $results = array();
    $cids = $this->get_cache_id('bulletin', $bulletins_ids);
    $bulletins = cache_get_multiple($cids, 'cache_pmb_connector');
    if ($bulletins) {
      foreach ($bulletins as $bulletin) {
        $results[$bulletin->data['id']] = $bulletin->data;
      }
    }

    // Get non cached bulletins if any.
    $bulletins_ids = array_diff($bulletins_ids, array_keys($results));
    if ($bulletins_ids) {
      if ($this->reader)
        $this->reader_check_session();

      $cache_duration = pmb_variable_get('pmb_cache_duration_notice');
      $bulletins = $this->gateway->get_bulletins($bulletins_ids);
      foreach ($bulletins as $bulletin) {
        // Is this check useful?
        if ($bulletin) {
          // Add readable, displayable and checked fields.
          $bulletin['fields'] = $this->get_notice_fields($bulletin, FALSE);

          $results[$bulletin['id']] = $bulletin;
          cache_set($this->get_cache_id('bulletin', $bulletin['id']), $bulletin, 'cache_pmb_connector', REQUEST_TIME + $cache_duration);
        }
      }
    }

    return $results;
  }

  public function find_notice_bulletin($notice_id) {
    if (!$this->gateway)
      return array();

    $notice_id += 0;
    if (!$notice_id)
      return array();

    return $this->gateway->find_notice_bulletin($notice_id);
  }

  public function get_author($author_id) {
    if (!$this->gateway)
      return array();

    $author_id += 0;
    if (!$author_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('author', $author_id);
    $author = cache_get($cid, 'cache_pmb_connector');
    if ($author) {
      return $author->data;
    }

    $author = $this->gateway->get_author($author_id);
    if ($author) {
      if (isset($author->notice_ids)) {
        $author->notices_ids = &$author->notice_ids;
        unset($author->notice_ids);
      }

      cache_set($cid, $author, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_author'));
    }
    return $author;
  }

  public function get_publisher($publisher_id) {
    if (!$this->gateway)
      return array();

    $publisher_id += 0;
    if (!$publisher_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('publisher', $publisher_id);
    $publisher = cache_get($cid, 'cache_pmb_connector');
    if ($publisher) {
      return $publisher->data;
    }

    $publisher = $this->gateway->get_publisher($publisher_id);
    if ($publisher) {
      if (isset($publisher->notice_ids)) {
        $publisher->notices_ids = &$publisher->notice_ids;
        unset($publisher->notice_ids);
      }

      cache_set($cid, $publisher, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_publisher'));
    }
    return $publisher;
  }

  public function get_serie($serie_id) {
    if (!$this->gateway)
      return array();

    $serie_id += 0;
    if (!$serie_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('serie', $serie_id);
    $serie = cache_get($cid, 'cache_pmb_connector');
    if ($serie) {
      return $serie->data;
    }

    $serie = $this->gateway->get_serie($serie_id);
    if ($serie) {
      // Change 'collection' by the matching English 'serie'.
      if (isset($serie->information->collection_id)) {
        $serie->information->serie_id = $serie->information->collection_id;
        $serie->information->serie_name = $serie->information->collection_name;
        $serie->information->serie_parent = $serie->information->collection_parent;
        $serie->information->serie_issn = $serie->information->collection_issn;
        $serie->information->serie_web = $serie->information->collection_web;
        $serie->information->serie_links = $serie->information->collection_links;
        unset($serie->information->collection_id);
        unset($serie->information->collection_name);
        unset($serie->information->collection_parent);
        unset($serie->information->collection_issn);
        unset($serie->information->collection_web);
        unset($serie->information->collection_links);
      }

      if (isset($serie->notice_ids)) {
        $serie->notices_ids = &$serie->notice_ids;
        unset($serie->notice_ids);
      }

      cache_set($cid, $serie, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_serie'));
    }
    return $serie;
  }

  public function get_subserie($subserie_id) {
    if (!$this->gateway)
      return array();

    $subserie_id += 0;
    if (!$subserie_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('subserie', $subserie_id);
    $subserie = cache_get($cid, 'cache_pmb_connector');
    if ($subserie) {
      return $subserie->data;
    }

    $subserie = $this->gateway->get_subserie($subserie_id);
    if ($subserie) {
      // Change 'sous-collection' by the matching English 'subserie'.
      if (isset($subserie->information->sous_collection_id)) {
        $subserie->information->subserie_id = $subserie->information->sous_collection_id;
        $subserie->information->subserie_name = $subserie->information->sous_collection_name;
        $subserie->information->subserie_parent = $subserie->information->sous_collection_parent;
        $subserie->information->subserie_issn = $subserie->information->sous_collection_issn;
        $subserie->information->subserie_web = $subserie->information->sous_collection_web;
        $subserie->information->subserie_links = $subserie->information->sous_collection_links;
        unset($subserie->information->sous_collection_id);
        unset($subserie->information->sous_collection_name);
        unset($subserie->information->sous_collection_parent);
        unset($subserie->information->sous_collection_issn);
        unset($subserie->information->sous_collection_web);
        unset($subserie->information->sous_collection_links);
      }

      if (isset($subserie->notice_ids)) {
        $subserie->notices_ids = &$subserie->notice_ids;
        unset($subserie->notice_ids);
      }

      cache_set($cid, $subserie, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_subserie'));
    }
    return $subserie;
  }

  public function get_locations_and_sections() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = 'locations_and_sections';
    $locations_and_sections = cache_get($cid, 'cache_pmb_connector');
    if ($locations_and_sections) {
      return $locations_and_sections->data;
    }

    $locations_and_sections = $this->gateway->get_locations_and_sections();
    if ($locations_and_sections) {
      cache_set($cid, $locations_and_sections, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_locations_and_sections'));
    }
    return $locations_and_sections;
  }

  public function get_section_notices_ids($section_id) {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('section', $section_id);
    $notices = cache_get($cid, 'cache_pmb_connector');
    if ($notices) {
      return $notices->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $notices = $this->gateway->get_section_notices_ids($section_id);
    if ($notices) {
      cache_set($cid, $notices, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_locations_and_sections'));
    }
    return $notices;
  }

  public function get_shelves() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = 'shelves';
    $shelves = cache_get($cid, 'cache_pmb_connector');
    if ($shelves) {
      return $shelves->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $shelves = $this->gateway->get_shelves();
    if ($shelves) {
      cache_set($cid, $shelves, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_shelves'));
    }
    return $shelves;
  }

  public function get_shelf_notices_ids($shelf_id) {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('shelf', $shelf_id);
    $notices = cache_get($cid, 'cache_pmb_connector');
    if ($notices) {
      return $notices->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $notices = $this->gateway->get_shelf_notices_ids($shelf_id);
    if ($notices) {
      cache_set($cid, $notices, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_shelves'));
    }
    return $notices;
  }

  public function get_thesauri() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = 'thesauri';
    $thesauri = cache_get($cid, 'cache_pmb_connector');
    if ($thesauri) {
      return $thesauri->data;
    }

    $thesauri = $this->gateway->get_thesauri();
    if ($thesauri) {
      cache_set($cid, $thesauri, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_thesauri'));
    }
    return $thesauri;
  }

  public function get_category($category_id) {
    if (!$this->gateway)
      return array();

    $category_id += 0;
    if (!$category_id)
      return array();

    $this->delete_old_cache_objects();

    $cid = $this->get_cache_id('category', $category_id);
    $category = cache_get($cid, 'cache_pmb_connector');
    if ($category) {
      return $category->data;
    }

    $category = $this->gateway->get_category($category_id);
    if ($category) {
      if (isset($category->notice_ids)) {
        $category->notices_ids = &$category->notice_ids;
        unset($category->notice_ids);
      }

      cache_set($cid, $category, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_category'));
    }
    return $category;
  }

  public function get_search_fields() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = 'search_fields';
    $search_fields = cache_get($cid, 'cache_pmb_connector');
    if ($search_fields) {
      return $search_fields->data;
    }

    $search_fields = $this->gateway->get_search_fields();
    if ($search_fields) {
      cache_set($cid, $search_fields , 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_search_fields'));
    }
    return $search_fields;
  }

  public function get_search_notices_ids($search_terms, $search_fields) {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $search_id = md5(serialize(array($search_terms, $search_fields)));
    $cid = $this->get_cache_id('search', $search_id);
    $search = cache_get($cid, 'cache_pmb_connector');
    if ($search) {
      return $search->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $search = $this->gateway->get_search_notices_ids($search_terms, $search_fields);
    // Cache even search without results.
    cache_set($cid, $search , 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_search'));

    return $search;
  }

  public function get_search_notice_external_ids($search_terms, $search_fields, $source_ids) {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    // Oui, on veut faire cela bien.
    if ($this->reader)
      $this->reader_check_session();

    $result = array();
    foreach ($source_ids as $source_id) {
      $search_id = md5(serialize(array($search_terms, $search_fields, $source_id)));
      $cid = $this->get_cache_id('search_external', $search_id);
      $search = cache_get($cid, 'cache_pmb_connector');
      if ($search) {
        $result[$source_id] = $search->data;
      }
      else {
        $search = $this->gateway->get_search_notice_external_ids($search_terms, $search_fields, array($source_id));
        if ($search) {
          $result[$source_id] = $search->data;
        }
        // Cache even search without results.
        cache_set($cid, $search , 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_search_external'));
      }
    }

    return $result;
  }

  public function get_search_external_sources() {
    if (!$this->gateway)
      return array();

    $this->delete_old_cache_objects();

    $cid = 'search_external_sources';
    $search_external_sources = cache_get($cid, 'cache_pmb_connector');
    if ($search_external_sources) {
      return $search_external_sources->data;
    }

    if ($this->reader)
      $this->reader_check_session();

    $search_external_sources = $this->gateway->get_search_external_sources();
    if ($search_external_sources) {
      cache_set($cid, $search_external_sources, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_search_external_sources'));
    }
    return $search_external_sources;
  }

  public function get_search_advanced_fields($fetch_values = FALSE) {
    if (!$this->gateway)
      return FALSE;

    if ($this->reader)
      $this->reader_check_session();

    $fields = $this->gateway->get_search_advanced_fields($fetch_values);
    return $fields;
  }

  public function reader_get_session_id($login, $password, $mode = '') {
    if (!$this->gateway || !$login || !$password)
      return FALSE;

    switch ($mode) {
      case 'md5' :
        $session_id = $this->gateway->reader_login_md5($login, $password);
        break;
      case 'full_hash' :
        $session_id = $this->gateway->reader_login_crypt($login, $password, $mode);
        break;
      case 'drupal' :
        $session_id = $this->gateway->reader_login_authenticated($login, $mode);
        break;
      case 'plain' :
      case '' :
      default :
        $session_id = $this->gateway->reader_login($login, $password);
        break;
    }

    if (!$session_id)
      return FALSE;

    watchdog('PMB Connector', 'Open remote PMB session for reader "%name".', array('%name' => $login));

    $this->gateway->set_session_id($session_id);

    // Save session infos for cache and in case of remote session expires.
    // Other data don't need to be check, because Drupal user doesn't change.
    if ($this->reader) {
      $to_save = array('pmb_reader' => array());
      $to_save['pmb_reader']['credentials'] = array(
        'login' => $login,
        'password' => $password,
        'mode' => $mode,
      );
      $to_save['pmb_reader']['session'] = array(
        'sessid' => $session_id,
        'sessid_time' => REQUEST_TIME,
      );
      user_save($this->reader, $to_save);
    }

    return $session_id;
  }

  public function reader_logout() {
    if (!$this->gateway || !$this->reader)
      return FALSE;

    // If sessid is not set, user is not a reader but a normal Drupal user.
    if (isset($this->reader->data['pmb_reader']['session']['sessid'])) {
      $this->gateway->set_session_id($this->reader->data['pmb_reader']['session']['sessid']);
      $result = $this->gateway->reader_logout();

      if ($result) {
        watchdog('PMB Connector', 'Remote PMB session closed for reader "%name".', array('%name' => $this->reader->name));
      }
      else {
        watchdog('PMB Connector', 'Unable to close remote PMB session for reader "%name".', array('%name' => $this->reader->name), WATCHDOG_WARNING);
        drupal_set_message(t('Unable to close remote PMB session for reader "@name".', array('@name' => $this->reader->name)), 'warning');
      }
    }

    $this->gateway->unset_session_id();

    if (isset($this->reader->data['pmb_reader'])) {
      unset($this->reader->data['pmb_reader']);
    }

    cache_clear_all('reader:' . $this->reader->uid, 'cache_pmb_connector', TRUE);

    return TRUE;
  }

  public function reader_change_password($old_password, $new_password) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $result = $this->gateway->reader_change_password($old_password, $new_password);
    return $result;
  }

  public function reader_get_account_info() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $result = $this->gateway->reader_get_account_info();
    return $result;
  }

  public function reader_get_cart() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $cart = $this->gateway->reader_get_cart();
    return $cart;
  }

  public function reader_add_notices_to_cart($notices_ids) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_add_notices_to_cart($notices_ids);
    return TRUE;
  }

  public function reader_delete_notices_from_cart($notices_ids) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_delete_notices_from_cart($notices_ids);
    return TRUE;
  }

  public function reader_empty_cart() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_empty_cart();
    return TRUE;
  }

  public function reader_get_reservations() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->delete_old_cache_objects();

    $cid = 'reader:' . $this->reader->uid . ':reservations';
    $resas = cache_get($cid, 'cache_pmb_connector');
    if ($resas) {
      return $resas->data;
    }

    $resas = $this->gateway->reader_get_reservations();
    if ($resas) {
      cache_set($cid, $resas, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_reader_reservations'));
    }
    return $resas;
  }

  public function reader_can_reserve_notice($notice_id, $bulletin_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    if (!($notice_id || $bulletin_id))
      return FALSE;

    return $this->gateway->reader_can_reserve_notice($notice_id, $bulletin_id);
  }

  public function reader_get_reservation_locations($notice_id, $bulletin_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    if (!($notice_id || $bulletin_id))
      return FALSE;

    return $this->gateway->reader_get_reservation_locations($notice_id, $bulletin_id);
  }

  public function reader_add_reservation($notice_id, $bulletin_id, $location_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    if (!($notice_id || $bulletin_id))
      return FALSE;

    $resa = $this->gateway->reader_add_reservation($notice_id, $bulletin_id, $location_id);
    if ($resa) {
      // Update cache of reservations.
      $this->delete_old_cache_objects();

      $cid = 'reader:' . $this->reader->uid . ':reservations';
      $resas = $this->gateway->reader_get_reservations();
      if ($resas) {
        cache_set($cid, $resas, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_reader_reservations'));
      }
    }

    return $resa;
  }

  public function reader_delete_reservation($reservation_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_delete_reservation($reservation_id);

    $cid = 'reader:' . $this->reader->uid . ':reservations';
    cache_clear_all($cid, 'cache_pmb_connector');

    return TRUE;
  }

  public function reader_get_loans() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->delete_old_cache_objects();

    $cid = 'reader:' . $this->reader->uid . ':loans';
    $loans = cache_get($cid, 'cache_pmb_connector');
    if ($loans) {
      return $loans->data;
    }

    $loans = $this->gateway->reader_get_loans();
    if ($loans) {
      cache_set($cid, $loans, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_reader_loans'));
    }
    return $loans;
  }

  public function reader_get_reading_lists() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $lists = $this->gateway->reader_get_reading_lists();
    return $lists;
  }

  public function reader_get_reading_lists_public() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $lists = $this->gateway->reader_get_reading_lists_public();
    return $lists;
  }

  public function reader_add_notices_to_reading_list($list_id, $notices_ids) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_add_notices_to_reading_list($list_id, $notices_ids);
    return TRUE;
  }

  public function reader_remove_notices_to_reading_list($list_id, $notices_ids) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_remove_notices_to_reading_list($list_id, $notices_ids);
    return TRUE;
  }

  public function reader_empty_reading_list($list_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->gateway->reader_empty_reading_list($list_id);
    return TRUE;
  }

  public function reader_get_suggestions() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $suggs = $this->gateway->reader_get_suggestions();
    return $suggs;
  }

  public function reader_add_suggestion($suggestion) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $result = $this->gateway->reader_add_suggestion($suggestion);
    return $result;
  }

  public function reader_edit_suggestion($suggestion) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $result = $this->gateway->reader_edit_suggestion($suggestion);
    return $result;
  }

  public function reader_delete_suggestion($suggestion_id) {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $result = $this->gateway->reader_delete_suggestion($suggestion_id);
    return $result;
  }

  public function reader_get_suggestion_sources_and_categories() {
    if (!$this->gateway || !$this->reader || !$this->reader_check_session())
      return FALSE;

    $this->delete_old_cache_objects();

    $cid = 'reader_suggestions_sources_and_categories';
    $sugg_sources_and_categs = cache_get($cid, 'cache_pmb_connector');
    if ($sugg_sources_and_categs) {
      return $sugg_sources_and_categs->data;
    }

    $sugg_sources_and_categs = $this->gateway->reader_get_suggestion_sources_and_categories();
    if ($sugg_sources_and_categs) {
      cache_set($cid, $sugg_sources_and_categs, 'cache_pmb_connector', REQUEST_TIME + pmb_variable_get('pmb_cache_duration_suggs_items'));
    }
    return $sugg_sources_and_categs;
  }

  /**
   * @defgroup pmb_notice_convert Convert of a PMB notice to a usable one.
   * @{
   * These functions helps to get a readable, displayable and checked notice, in
   * fact an array of standard fields, understandable by a non-librarian, from a
   * PMB UNIMARC notice.
   * @note
   * These functions should be on the PMB server side, but as they don't, we
   * need to manage all notices here.
   */

  // This flag is used to keep plain text fields in place of links.
  private $flag_fields_links = TRUE;

  // This flag is used to add, or not, 's.l.' (sine loco [Latin: no place
  // mentioned]), 's.n.' (sine nomine [Latin: no name mentioned]) and 's.d.'
  // (sine datum: [Latin: no date mentioned]).
  // This flag is used to set a default author and a default title too, so a
  // basic good title can be easily get.
  private $flag_fields_sine = TRUE;

  /**
   * Get readable, displayable and checked items from a UNIMARC notice.
   *
   * @note
   * This function contains a lot of checks because the PMB demo set contains
   * lot of errors that are cleaned by these checks.
   *
   * @param array $notice_full
   * @param boolean $flag_fields_links
   *
   * @return array
   *   Array of readable, displayable and checked fields according to mode.
   *
   * @todo Find XSL files to make it easier. Use PMB ones? Or Koha ones, because
   * they are better and more compliant with standards?
   * @todo Use class and subclasses to simplify code.
   */
  private function get_notice_fields($notice_full, $flag_fields_links = TRUE) {
    if (!$notice_full['notice'] || $notice_full['notice'] == 0) {
      return array();
    }
    $notice = &$notice_full['notice'];
    $this->flag_fields_links = $flag_fields_links;

    $notice_fields = array();

    // Define main type.
    if ($notice['header']['hl'] == '1') {
      $notice_fields['main_type'] = t('Serial');
    }
    elseif ($notice['header']['rs'] == 'n'
      && $notice['header']['dt'] == 'a'
      && $notice['header']['bl'] == 's'
      && $notice['header']['hl'] == '2'
    ) {
      $notice_fields['main_type'] = t('Bulletin');
    }
    elseif ($notice['header']['rs'] == 'n'
      && $notice['header']['dt'] == 'a'
      && $notice['header']['bl'] == 'a'
      && $notice['header']['hl'] == '2'
    ) {
      $notice_fields['main_type'] = t('Article');
    }
    else {
      $notice_fields['main_type'] = t('Monograph');
    };

    // Determine the document type.
    $record_status = array(
      'c' => t('Corrected record'),
      'd' => t('Deleted record'),
      'n' => t('New record'),
      'o' => t('Previously issued higher level record'),
      'p' => t('Previously issued as incomplete (prepublication)'),
    );
    $document_type = array(
      'a' => t('Printed text'),
      'b' => t('Manuscript text'),
      'c' => t('Musical score - printed'),
      'd' => t('Musical score - manuscript'),
      'e' => t('Cartographic document - printed'),
      'f' => t('Cartographic document - manuscript'),
      'g' => t('Video, projectable document'),
      'h' => t('Microform'),
      'i' => t('Sound recording - non musical'),
      'j' => t('Sound recording - musical'),
      'k' => t('2D graphical document'),
      'l' => t('Electronic document'),
      'm' => t('Multimedia document'),
      'n' => t('3D object, artifact...'),
      'r' => t('3D object, artifact...'),
    );
    $bibliographic_level = array(
      'a' => t('Article'),
      // 'Serial check-in' or 'Bulletin': Not in Unimarc, but in Marc 21.
      'b' => t('Issue'),
      'c' => t('Collection'),
      'm' => t('Monograph'),
      's' => t('Serial'),
      // Not in Unimarc, but in Marc 21.
      'd' => t('Subunit'),
    );
    $hierarchical_level = array(
      '0' => t('No hierarchy'),
      '1' => t('Highest level record'),
      '2' => t('Record below highest record'),
    );
    $encoding_level = array(
      '1' => t('No piece examined'),
      '2' => t('Prepublication record'),
      '3' => t('Preliminary cataloging'),
    );
    $record_update = array(
      '#' => t('Full ISBD'),
      'i' => t('Partial/incomplete ISBD'),
      'n' => t('Non-ISBD'),
    );
    $notice_fields['record_status'] = $record_status[$notice['header']['rs']];
    $notice_fields['document_type'] = $document_type[$notice['header']['dt']];
    $notice_fields['bibliographic_level'] = $bibliographic_level[$notice['header']['bl']];
    $notice_fields['hierarchical_level'] = $hierarchical_level[$notice['header']['hl']];
    $notice_fields['encoding_level'] = $encoding_level[$notice['header']['el']];
    $notice_fields['record_update'] = $record_update[$notice['header']['ru']];

    // Manage simple and non-repetitive fields.
    $simple_fields = array(
      'isbn' => array('010' => 'a'),
      'issn' => array('010' => 'a'),
      'price' => array('010' => 'd'),

      'edition_statement' => array('205' => 'a'),

      'pagination' => array('215' => 'a'),
      'layout' => array('215' => 'c'),
      'size' => array('215' => 'd'),
      'accompanying_material' => array('215' => 'e'),

      'note_general' => array('300' => 'a'),
      'note_content' => array('327' => 'a'),
      // Abstract is used for description.
      'abstract' => array('330' => 'a'),

      'class_index' => array('676' => 'a'),

      'source' => array('801' => '9'),

      'online_doc_url' => array('856' => 'u'),
      'online_doc_format' => array('856' => 'q'),
      'cover_url' => array('896' => 'a'),
    );
    // Warning: PMB doesn't save ISSN in 011a, but in 010a, as any documents.
    if ($notice['header']['bl'] == 's') {
      unset($simple_fields['isbn']);
    }
    else {
      unset($simple_fields['issn']);
    }
    foreach ($simple_fields as $readable_field => $marc_field) {
      if (isset($notice['f'][key($marc_field)][0][current($marc_field)])) {
        $notice_fields[$readable_field] = check_plain($notice['f'][key($marc_field)][0][current($marc_field)]);
      }
    }

    // Manage specific fields.
    // Manage languages [101a] and [101c].
    // TODO Get full name and translated language.
    if (isset($notice['f']['101'])) {
      foreach ($notice['f']['101'] as &$item) {
        if (isset($item['a'])) {
          $o = &$notice_fields['language'];
          $o = array();
          if (is_array($item['a'])) {
            foreach ($item['a'] as &$subitem) {
              $o[] = check_plain(ucfirst($subitem));
            }
          }
          else {
            $notice_fields['language'][] = check_plain(ucfirst($item['a']));
          }
        }
        if (isset($item['c'])) {
          $o = &$notice_fields['language_original'];
          $o = array();
          if (is_array($item['c'])) {
            foreach ($item['c'] as &$subitem) {
              $o[] = check_plain(ucfirst($subitem));
            }
          }
          else {
            $o[] = check_plain(ucfirst($item['c']));
          }
        }
      }
    }

    // Manage titles [200].
    if (isset($notice['f']['200'])) {
      foreach ($notice['f']['200'] as $key => &$item) {
        $o = &$notice_fields['title'][];
        $o = array();
        $o['id'] = $key;
        $o['full'] = '';
        if (isset($item['a'])) {
          $o['name'] = $o['full'] = check_plain($item['a']);
          // This item is used internally only.
          if ($notice['header']['bl'] == 's' && !isset($title_link)) {
            $title_link = $this->set_link_content($item['a'], _pmb_p('catalog/serial/') . $notice['id']);
          }
        }
        elseif ($this->flag_fields_sine) {
          $o['name'] = $o['full'] = '[' . t('No title mentioned') . ']';
        }
        if (isset($item['c'])) {
          $o['other_author'] = check_plain($item['c']);
        }
        if (isset($item['d'])) {
          $o['parallel'] = check_plain($item['d']);
        }
        if (isset($item['e'])) {
          $o['other_info'] = check_plain($item['e']);
          $o['full'] .= ' : ' . $o['other_info'];
        }
        if ($o['full'] == '') {
          unset($o['full']);
        }
      }
    }
    // Manage publishers [210c].
    if (isset($notice['f']['210'])) {
      $path = _pmb_p('catalog/publisher/');
      foreach ($notice['f']['210'] as &$item) {
        // Manage full main publication reference, without date (210a : 210c).
        $o = &$notice_fields['publisher'][$item['id']];
        $o = array();
        $o['id'] = $item['id'];
        $o['full'] = '';
        if (isset($item['a'])) {
          $o['place'] = $o['full'] = check_plain($item['a']);
        }
        elseif ($this->flag_fields_sine) {
          $o['place'] = $o['full'] = '[' . t('s.l.') . ']';
        }
        if (isset($item['c'])) {
          $o['name'] = $this->set_link_content($item['c'], $path . $o['id']);
          $o['full'] .= ($o['full'] ? ' : ' : '') . $o['name'];
        }
        elseif ($this->flag_fields_sine) {
          $o['name'] = '[' . t('s.n.') . ']';
          $o['full'] .= ' : ' . $o['name'];
        }
        if (isset($item['d'])) {
          $o['date'] = check_plain($item['d']);
        }
        elseif ($this->flag_fields_sine) {
          $o['date'] = '[' . t('s.d.') . ']';
        }
        if ($o['full'] == '') {
          unset($o['full']);
        }
      }
    }

    // Manage series/subseries [225] and [410/411].
    if (isset($notice['f']['225'])) {
      $path_serie = _pmb_p('catalog/serie/');
      $path_subserie = _pmb_p('catalog/subserie/');
      foreach ($notice['f']['225'] as &$item) {
        // Check if item is a serie.
        if (isset($item['a'])) {
          $o = &$notice_fields['serie'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          $o['name'] = $o['full'] = $this->set_link_content($item['a'], $path_serie . $o['id']);
          if (isset($item['v'])) {
            $o['volume'] = check_plain($item['v']);
            $o['full'] .= ', ' . $o['volume'];
          }
          if (isset($item['x'])) {
            $o['issn'] = check_plain($item['x']);
          }
        }
        // Check if item is a subserie.
        if (isset($item['i'])) {
          $o = &$notice_fields['subserie'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['name'] = $o['full'] = $this->set_link_content($item['i'], $path_subserie . $o['id']);
          if (isset($item['x'])) {
            $o['issn'] = check_plain($item['x']);
          }
        }
        if ($o['full'] == '') {
          unset($o['full']);
        }
      }
    }
    // Complete 225 with 410 and 411 if there is no subarray ['9'].
    if (isset($notice['f']['410'])) {
      foreach ($notice['f']['410'] as &$item) {
        $check = (isset($item['9']) && is_array($item['9']));
        if (!$check && isset($item['t'])) {
          if (!isset($notice_fields['serie'][$item['id']])) {
            $path = _pmb_p('catalog/serie/');
            $o = &$notice_fields['serie'][$item['id']];
            $o = array();
            $o['id'] = $item['id'];
            $o['name'] = $o['full'] = $this->set_link_content($item['t'], $path . $o['id']);
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
      }
    }
    if (isset($notice['f']['411'])) {
      foreach ($notice['f']['411'] as &$item) {
        $check = (isset($item['9']) && is_array($item['9']));
        // Complete 225 with 410 and 411 if there is no subarray ['9'].
        if (!$check && isset($item['t'])) {
          if (!isset($notice_fields['subserie'][$item['id']])) {
            $path = _pmb_p('catalog/subserie/');
            $o = &$notice_fields['subserie'][$item['id']];
            $o = array();
            $o['id'] = $item['id'];
            $o['name'] = $o['full'] = $this->set_link_content($item['t'], $path . $o['id']);
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
      }
    }

    // Manage common relations.
    // PMB doesn't manage all of UNIMARC fields, but they are added in case of.
    // This function manages only lnk:parent and lnk:child. Other items are
    // processed below.
    foreach (array(
      // Manage series [410].
      '410',
      // Manage subseries [411].
      '411',

      // Manage supplements [421].
      '421',
      // Manage parent of supplements [422].
      '422',
      // Manage issued with [423].
      '423',

      // Manage continues [430].
      '430',
      // Manage continues in part [431].
      '431',
      // Manage supersedes [432].
      '432',
      // Manage supersedes in part [433].
      '433',
      // Manage absorbed [434].
      '434',
      // Manage absorbed in part [435].
      '435',
      // Manage formed by the merger of [436].
      '436',
      // Manage separated from [437].
      '437',

      // Manage continued by [440].
      '440',
      // Manage continued in part by [441].
      '441',
      // Manage superseded by [442].
      '442',
      // Manage superseded in part by [443].
      '443',
      // Manage absorbed by [444].
      '444',
      // Manage absorbed in part by [445].
      '445',
      // Manage split into [446].
      '446',
      // Manage merged with to form [447].
      '447',
      // Manage changed back to [447].
      '448',

      // Manage other edition in the same medium [451].
      '451',
      // Manage other edition in the another medium [452].
      '452',
      // Manage translated as [453].
      '453',
      // Manage translation of [454].
      '454',
      // Manage reproduction of [455].
      '455',
      // Manage reproduced as [456].
      '456',

      // Manage set [461].
      '461',
      // Manage subset [462].
      '462',
      // Manage piece [463].
      '463',
      // Manage piece analytic [464].
      '464',

      // Manage item reviewed [470].
      '470',
      // Manage other related work [488].
      '488',
    ) as $value) {
      if (isset($notice['f'][$value])) {
        foreach ($notice['f'][$value] as &$item) {
          $check = (isset($item['9']) && is_array($item['9']));
          if ($check
            && (in_array('lnk:parent', $item['9']) || in_array('lnk:child', $item['9']))) {
            $this->get_notice_fields_relation($notice_fields, $item);
          }
        }
      }
    }

    // Manage serials/collections [461].
    if (isset($notice['f']['461'])) {
      foreach ($notice['f']['461'] as &$item) {
        $check = (isset($item['9']) && is_array($item['9']));
        // This a serial.
        if ($check && in_array('lnk:perio', $item['9'])) {
          $path = _pmb_p('catalog/serial/');
          $o = &$notice_fields['serial'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          if (isset($item['t'])) {
            $o['name'] = $o['full'] = $this->set_link_content($item['t'], $path . $o['id']);
          }
          if (isset($item['v'])) {
            $o['volume'] = check_plain($item['v']);
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['volume'];
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
        // This a main part of a collection.
        elseif (!$check) {
          $path = _pmb_p('catalog/collection/');
          $o = &$notice_fields['collection'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          if (isset($item['t'])) {
            $o['name'] = $o['full'] = $this->set_link_content($item['t'], $path . $o['id']);
          }
          if (isset($item['v'])) {
            $o['volume'] = check_plain($item['v']);
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['volume'];
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
      }
    }

    // Manage issues [463].
    if (isset($notice['f']['463'])) {
      foreach ($notice['f']['463'] as &$item) {
        $check = (isset($item['9']) && is_array($item['9']));
        if ($check && in_array('lnk:bull', $item['9'])) {
          $path = _pmb_p('catalog/issue/');
          $o = &$notice_fields['issue'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          if (isset($item['v'])) {
            $o['volume'] = $o['full'] = $this->set_link_content($item['v'], $path . $o['id']);
          }
          if (isset($item['d'])) {
            $o['date'] = check_plain(pmb_convert_date($item['d']));
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['date'];
          }
          if (isset($item['e'])) {
            $o['date_period'] = check_plain($item['e']);
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['date_period'];
          }
          if (isset($item['t']) && trim($item['t'])) {
            $o['name'] = check_plain($item['t']);
            $o['full'] .= ($o['full'] ? ' ' : '') . '[' . $o['name'] . ']';
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
        // Else, this is a piece of something.
        elseif (!$check) {
          $o = &$notice_fields['piece'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          if (isset($item['t'])) {
            $o['name'] = $o['full'] = check_plain($item['t']);
          }
          if (isset($item['v'])) {
            $o['volume'] = check_plain($item['v']);
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['volume'];
          }
          if (isset($item['d'])) {
            $o['date'] = check_plain(pmb_convert_date($item['d']));
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['date'];
          }
          if (isset($item['e'])) {
            $o['date_period'] = check_plain($item['e']);
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['date_period'];
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
      }
    }

    // Manage piece analytic [464].
    if (isset($notice['f']['464'])) {
      $path = _pmb_p('catalog/record/');
      foreach ($notice['f']['464'] as $key => &$item) {
        $check = (isset($item['9']) && is_array($item['9']));
        if ($check && in_array('lnk:art', $item['9'])) {
          $o = &$notice_fields['article'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          if (isset($item['t'])) {
            // Warning: What does the second item 't1' mean?
            $i = (is_array($item['t']) ? $item['t'][0] : $item['t']);
            $o['name'] = $o['full'] = $this->set_link_content($i, $path . $o['id']);
          }
          if (isset($item['v'])) {
            // TODO Add a link to the issue.
            $o['volume'] = check_plain($item['v']);
            $i = (isset($title_link) ? t('in') . ' ' . $title_link . ', ' : '');
            $o['full'] .= ($o['full'] ? ', ' : '') . $i . $o['volume'];
          }
          if (isset($item['d'])) {
            $o['date'] = check_plain(pmb_convert_date($item['d']));
            $o['full'] .= ($o['full'] ? ', ' : '') . $o['date'];
          }
          foreach ($item['9'] as $subitem) {
            if ((substr($subitem, 0, 5) == 'page:') && (strlen($subitem) > 5)) {
              $o['pagination'] = substr($subitem, 5);
              $i = (is_numeric($o['pagination']) ? t('p.') . ' ' : '');
              $o['full'] .= ($o['full'] ? ', ' : '') . $i . $o['pagination'];
            }
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
        }
      }
    }

    // Manage uniform title [500].
    // WARNING Currently PMB doesn't provide service to get these uniform items,
    // except name.
    if (isset($notice['f']['500'])) {
      $path = _pmb_p('catalog/uniform_title/');
      foreach ($notice['f']['500'] as $key => &$item) {
        $o = &$notice_fields['uniform_title'][$key];
        $o = array();
        $o['id'] = $key;
        $o['full'] = '';
        $added_text = array();
        if (isset($item['a'])) {
          $o['name'] = $o['full'] = $this->set_link_content($item['a'], $path . $o['id']);
        }
        if (isset($item['i'])) {
          $o['part'] = $added_text[] = check_plain($item['i']);
        }
        if (isset($item['k'])) {
          $o['date'] = $added_text[] = check_plain($item['k']);
        }
        if (isset($item['l'])) {
          $o['subheading'] = $added_text[] = check_plain($item['l']);
        }
        if (isset($item['m'])) {
          $o['language'] = $added_text[] = check_plain($item['m']);
        }
        if (isset($item['q'])) {
          $o['version'] = $added_text[] = check_plain($item['q']);
        }
        if (isset($item['t'])) {
          $o['arranged_statement'] = $added_text[] = check_plain($item['t']);
        }
        // Manage details.
        if ($added_text) {
          $o['full'] .= ' (' . implode(', ', $added_text) . ')';
        }
        if ($o['full'] == '') {
          unset($o['full']);
        }
      }
    }

    // Manage terms [606a|9]: ['9'] is the code, [a] the name.
    // TODO Add full path of each item.
    if (isset($notice['f']['606'])) {
      $path = _pmb_p('catalog/category/');
      foreach ($notice['f']['606'] as &$item) {
        if (isset($item['9']) && isset($item['a'])) {
          $notice_fields['category'][$item['9']] = $this->set_link_content($item['a'], $path . $item['9']);
        }
      }
    }

    // Manage keywords [610a]: list of keywords separated by ';'.
    if (isset($notice['f']['610'][0]['a'])) {
      $items = explode(';', $notice['f']['610'][0]['a']);
      $path = _pmb_p('catalog/search/local/all_fields/');
      foreach ($items as &$item) {
        $notice_fields['keyword'][] = $this->set_link_content($item, $path . $item);
      }
    }

    // Manage class index [675a].
    // TODO Get full name of the class.
    // WARNING Currently, manage only one class [675/676]. Which path to set?
    if (isset($notice['f']['675'])) {
      $notice_fields['class_type'] = 'CDU';
      $path = _pmb_p('catalog/class/');
      foreach ($notice['f']['675'] as &$item) {
        if (isset($item['a'])) {
          $notice_fields['class'][$item['a']] = $this->set_link_content($item['a'], $path . $item['a']);
        }
      }
    }

    // Manage class index [676a].
    // TODO Get full name of the class.
    if (isset($notice['f']['676'])) {
      $notice_fields['class_type'] = 'Dewey';
      $path = _pmb_p('catalog/class/');
      foreach ($notice['f']['676'] as &$item) {
        if (isset($item['a'])) {
          $notice_fields['class'][$item['a']] = $this->set_link_content($item['a'], $path . $item['a']);
        }
      }
    }

    // Manage authors [700 to 712]. They are managed here to keep order.
    // TODO Get full role name.
    $order = 0;
    foreach (array(
        '700',
        '710',
        '701',
        '711',
        '702',
        '712',
      ) as $value) {
      if (isset($notice['f'][$value])) {
        $path = _pmb_p('catalog/author/');
        foreach ($notice['f'][$value] as &$item) {
          $o = &$notice_fields['author'][$item['id']];
          $o = array();
          $o['id'] = $item['id'];
          $o['full'] = '';
          $o['order'] = $order++;
          // Manage people.
          if ($value == '700' || $value == '701' || $value == '702') {
            $o['type'] = 'personal';
            if (isset($item['b'])) {
              $o['out'] = $o['full'] = check_plain($item['b']);
            }
            if (isset($item['a'])) {
              $o['name'] = $this->set_link_content($item['a'], $path . $o['id']);
              $o['full'] = $this->set_link_content(($o['full'] ? $o['full'] . ' ' . $item['a'] : $item['a']), $path . $o['id']);
            }
            elseif ($this->flag_fields_sine) {
              $o['name'] = $o['full'] = '[' . t('No author mentioned') . ']';
            }
            if (isset($item['f'])) {
              $o['date'] = check_plain($item['f']);
              $o['full'] .= ' (' . $o['date'] . ')';
            }
            $o['full_name'] = $o['full'];
            if (isset($item['4'])) {
              $o['role'] = check_plain($item['4']);
              $o['full'] .= ' [' . $o['role'] . ']';
            }
          }
          // Manage corporate and conventions.
          else {
            $o['type'] = (substr($item['ind'], 0, 1) == '1') ? 'convention' : 'corporate';
            if (isset($item['g'])) {
              $o['out'] = $o['full'] = check_plain($item['g']);
            }
            if (isset($item['a'])) {
              $o['name'] = $this->set_link_content($item['a'], $path . $o['id']);
              $o['full'] = $this->set_link_content(($o['full'] ? $o['full'] . ' ' . $item['a'] : $item['a']), $path . $o['id']);
            }
            elseif ($this->flag_fields_sine) {
              $o['name'] = $o['full'] = '[' . t('No author mentioned') . ']';
            }
            $added_text = array();
            if (isset($item['b'])) {
              $o['subdivision'] = $added_text[] = check_plain($item['b']);
            }
            if (isset($item['d'])) {
              $o['number'] = $added_text[] = check_plain($item['d']);
            }
            if (isset($item['f'])) {
              $o['date'] = $added_text[] = check_plain($item['f']);
            }
            if (isset($item['e'])) {
              $o['full_location'] = $added_text[] = check_plain($item['e']);
            }
            if (isset($item['K'])) {
              $o['location'] = check_plain($item['K']);
            }
            if (isset($item['L'])) {
              $o['place'] = $added_text[] = check_plain($item['L']);
            }
            if (isset($item['M'])) {
              $o['country'] = $added_text[] = check_plain($item['M']);
            }
            // Manage details.
            if ($added_text) {
              $o['full'] .= ' (' . implode(', ', $added_text) . ')';
            }
            $o['full_name'] = $o['full'];
            if (isset($item['4'])) {
              $o['role'] = check_plain($item['4']);
              $o['full'] .= ' [' . $o['role'] . ']';
            }
          }
          if ($o['full'] == '') {
            unset($o['full']);
          }
          if ($o['full_name'] == '') {
            unset($o['full_name']);
          }
          if ($o['type'] == 'convention') {
            $notice_fields['convention'][$item['id']] = $notice_fields['author'][$item['id']];
          }
        }
      }
    }

    // The PMB demo set contains lot of errors that are cleaned by these checks.
    if ($this->flag_fields_sine) {
      if (!isset($notice_fields['author'])) {
        $o = &$notice_fields['author'][];
        $o = array();
        $o['id'] = 0;
        $o['name'] = $o['full'] = $o['full_name'] = '[' . t('No author mentioned') . ']';
      }
      if (!isset($notice_fields['title'])) {
        $o = &$notice_fields['title'][];
        $o = array();
        $o['id'] = 0;
        $o['name'] = $o['full'] = '[' . t('No title mentioned') . ']';
      }
      if (!isset($notice_fields['publisher'])) {
        $o = &$notice_fields['publisher'][];
        $o = array();
        $o['id'] = 0;
        $o['place'] = '[' . t('s.l.') . ']';
        $o['name'] = '[' . t('s.n.') . ']';
        $o['full'] = $o['place'] . ' : ' . $o['name'];
        $o['date'] = '[' . t('s.d.') . ']';
      }
    }

    // Manage main informations. These item are duplicate of main informations,
    // for easier management.
    if (isset($notice_fields['author'])) {
      $item = reset($notice_fields['author']);
      $notice_fields['main_author'] = isset($item['full_name']) ? $item['full_name'] : '[' . t('No author mentioned') . ']';
    }
    else {
      $notice_fields['main_author'] = '[' . t('No author mentioned') . ']';
    }

    if (isset($notice_fields['title'])) {
      $item = reset($notice_fields['title']);
      $notice_fields['main_title'] = isset($item['full']) ? $item['full'] : '[' . t('No title mentioned') . ']';
    }
    else {
      $notice_fields['main_title'] = '[' . t('No title mentioned') . ']';
    }

    if (isset($notice_fields['publisher'])) {
      $item = reset($notice_fields['publisher']);
      $notice_fields['main_publisher'] = isset($item['place']) ? $item['place'] : '[' . t('s.l.') . ']';
      $notice_fields['main_publisher'] .= ' : ' . (isset($item['name']) ? $item['name'] : '[' . t('s.n.') . ']');
      $notice_fields['main_date'] = isset($item['date']) ? $item['date'] : '[' . t('s.d.') . ']';
    }
    else {
      $notice_fields['main_publisher'] = '[' . t('s.l.') . ' ' . t('s.n.') . ']';
      $notice_fields['main_date'] = '[' . t('s.d.') . ']';
    }

    return $notice_fields;
  }

  /**
   * Helper for get_notice_fields() to get parent and child notices and infos.
   *
   * @note
   * A recursive call to get_notice_fields() can't be done, because the linked
   * notice doesn't always exist.
   *
   * @todo Use of subfunctions.
   * @todo Include lnk:bull, lnk:perio, lnk:art, lnk:bull_expl.
   *
   * @see pmb_software/doc/import_unimarc_lien.txt
   * @see pmb_software/includes/marc_tables/en_UK/*
   * @see pmb_software/admin/convert/export.class.php
   *
   * @param array $notice_fields
   * @param array $field
   *
   * @return array
   *   Array of readable, displayable and checked fields.
   */
  private function get_notice_fields_relation(&$notice_fields, &$field) {
    $relation_type = array(
      'parent' => array(
        // Analytic part of a serial (the component part is dependent upon the
        // physical identification and location of the containing item).
        'a' => t('In'),
        // Issue of a serial.
        'b' => t('Is an issue of'),
        // Multivolume monograph, like an encylopedia.
        'c' => t('Is a part of'),
        // Is in a box (bibliographic item that is a made-up collection).
        'd' => t('Is in'),
        // Others.
        'e' => t('Supplement of'),
        'f' => t('Previous title'),
        'g' => t('Next title'),
        'h' => t('Translated under the title'),
        'i' => t('Translation from'),
        'j' => t('Comes with'),
      ),
      'child' => array(
        // Analytic part of a serial (the component part is dependent upon the
        // physical identification and location of the containing item).
        'a' => t('Content'),
        // Issue of a serial.
        'b' => t('Issues'),
        // Multivolume monograph, like an encylopedia.
        'c' => t('Is composed of'),
        // Is in a box (bibliographic item that is a made-up collection).
        'd' => t('Contains'),
        // Others.
        'e' => t('Supplements'),
        'f' => t('Next title'),
        'g' => t('Previous title'),
        'h' => t('Translation from'),
        'i' => t('Translated from'),
        'j' => t('Comes with'),
      ),
    );
    $main_type = array(
      'm0' => t('Monograph'),
      's1' => t('Serial'),
      // PMB is not clear when code b2 and s2 are used.
      's2' => t('Issue'),
      'b2' => t('Issue'),
      'a2' => t('Article'),
    );
    $path_name = array(
      'm0' => _pmb_p('catalog/record/'),
      's1' => _pmb_p('catalog/serial/'),
      's2' => _pmb_p('catalog/issue/'),
      'b2' => _pmb_p('catalog/issue/'),
      // In pmb, articles are normal records.
      'a2' => _pmb_p('catalog/record/'),
    );

    $item = &$field;
    $o = &$notice_fields['relation'][];
    $o = array();
    $o['id'] = 0;
    $o['full'] = '';
    $added_text = array();

    foreach ($item['9'] as $subitem) {
      if ((substr($subitem, 0, 4) == 'lnk:') && (strlen($subitem) > 4)) {
        $op = substr($subitem, 4);
        switch ($op) {
          case 'parent' :
            $o['type_relation'] = t('Parent');
            break;
          case 'child' :
            $o['type_relation'] = t('Child');
            break;
          case 'bull' :
            $o['type_relation'] = t('Issue');
            break;
          case 'perio' :
            $o['type_relation'] = t('Serial');
            break;
          case 'art' :
            $o['type_relation'] = t('Article');
            break;
          case 'bull_expl' :
            $o['type_relation'] = t('Issue item');
            break;
          default :
            $o['type_relation'] = substr($subitem, 4);
        }
      }
    }
    foreach ($item['9'] as $subitem) {
      if ((substr($subitem, 0, 3) == 'id:') && (strlen($subitem) > 3)) {
        $o['id'] = substr($subitem, 3);
      }
      if ((substr($subitem, 0, 3) == 'bl:') && (strlen($subitem) > 3)) {
        $i = substr($subitem, 3);
        if (isset($main_type[$i])) {
          $o['main_type'] = $main_type[$i];
          $path = $path_name[$i];
        }
        else {
          $o['main_type'] = $main_type['m0'];
          $path = $path_name['m0'];
        }
      }
      if ((substr($subitem, 0, 9) == 'type_lnk:') && (strlen($subitem) > 9)) {
        $o['type_link'] = $relation_type[$op][substr($subitem, 9)];
      }
    }
    if (isset($item['t'])) {
      if (is_array($item['t'])) {
        $o['name'] = check_plain($item['t'][0]);
        if (isset($item['t'][1]) && trim($item['t'][1])) {
          // Title of issue is often missing, but PMB changes it to ' ', so we
          // trim it.
          $o['issue'] = check_plain(trim($item['t'][1]));
        }
        if (isset($item['t'][2])) {
          $o['serial'] = check_plain($item['t'][2]);
        }
      }
      else {
        $o['name'] = check_plain($item['t']);
      }
    }
    if (isset($item['v'])) {
      $o['volume'] = $added_text[] = check_plain($item['v']);
    }
    if (isset($item['d'])) {
      $o['date'] = $added_text[] = check_plain(pmb_convert_date($item['d']));
    }
    if (isset($item['e'])) {
      $o['date_period'] = $added_text[] = check_plain($item['e']);
    }
    if (isset($item['x'])) {
      $o['issn'] = check_plain($item['x']);
    }
    if (isset($item['y'])) {
      $o['isbn'] = check_plain($item['y']);
    }

    switch ($o['main_type']) {
      case t('serial') :
        $o['name'] = $o['serial'];
        break;
      case t('Issue') :
        $o['name'] = $o['issue'];
        if ($o['serial']) {
          array_unshift($added_text, $o['serial']);
        }
        break;
      case t('Article') :
        if (isset($o['issue'])) {
          array_unshift($added_text, $o['issue']);
        }
        if (isset($o['serial'])) {
          array_unshift($added_text, $o['serial']);
        }
        break;
      default :
    }
    // In normal case. Issue is an exception, because its title is often missing
    // so we use full name of issue to set link.
    if ($o['main_type'] == t('Issue')) {
      $i = (isset($o['issue']) ? $o['issue'] : '')
        . (isset($o['serial']) ? ', ' . $o['serial'] : '')
        . (isset($o['volume']) ? ', ' . $o['volume'] : '')
        . (isset($o['date']) ? ', ' . $o['date'] : '')
        . (isset($o['date_period']) ? ', ' . $o['date_period'] : '');
      $o['full_name'] = $this->set_link_content_plain($i , $path . $o['id']);
    }
    // In normal case.
    else {
      $i = implode(', ', $added_text);
      $o['full_name'] = $this->set_link_content_plain($o['name'], $path . $o['id']) . ($i ? ' (' . $i . ')' : '');
    }
    $o['name'] = $this->set_link_content_plain($o['name'], $path . $o['id']);
    $o['full'] = $o['type_link'] . ' ' . $o['full_name'];
  }

  /**
   * Helper to set a link for a content or not.
   *
   * @param string $text
   * @param string $path
   *
   * @return
   *   Formatted content according to chosen mode.
   */
  private function set_link_content($text = '', $path = '') {
    return ($this->flag_fields_links && $path != '') ?
      l($text, $path) :
      check_plain($text);
  }

  /**
   * Helper to set a link for a content or not. Check
   *
   * @see set_link_content()
   */
  private function set_link_content_plain($text = '', $path = '') {
    return ($this->flag_fields_links && $path != '') ?
      l($text, $path, array('html' => TRUE)) :
      $text;
  }

  /**
   * @} End of "defgroup pmb_notice_convert".
   */
}
