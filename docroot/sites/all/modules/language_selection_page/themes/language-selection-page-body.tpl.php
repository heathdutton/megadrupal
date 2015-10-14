<?php

/**
 * @file
 * The body template file of the module language_selection_page
 *
 * Variables used:
 *  - $language_selection_page: array of informations related to this module.
 *
 *  - $language_selection_page['from'] contains an array with these properties:
 *     - text: the URL in text of the page it's coming from
 *     - query: the query of this url, if any
 *     - url: the URL in text, already generated with url()
 *     - link: the HTML link, already generated with l()
 *
 *  - $language_selection_page['links'] contains an array of arrays for each
 *    enabled language with these properties:
 *     - language: the Drupal's language object
 *     - from: the url it's coming from
 *     - query: the query parameters if any
 *     - url: the URL in text already generated with url()
 *     - link: the HTML link, already generated with l()
 */
?>

<div class="language_selection_page_body">
  <div class="language_selection_page_body_inner">
    <p>Hello,</p>
    <p>You are coming from <?php print $language_selection_page['from']['link']; ?></p>
    <p>You should go to:</p>

    <ul>
    <?php foreach($language_selection_page['links'] as $data): ?>
      <li><?php echo $data['link']; ?></li>
    <?php endforeach; ?>
    </ul>

  </div>
</div>
