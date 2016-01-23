<?php

/**
 * @file
 * Defines the pipeline for panels_dashboard module
 */

/**
 * Implementation of hook_default_panels_renderer_pipeline()
 */
function panels_dashboard_default_panels_renderer_pipeline() {
  $pipelines = array();

  $pipeline = new stdClass;
  $pipeline->disabled = FALSE; /* Edit this to true to make a default pipeline disabled initially */
  $pipeline->api_version = 1;
  $pipeline->name = 'dashboard';
  $pipeline->admin_title = t('Dashboard');
  $pipeline->admin_description = t('Allows users to locally rearrange and roll up panes on the panel and save state per account.');
  $pipeline->weight = 0;
  $pipeline->settings = array(
    'renderers' => array(
      0 => array(
        'access' => array(
          'plugins' => array(
            0 => array(
              'name' => 'perm',
              'settings' => array(
                'perm' => 'maintain dashboard',
              ),
              'context' => 'logged-in-user',
            ),
          ),
          'logic' => 'and',
        ),
        'renderer' => 'dashboard',
        'options' => array(),
      ),
    ),
  );
  $pipelines[$pipeline->name] = $pipeline;

  return $pipelines;
}
