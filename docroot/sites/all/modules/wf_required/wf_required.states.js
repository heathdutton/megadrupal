/**
 * @file
 * Supplementary DOM manipulation in workflow admin page.
 */

(function ($) {

/**
 * Move the lost ignore required checboxes into the workflow states table
 */
Drupal.behaviors.wf_required_state = {
  attach: function (context) {
    // Add another column to the states table.
    $('#workflow_admin_ui_states', context).once('wf-required').each(function(context) {
      $(this).find('thead tr:first').append('<th class="wf-required">' + Drupal.t('Ignore required') + ' (*)</th>');

      // Walk through all states and move the matching checkbox to its respective row/column.
      for (var i in Drupal.settings.wf_required.states) {
        var sid = Drupal.settings.wf_required.states[i];
        var $required = $('.form-item-states-' + sid +'-required', context);
        var $row = $('.form-item-states-' + sid + '-state', context).parent().parent();
        var $td = $('<td></td>').addClass('wf-required');
        $row.append($td);
        $row.find('td.wf-required').append($required);
      }
    });
  }
};

})(jQuery);
