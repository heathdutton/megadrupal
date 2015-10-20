<?php

/**
 * @file
 *   Template file for the twitter widget.
 */

print '<a href="https://twitter.com/share?url=' . urlencode(drupal_strip_dangerous_protocols($params['url'])) . '" class="twitter-share-button" data-count="horizontal">' . t('Tweet') . '</a>';