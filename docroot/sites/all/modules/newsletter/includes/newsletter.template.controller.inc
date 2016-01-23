<?php

/**
 * @file
 * Controller class definition file for newsletter_template entity.
 */

/**
 * Newsletter Template Controller
 */
class NewsletterTemplateController extends DrupalDefaultEntityController {

  public function __construct($entityType) {
    parent::__construct('newsletter_template');
  }

  public function create() {
    $template = new stdClass();

    $template->subject = NULL;
    $template->basic = 0;
    $template->exposed = 0;

    return $template;
  }

  /*
   * Saves a new newsletter template in database.
   *
   * @param $template
   *   An object conatining the following:
   *    title - Required.
   *    send_rate - Required.
   *    ntid - Required.
   *    description - Optional.
   * @return
   *   The full, saved template object.
   */
  public function save($template) {
    $template->exposed = isset($template->exposed) ? $template->exposed : 0;
    field_attach_presave('newsletter_template', $template);
    if (!isset($template->ntid)) {
      $template->created = REQUEST_TIME;
      drupal_write_record('newsletter_template', $template);
      field_attach_insert('newsletter_template', $template);

      // Make sure to invoke the insert hook.
      module_invoke_all('entity_insert', $template, 'newsletter_template');
    }
    else {
      drupal_write_record('newsletter_template', $template, 'ntid');
      field_attach_update('newsletter_template', $template);

      // Make sure to invoke the update hook.
      module_invoke_all('entity_update', $template, 'newsletter_template');
    }

    return $template;
  }


  public function delete($ntid) {

    $template = newsletter_template_load($ntid);
    db_delete('newsletter_template')
      ->condition('ntid', $ntid)
      ->execute();

    module_invoke_all('entity_delete', $template, 'newsletter_template');
    field_attach_delete('newsletter_template', $template);

    cache_clear_all();
    $this->resetCache();
    return TRUE;
  }

}
