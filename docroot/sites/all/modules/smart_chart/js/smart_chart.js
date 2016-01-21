(function ($) {

  // Mobile breakpoint.
  var mobile_bp = 768;

  $(document).ready(function () {
    $(document).scRegisterEvents();
  });

  // Function to call from backend with ajax_command_invoke to register events.
  $.fn.scRegisterEvents = function () {

    // Show Actions only on hovered containers.
    $('.chart-container .chart-item-wrapper').hover(function () {
      var current_document_width = $(document).width();
      if (current_document_width > mobile_bp) {
        $(this).children('.chart-actions').children('.form-item').children('a').not('.no-hide').removeClass('hidden');
      }
    }, function () {
      var current_document_width = $(document).width();
      if (current_document_width > mobile_bp) {
        $(this).children('.chart-actions').children('.form-item').children('a').not('.no-hide').addClass('hidden');
      }
    });

    // Toggle actions visibility.
    $('.chart-actions .toggle-actions').click(function () {
      var current_document_width = $(document).width();
      if (current_document_width <= mobile_bp) {
        $(this).parent().parent().children('.form-item').children('a').not('.no-hide').toggleClass('hidden');
      }
    });

    // New element highlight.
    $('.chart-container.refreshed').removeClass('refreshed');
  };

  // Function to call from backend with ajax_command_invoke to scroll to item.
  $.fn.scScrollToItemBackEnd = function ($button_id) {
    scScrollToItem($('input[id*=\'' + $button_id + '\']'));
  };

  // Scroll to item.
  function scScrollToItem(expand_button) {
    var header = document.querySelector('#body');
    var headerPosition = sc_get_position(header);
    var element = document.querySelector('#' + expand_button.parent().parent().parent().attr('id'));
    var elementPosition = sc_get_position(element);
    var fromTop = elementPosition - headerPosition;
    $('body,html').animate({scrollTop: fromTop}, 500);
  }

  // Get position of an item.
  function sc_get_position(element) {
    var position = 0;

    while (element) {
      position += (element.offsetTop - element.scrollTop + element.clientTop);
      element = element.offsetParent;
    }
    return position;
  }

})(jQuery);
