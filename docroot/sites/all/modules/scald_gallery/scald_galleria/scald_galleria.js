/**
 * @file
 * scald_galleria.js
 *
 * General alterations on the galleria library.
 */
(function ($) {

  /**
   * Handle the alt attribute provided on the images.
   */
  Drupal.behaviors.handleAltAttribute = {
    attach: function (context, settings) {
      if (typeof Galleria != 'undefined') {
        Galleria.configure({
          dataConfig: function(img) {
            return {
              alt_attr: img.context.alt,
              title_attr: img.context.title,
              authors: $(img).data('authors')
            }
          },
          extend: function () {
            var gallery = this;
            gallery.bind('image', function(e) {
              if (e.imageTarget) {
                // For some reason the imageTarget points to a markup element which does not exist
                // so we need this workaround to specify the alt.
                setTimeout(function() {
                  var $imageTarget = $(e.target).find('.galleria-images .galleria-image').eq(e.index).find('img');
                  if (e.galleriaData.alt_attr) {
                    $imageTarget.attr('alt', e.galleriaData.alt_attr);
                  }
                  if (e.galleriaData.title_attr) {
                    $imageTarget.attr('title', e.galleriaData.title_attr);
                  }
                  if ($(e.target).find('.galleria-info-text .galleria-info-authors').length > 0) {
                    $(e.target).find('.galleria-info-text .galleria-info-authors').remove();
                  }
                  if (e.galleriaData.authors) {
                    $(e.target).find('.galleria-info-text').append($('<div></div>', {
                      'class' : 'galleria-info-authors'
                    }).html(e.galleriaData.authors));
                  }
                }, 100);
              }
            });
            gallery.bind('thumbnail', function(e) {
              if (e.thumbTarget) {
                if (e.galleriaData.alt_attr) {
                  $(e.thumbTarget).attr('alt', e.galleriaData.alt_attr);
                }
                if (e.galleriaData.title_attr) {
                  $(e.thumbTarget).attr('title', e.galleriaData.title_attr);
                }
              }
            });
          }
        });
      }
    }
  }

}(jQuery));
