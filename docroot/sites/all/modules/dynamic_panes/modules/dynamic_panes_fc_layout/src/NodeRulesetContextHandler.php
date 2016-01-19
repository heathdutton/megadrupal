<?php

/**
 * @file
 */

namespace Drupal\dynamic_panes_fc_layout;

use Drupal\dynamic_panes\ContextHandler;
/**
 * Class NodeRulesetContextHandler.
 */
class NodeRulesetContextHandler extends ContextHandler {

  protected function initLayouts() {
    $query = new \EntityFieldQuery();

    try {
      $query->entityCondition('entity_type', DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME)
        ->entityCondition('bundle', DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME)
        ->fieldCondition(DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_RULESET, 'value', 'NULL', '!=');
    }
    catch (Exception $e) {
      return;
    }

    $result = $query->execute();

    if (isset($result[DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME])) {
      ctools_include('plugins');

      foreach ($result[DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME] as $item) {
        $layout_wrapper = entity_metadata_wrapper(DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME, $item->id);
        if ($layout_wrapper->__isset(DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_RULESET)) {
          foreach ($layout_wrapper->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_RULESET}->value() as $ruleset) {
            if ($plugin = ctools_get_plugins('ctools', 'access', 'ruleset:' . $ruleset)) {
              // TODO: check with other pane contexts too.
              $access_check_result = ctools_ruleset_ctools_access_check(FALSE, array($this->context), $plugin);
              if ($access_check_result) {
                $this->addLayout($item->id, $layout_wrapper->value());
              }
            }
          }
        }
      }
    }
  }
}
