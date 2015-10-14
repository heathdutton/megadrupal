<?php

namespace Drupal\campaignion_manage;

class SupporterPage extends Page {
  public function __construct($query) {
    $this->baseQuery = $query;

    $filters['name'] = new Filter\SupporterName();
    if (module_exists('campaignion_supporter_tags')) {
      $filters['tags'] = new Filter\SupporterTag($this->baseQuery->query());
    }
    $filters['country']  = new Filter\SupporterCountry($this->baseQuery->query());
    $filters['activity'] = new Filter\SupporterActivity($this->baseQuery->query());
    $default[] = array('type' => 'name', 'removable' => FALSE);
    $this->filterForm = new FilterForm('supporter', $filters, $default);

    $bulkOps = array();
    if (module_exists('campaignion_supporter_tags')) {
      $bulkOps['tag']   = new BulkOp\SupporterTag(TRUE);
      $bulkOps['untag'] = new BulkOp\SupporterTag(FALSE);
    }
    $bulkOps['export'] = new BulkOp\SupporterExport();
    if (module_exists('campaignion_newsletters')) {
      $bulkOps['newsletter'] = new BulkOp\SupporterNewsletter();
    }
    $this->listing = new SupporterListing(20);
    $this->bulkOpForm = new BulkOpForm($bulkOps);
  }

  protected function getSelectedIds($form, &$form_state) {
    $element = &$form['listing'];
    $result = $this->baseQuery->result();
    $values = &drupal_array_get_nested_value($form_state['values'], $element['#array_parents']);
    if (empty($values['bulkop_select_all_matching'])) {
      $result->purge();
      $query = db_insert('campaignion_manage_result')
        ->fields(array('meta_id', 'contact_id'));
      $ids = array();
      foreach ($values['bulk_id'] as $id => $selected) {
        if ($selected) {
          $query->values(array(
            'meta_id' => $result->id,
            'contact_id' => $id,
          ));
        }
      }
      $query->execute();
    }
    return $result;
  }
}
