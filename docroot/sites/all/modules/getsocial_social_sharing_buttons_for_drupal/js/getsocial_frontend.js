/**
 * @file
 * Javascript global binds.
 *
 * Helper funtions to make ajax calls and manipulate DOM.
 */

(function () {
  'use strict';

  jQuery(document).ready(function ($) {

    var gsUrlApi = Drupal.settings.getsocial_share_buttons.getSocialUrlApi;
    var gsIdentifier = Drupal.settings.getsocial_share_buttons.getSocialIndentifier;

    var po = document.createElement("script");
    po.type = "text/javascript";
    po.async = true;
    po.src = gsUrlApi + '/widget/v1/gs_async.js?id=' + gsIdentifier;
    var s = document.getElementsByTagName("script")[0];
    s.parentNode.insertBefore(po, s);

  });
})();
