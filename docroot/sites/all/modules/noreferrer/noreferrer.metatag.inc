<?php

/**
 * Implements hook_metatag_info().
 */
function noreferrer_metatag_info() {
  $info['tags']['referrer'] = array(
    'label' => t('Referrer policy'),
    'description' => t('Using the <a href="https://w3c.github.io/webappsec/specs/referrer-policy/#referrer-policy-delivery-meta">referrer policy &lt;meta&gt; element</a>, a page can control the behavior of the Referer HTTP header attached to requests that originate from the page.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'advanced',
    'form' => array(
      '#type' => 'select',
      '#options' => array(
        'no-referrer' => t('No referrer'),
        'origin' => t('Origin'),
        'no-referrer-when-downgrade' => t('No referrer when downgrade'),
        'origin-when-crossorigin' => t('Origin when cross-origin'),
        'unsafe-url' => t('Unsafe URL'),
      ),
      '#empty_option' => t('- None -'),
    ),
  );
  return $info;
}
