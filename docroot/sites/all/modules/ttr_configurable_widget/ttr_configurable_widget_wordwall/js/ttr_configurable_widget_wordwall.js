(function($) {
  Drupal.behaviors.limitSelectableOptions = {
    attach: function (context) {
      if(Drupal.settings.ttr_configurable_widget_wordwall.cardinality !== undefined) {
        var cardinality = Drupal.settings.ttr_configurable_widget_wordwall.cardinality;

        // Scan the fields on page and check their default state.
        check_and_disable();

        $('.form-checkbox').click (function() {
          check_and_disable();
        });

        function check_and_disable(){
          if(cardinality !== undefined){
              $('div.word-checkbox-list').each(function() {
                container_id = $(this).attr("id");
                var container_id = '#' + $(this).closest('.word-checkbox-list').attr("id");
                var number_of_checked = count_checked_checkboxes(container_id);
                if(number_of_checked == cardinality){
                  $(container_id + " input.form-checkbox:checkbox:not(:checked)").attr('disabled', true);
                }else{
                  $(container_id + ' input.form-checkbox').removeAttr('disabled');
                }
              });
            }
        }

        function count_checked_checkboxes(container_id){
          the_number = $(container_id + ' input.form-checkbox:checked').size();
          return the_number;
        }
      }
    }
  }
})(jQuery);
