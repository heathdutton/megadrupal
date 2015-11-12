/**
 * Collapses a hierarchial table's rows on click
 */
(function ($) {
  $(document).ready(function() {
    $('#time_statistics tbody tr td.label').css('cursor', 'pointer');

    $('#time_statistics tbody tr').click(function() {
      var collapsed = $(this).hasClass('collapsed');
      var id = '^' + $(this).attr('id') + '/';
      var hasChild = false;
      // Finds and hides/shows children. It is a child if it's id starts as the parent's id.
      $('#time_statistics tbody tr').each(function (index, domEle) {
        if (typeof $(domEle).attr('id') != 'undefined' && $(domEle).attr('id').match(id)) {
          hasChild = true;
          if (collapsed) {
            $(domEle).show();
          }
          else {
            $(domEle).hide();
          }
        }
      });

      // Changes its own class.
      if (hasChild) {
        if (collapsed) {
          $(this).removeClass('collapsed');
        }
        else {
          $(this).addClass('collapsed');
        }
      }
    });
  });
}(jQuery));