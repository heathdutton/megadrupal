<?php
/**
 * @file
 * Acta HTML5 theme's implementation to display the basic html
 * structure of a single Drupal page.
 */
?>

  <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
  <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>
    <head>
      <?php print $head; ?>
      <title><?php print $head_title; ?></title>
      <?php print $styles; ?>
      <?php print $scripts; ?>
	  	  <script type="text/javascript"><!-- JavaScript for Slider -->
 
function slideSwitch() {
    var $active = $('#slider li.active');
 
    if ( $active.length == 0 ) $active = $('#slider li:last');
 
    // use this to pull the images in the order they appear in the markup
    var $next =  $active.next().length ? $active.next()
        : $('#slider li:first');
 
    // uncomment the 3 lines below to pull the images in random order
 
    // var $sibs  = $active.siblings();
    // var rndNum = Math.floor(Math.random() * $sibs.length );
    // var $next  = $( $sibs[ rndNum ] );
 
    $active.addClass('last-active');
 
    $next.css({opacity: 0.0})
        .addClass('active')
        .animate({opacity: 1.0}, 1000, function() {
            $active.removeClass('active last-active');
        });
}
 
$(function() {
    setInterval( "slideSwitch()", 5000 );
});
 
</script>

    </head>
    <body class="<?php print $classes; ?>"<?php print $attributes; ?>>
      <?php print $page_top; ?>
      <?php print $page; ?>
      <?php print $page_bottom; ?>
    </body>
  </html>
