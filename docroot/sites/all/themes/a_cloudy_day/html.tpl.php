<?php // $Id$ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"
  <?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
	   <!--[if lt IE 7.]>
        <style type="text/css" media="screen">
          @import url(<?php print base_path() . path_to_theme() ?>/ie_styles.css);
        </style>
    <![endif]--> 
    <script type="text/javascript">
      <!--//--><![CDATA[//><!--
      Drupal.behaviors.tntIEFixes = function (context) {
          if ($.browser.msie && ($.browser.version < 7)) {
            $('#topmenu li').hover(function() {
                $(this).addClass('hover');
              }, function() {
                $(this).removeClass('hover');
            });
          };
      };
      //--><!]]>
    </script>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
