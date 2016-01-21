<?php

namespace Drupal\campaignion_manage;

class ContentPage extends Page {
  public function __construct($query) {
    $this->baseQuery = $query;
    $select = $query->query();

    $filters['title'] = new Filter\ContentTitle();
    $defaultActive = array('title');
    if (module_exists('campaignion_microsite')) {
      $filters['microsite'] = new Filter\ContentMicrosite($select);
    }
    if (module_exists('campaignion_campaign')) {
      $filters['campaign'] = new Filter\ContentCampaign($select);
    }
    $filters['language'] = new Filter\ContentLanguage($select);
    $filters['type'] = new Filter\ContentType($select);
    $filters['status'] = new Filter\ContentStatus();
    $default[] = array('type' => 'title', 'removable' => FALSE);
    $this->filterForm = new FilterForm('content', $filters, $default);

    $this->listing = new ContentListing(20);
    $this->bulkOpForm = new BulkOpForm(array(
      'publish' => new BulkOp\ContentPublish(),
      'unpubslih' => new BulkOp\ContentUnpublish(),
    ));
  }

  protected function getSelectedIds($form, &$form_state) {
    return $this->listing->selectedIds($form['listing'], $form_state, $this->baseQuery);
  }
}
