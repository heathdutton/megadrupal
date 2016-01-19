<?php


class entityaspect_ServiceFactory {

  protected $config;

  function __construct($config = array()) {
    $this->config = $config;
  }

  function pageInfoCache($registry) {
    return new entityaspect_PageInfoCache($registry->entitySystem);
  }

  function info($registry) {
    // Invoke the hook.
    $data = array(
      'hook_menu' => array(),
      'hook_menu_alter' => array(),
      'view_modes' => array(),
    );
    $api = new entityaspect_InjectedAPI_hookEntityAspect($data);
    module_invoke_all('entityaspect', $api);

    return new entityaspect_InfoWrapper($data);
  }

  function entitySystem($registry) {
    return new entityaspect_EntitySystem();
  }
}
