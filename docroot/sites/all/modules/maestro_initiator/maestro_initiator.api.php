<?php

/**
 * Return the list of processes that allow workflow to be initiated.
 * 
 * hook_maestro_enabled_processes().
 * 
 * @return array as described below.
 * The processes must be indexed on keys using <modulename>_<processname> to ensure that there is no
 * namespace violation.
 */
function hook_maestro_enabled_processes() {
  return array('mymodule' => array(
      'title' => t('My module Name'), 
      'processes' => array(
        'mymodule_oncreate' => array('title' => t('Creation of my form')),
        'mymodule_onedit' => array('title' => t('Edit of my form')),
        'mymodule_ondelete' => array('title' => t('Delete of my form')),
      ),
    )
  );
}
