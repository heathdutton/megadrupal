/**
 * @file
 * JavaScript functions for XC Auth module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
(function ($) {
  $(document).ready(function() {
    $('input[name=xc_auth_linkmode]').change(function() {
      var openSet = $('#' + this.value);
      var closeSet;
      if (this.value == 'demouser') {
        closeSet = $('#multiuser');
      }
      else {
        closeSet = $('#demouser');
      }
      openSet.removeClass("collapsed");
      closeSet.addClass("collapsed");
    });
  });
}(jQuery));