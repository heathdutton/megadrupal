(function ($) {
  Drupal.behaviors.airLinkTrigger = {
    attach: function (context, settings) {
      $('.air-links-trigger', context).each(function (i, l) {
        var p = $(l).parent();
        $(l).bind('click', function (e) {
          e.preventDefault();
        });
        $(l).hover(function () {
          $('.air-links-list', p).show();
        }, function() {
          $('.air-links-list', p).hide();
        });
        $('.air-links-list li a', p).hover(function () {
          $('.air-links-list', p).show();
        }, function() {
          $('.air-links-list', p).hide();
        });
        $('.air-links-list li a', p).bind('click', function () {
          $('.air-links-list', p).hide();
        });
      });
    }
  }

})(jQuery);