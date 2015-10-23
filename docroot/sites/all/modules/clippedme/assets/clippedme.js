/**
 * @file
 * JS for Clipped.me module.
 */

(function($) {
  Drupal.behaviors.clippedMe = {
    attach: function (context, settings) {

      var theme = Drupal.settings.clippedMe.theme;

      $('a.' + Drupal.settings.clippedMe.selector).poshytip({
        className: theme,
        allowTipHover: true,
        alignTo: 'cursor',
        alignX: 'center',
        offsetY: 13,
        content: function(updateCallback) {

          // Initial display - 'Loading...'.
          var container = $('<div/>').addClass('clipped-box').html(Drupal.t('Loading summary...'));
          var href = $(this).attr('href');

          // No link? No summary.
          if (!href) {
            container.html(Drupal.t('No summary available'));
            return container;
          }

          // Talk to local proxy about this.
          $.getJSON(Drupal.settings.basePath + 'clippedme', {url: href}, function(data) {
            // Proxy said it has nothing for us. Or not enough.
            if (data.error || !data.summary) {
              container.html(Drupal.t('No summary available'));
              updateCallback(container);
              return;
            }
            // Got data? Format it neatly.
            container.html($('<h2/>').html(data.title));
            $('<a/>').html(data.source).attr('href', 'http://' + data.source).appendTo(container);
            var bullets = $('<ul/>');
            $.each(data.summary, function(i, item) {
              $('<li/>').html(item).appendTo(bullets);
            });
            bullets.appendTo(container);
            $('<hr/>').appendTo(container);
            $('<a/>').html($('<img/>').attr('src', Drupal.settings.clippedMe.branding)).attr('href', 'http://clipped.me').appendTo(container);
            // Replace initial display.
            updateCallback(container);
          });

          return container;
        }
      });

    }
  }
}(jQuery));
