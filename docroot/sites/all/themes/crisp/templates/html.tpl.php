<?php
/**
* @file
* This is the html.tpl.php template
* 
* This template outputs html tags, content in between and including head, and the body tags
* 
*/
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>	
</head>

<body>
   <?php print $page_top; ?>
   <?php print $page; ?>
   <?php print $page_bottom; ?>

</body>

</html>
