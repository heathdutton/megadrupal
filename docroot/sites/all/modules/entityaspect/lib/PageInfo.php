<?php


class entityaspect_PageInfo {

  protected $info;
  protected $entityType;
  protected $entity;
  protected $route;

  function __construct(array $info, $entity_type, $entity, $route) {
    $this->info = $info;
    $this->entityType = $entity_type;
    $this->entity = $entity;
    $this->route = $route;
  }

  function entityType() {
    return $this->entityType;
  }

  function entity() {
    return $this->entity;
  }

  function route() {
    return $this->route;
  }

  function localTaskWeight() {
    return $this->info['tab'] ? $this->info['weight'] : FALSE;
  }

  function alterLocalTask(&$item) {

    if (empty($this->info['tab'])) {
      $item['#link']['weight'] = FALSE;
    }
    elseif (isset($this->info['weight'])) {
      $item['#link']['weight'] = $this->info['weight'];
    }

    if (1
      && isset($this->info['type'])
      && $this->info['type'] | MENU_LINKS_TO_PARENT
      && !empty($item['#link']['tab_parent_href'])
    ) {
      $item['#link']['href'] = $item['#link']['tab_parent_href'];
    }
  }

  /**
   * Access callback.
   * This only returns an internal variable.
   */
  function access() {
    return !empty($this->info['access']);
  }

  /**
   * Title callback.
   * This only returns an internal variable.
   */
  function title() {
    return $this->info['title'];
  }

  /**
   * Page callback.
   */
  function page() {
    if (!empty($this->info['page_title'])) {
      drupal_set_title($this->info['page_title']);
    }
    if (isset($this->info['view_mode'])) {
      return entity_view(
        $this->entityType,
        array($this->entity),
        $this->info['view_mode'],
        // langcode can be NULL
        NULL,
        $this->info['render as page']
      );
    }
    elseif (isset($this->info['page callback'])) {
      if (!empty($this->info['file'])) {
        require_once $this->info['file'];
      }
      return call_user_func_array($this->info['page callback'], $this->info['page arguments']);
    }
    elseif (isset($this->info['redirect'])) {
      // TODO: drupal_goto().
    }
  }

  function hook_pageapi($api) {
    if (!empty($this->info['pageapi_callback'])) {
      $f = $this->info['pageapi_callback'];
      $api->setModule($this->info['module']);
      if (empty($this->info['pageapi_arguments'])) {
        // We assume that the callback already knows entity type and route,
        // or does not need it, or has it in the extra arguments.
        $f($api, $this->entity);
      }
      else {
        // We assume that the callback already knows entity type and route,
        // or does not need it, or has it in the extra arguments.
        $args = array($api, $this->entity);
        $args = array_merge($args, $this->info['pageapi_arguments']);
        call_user_func_array($f, $args);
      }
    }
  }

  function hook_preprocess_page(&$vars) {
    
  }
}
