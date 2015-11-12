<?php

/**
 * @file
 * Contains ComstackRestfulEntityBase.
 */

/**
 * An abstract extension of of RestfulEntityBase.
 *
 * The purpose of this class is to optionally do all of the work needed for
 * cursor paging to work, though we also need a formatter as well.
 *
 * @see \
 * http://www.sitepoint.com/paginating-real-time-data-cursor-based-pagination
 */
abstract class ComstackRestfulEntityBase extends \RestfulEntityBase {
  protected $cursor_paging = FALSE;
  protected $cursor_paging_data = array();
  protected $wildcard_entity_id = NULL;

  /**
   * Overrides \RestfulBase::addCidParams().
   *
   * Make this function sensitive to our new pagination parameters if in use.
   */
  protected static function addCidParams($keys) {
    $cid_params = array();
    $request_params_to_ignore = array(
      '__application',
      'filter',
      'loadByFieldName',
      'q',
      'range',
      'sort',
    );

    if (!$this->cursor_paging) {
      $request_params_to_ignore[] = 'page';
    }
    else {
      $request_params_to_ignore[] = 'after';
      $request_params_to_ignore[] = 'before';
    }

    foreach ($keys as $param => $value) {
      if (in_array($param, $request_params_to_ignore)) {
        continue;
      }
      $values = explode(',', $value);
      sort($values);
      $value = implode(',', $values);

      $cid_params[] = substr($param, 0, 2) . ':' . $value;
    }
    return $cid_params;
  }

  /**
   * Return the direction we're meant to be paging in to be used with queries.
   */
  public function getPagingDirection() {
    $request = $this->getRequest();
    $after = isset($request['after']) && ctype_digit((string) $request['after']) ? $request['after'] : NULL;
    $before = isset($request['before']) && ctype_digit((string) $request['before']) ? $request['before'] : NULL;

    return !$after && $before ? 'ASC' : 'DESC';
  }

  /**
   * Overrides \RestfulDataProviderEFQ::queryForListSort().
   */
  protected function queryForListSort(\EntityFieldQuery $query) {
    if (!$this->cursor_paging) {
      parent::queryForListSort($query);
    }
    else {
      // Cursor crazy stuff here we go.
      // Ignore any sort instructions from controllers in favour of entity ID.
      // The sort direction by default will be DESC descending unless the
      // "before" query string is present, in which case we're going backwards
      // and the sort order should be ascending.

      // Hard code to "id", entity id...
      $query->entityOrderBy('entity_id', $this->getPagingDirection());
    }
  }

  /**
   * Overrides \RestfulDataProviderEFQ::queryForListPagination().
   */
  protected function queryForListPagination(\EntityFieldQuery $query) {
    list($offset, $range) = $this->parseRequestForListPagination();

    // Normal pager stuff.
    if (!$this->cursor_paging) {
      $query->range($offset, $range);
    }
    // Cursor paging!
    else {
      $request = $this->getRequest();
      $after = isset($request['after']) && ctype_digit((string) $request['after']) ? $request['after'] : NULL;
      $before = isset($request['before']) && ctype_digit((string) $request['before']) ? $request['before'] : NULL;

      $operator = $this->getPagingDirection() === 'DESC' ? '<' : '>';

      // You can only have the one, before or after, if after is present then
      // go with that.
      if ($after) {
        $query->entityCondition('entity_id', $after, $operator);
      }
      if (!$after && $before) {
        $query->entityCondition('entity_id', $before, $operator);
      }

      // Add limit from range.
      $query->range(0, $range);
    }
  }

