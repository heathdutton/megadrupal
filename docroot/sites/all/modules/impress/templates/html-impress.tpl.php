<!doctype html>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

  <head profile="<?php print $grddl_profile; ?>">

    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <meta name="viewport" content="width=1024" />
    <meta name="apple-mobile-web-app-capable" content="yes" />

    <?php print $styles; ?>
      
  </head>

  <body class="impress-not-supported <?php print $classes; ?>" <?php print $attributes;?>>

    <?php //this fallback message is only visible when there is `impress-not-supported` class on body. ?> 
    <div class="fallback-message">
      <p>Your browser <b>doesn't support the features required</b> by impress.js, so you are presented with a simplified version of this presentation.</p>
      <p>For the best experience please use the latest <b>Chrome</b>, <b>Safari</b> or <b>Firefox</b> browser.</p>
    </div>

    <div id="impress">

      <?php print views_embed_view('impress', 'page'); ?>

    </div>

    <div class="hint">
      <p><?php print t('Use a spacebar or arrow keys to navigate'); ?></p>
    </div>

    <?php print $scripts; ?>
    <script>impress().init();</script>

  </body>
  
</html>
