
/**
 * @file
 * Javascript related for the server monitor block.
 */

(function ($) {
  jQuery(document).ready(function($) {
    $.getJSON(Drupal.settings.basePath + 'server_monitor/status.json', function(data) {
      var servers = [];
      $('#server-monitor-status-table tbody tr:first').remove();
      $.each(data, function(key, val) {
        $('#server-monitor-status-table tbody').append('<tr><td>' + val['link'] + '</td><td class="status status-' + val['status'] + '">' + val['status_graphical'] + '</td></tr>');
      });
      $('#server-monitor-status-table tr:even').addClass('even');
      $('#server-monitor-status-table tr:odd').addClass('odd');
    });
  });

})(jQuery);
