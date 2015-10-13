<?php

namespace Drupal\campaignion_newsletters;

use \Drupal\campaignion_newsletters\Subscription;
use \Drupal\campaignion\CRM\Import\Source\CombinedSource;
use \Drupal\campaignion\ContactTypeManager;

abstract class ProviderBase implements NewsletterProviderInterface {
  protected function getSource(Subscription $subscription, $target) {
    $source = NULL;
    if ($subscription->source) {
      $source = $subscription->source;
    }

    $language = NULL;
    if ($l = $subscription->newsletterList()) {
      $language = language_list()[$l->language];
    }

    $manager = ContactTypeManager::instance();
    if ($manager->crmEnabled() && ($exporter = $manager->exporterByEmail($subscription->email, $target, NULL, $language))) {
      $source = $source ? new CombinedSource($source, $exporter) : $exporter;
    }

    return $source;
  }
}
