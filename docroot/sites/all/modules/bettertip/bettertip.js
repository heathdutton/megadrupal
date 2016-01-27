(function ($) {
  var $bettertip;
  Drupal.behaviors.bettertip = {
    attach: function (context, settings) {
      if (!$bettertip) {
        $bettertip = $('<p class="bettertip"></p>');
        $bettertip.appendTo('body').hide();
      }

      $('[title]').once('bettertip-processed').each(function () {
        var $$ = $(this),
        titleText = $$.attr('title');

        $$.removeAttr('title');

        $$.hover(function (e) {
		if (titleText != "") {
          $bettertip
            .text(titleText)
            .css('top', (e.pageY - 10) + 'px')
            .css('left', (e.pageX + 20) + 'px')
            .fadeIn('slow');
			}}, function () {
          $('.bettertip').hide();
        });

        $$.mousemove(function (e) {
          $bettertip
            .css('top', (e.pageY - 10) + 'px')
            .css('left', (e.pageX + 20) + 'px');
        });
      });
    }
  };
})(jQuery);
