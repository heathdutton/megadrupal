<?php
module_load_include('inc', 'amazon_store', 'amazon_store.pages');
$form = drupal_get_form('amazon_store_addcart_form', (string)$amazon_item->ASIN);
print drupal_render($form);


