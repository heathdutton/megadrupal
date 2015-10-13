(function($) {
  Drupal.behaviors.mediaBassetsBrowser = {
    attach: function(context, settings) {
      var form = $('#bassets-media-search-form');
      var fieldset = $('fieldset', form);
      var item = $('.media-item', form);
      var labels = $('.label-wrapper', item);

      labels.each(function(k, v) {
        $(this).html(
            '<label class="media-filename">' + $(this).text() + '</label>');
      });

      var radios = $('.form-radio', form);
      radios.hide();

      $('.form-item-bassets .form-item-bassets', form).css('padding', 0);

      // Catch the click on a media item
      item.bind('click', function() {
        // Remove all currently selected files
        $('.media-item').removeClass('selected');
        // Set the current item to active
        $(this).addClass('selected');
      });

      // TODO Temporary fix until media has a better fix.
      // Similar bug on media_youtube https://drupal.org/node/1551376.
      $('.pager a', form).each(function(k, v) {
        _this = $(this);
        _this.attr('href', _this.attr('href') + '#media-tab-bassets');
      });

      // Filter elements.
      if (fieldset.length > 0) {
        fieldset.bind('collapsed', function() {
          window.setTimeout(Drupal.media.browser.resizeIframe, 400);
        });
      }
    }
  };
}(jQuery));