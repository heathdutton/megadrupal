(function($) {
  Drupal.behaviors.nodeCompareTable = {
    attach: function(context, settings) {
      var wrapper = $('#compare-content-wrapper');
      if (wrapper.length) {
        table = $('#comparison-table', wrapper);
        $('#compare-view-mode-box:not(.compare-hidden)').show();
        $('#compare-only-diff').change(function() {
          if ($(this).is(':checked')) {
            $.each($('tr.compare-field-row', table), function() {
              var flag = $(this).find('td').eq(1).html();
              var diff = false;
              $.each($(this).find('td'), function() {
                if (!$(this).hasClass('compare-field-label')) {
                  if (flag != $(this).html()) {
                    diff = true;
                    return false;
                  }
                }
              });
              var diffClass = (diff === false) ? 'compare-no-diff' : 'compare-has-diff';
              $(this).addClass(diffClass);
            });
          } else {
            $('tr', table).removeClass('compare-no-diff compare-has-diff');
          }
        });
        if ((itemsCount = $('th.item-title', table).length) > 2) {
          $('th', table).each(function() {
            $(this).width($(this).width());
          });
          table.width('auto');
          var delCol = $('<a />', {
            href: '#',
            'class': 'remove-col',
            text: Drupal.t('remove (x)'),
            title: Drupal.t('Remove from the table'),
            click: function(e) {
              e.preventDefault();
              var index = $(this).parent('th').index();
              // For sticky table also.
              $('table', wrapper).find('tr').each(function() {
                this.removeChild(this.cells[index]);
              });
              if (--itemsCount <= 1) {
                $('#compare-view-mode-box, #compare-content-wrapper .remove-col').addClass('compare-hidden');
              }
            }
          });
          $('th.item-title', table).each(function() {
            $(this).prepend(delCol.clone(true));
          });
        }
      }
    }
  };
})(jQuery);
