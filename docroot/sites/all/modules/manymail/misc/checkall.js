(function ($) {

  function checkAllBoxes() {
    var checked = $(this).is(':checked'),
        checkAll = Drupal.settings.ManyMailCheckAll;

    if (!arguments.callee.setUp) {
      arguments.callee.setUp = true;

      if (!checked) {
        return;
      }
    }

    $(checkAll.list).find(':checkbox:not(' + checkAll.key + ')').attr({
      'checked' : checked,
      'disabled' : checked
    });
  }

  /**
   * Attaches the check all behavior.
   */
  Drupal.behaviors.ManyMailCheckAll = {
    attach: function (context, settings) {
      checkAllBoxes.call($(settings.ManyMailCheckAll.key).change(checkAllBoxes));
    }
  };

})(jQuery);
