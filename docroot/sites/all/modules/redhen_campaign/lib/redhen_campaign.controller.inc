<?php
/**
 * @file
 * The controller for the campaign entity containing the CRUD operations.
 */

/**
 * The controller class for campaigns contains methods for the campaign CRUD
 * operations. The load method is inherited from the default controller.
 */
class RedhenCampaignEntityController extends EntityAPIController {

  /**
   * Saves a campaign.
   *
   * @param RedhenCampaign $campaign
   *   The full campaign object to save.
   *
   * @return RedhenCampaign
   *   The saved campaign object.
   */
  public function save($campaign) {
    $campaign->updated = REQUEST_TIME;

    // New campaign, set created prop.
    if (isset($campaign->is_new) && $campaign->is_new) {
      $campaign->created = REQUEST_TIME;
    }
    // Move any images into the appropriate directory.
    $this->organizeImages($campaign);
    parent::save($campaign);

    return $campaign;
  }

  /**
   * Deletes multiple campaigns by id.
   *
   * @param array $campaign_ids
   *   An array of campaign IDs to delete.
   * @param DatabaseTransaction $transaction
   *   DB transaction object.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   *
   * @throws Exception
   */
  public function delete($campaign_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($campaign_ids)) {
      $campaigns = $this->load($campaign_ids, array());

      // Ensure the campaigns can actually be deleted.
      foreach ((array) $campaigns as $campaign_id => $campaign) {
        module_invoke_all('redhen_entity_predelete', $campaign, 'redhen_campaign');
      }
      // @todo: delete campaign page pages
      $transaction = db_transaction();
      try {
        parent::delete($campaign_ids, $transaction);
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

  /**
   * Helper function to move images into organized folders.
   */
  protected function organizeImages($entity, $subpath = '') {
    ctools_include('cleanstring');
    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
    foreach ($wrapper->getPropertyInfo() as $field_name => $property) {
      if ($property['type'] == 'field_item_image') {
        $file_data = $wrapper->{$field_name}->value();
        if ($file_data) {
          $file = file_load($file_data['fid']);
          $uri_scheme = file_uri_scheme($file->uri);
          if (in_array($uri_scheme, array('public', 'private'))) {
            $info = field_info_instance($this->entityType, $field_name, $entity->type);
            $target_dir = $uri_scheme . '://' . $info['settings']['file_directory'] . '/' . $subpath . ctools_cleanstring($entity->label());
            file_prepare_directory($target_dir, FILE_CREATE_DIRECTORY);
            file_move($file, $target_dir . '/' . $file->filename);
          }
        }
      }
    }
  }
}
