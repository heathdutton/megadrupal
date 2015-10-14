/* jshint */

(function ($) {
  'use strict';

  Drupal.behaviors.campaignion_ux = {};
  Drupal.behaviors.campaignion_ux.attach = function(context) {
    // generate an dialog
    // used fo graying out the content while using the "new" action
    if($('.campaignion-dialog-wrapper').length < 1) {
      var $dialog = $('<div class="campaignion-dialog-wrapper"><div class="camapignion-dialog-content"></div></div>');
      $dialog.appendTo($('body'));
    }
  };
}(jQuery));


