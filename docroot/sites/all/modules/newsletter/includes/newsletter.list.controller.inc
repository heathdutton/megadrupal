<?php

/**
 * @file
 * Controller class definition file for newsletter_list entity.
 */

/**
 * Newsletter List Controller
 */
class NewsletterListController extends DrupalDefaultEntityController {

  public function __construct($entityType) {
    parent::__construct('newsletter_list');
  }

  public function create() {
    $list = new stdClass();

    $list->title = NULL;
    $list->description = NULL;
    $list->send_rate = NULL;

    return $list;
  }

  /*
   * Saves a new newsletter list in database.
   *
   * @param $list
   *   An object containing the following:
   *    title - Required.
   *    send_rate - Required.
   *    ntid - Required.
   *    description - Optional.
   * @return
   *   The full, saved list object.
   */
  public function save($list) {
    field_attach_presave('newsletter_list', $list);
    if (!isset($list->nlid)) {
      $list->created = REQUEST_TIME;
      drupal_write_record('newsletter_list', $list);
      field_attach_insert('newsletter_list', $list);

      // Make sure to invoke the insert hook.
      module_invoke_all('entity_insert', $list, 'newsletter_list');
    }
    else {
      drupal_write_record('newsletter_list', $list, 'nlid');
      field_attach_update('newsletter_list', $list);

      // Make sure to invoke the update hook.
      module_invoke_all('entity_update', $list, 'newsletter_list');
    }

    return $list;
  }

  public function delete($nlid) {

    $list = newsletter_list_load($nlid);
    $deleted = db_delete('newsletter_list')
      ->condition('nlid', $nlid)
      ->execute();

    module_invoke_all('entity_delete', $list, 'newsletter_list');
    field_attach_delete('newsletter_list', $list);

    cache_clear_all();
    $this->resetCache();
    return $deleted ? TRUE : FALSE;
  }

}
