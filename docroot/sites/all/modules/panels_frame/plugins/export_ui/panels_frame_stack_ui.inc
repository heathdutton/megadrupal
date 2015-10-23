<?php

$plugin = array(
  'schema' => 'panels_frame',
  'access' => 'administer panels frame',
  'handler' => array(
    'class' => 'panels_frame_stack_ui',
    'parent' => 'panels_frame_ui',
  ),

  'menu' => array(
    'menu prefix' => 'admin/structure/panels/frame',
    'menu item' => 'stacks',
    'menu title' => 'Stacks',
    'menu description' => 'Add, edit or delete stacks, single-dimension compilations of panels layouts.',
  ),

  'title singular' => t('stack'),
  'title singular proper' => t('Stack'),
  'title plural' => t('stacks'),
  'title plural proper' => t('Stacks'),
);