  /**
   * Determine if an entity is valid, and accessible.
   *
   * @param $op
   *   The operation to perform on the entity (view, update, delete).
   * @param $entity_id
   *   The entity ID.
   *
   * @return bool
   *   TRUE if entity is valid, and user can access it.
   *
   * @throws RestfulNotFoundException
   * @throws RestfulForbiddenException
   * @throws RestfulUnprocessableEntityException
   */
  protected function isValidEntity($op, $entity_id) {
    $entity_type = $this->entityType;

    $params = array(
      '@id' => $entity_id,
      '@resource' => $this->getPluginKey('label'),
    );

    if (!$entity = entity_load_single($entity_type, $entity_id)) {
      if (!$this->isListRequest()) {
        throw new RestfulNotFoundException(format_string('The entity ID @id for @resource does not exist.', $params));
      }
      else {
        return FALSE;
      }
    }

    list(,, $bundle) = entity_extract_ids($entity_type, $entity);

    $resource_bundle = $this->getBundle();
    if ($resource_bundle && $bundle != $resource_bundle) {
      throw new RestfulUnprocessableEntityException(format_string('The entity ID @id is not a valid @resource.', $params));
    }

    if ($this->checkEntityAccess($op, $entity_type, $entity) === FALSE) {

      if ($op == 'view' && !$this->getPath()) {
        // Just return FALSE, without an exception, for example when a list of
        // entities is requested, and we don't want to fail all the list because
        // of a single item without access.
        return FALSE;
      }

      // Entity was explicitly requested so we need to throw an exception.
      throw new RestfulForbiddenException(format_string('You do not have access to entity ID @id of resource @resource', $params));
    }

    return TRUE;
  }

  /**
   * Overrides \RestfulEntityBase::getList().
   */
  public function getList() {
    $request = $this->getRequest();
    $autocomplete_options = $this->getPluginKey('autocomplete');
    if (!empty($autocomplete_options['enable']) && isset($request['autocomplete']['string'])) {
      // Return autocomplete list.
      return $this->getListForAutocomplete();
    }

    $entity_type = $this->entityType;
    $result = $this
      ->getQueryForList()
      ->execute();

    if (empty($result[$entity_type])) {
      return array();
    }

    $ids = array_keys($result[$entity_type]);

    // Pre-load all entities if there is no render cache.
    $cache_info = $this->getPluginKey('render_cache');
    if (!$cache_info['render']) {
      entity_load($entity_type, $ids);
    }

    $return = array();

    // If no IDs were requested, we should not throw an exception in case an
    // entity is un-accessible by the user.
    foreach ($ids as $id) {
      if ($row = $this->viewEntity($id)) {
        $return[] = $row;
      }
    }

    // If nothing was found, set the status code to 204.
    if (empty($return)) {
      $this->setHttpHeaders('Status', 204);
    }

    // Deal with cursor pagination data.
    if ($this->cursor_paging && !empty($ids)) {
      // Reverse the data set, weird I know.
      if ($this->getPagingDirection() === 'ASC') {
        $ids = array_reverse($ids);
        $return = array_reverse($return);
      }

      $first_id = $ids[0];
      $last_id = $ids[count($ids) - 1];

      $is_first_page = !isset($request['after']) && !isset($request['before']);

      // Alter the request so generated URLs are for other pages.
      $request['after'] = $last_id;
      $request['before'] = $first_id;

      $cursor_paging_data = array(
        'cursors' => array(
          'after' => $last_id,
          'before' => $first_id,
        ),
        'previous' => NULL,
        'next' => NULL,
        'range' => intval($this->getRange()),
        'total' => intval($this->getTotalCount()),
      );

      // Provide a "Previous" paging link if we're not on the first page.
      // If before or after haven't been specified in the request then it's
      // fair to assume this is the first page.
      if (!$is_first_page) {
        unset($request['after']);
        $request['before'] = $first_id;
        $cursor_paging_data['previous'] = array(
          'title' => t('Previous'),
          'href' => $this->getUrl($request),
        );
      }

      // Provide a "Next" paging link if there's more data to have.
      if ($this->getRange() == count($ids)) {
        unset($request['before']);
        $request['after'] = $last_id;
        $cursor_paging_data['next'] = array(
          'title' => t('Next'),
          'href' => $this->getUrl($request),
        );
      }

      $this->cursor_paging_data = $cursor_paging_data;
    }

    return $return;
  }

