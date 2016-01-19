<?php
/**
 * @file
 * Functions for module 2gis.
 */

/**
 * Build request to 2gis according to params.
 */
function dgis_build_request($service, $params = array()) {
  $params['key'] = variable_get('dgis_api_key', FALSE);
  $params['version'] = variable_get('dgis_api_version', '1.3');
  $params['output'] = 'json';
  if (empty($params['page']) || $params['page'] < 1) {
    $params['page'] = 1;
  }
  if (empty($params['pagesize']) || $params['pagesize'] > DGIS_PAGE_SIZE_MAX || $params['pagesize'] < DGIS_PAGE_SIZE_MIN) {
    $params['pagesize'] = DGIS_PAGE_SIZE_MIN;
  }

  switch ($service) {
    case 'search':
      if (empty($params['what'])) {
        return FALSE;
      }
      $params['sort'] = $params['sort'] ? $params['sort'] : 'relevance';
      break;

    case 'rubricator':
      if (!is_numeric($params['id'])) {
        unset($params['id']);
      }
      $params['sort'] = $params['sort'] ? $params['sort'] : 'name';
      break;

    case 'firmsByFilialId':
      if (empty($params['firmid'])) {
        return FALSE;
      }
      break;

    case 'profile':
      if (empty($params['id']) || empty($params['hash'])) {
        return FALSE;
      }
      break;
  }
  $params['where'] = !empty($params['where']) ? $params['where'] : variable_get('dgis_city', DGIS_DEFAULT_CITY);
  $params['lang'] = variable_get('dgis_lang', 'ru');

  return variable_get('dgis_api_service_url_catalog', 'http://catalog.api.2gis.ru').
    '/' . $service . '?' . http_build_query($params);
}

/**
 * Send request.
 */
function dgis_make_request($request, $result_key = 'result') {
  if (empty($request)) {
    return FALSE;
  }
  $result = drupal_http_request($request);
  if ($result->code == 200) {
    $data = drupal_json_decode($result->data);

    if ($result_key && !is_array($result_key)) {
      $result_key = array($result_key);
    }
    $return = array();
    foreach ($result_key as $k) {
      if (!empty($data[$k])) {
        $return[$k] = $data[$k];
      }
    }
    if (!empty($return)) {
      return (count($return) == 1) ? array_shift($return) : $return;
    }
    elseif (!empty($data['error_code'])) {
      drupal_set_message($data['error_message'] . ' (' . $data['error_code'] . ')', 'warning');
    }
    elseif (empty($result_key) && !empty($data)) {
      return $data;
    }
  }
  else {
    drupal_set_message(t('2gis service is temporary unavailable'), 'error');
  }

  return FALSE;
}

/**
 * Get the list of firms from 2gis by search query.
 */
function dgis_get_firms($query, $sort = 'relevance', $page = 1, $limit = 20, $get_profile = FALSE, $get_branches = FALSE, $city = FALSE, &$total = 0) {
  if (empty($query)) {
    return array();
  }
  $request = dgis_build_request(
    'search',
    array(
      'page' => $page,
      'pagesize' => $limit,
      'sort' => $sort,
      'what' => $query,
      'where' => $city,
    )
  );
  $data = dgis_make_request($request, array('result', 'total'));
  $total = $data['total'];
  $data = $data['result'];
  if ($get_branches) {
    foreach ($data as $k => $firm) {
      if (!empty($firm['firm_group'])) {
        $data[$k]['firm_group']['items'] = array();
        $page = 1;
        while (count($data[$k]['firm_group']['items']) < $firm['firm_group']['count'] && $page < 5) {
          $data[$k]['firm_group']['items'] = array_merge(
            $data[$k]['firm_group']['items'],
            dgis_get_branch_offices_by_firm_group_id($firm['firm_group']['id'], $page, DGIS_PAGE_SIZE_MAX)
          );
          $page++;
        }
      }
    }
  }
  if ($get_profile) {
    foreach ($data as $k => $firm) {
      $data[$k]['profile'] = dgis_get_firm_profile($firm['id'], $firm['hash']);
    }
  }

  return $data ? $data : array();
}

/**
 * Get the rubric name by the ID.
 */
function dgis_get_rubric_by_id($id) {
  if (empty($id)) {
    return array();
  }
  $request = dgis_build_request('rubricator', array('id' => $id));
  $data = dgis_make_request($request);

  return $data ? $data : array();
}

/**
 * Get the filials by group ID.
 */
function dgis_get_branch_offices_by_firm_group_id($group_id, $page = 1, $page_size = 50) {
  if (empty($group_id)) {
    return array();
  }
  $request = dgis_build_request(
    'firmsByFilialId',
    array(
      'firmid' => $group_id,
      'page' => $page,
      'pagesize' => $page_size,
    )
  );
  $data = dgis_make_request($request);

  return $data ? $data : array();
}

/**
 * Get firm profile by ID.
 */
function dgis_get_firm_profile($id, $hash) {
  if (empty($id) || empty($hash)) {
    return array();
  }
  $request = dgis_build_request('profile', array('id' => $id, 'hash' => $hash, 'show_see_also' => TRUE));
  $data = dgis_make_request($request, '');
  unset($data['api_version']);
  unset($data['response_code']);

  return $data ? $data : array();
}

/**
 * Get cities, which is available on the 2gis service.
 */
function dgis_get_cities() {
  $request = dgis_build_request('project/list');
  $data = dgis_make_request($request);

  return $data ? $data : array();
}
