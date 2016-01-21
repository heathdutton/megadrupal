<?php

function dcycle_page_site_version() {
  if (variable_get('dcycle_show_site_version', TRUE)) {
    return dcycle_git_version();
  }
  else {
    return t('Cannot display site version because the dcycle_show_site_version variable is set to FALSE.');
  }
}
