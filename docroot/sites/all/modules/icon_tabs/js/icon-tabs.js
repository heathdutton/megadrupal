(function ($) {

  Drupal.behaviors.iconSelector = {
    attach: function (context) {
      var $iconDialog = $("#icon-dialog");

      $('.icon-input').hide();

      $iconDialog.once('icon-selector', function() {
        // Dialog settings.
        $(this).dialog({
          autoOpen: false,
          width: $(window).width() * 0.8,
          height: $(window).height() * 0.8
        });
      });

      // Button to open icon select dialog.
      $(".icon-dialog-opener, .icon-selector-default-icon").click(function(e) {
        e.preventDefault();

        // Get the correct select menu and default icon slot.
        $iconInput = $(this).siblings('.form-item').find('.icon-input');
        $defaultSlot = $(this).parent().find('.icon-selector-default-icon');

        // Open the dialog.
        $iconDialog.dialog('open');
      });

      // When an icon is selected.
      $('.icon-wrapper').click(function() {
        // Get the correct option key from the icon.
        var option = $(this).data("option");

        // Update the default icon slot with the new icon.
        $defaultSlot.empty();
        $defaultSlot.append($(this).text());

        // Update the select menu and close the dialog.
        $iconInput.val(option);
        $iconDialog.dialog('close');

      });

      $('#edit-icons .icon-clear').click(function(e) {
        e.preventDefault();
        $(this).siblings('.form-item').find('.icon-input').val('');
        $(this).siblings('.icon-selector-default-icon').empty();
      });

    }
  }

})(jQuery);