  /**
   * Overrides \RestfulEntityBase::viewEntity().
   *
   * Entity metadata wrapper will grab a value blindly and not assign the
   * correct PHP var type when returning a value (I think), alter view entity
   * to hard force the PHP var type so when json_encode goes near it the output
   * is correct.
   */
  public function viewEntity($id) {
    $entity_id = $this->getEntityIdByFieldId($id);
    $request = $this->getRequest();

    $cached_data = $this->getRenderedCache($this->getEntityCacheTags($entity_id));
    if (!empty($cached_data->data)) {
      return $cached_data->data;
    }

    if (!$this->isValidEntity('view', $entity_id)) {
      return;
    }

    $wrapper = entity_metadata_wrapper($this->entityType, $entity_id);
    $wrapper->language($this->getLangCode());
    $values = array();

    $limit_fields = !empty($request['fields']) ? explode(',', $request['fields']) : array();

    foreach ($this->getPublicFields() as $public_field_name => $info) {
      if ($limit_fields && !in_array($public_field_name, $limit_fields)) {
        // Limit fields doesn't include this property.
        continue;
      }

      $value = NULL;

      if ($info['create_or_update_passthrough']) {
        // The public field is a dummy one, meant only for passing data upon
        // create or update.
        continue;
      }

      if ($info['callback']) {
        $value = static::executeCallback($info['callback'], array($wrapper));
      }
      else {
        // Exposing an entity field.
        $property = $info['property'];
        $sub_wrapper = $info['wrapper_method_on_entity'] ? $wrapper : $wrapper->{$property};

        // Check user has access to the property.
        if ($property && !$this->checkPropertyAccess('view', $public_field_name, $sub_wrapper, $wrapper)) {
          continue;
        }

        if (empty($info['formatter'])) {
          if ($sub_wrapper instanceof EntityListWrapper) {
            // Multiple values.
            foreach ($sub_wrapper as $item_wrapper) {
              $value[] = $this->getValueFromProperty($wrapper, $item_wrapper, $info, $public_field_name);
            }
          }
          else {
            // Single value.
            $value = $this->getValueFromProperty($wrapper, $sub_wrapper, $info, $public_field_name);
          }
        }
        else {
          // Get value from field formatter.
          $value = $this->getValueFromFieldFormatter($wrapper, $sub_wrapper, $info);
        }

        // Force the PHP variable type, if the type has been set in the field
        // or property info.
        if (!is_null($value)) {
          $item_info = $sub_wrapper->info();
          $item_type = !empty($item_info['type']) ? $item_info['type'] : NULL;

          if (isset($info['wrapper_method']) && $info['wrapper_method'] === 'getIdentifier' || $item_type === 'integer') {
            $value = (int) $value;
          }
          elseif ($item_type === 'boolean') {
            $value = (boolean) $value;
          }
          elseif ($item_type === 'decimal') {
            $value = (float) $value;
          }
        }
      }

      if ($value && $info['process_callbacks']) {
        foreach ($info['process_callbacks'] as $process_callback) {
          $value = static::executeCallback($process_callback, array($value));
        }
      }

      $values[$public_field_name] = $value;
    }

    $this->setRenderedCache($values, $this->getEntityCacheTags($entity_id));
    return $values;
  }

  /**
   * Add paging information to the returned array if using cursor paging.
   *
   */
  public function additionalHateoas() {
    $data = array();

    if ($this->cursor_paging) {
      $data['paging'] = $this->cursor_paging_data;
    }

    return $data;
  }

