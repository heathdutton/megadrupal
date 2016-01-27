<?php

$plugin = array(
  'title' => t('Per-project commit log (Project)'),
  'description' => t('A per-project view of the global commitlog.'),
  'handler' => array(
    'class' => 'VersioncontrolProjectViewsSetProjectCommitView',
  ),
);
