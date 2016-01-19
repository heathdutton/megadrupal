<?php
/**
 * @file
 * Cif_core_rulesets.ctools_rulesets.inc.
 */

/**
 * Implements hook_default_ctools_access_ruleset().
 */
function dynamic_panes_fc_layout_default_ctools_access_ruleset() {
  $export = array();

  $panelizers = dynamic_panes_fc_layout_get_panelizers();
  foreach ($panelizers as $panelizer) {
    $ruleset = new stdClass();
    $ruleset->disabled = FALSE; /* Edit this to true to make a default ruleset disabled initially */
    $ruleset->api_version = 1;
    $ruleset->name = 'panelizer_' . str_replace(':', '_', $panelizer->name);
    $ruleset->admin_title = 'Panelizer: ' . $panelizer->name;
    $ruleset->admin_description = '';
    $ruleset->requiredcontexts = array(
      0 => array(
        'identifier' => 'Node',
        'keyword' => 'node',
        'name' => 'entity:node',
        'entity_id' => '',
        'id' => 1,
      ),
    );
    $ruleset->contexts = array();
    $ruleset->relationships = array();
    $ruleset->access = array(
      'plugins' => array(
        0 => array(
          'name' => 'panelizer_layout',
          'settings' => array(
            'panelizer' => array(
              $panelizer->name => $panelizer->name,
            ),
          ),
          'context' => 'requiredcontext_entity:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    );
    $export[$ruleset->name] = $ruleset;
  }

  return $export;
}