  /**
   * Overrides \RestfulBase::versionedUrl().
   *
   * Alter the URL if there's a wildcard in it.
   */
  public function versionedUrl($path = '', $options = array(), $version_string = TRUE) {
    // Make the URL absolute by default.
    $options += array('absolute' => TRUE);
    $plugin = $this->getPlugin();
    if (!empty($plugin['menu_item'])) {
      $url = $plugin['menu_item'] . '/' . $path;
      return url(rtrim($url, '/'), $options);
    }

    // If there's an entity ID and a wildcard in the resource path replace it.
    if ($this->wildcard_entity_id && strpos($plugin['resource'], '%') !== FALSE) {
      $plugin['resource'] = str_replace('%', $this->wildcard_entity_id, $plugin['resource']);
    }

    $base_path = variable_get('restful_hook_menu_base_path', 'api');
    $url = $base_path;
    if ($version_string) {
      $url .= '/v' . $plugin['major_version'] . '.' . $plugin['minor_version'];
    }
    $url .= '/' . $plugin['resource'] . '/' . $path;
    return url(rtrim($url, '/'), $options);
  }

  /**
   * Return the maximum range that the endpoint will allow, if one isn't set
   * then return the default range.
   */
  public function getMaxRange() {
    return !empty($this->max_range) ? $this->max_range : $this->getRange();
  }

  /**
   * Overrides \RestfulBase::overrideRange().
   *
   * Allow exposed range to be set higher than the default.
   */
  protected function overrideRange() {
    $request = $this->getRequest();
    if (!empty($request['range'])) {
      $url_params = $this->getPluginKey('url_params');
      if (!$url_params['range']) {
        throw new \RestfulBadRequestException('The range parameter has been disabled in server configuration.');
      }

      if (!ctype_digit((string) $request['range']) || $request['range'] < 1) {
        throw new \RestfulBadRequestException('"Range" property should be numeric and higher than 0.');
      }
      if ($request['range'] <= $this->getMaxRange()) {
        // If there is a valid range property in the request override the range.
        $this->setRange($request['range']);
      }
    }
  }

  /**
   * Return the entity ID found from the request URL.
   */
  protected function getEntityID() {
    // If we've not set the entity id, do it.
    if (!$this->wildcard_entity_id) {
      $entity_id = NULL;

      // Ugly :/ would use getRequest here but the path gets removed from it if
      // cleanRequest has been executed.
      $path = $_GET['q'];

      // Take the request path, find the last numeric chunk.
      $url_parts = explode('/', $path);
      foreach (array_reverse($url_parts) as $part) {
        if (ctype_digit((string) $part) && $part > 0) {
          $this->wildcard_entity_id = $part;
          break;
        }
      }

      // Still?? Something isn't right here, throw an exception.
      if (!$this->wildcard_entity_id) {
        throw new RestfulBadRequestException('Path does not exist');
      }
    }

    return $this->wildcard_entity_id;
  }

  /**
   * Return an array of data from the payload. This doesn't do any of the
   * processing or validation against properties, simply grabs the request
   * data and tidies it.
   *
   * @return array
   *
   * @see \RestfulEntityBase::setPropertyValues()
   */
  protected function getRequestData() {
    $request = $this->getRequest();
    static::cleanRequest($request);

    // Make sure any text values are trimmed first.
    $strip_non_utf8 = variable_get('comstack_apis_strip_non_utf8', TRUE);
    $use_mb = function_exists('mb_check_encoding');

    foreach ($request as $k => $v) {
      if (is_string($v)) {
        if ($strip_non_utf8) {
          // Check for non utf8 characters.
          // http://stackoverflow.com/questions/1523460/ensuring-valid-utf-8-in-php
          if ($use_mb && mb_check_encoding($v, 'UTF-8') || !$use_mb && max(array_map('ord', str_split($v))) >= 240) {
            // Remove em.
            $v = preg_replace_callback('/./u', function (array $match) {
              return strlen($match[0]) >= 4 ? null : $match[0];
            }, $v);
          }
        }

        $request[$k] = trim($v);
      }
    }

    return $request;
  }
}
