<?php
/**
 * @file
 * Contains the SearchApiAlterNodeDomainAccess class.
 */

/**
 * Adds node domain access information to node indexes.
 */
class SearchApiAlterNodeDomainAccess extends SearchApiAbstractAlterCallback {

  /**
   * Overrides SearchApiAbstractAlterCallback::supportsIndex().
   *
   * This plugin only supports indexes containing nodes.
   */
  public function supportsIndex(SearchApiIndex $index) {
    // Currently only node access is supported.
    return $index->getEntityType() === 'node';
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
    //  - domain_domain_site (send-to-all property (everyone has access)).

    $old_status = !empty($form_state['index']->options['data_alter_callbacks']['search_api_alter_node_domain_access']['status']);
    $new_status = !empty($form_state['values']['callbacks']['search_api_alter_node_domain_access']['status']);

    if (!$old_status && $new_status) {
      $form_state['index']->options['fields']['domain_domain_site']['type'] = 'boolean';
      $form_state['index']->options['fields']['domain_domain_id']['type'] = 'integer';
    }

    return parent::configurationFormSubmit($form, $values, $form_state);
  }

}
