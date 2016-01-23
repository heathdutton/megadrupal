Drupal.behaviors.selectAllMedia = {
  attach:function (context, settings) {
    jQuery('table.library th.all input').live('click', function () {
      var table = jQuery(this).closest('table.library');

      if (jQuery(this).attr('checked')) {
        Drupal.behaviors.selectAllMedia.checkAll(table);
      } else {
        Drupal.behaviors.selectAllMedia.uncheckAll(table);
      }
    });
  },

  checkAll:function (table) {
    jQuery('tbody tr', table).each(function () {
      jQuery('td:first input', this).attr('checked', 'checked');
    });
  },

  uncheckAll:function (table) {
    jQuery('tbody tr', table).each(function () {
      jQuery('td:first input', this).attr('checked', '');
    });
  }
}