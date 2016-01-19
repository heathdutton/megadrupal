<?php

function paas_metric_hookinfo_call($value) {
  if ($value == 'list') {
    header('Content-Type: text/plain');
    print 'hook,group';
    foreach (module_invoke_all('hook_info') as $hook => $info) {
      if (!empty($info['group'])) {
        print "\n{$hook},{$info['group']}";
      }
    }
  }
}
