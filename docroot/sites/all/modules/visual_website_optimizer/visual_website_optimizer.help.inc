<?php

/**
 * @file
 * Help hook for Visual Website Optimizer module.
 */

/**
 * Internal version of hook_help.
 */
function _visual_website_optimizer_help($path, $arg) {
  if ($path == 'admin/config/system/visual_website_optimizer' || $path == 'admin/config/system/visual_website_optimizer/settings') {
    $h  = '<p>' . t('Visual Website Optimizer is an easy to use A/B, split and multivariate testing tool.') . '</p>';
    $h .= '<p>' . t('In order to use this module, you will need a Visual Website Optimizer account. A Free 30 day trial account is available from the VWO webite by clicking on either of the links below. To contribute towards the continued development of this module for free and with no effort, click on the affilitate link. Otherwise, click the other link without the affiliate marker.') . '</p>';
    $h .= '<p>'
      . t('<a href="@url">Sign up for a Free 30 day trial account AND help out</a>',
          array(
            '@url' => url('http://visualwebsiteoptimizer.com/create-account.php',
              array(
                'absolute' => TRUE,
                'query' => array(
                  'plan' => 'FREE_TRIAL',
                  'partner' => 'WSDRLBXN',
                ),
              )
            ),
          )
        ) . ', ' . t('or') . ' ';
    $h .= t('<a href="@url">Sign up for a Free 30 day trial without affiliate link.</a>',
          array(
            '@url' => url('http://visualwebsiteoptimizer.com/create-account.php',
              array(
                'absolute' => TRUE,
                'query' => array(
                  'plan' => 'FREE_TRIAL',
                ),
              )
            ),
          )
        )
      . '</p>';
    $h .= '<p>' . t('Once the Account ID has been set on this page, the configuration of your A/B and other tests is done by logging into your account on the VWO website.') . '</p>';

    return $h;
  }
  elseif ($path == 'admin/config/system/visual_website_optimizer/filter') {
    $h = '<p>' . t('VWO adds javascript to each page by default. If you are only running tests on parts of your site, or only wish to include certain users in your tests based on Drupal settings, you may filter which pages the javascritpt is added to with these filters.') . '</p>';
    $h .= '<p>' . t('If more than one setting on this page is set, they are treated as BOOLEAN AND.') . '</p>';

    return $h;
  }
  elseif ($path == 'admin/config/system/visual_website_optimizer/vwoid') {
    $h  = '<p>' . t('Your VWO Account ID can be extracted from the "VWO Tracking Code" available from the VWO site.') . '</p>';
    $h .= '<p>' . t('This can be found by following these simple directions:');
    $h .= '<ul><li>' . t('Log into your Visual Website Optimizer account.') . '</li>';
    $h .= '<li>' . t('Locate the <strong>Tools</strong> menu in the top right corner of the site.') . '</li>';
    $h .= '<li>' . t('Click on <strong>Get Tracking Code</strong>.') . '</li>';
    $h .= '<li>' . t('Copy and Paste the full tracking code text to the textarea below and click Save.') . '</li></ul></p>';
    $h .= '<p>' . t('In order to use this module, you will need a Visual Website Optimizer account. A Free 30 day trial account is available from the VWO webite by clicking on either of the links below. To contribute towards the continued development of this module for free and with no effort, click on the affilitate link. Otherwise, click the other link without the affiliate marker.') . '</p>';
    $h .= '<p>'
      . t('<a href="@url">Sign up for a Free 30 day trial account AND help out</a>',
          array(
            '@url' => url('http://visualwebsiteoptimizer.com/create-account.php',
              array(
                'absolute' => TRUE,
                'query' => array(
                  'plan' => 'FREE_TRIAL',
                  'partner' => 'WSDRLBXN',
                ),
              )
            ),
          )
        ) . ', ' . t('or') . ' ';
    $h .= t('<a href="@url">Sign up for a Free 30 day trial without affiliate link.</a>',
          array(
            '@url' => url('http://visualwebsiteoptimizer.com/create-account.php',
              array(
                'absolute' => TRUE,
                'query' => array(
                  'plan' => 'FREE_TRIAL',
                ),
              )
            ),
          )
        )
      . '</p>';
    return $h;
  }
}
