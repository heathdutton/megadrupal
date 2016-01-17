<?php

/**
 * @file
 * A small include that gives users options when an eBook is unavailable. Appears in a lightbox.
 */

if (isset($_GET['catalog-url'])) {
  $catalog_url = urldecode($_GET['catalog-url']) . '?' . $_GET['index'] . '=' . urldecode($_GET['term']);
}

?>
<div style="text-align: left; padding: 10px 10px 0 10px;">
  <h2>Why can't I get this eBook from a library?</h2>
  <ol>
    <li>Several publishers do not to sell eBooks to libraries. <a href="/why-doesnt-library-carry-ebook">Read more here</a>.</li>
    <li>It may be self-published - Many eBooks are self-published, meaning we can't get those items through our eBook vendors, 3M and Overdrive. If you purchase this book through Amazon.com, a portion of your sale is donated back to the library.</li>
    <?php if ($catalog_url) : ?>
      <li><a href="<?php print $catalog_url; ?>">Click here to search for the print version</a></li>
    <?php endif; ?>
    <li>We might not have it in our catalog. <a href="/request-purchase">Request it!</a></li>
  </ol>
</div>