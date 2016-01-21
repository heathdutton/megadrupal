(function ($) {

  // hide disabled scripts from admin options form
  Drupal.behaviors.voipextensionAdminOptions = {
    attach: function (context) {
      $('#edit-voipextension-default-script-name option', context).each(function() {
        var $thisOption = $(this);
        var $thisValue = 'input[value=' + this.value + ']';
        var $thisCheckBox = $($thisValue);
        if (! $thisCheckBox.is(':checked')) {
          $thisOption.hide();
          $thisOption.removeAttr('selected');
        }
        $thisCheckBox.click(function() {
          if (this.checked) {
            $thisOption.show();
          }
          else {
            $thisOption.hide();
            $thisOption.removeAttr('selected');
          }
        });
      });
    }
  };

})(jQuery);
