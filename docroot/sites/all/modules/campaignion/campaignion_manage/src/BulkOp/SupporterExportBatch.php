<?php

namespace Drupal\campaignion_manage\BulkOp;

use \Drupal\campaignion\ContactTypeManager;

class SupporterExportBatch extends BatchBase {
  protected $exporter;
  protected $file;
  protected $fields;

  public function __construct(&$data) {
    $this->fields = $data['fields'];
    $manager = ContactTypeManager::instance();
    $this->exporter = $manager->exporter('campaignion_manage');
    if (!($handle = fopen($data['csv_name'], 'a'))) {
      $context['results']['errors'] = t('Couldn\'t open temporary file to export supporters.');
    }
    $this->file = $handle;
  }

  public function apply($contact) {
    $this->exporter->setContact($contact);
    $csv_line = array();
    foreach ($this->fields as $field_name => $field_label) {
      $value = $this->exporter->value($field_name);
      if (is_array($value)) {
        if (empty($value)) {
          $value = array($field_name => '');
        }
      }
      else {
        $value = array($field_name => $value);
      }
      $csv_line += $value;
    }
    fputcsv($this->file, $csv_line);
  }

  public function commit() {
    fclose($this->file);
  }
}
