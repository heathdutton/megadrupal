(function ($) {
  Drupal.behaviors.match_list = {
    attach: function (context) {
      $('#match-list-tabs li a').click(function() {

        $('#match-list-tabs li a').removeClass('active');
        $(this).addClass('active');

        $('.match-list-matches-wrapper').removeClass('active');

        var delta = $(this).attr('delta');
        $('#match-list-matches-' + delta).addClass('active');

        return false;
      });
    }
  }
})(jQuery);
