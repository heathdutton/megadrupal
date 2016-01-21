(function ($) {
  Drupal.behaviors.hansel = {
    attach: function(context) {
      if ($('.hansel-children', context).length) {
        $('#hansel-rules', context).prepend('<div id="hansel-expand-buttons"><a href="#" class="expand" /> <a href="#" class="collapse" /></div>');
        $('#hansel-expand-buttons .expand', context).text(Drupal.t('Expand'));
        $('#hansel-expand-buttons .collapse', context).text(Drupal.t('Collapse'));
        $('#hansel-expand-buttons .expand', context).click(function() {
          $('.hansel-children', context).show();
          return false;
        });
        $('#hansel-expand-buttons .collapse', context).click(function() {
          $('.hansel-children', context).hide();
          return false;
        });
      }

      $('.hansel-rule:not(.hansel-processed)', context).addClass('hansel-processed').each(function () {
        var rule = $(this);

        if ($('.hansel-children', rule).length == 0) {
          return;
        }

        var rule_name = $('.hansel-rule-name:first', rule).text();
        $('.hansel-rule-name:first', rule).wrap('<a href="#" class="hansel-rule-name"></a>');
        $('.hansel-rule-name:first', rule).click(function() {
          $('.hansel-children:first', rule).slideToggle();
          return false;
        });
      });
    }
  };
})(jQuery);
