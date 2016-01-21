/**
 * 
 */

(function ($) {

  Drupal.viewModePerRole = {};

  Drupal.viewModePerRole.resetTabledrag = function(table, settings) {
    var base = $(table).attr('id');
    $(table).removeClass('tabledrag-processed')
            .find('.tabledrag-handle').remove()
            .end().find('.tabledrag-hide').removeClass('tabledrag-hide')
            .end().parent().find('.tabledrag-toggle-weight-wrapper').remove();
    Drupal.tableDrag[base] = new Drupal.tableDrag(table.get(0), settings.tableDrag[base]);
  };

  Drupal.viewModePerRole.clickAddMore = function (e, context, settings) {
      e.preventDefault();

      var base = 'view-mode-per-role-settings';
      var table = $('#' + base, context);

      // Clone the last existing line
      var new_item   = $('#' + base + ' tbody tr:last-child', context).clone();
      var old_number = parseInt(new_item.attr('id').substr(8),10),
          new_number = old_number + 1;

      // Rewrite classes, names and ids
      new_item.attr('id', 'setting-' + new_number)
              .toggleClass('even').toggleClass('odd');
      new_item.find('.form-type-select').each(function() {
        var select = $(this).find('select'),
            label  = $(this).find('label'),
            type   = select.attr('id').substr(select.attr('id').lastIndexOf('-') + 1);
         $(this).removeClass('form-item-vmpr-settings-' + (new_number-1) + '-' + type)
                .addClass('form-item-vmpr-settings-' + new_number + '-' + type);
         label.attr('for', 'edit-vmpr-settings-' + new_number + '-' + type);
         select.attr('id', 'edit-vmpr-settings-' + new_number + '-' + type)
               .attr('name', 'vmpr_settings[' + new_number + '][' + type + ']');
      });
      new_item.find('.field-delete-submit').attr('id', 'edit-vmpr-settings-' + new_number + '-delete');
      new_item.find('.setting-deleted').attr('name', 'vmpr_settings[' + new_number + '][deleted]').val(0);

      // Inject the new line
      $(table).append(new_item.show());

      // Reset table drag and custom events
      Drupal.viewModePerRole.resetTabledrag(table, settings);
      Drupal.viewModePerRole.attach(context, settings);
  };

  Drupal.viewModePerRole.clickDelete = function (e, context, settings) {
      e.preventDefault();

      var base = 'view-mode-per-role-settings';
      var table = $('#' + base, context);
      $(e.srcElement).parents('tr').find('select').val('_none')
                                   .end().find('.setting-deleted').val(1)
                                   .end().hide();

      Drupal.viewModePerRole.resetTabledrag(table, settings);
  };
  
  Drupal.viewModePerRole.attach = function (context, settings) {
    $('.field-add-more-submit', context).unbind('click')
                                        .bind('click', function (e) { Drupal.viewModePerRole.clickAddMore(e, context, settings); });
    $('.field-delete-submit', context).unbind('click')
                                      .bind('click', function (e) { Drupal.viewModePerRole.clickDelete(e, context, settings); });
  };

  Drupal.behaviors.viewModePerRole = {
    attach: Drupal.viewModePerRole.attach
  };

})(jQuery);
