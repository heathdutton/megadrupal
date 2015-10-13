/**
 * @file
 * Contains the javascript for the Views docket.
 */

(function ($) {
  Drupal.behaviors.views_docket = {
    attach: function (context) {
      var $container = $('.views-docket', context);
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
          $('tr.views-docket--' + selector_escape(tag), $content).show();
        }

        $content.find('tr').removeClass('odd even');
        $content.find('tr:visible').filter(':even').addClass('odd');
        $content.find('tr:visible').filter(':odd').addClass('even');
      });

      function selector_escape(selector) {
        return selector.replace( /(:|\.|\@|\[|\])/g, "\\$1" );
      }
    }
  };
})(jQuery);
