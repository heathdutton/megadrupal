<?php
/**
 * @file
 * Contains the SearchApiAlterUserDomainAccess class.
 */

/**
 * Adds user domain access information to user indexes.
 */
class SearchApiAlterUserDomainAccess extends SearchApiAbstractAlterCallback {

  /**
   * Overrides SearchApiAbstractAlterCallback::supportsIndex().
   *
   * This plugin only supports indexes containing users.
   */
  public function supportsIndex(SearchApiIndex $index) {
    return $index->getEntityType() == 'user';
  }

  /**
   * {@inheritdoc}
   */
  public function alterItems(array &$items) {

  }

  /**
   * Overrides SearchApiAbstractAlterCallback::configurationFormSubmit().
   *
   * If the data alteration is being enabled, set "Published" and "Author" to
   * "indexed", because both are needed for the node access filter.
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    // Ensure the relevant domain properties are added to the index:
    //  - domain_domain_id (list of ID's the User has access to).

    $old_status = !empty($form_state['index']->options['data_alter_callbacks']['search_api_alter_userdomain_access']['status']);
    $new_status = !empty($form_state['values']['callbacks']['search_api_alter_user_domain_access']['status']);

    if (!$old_status && $new_status) {
      $form_state['index']->options['fields']['domain_domain_id']['type'] = 'integer';
    }

    return parent::configurationFormSubmit($form, $values, $form_state);
  }

}
