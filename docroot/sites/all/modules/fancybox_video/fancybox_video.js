/**
 * @file
 * Integration file for fancyBox video module.
 */

(function($) {
  Drupal.behaviors.fancyBoxVideo = {
    attach: function(context, settings) {
      $(".fancybox-video").click(function() {
        $.fancybox.showLoading();

        var wrap = $('<div id="fancybox-video-wrapper" style="display:none;"></div>').appendTo('body');
        var el   = $(this).clone().appendTo(wrap);

        var maxWidth = $(this).data().width;
        var maxHeight = $(this).data().height;

        el.oembed(null, {
          embedMethod : 'replace',
          maxWidth: maxWidth,
          maxHeight: maxHeight,
          afterEmbed  : function(rez) {
            var what = $(rez.code);
            var type = 'html';
            var scrolling = 'no';

            if (rez.type == 'photo') {
              what = what.find('img:first').attr('src');
              type = 'image';
            } else if (rez.type === 'rich') {
              scrolling = 'auto';
            }

            // Set width and height for iframe.
            what.attr('width', maxWidth);
            what.attr('height', maxHeight);

            $.fancybox.open({
              href      : what,
              type      : type,
              scrolling : scrolling,
              title     : rez.title || $(this).attr('title'),
              width     : maxWidth,
              height    : maxHeight,
              autoSize  : false
            });

            wrap.remove();
          },
          onError : function() {
            $.fancybox.open(this);
          }
        });

        return false;
      });
    }
  };
})(jQuery);
