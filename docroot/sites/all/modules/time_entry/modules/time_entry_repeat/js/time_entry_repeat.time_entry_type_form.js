(function($) {

/**
 * Disable repeat checkboxes when "all" is selected.
 */
Drupal.behaviors.timeEntryRepeatCheckboxes = {
  attach: function(context) {
    var $repeat = $('.form-item-time-entry-repeat', context);

    $repeat.each(function() {
      var $repeat = $(this),
          $checkboxes = $repeat.find('input[type="checkbox"]'),
          $all = $checkboxes.filter('[value="all"]');

      $checkboxes.not('[value="all"]')
        .bind('repeatUpdate', function() {
          $checkbox = $(this);
          if ($all[0].checked) {
            $checkbox
              .not(':checked')
                .trigger('click')
              .end()
                .attr('disabled', 'disabled');
          }
          else {
            $checkbox.removeAttr('disabled');
          }
        })
        .trigger('repeatUpdate');

      $all
        .bind('click', function() {
          $checkboxes.trigger('repeatUpdate');
        })
    });
  }
};

})(jQuery);