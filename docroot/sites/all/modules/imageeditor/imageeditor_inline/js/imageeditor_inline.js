/*global Drupal: false, jQuery: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.behaviors.imageeditor_inline = {};
  Drupal.behaviors.imageeditor_inline.attach = function(context, settings) {
    var $images = $('img:not(' + Drupal.settings.imageeditor_inline.ignore + ')', context);

    if (Drupal.settings.imageeditor_inline.access_check == 1) {
      var images = [];
      $images.each(function(index, Element) {
        var origurl = $(this).attr('src');
        if (!origurl) {
          return true;
        }
        images.push(Drupal.imageeditor_inline.fullurl(origurl));
      });
      $.ajax({
        type: 'POST',
        url: Drupal.settings.imageeditor_inline.access_check_url,
        async: false,
        data: {
          images: images.toString()
        },
        success: function(data) {Drupal.settings.imageeditor_inline.access = data;},
        error: function(msg) {alert("Something went wrong: " + msg);}
      });
    }

    Drupal.imageeditor_inline.initialize($images);
  };

  Drupal.imageeditor_inline = {
    initialize: function($images) {
      var self = this;

      $images.each(function(index, Element) {
        if (Drupal.settings.imageeditor_inline.access_check == 0 || Drupal.settings.imageeditor_inline.access[index]) {
          if (this.naturalWidth == 0 || this.naturalHeight == 0) {
            $(this).bind('load', function() {
              self.addInlineEditor.call(this);
            });
          }
          else {
            self.addInlineEditor.call(this);
          }
        }
      });
    },
    addInlineEditor: function() {
      if (!$(this).hasClass('imageeditor-inline-processed')) {
        $(this).addClass('imageeditor-inline-processed');
        if (this.naturalWidth >= Drupal.settings.imageeditor_inline.min_dimention || this.naturalHeight >= Drupal.settings.imageeditor_inline.min_dimention) {
          var origurl = $(this).attr('src');
          var fullurl = Drupal.imageeditor_inline.fullurl(origurl);

          var options = {
            editors: Drupal.settings.imageeditor_inline.editors,
            uploaders: Drupal.settings.imageeditor_inline.uploaders,
            image: {url: fullurl},
            data: {fullurl: fullurl, origurl: origurl},
            $element: $(this),
            method: 'after',
            callback: Drupal.imageeditor_inline.save
          };
          Drupal.imageeditor.initialize(options);

          if (Drupal.settings.imageeditor_inline.icons_position == 1) {
            var $imageeditor_inline_parent = $(this).parent();
            var $imageeditor_div = $(this).next('span.imageeditor');
            if ($imageeditor_inline_parent.css('position') == 'static') {
              $imageeditor_inline_parent.css({position: 'relative'});
            }
            $imageeditor_div.css({position: 'absolute', bottom: '0px', margin: $(this).css('margin'), width: this.width}).hide();
            $imageeditor_inline_parent.hover(
              function(event) {
                $imageeditor_div.stop(true, true).fadeIn();
              },
              function(event) {
                $imageeditor_div.stop(true, true).fadeOut();
              }
            );
          }
        }
      }
    },
    save: function() {
      $.ajax({
        type: 'POST',
        url: Drupal.settings.imageeditor_inline.save_url,
        async: false,
        data: {
          image: Drupal.settings.imageeditor.save.image,
          fullurl: Drupal.settings.imageeditor.save.fullurl,
          origurl: Drupal.settings.imageeditor.save.origurl
        },
        success: function(data) {
          alert(data);
          $('img[src^="' + Drupal.settings.imageeditor.save.origurl + '"]')
            .attr('src', Drupal.settings.imageeditor.save.origurl + '#' + new Date().getTime());
        },
        error: function(msg) {alert("Something went wrong: " + msg);}
      });
    },
    fullurl: function(origurl) {
      Drupal.imageeditor_inline.cache = Drupal.imageeditor_inline.cache || {};
      if (!Drupal.imageeditor_inline.cache[origurl]) {
        // Styles are using $scheme . '://styles/' . $style_name . '/' . $scheme . '/' . $path
        // First regexp to detect and remove 'styles/style_name/scheme_name/'
        // Second regexp to remove "?itok=..."
        Drupal.imageeditor_inline.cache[origurl] = origurl
          .replace(new RegExp('/styles/[0-9a-zA-Z_]+/[0-9a-zA-Z_]+/', 'g'), '/')
          .replace(new RegExp('\\?itok=[0-9a-zA-Z_-]+$'), '')
          // Collage Formatter compatibility.
          .replace(new RegExp('collageformatter/'), '')
          .replace(new RegExp('[\\d]+x[\\d]+_(fake|copy|symlink)_'), '');
      }
      return Drupal.imageeditor_inline.cache[origurl];
    }
  };

})(jQuery);
