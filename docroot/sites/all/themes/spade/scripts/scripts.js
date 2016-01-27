(function ($) {

  /**
    * UI Improvements for the Content Editing Form
    */
  Drupal.behaviors.spadePathauto = {
    attach: function (context, settings) {
      var width = $('.node-form .form-item-path-alias input').width() - $('.node-form .form-item-path-alias label').width() - 5;
      $('.node-form .form-item-path-alias input').css('width', width);
    }
  }

})(jQuery)

// main.js
// plugin.js
