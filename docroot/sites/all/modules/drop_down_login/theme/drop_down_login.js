/**
 * @file
 * Javascript and jQuery functions which are useful for the drop_down_login module.
 */

(function($, Drupal) {
  // Drupal.behaviors.
  Drupal.behaviors.drop_down_login = {
    attach:function(context, settings) {
      // Drop down login animation.
      $('#drop-down-login-wrapper',context).addClass('enable-dd');
      $('#drop-down-login-wrapper .login',context).click(function() {
        $(this).toggleClass('open').siblings('.dropdown').slideToggle("slow");
        return false;
      });
    }
  };
}(jQuery, Drupal));
