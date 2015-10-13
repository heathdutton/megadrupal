<?php
/**
 * @file
 * The controller for the campaign page entity containing the CRUD operations.
 */

/**
 * The controller class for campaign pages contains methods for the CRUD
 * operations.
 */
class RedhenCampaignPageEntityController extends RedhenCampaignEntityController {

  /**
   * Deletes multiple campaign pages by id.
   *
   * @param array $campaign_page_ids
   *   An array of campaign page IDs to delete.
   * @param DatabaseTransaction $transaction
   *   DB transaction object.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   *
   * @throws Exception
   */
  public function delete($campaign_page_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($campaign_page_ids)) {
      $campaign_pages = $this->load($campaign_page_ids, array());

      // Ensure the $campaign_pages can actually be deleted.
      foreach ((array) $campaign_pages as $campaign_page_id => $campaign_page) {
        module_invoke_all('redhen_entity_predelete', $campaign_page, 'redhen_campaign_page');
      }
      $transaction = db_transaction();
      try {
        parent::delete($campaign_page_ids, $transaction);
      }
      catch (Exception $e) {
        if (isset($transaction)) {
          $transaction->rollback();
        }
        watchdog_exception($this->entityType, $e);
        throw $e;
      }
    }

    return TRUE;
  }

  protected function organizeImages($entity, $subpath = '') {
    ctools_include('cleanstring');
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
    $subpath .= ctools_cleanstring($wrapper->campaign->label()) . '/pages/';
    parent::organizeImages($entity, $subpath);
  }

}
