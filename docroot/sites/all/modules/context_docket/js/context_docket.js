/**
 * @file
 * Contains the javascript for the Context docket.
 */

(function ($) {
  Drupal.behaviors.context_docket = {
    attach: function (context) {
      var $container = $('.context-docket', context);
      var $content = $('#docket-list tbody', $container);
      var $tabs = $('#docket-tabs', $container);

      $tabs.find('a[data-tag="all"]').once().addClass('selected');

      $tabs.find('a').bind('click', function() {
        $tabs.find('a').removeClass('selected');
        $(this).addClass('selected');
        var tag = $(this).data('tag');
        if (tag === 'all') {
          $('tr', $content).show();
        }
        else {
          $('tr', $content).hide();
          $('tr[data-tag="' + tag + '"]', $content).show();
        }

        $content.find('tr').removeClass('odd even');
        $content.find('tr:visible').filter(':even').addClass('odd');
        $content.find('tr:visible').filter(':odd').addClass('even');
      });

    }
  };
})(jQuery);
