<?php

class drulenium_presets_ui extends ctools_export_ui {

  function hook_menu(&$items) {

    // Change the item to a tab on the drulenium configuration page.
    $this->plugin['menu']['items']['list callback']['type'] = MENU_LOCAL_TASK;
    $this->plugin['menu']['items']['list callback']['weight'] = 10;

    parent::hook_menu($items);
  }
  
}
