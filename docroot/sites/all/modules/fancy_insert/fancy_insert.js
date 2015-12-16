
(function($) {

Drupal.behaviors.fancy_insert = {
  attach: function (context, settings) {
    $.each(Drupal.settings.fancy_insert, function(key, widget) {
      var value = widget['id'];
      if ($('#' + value + '-fancy_insert-button').length > 0) {
        return false;
      }
      else {
        $("#" + value + " .fieldset-wrapper").before('<div class ="fancy-insert-button"><a href ="javascript:;" id = "' + value + '-fancy_insert-button" class = "fancy_insert-button">' + Drupal.t('Expand') + '</a></div>');
      }

      $('#' + value + '-fancy_insert-button').bind('click', function (event) {
        if ($(this).text() == Drupal.t('Expand')) {
          $('#' + value).hide().addClass('fancy-insert').fadeIn();
          $('#' + value + ' tbody .draggable td').hide();

          $('#' + value + ' img').parents("td").show().css('display', 'table-cell');

          // Click on Insert button when an image is clicked
          $('#' + value + ' img').bind('click', function (e) {
            $(this).parent().next().find(".insert-button").trigger("click");
          });

          // Wrap each tr inside of a div. This avoid browser issues
          $('#' + value + ' tbody tr').wrap('<div class ="tr-wrapper" />');
          $(this).addClass('expanded').text(Drupal.t('Close'));
          if (widget['draggable'] && jQuery().draggable) {
            $('#' + value).draggable({ refreshPositions: true });
          }
        }
        else {
          $('#' + value).removeClass('fancy-insert');
          $('#' + value + ' tbody .draggable td').show().css('display', 'table-cell');
          $('#' + value + ' tbody td.tabledrag-hide').hide();

          $('#' + value + ' img').unbind('click');
          $('#' + value + ' tbody tr').unwrap();
          $('#' + value).removeAttr('style');
          $(this).removeClass('expanded').text(Drupal.t('Expand'));
        }
      });

    });
  }
};

})(jQuery);
