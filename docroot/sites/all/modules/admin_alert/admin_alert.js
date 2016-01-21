(function ($) {

  Drupal.behaviors.adminAlert = {
    attach: function (context, settings) {
      $("a.admin-alert-link:not(.admin-alert-link-processed)", context).click(function() {
        var item = $(this).parents("li.admin-alert");
        var link = $(this);
        var fullitem = $(this).parents("div.admin-alert-full");
        item.hide("slow");
        link.hide();
        fullitem.prepend('<div class="changed">' + Drupal.t('changed') + '</div>');
        var saveAlert = function(data) {
          if (data = "error") {
            link.show();
            item.show();
            item.prepend('<div class="error">' + Drupal.t('changes not saved!') + '</div>')
            fullitem.children("div.changed").text(Drupal.t('changes not saved!')).addClass('error');
          }
          else {
            item.remove();
          }
        }
        $.ajax({
          type: 'POST',
          url: this.href,
          dataType: 'json',
          success: saveAlert,
          data: js=1
        })
        return false;
      })
      .addClass('admin-alert-link-processed');
    }
  }

})(jQuery);