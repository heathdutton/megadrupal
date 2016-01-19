<?php

// We need to bootstrap the Flexiform module in the AJAX callback process.
// This whole file handles that and is added in hook_menu_alter for file/ajax
module_load_include('module', 'flexiform');
module_load_include('inc','flexiform','includes/flexiform.flexiform');
