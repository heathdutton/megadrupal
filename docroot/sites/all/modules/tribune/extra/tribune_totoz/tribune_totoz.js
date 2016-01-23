(function($) {

Drupal.behaviors.tribuneTotozSearch = {
  attach: function(context, settings) {
    $('div.tribune-wrapper', context).each(function(index, element) {
      $(this).delegate('.totoz-autocomplete-result', 'click', (function(tribune) {
          return function() {
            Drupal.tribune.insertText(tribune, ' [:' + $(this).data('totoz') + '] ');
            return false;
          };
        })($(this)));
    })

    $('.tribune-totoz', context).hover(
      function(e) {
        var link = $(this);
        var img = link.find('img');

        var container = link.parents('.tribune-posts:first');

        var imgWidth = img.width();
        var imgHeight = img.height();

        var containerWidth = container.width();
        var containerHeight = container.height();

        var linkWidth = link.width();
        var linkHeight = link.height();

        var linkPosition = link.position();

        var imgX = linkPosition.left + linkWidth + imgWidth;
        var imgY = linkPosition.top + linkHeight + imgHeight;

        var left_position = linkWidth;
        if (imgX > containerWidth) {
          left_position = (-1 * imgWidth);
        }

        var top_position = linkHeight;
        if (imgY > containerHeight) {
          top_position = (-1 * imgHeight) - linkHeight;
        }

        img.css('left', left_position);
        img.css('top', top_position);

        img.fadeIn(100);
      },
      function() {
        $(this).find('img').fadeOut(100);
      }
    );
  }
};

})(jQuery);



