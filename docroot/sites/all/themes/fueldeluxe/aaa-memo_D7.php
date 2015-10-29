
API templates ressources and links:

// html.tpl.php
modules/system/html.tpl.php
http://api.drupal.org/api/drupal/modules--system--html.tpl.php/7/source


// page.tpl.php
modules/system/page.tpl.php
http://api.drupal.org/api/drupal/modules--system--page.tpl.php/7/source

themes/bartik/templates/page.tpl.php
http://api.drupal.org/api/drupal/themes--bartik--templates--page.tpl.php/7/source


// node.tpl.php
modules/node/node.tpl.php
http://api.drupal.org/api/drupal/modules--node--node.tpl.php/7/source
themes/bartik/templates/node.tpl.php
http://api.drupal.org/api/drupal/themes--bartik--templates--node.tpl.php/7/source


// block.tpl.php
modules/block/block.tpl.php
http://api.drupal.org/api/drupal/modules--block--block.tpl.php/7/source


//maintenance-page.tpl.php
modules/system/maintenance-page.tpl.php
http://api.drupal.org/api/drupal/modules--system--maintenance-page.tpl.php/7/source
themes/bartik/templates/maintenance-page.tpl.php
http://api.drupal.org/api/drupal/themes--bartik--templates--maintenance-page.tpl.php/7/source


//comment.tpl.php
modules/comment/comment.tpl.php
http://api.drupal.org/api/drupal/modules--comment--comment.tpl.php/7/source
themes/bartik/templates/comment.tpl.php
http://api.drupal.org/api/drupal/themes--bartik--templates--comment.tpl.php/7/source







node.tpl.php debug:
------------------------------------------------------------------------------------------

Available variables: $content (array)
-------------------------------------
An array of node items. Use render($content) to print them all,
or print a subset such as render($content['field_example']). Use
hide($content['field_example']) to temporarily suppress the printing of a
given element.
Devel Render = $content (array)
<?php dpm($content); ?> gives : 

on node listing:
[body] => Array
[links] => Array
[field_tags] => Array

on full node:
[links] => Array
[body] => Array
[comments] => Array
[field_tags] => Array


<?php print render($content['links']); ?>
<?php print render($content['comments']); ?>
<?php print render($content['field_tags']); ?>
<?php print render($content); ?>

<?php hide($content['links']); ?>
<?php hide($content['field_tags']); ?>

// example:
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</div>


// example with custom field
<div>
  <?php render($content['field_name_of_image']); ?>
  <h2><?php print $title; ?></h2>
  <div class="content">
    <?php render($content); ?>
  </div>
</div>

Using render, you can output an element. This is more clever than simply printing stuff, as Drupal will keep track of what has been rendered, to avoid the same thing being printed twice on the place. When rendering the field above the title you are also removing for what's rendered when $content is rendered.








Other variables: $node (object)
-------------------------------
Full node object. Contains data that may not be safe.
Devel Load = $node Object

Print the full body:
<?php print $node->body['und']['0']['safe_value']; ?>

Print the summary:
<?php print $node->body['und']['0']['safe_summary']; ?>

Other fields:
<?php print $node->field_name['und']['0']['value']; ?>

