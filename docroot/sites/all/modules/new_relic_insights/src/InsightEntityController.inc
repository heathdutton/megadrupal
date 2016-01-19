<?php

/**
 * @file
 * Contains the Entity controller for the Insight entity.
 */

class InsightEntityController extends RemoteEntityAPIDefaultController {

  /**
   * {@inheritdoc}
   */
  protected function getNewEntityBundle($remote_entity) {
    foreach ($this->entityInfo['bundles'] as $bundle_name => $bundle_info) {
      if (isset($bundle_info['remote entity conditions'])) {
        $bundle_conditions_apply = FALSE;
        foreach ($bundle_info['remote entity conditions'] as $condition_property => $condition) {
          // All conditions defined in the bundle info must pass.
          switch ($condition) {
            case 'isset':
              if (isset($remote_entity->$condition_property)) {
                $bundle_conditions_apply = TRUE;
              }
              else {
                $bundle_conditions_apply = FALSE;
                break 2;
              }
              break;

              default:
                if ($remote_entity->$condition_property == $condition) {
                  $bundle_conditions_apply = TRUE;
                }
                else {
                  // One failed: this bundle is no good. Give up on it.
                  $bundle_conditions_apply = FALSE;
                  break 2;
                }
          }
        }

        // If the bundle passed, it's the right one.
        // (It's up to implementors to ensure their conditions are mutually
        // exclusive!)
        if ($bundle_conditions_apply) {
          return $bundle_name;
        }
      }
    }

    // If we get here then no bundle name matched.
    $remote_id_key = $this->entityInfo['remote entity keys']['remote id'];
    throw new Exception(t("No bundle name found for remote entity id '@id'.", array(
      '@id' => $remote_entity->{$remote_id_key},
    )));
  }

  /**
   * {@inheritdoc}
   */
  public function pack($remote_entity) {
    // If the custom "Insight ID" we append to all Transactions is not present,
    // generate one by hashing the whole entity. This is necessary for events
    // created prior to this module being enabled.
    $remote_id_key = $this->entityInfo['remote entity keys']['remote id'];
    if (!isset($remote_entity->{$remote_id_key})) {
      $remote_entity->{$remote_id_key} = md5(serialize($remote_entity));
    }

    // Update timestamp to match what's expected in Drupal (less precision).
    $remote_entity->timestamp = round($remote_entity->timestamp / 1000);

    return parent::pack($remote_entity);
  }

}
