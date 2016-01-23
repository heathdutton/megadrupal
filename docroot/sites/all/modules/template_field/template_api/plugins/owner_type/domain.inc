<?php

/**
 * The abstract class for owner types
 */
class TemplateApiDomainOwnerPlugin extends TemplateApiOwnerType {


  /**
   * Permits access to an action and item
   *
   * @abstract
   *
   * @param $op
   * @param $item
   * @return Boolean | NULL
   *  Boolean if setting access and NULL if it has no impact
   *
   * @see TemplateAPIExportUI::access()
   *
   */
  public function access($op, Template $template) {
    global $user;

    //check if the owner has access to the domain or has access to master
    if (!empty($template->owner) && !isset($user->domain_user[$template->owner])) {
      return FALSE;
    }
  }

  /**
   * Get the values that can be an owner
   *
   * @return array
   */
  public function values() {
    $values = array();
    //get the domains
    $domains = domain_domains();
    foreach ($domains as $domain) {
      $values[$domain['domain_id']] = $domain['sitename'];
    }
    return $values;

  }

  public function label(Template $template) {
    if (!empty($template->owner) && $domain = domain_load($template->owner)) {
      return $domain['sitename'];
    }
  }
}
