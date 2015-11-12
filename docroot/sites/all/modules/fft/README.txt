Field Formatter Template

Description
--------------------------
Field Formatter Template (FFT) allow you can easy create and select template for
any field formmater.

Installation
--------------------------
1. Copy the entire fft directory the Drupal sites/all/modules
directory or use Drush with drush dl fft.
2. Login as an administrator. Enable the module on the Modules page.

Usage
--------------------------
- Create your template, default formmater template store in folder
'sites/all/formatter', you can change this folder at 'admin/config/content/fft'.
Formatter template create as normal tpl template, example you create slideshow
template, create file with name 'slideshow.tpl.php' in folder
'sites/all/formatter' open file and type:
<code>
<?php
/*Template Name: Slideshow Template*/
print_r($data);
?>
</code>
Now open "Manage Display" of a content type, chose any field and chose
"Formatter Template" you can config and select "Slideshow Template". Your field
formmatter will display with 'slideshow.tpl.php'. Variables you can use in
template is $data and $entity
- $data: stored all data of selected field.
- $entity: attached entity of field.
