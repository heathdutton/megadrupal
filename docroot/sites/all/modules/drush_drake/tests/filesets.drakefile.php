<?php

$api = 1;

$context = array(
  'root' => drake_argument(1, 'Root.'),
);

$filesets['field-modules'] = array(
  'dir' => context('root'),
  'include' => array('**/field/**/*.module'),
);

$filesets['field-modules-less'] = array(
  'dir' => context('root'),
  'extend' => 'field-modules',
  'exclude' => array('options.module', '**/list/**'),
);

$filesets['system-module'] = array(
  'dir' => context('root'),
  'include' => array('system.module'),
);

$filesets['anchoring'] = array(
  'dir' => context('root'),
  'include' => array('/form.inc'),
);

$filesets['scripts'] = array(
  'dir' => context('root'),
  'include' => array('script'),
);

$filesets['php-module'] = array(
  'dir' => context('root'),
  'include' => array('php/*'),
);

$tasks['field-modules'] = array(
  'action' => 'print_file_list',
  'files' => fileset('field-modules'),
);

$tasks['field-modules-less'] = array(
  'action' => 'print_file_list',
  'files' => fileset('field-modules-less'),
);

$tasks['system-module'] = array(
  'action' => 'print_first_file',
  'files' => fileset('system-module'),
);

$tasks['anchoring'] = array(
  'action' => 'print_file_list',
  'files' => fileset('anchoring'),
);

$tasks['scripts'] = array(
  'action' => 'print_file_list',
  'files' => fileset('scripts'),
);

$tasks['php-module'] = array(
  'action' => 'print_file_list',
  'files' => fileset('php-module'),
);

$actions['print_file_list'] = array(
  'callback' => 'print_file_list',
);

function print_file_list($context) {
  foreach ($context['files'] as $file) {
    drush_print($file->path());
  }
}

$actions['print_first_file'] = array(
  'callback' => 'print_first_file',
);

function print_first_file($context) {
  drush_print($context['files'][0]->path());
  drush_print($context['files'][0]->fullPath());
  drush_print((string) $context['files'][0]);
}
