<?php
/**
 * @file
 * The theme system, which controls the output of Adverticum tags.
 */
?>
<?php if ($tag == 'goa3'): ?>
  <div id="zone<?php print $zid; ?>" class="goAdverticum"></div>
<?php elseif ($tag == 'js') : ?>
  <script type="text/javascript">
    // <![CDATA[
    var ord=Math.round(Math.random()*100000000);
    document.write('<sc'+'ript type="text/javascript" src="http://ad.adverticum.net/js.prm?zona=<?php print $zid; ?>&ord='+ord+'"><\/scr'+'ipt>');
    // ]]>
  </script>
  <noscript><a href="http://ad.adverticum.net/click.prm?zona=<?php print $zid; ?>&nah=!ie" target="_blank" title="<?php print t('Click on the advertisement!'); ?>">
    <img border="0" src="http://ad.adverticum.net/img.prm?zona=<?php print $zid; ?>&nah=!ie" alt="<?php print t('Advertisement'); ?>" /></a>
  </noscript>
<?php endif; ?>
