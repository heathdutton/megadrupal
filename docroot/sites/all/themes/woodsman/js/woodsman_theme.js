/**
 * @file
 * General javascript for woodsman theme
 */

(function($) {

  Drupal.behaviors.woodsman_theme = {
    attach: function (context) {
      if ($.fn.example) {
        $('#edit-search-block-form--2').example('Type your search and hit enter...');
      }
      WebFontConfig = {
        google : {
          families : ['Coustard::latin', 'Rufina::latin', 'Sintony::latin' ]
        }
      };
      (function() {
        var wf = document.createElement('script');
        wf.src = ('https:' == document.location.protocol ? 'https' : 'http') + '://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
        wf.type = 'text/javascript';
        wf.async = 'true';
        var s = document.getElementsByTagName('script')[0];
        s.parentNode.insertBefore(wf, s);
      })();
    }
  };

})(jQuery);
