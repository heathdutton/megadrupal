(function ($) {
  
  $(document).ready(function() {
    // Show Actions only on hovered containers.
    _smart_chart_register_event();
    $('.chart-item-form').on('DOMNodeInserted DOMNodeRemoved', '.chart-container', function(e) {
      _smart_chart_register_event();
    });
  });
  
  function _smart_chart_register_event() {
    $('.chart-container .chart-actions').hover(function() {
      $(this).children('.form-item').children('a').removeClass('hidden');
    }, function () {
      $(this).children('.form-item').children('a').addClass('hidden');
    });
  }
})(jQuery);
